#include <stdio.h>
#include <stdlib.h>

#include "debug.h"
#include "../../utils/assert.h"
#include "../../utils/U64.h"

#ifdef DEBUG

#include "../../utils/clu/bin/header.h"

num_p num_create_variadic(uint64_t n, va_list args)
{
    if(n == 0)
        return num_create(NULL, NULL);

    uint64_t value = va_arg(args, uint64_t);
    node_p tail = node_create(value, NULL, NULL);

    node_p node = tail;
    for(uint64_t i=1; i<n; i++)
    {
        uint64_t value = va_arg(args, uint64_t);
        node = node_create(value, node, NULL);
    }
    return num_create(node, tail);
}

num_p num_create_immed(uint64_t n, ...)
{
    va_list args;
    va_start(args, n);
    return num_create_variadic(n, args);
}



bool uint64(uint64_t u1, uint64_t u2)
{
    if(u1 != u2)
    {
        printf("\n\n\tUINT64 ASSERT ERROR\t| " U64PX " " U64PX "", u1, u2);
        return false;
    }

    return true;
}

bool uint128_immed(uint128_t u1, uint64_t v2h, uint64_t v2l)
{
    if(!uint64(HIGH(u1), v2h))
    {
        printf("\n\tUINT128 ASSERT ERROR\t| HIGH");
        return false;
    }

    if(!uint64(LOW(u1), v2l))
    {
        printf("\n\tUINT128 ASSERT ERROR\t| LOW");
        return false;
    }

    return true;
}



bool node_rec(node_p node_1, node_p node_2, uint64_t index, node_p node_tail)
{
    if(node_1 == NULL)
    {
        if(node_2 != NULL)
        {
            printf("\n\n\tNODE VALIDITY ERROR\t| NUMBER SHORTER THAN EXPECTED");
            return false;
        }

        return true;
    }

    if(node_2 == NULL)
    {
        printf("\n\n\tNODE VALIDITY ERROR\t| NUMBER LONGER THAN EXPECTED");
        return false;
    }

    if(node_1->next != NULL)
    {
        if(node_1->next->prev != node_1)
        {
            printf("\n\n\tNODE VALIDITY ERROR\t| INVALID LINKAGE");
            return false;
        }
    }
    else
    {
        if(node_1 != node_tail)
        {
            printf("\n\n\tNODE VALIDITY ERROR\t| INVALID TAIL");
            return false;
        }
    }

    if(!uint64(node_1->value, node_2->value))
    {
        printf("\n\tNUMBER ASSERT ERROR\t| DIFFERENCE IN VALUE " U64P "", index);
        return false;
    }

    return node_rec(node_1->next, node_2->next, index + 1, node_tail);
}

bool num_str_inner(num_p num_1, num_p num_2)
{
    if(num_1->head == NULL)
    {
        if(num_1->tail != NULL)
        {
            printf("\n\tNUMBER VALIDITY ERROR\t| HEAD IS NULL BUT IT HAS TAIL");
            return false;
        }

        if(num_2->head != NULL)
        {
            printf("\n\tNUMBER ASSERT ERROR\t| NUMBER SHORTER THAN EXPECTED");
            return false;
        }

        return true;
    }

    if(num_1->head->prev != NULL)
    {
        printf("\n\tNUMBER VALIDITY ERROR\t| HEAD HAS PREV");
        return false;
    }

    if(num_1->tail == NULL)
    {
        printf("\n\tNUMBER VALIDITY ERROR\t| NUMBER HAS HEAD BUT NO TAIL");
        return false;
    }

    if(num_1->tail->next != NULL)
    {
        printf("\n\tNUMBER VALIDITY ERROR\t| TAIL HAS NEXT");
        return false;
    }

    return node_rec(num_1->head, num_2->head, 0, num_1->tail);
}

bool num_str(num_p num_1, num_p num_2)
{
    if(!num_str_inner(num_1, num_2))
    {
        printf("\n");
        num_display_tag("\tnum_1", num_1);
        num_display_tag("\tnum_2", num_2);
        return false;
    }

    return true;
}

bool num_immed(num_p num, uint64_t n, ...)
{
    va_list args;
    va_start(args, n);
    num_p num_2 = num_create_variadic(n, args);

    bool res = num_str(num, num_2);

    num_free(num_2);
    return res;
}



node_p num_get_node(num_p num, uint64_t count)
{
    node_p node = num->head;
    for(uint64_t i=0; i<count && node; i++, node = node->next);
    return node;
}

#endif



uint64_t num_count(num_p num);

void node_display_rec(node_p node, uint64_t index)
{
    DBG_CHECK_PTR(node);
    if(node == NULL || index == 0)
        return;

    printf("" U64PX " ", node->value);
    node_display_rec(node->prev, index - 1);
}

uint64_t num_display_cap(num_p num, uint64_t index)
{
    DBG_CHECK_PTR(num);
    if(num->head == NULL)
    {
        printf("0\t| 0");
        return 0;
    }

    uint64_t cnt = num_count(num);
    printf("" U64P "\t| ", cnt);
    node_display_rec(num->tail, index);
    return cnt;
}

void num_display(num_p num)
{
    DBG_CHECK_PTR(num);
    uint64_t cnt = num_display_cap(num, 4);
    if(cnt > 4) printf("...");
}

void num_display_tag(char *tag, num_p num)
{
    DBG_CHECK_PTR(num);
    printf("\n%s: ", tag);
    num_display(num);
}

void num_display_full(char *tag, num_p num)
{
    DBG_CHECK_PTR(num);
    printf("\n%s: ", tag);
    num_display_cap(num, UINT64_MAX);
}



node_p node_create(uint64_t value, node_p next, node_p prev)
{
    DBG_CHECK_PTR(next);
    DBG_CHECK_PTR(prev);

    node_p node = malloc(sizeof(node_t));
    assert(node);

    if(next) next->prev = node;
    if(prev) prev->next = node;

    *node = (node_t)
    {
        .value = value,
        .next = next,
        .prev = prev
    };
    return node;
}

/* free NODE struct and return next */
node_p node_consume(node_p node)
{
    DBG_CHECK_PTR(node);
    if(node == NULL) return NULL;

    node_p node_next = node->next;
    free(node);

    if(node_next) node_next->prev = NULL;
    return node_next;
}

/* free NODE list */
void node_free(node_p node)
{
    DBG_CHECK_PTR(node);
    while(node) node = node_consume(node);
}

/* creates a NODE struct with value 0 if NUM is null */
node_p node_denormalize(node_p node)
{
    DBG_CHECK_PTR(node);
    return node ? node : node_create(0, NULL, NULL);
}

/* frees NODE if NODE->value is 0 and NODE->next is NULL, returns updated tail */
node_p node_normalize(node_p node)
{
    DBG_CHECK_PTR(node);
    if(node == NULL || node->value != 0 || node->next != NULL)
        return node;

    node_p node_prev = node->prev;
    if(node_prev) node_prev->next = NULL;

    free(node);
    return node_prev;
}

uint64_t node_count(node_p node)
{
    DBG_CHECK_PTR(node);
    uint64_t i=0;
    for(; node; node = node->next, i++);
    return i;
}



num_p num_create(node_p head, node_p tail)
{
    DBG_CHECK_PTR(head);
    DBG_CHECK_PTR(tail);

    num_p num = malloc(sizeof(num_t));
    assert(num);

    *num = (num_t)
    {
        .head = head,
        .tail = tail
    };
    return num;
}

void num_free(num_p num)
{
    DBG_CHECK_PTR(num);

    node_free(num->head);
    free(num);
}

/* returns empty list if value is 0, list with one value otherwise */
num_p num_wrap(uint64_t value)
{
    if(value == 0)
        return num_create(NULL, NULL);

    node_p node = node_create(value, NULL, NULL);
    return num_create(node,  node);
}

node_p num_insert(num_p num, uint64_t value) // TODO test
{
    DBG_CHECK_PTR(num);
    num->tail = node_create(value, NULL, num->tail);
    if(num->head == NULL)
        num->head = num->tail;

    return num->tail;
}

node_p num_insert_head(num_p num, uint64_t value) // TODO test
{
    DBG_CHECK_PTR(num);
    num->head = node_create(value, num->head, NULL);
    if(num->tail == NULL)
        num->tail = num->head;

    return num->head;
}

void num_insert_list(num_p num, node_p head, node_p tail) // TODO test
{
    DBG_CHECK_PTR(num);
    DBG_CHECK_PTR(head);
    DBG_CHECK_PTR(tail);

    if(head == NULL)
        return;

    if(num->head == NULL)
    {
        *num = (num_t)
        {
            .head = head,
            .tail = tail
        };
        return;
    }

    num->tail->next = head;
    head->prev = num->tail;
    num->tail = tail;
}

node_p num_denormalize(num_p num, node_p node)
{
    DBG_CHECK_PTR(num);
    DBG_CHECK_PTR(node);
    return node ? node : num_insert(num, 0);
}

/* return TRUE if removed an element */
bool num_normalize(num_p num) // TODO test
{
    DBG_CHECK_PTR(num);
    if(num->head == NULL)
        return false;

    node_p node_tail = num->tail;
    num->tail = node_normalize(num->tail);
    if(num->tail == node_tail)
        return false;

    if(num->tail == NULL)
        num->head = NULL;

    return true;
}

num_p num_copy(num_p num) //  TODO test
{
    DBG_CHECK_PTR(num);
    num_p num_res = num_create(NULL, NULL);
    for(node_p node = num->head; node; node = node->next)
        num_insert(num_res, node->value);
        
    return num_res;
}

uint64_t num_count(num_p num)
{
    DBG_CHECK_PTR(num);
    return node_count(num->head);
}



node_p num_add_uint_offset(num_p num, node_p node, uint64_t value)
{
    DBG_CHECK_PTR(num);
    DBG_CHECK_PTR(node);

    if(node == NULL)
        return value ? num_insert(num, value) : NULL;

    node->value += value;
    if(node->value < value)
        num_add_uint_offset(num, node->next, 1);

    return node;
}

void num_add_uint(num_p num, uint64_t value)
{
    DBG_CHECK_PTR(num);
    num_add_uint_offset(num, num->head, value);
}

void num_sub_uint_offset_rec(num_p num, node_p node, uint64_t value)
{
    DBG_CHECK_PTR(num);
    DBG_CHECK_PTR(node);

    if(value == 0)
        return;

    bool do_next = node->value < value;
    node->value -= value;
    if(do_next)
        num_sub_uint_offset_rec(num, node->next, 1);
}

/* returns TRUE if passed offset is TAIL and ELIMINATED */
bool num_sub_uint_offset(num_p num, node_p node, uint64_t value)
{
    DBG_CHECK_PTR(num);
    DBG_CHECK_PTR(node);

    bool is_tail = node == num->tail;

    num_sub_uint_offset_rec(num, node, value);

    return num_normalize(num) && is_tail;
}

void num_sub_uint(num_p num, uint64_t value)
{
    DBG_CHECK_PTR(num);
    num_sub_uint_offset(num, num->head, value);
}

node_p num_mul_uint_offset(num_p num_res, node_p node_res, node_p node_2, uint64_t value)
{
    DBG_CHECK_PTR(num_res);
    DBG_CHECK_PTR(node_res);
    DBG_CHECK_PTR(node_2);

    if(node_2 == NULL)
        return node_res;

    uint128_t u = MUL(node_2->value, value);
    node_res = num_add_uint_offset(num_res, node_res, LOW(u));
    node_res = num_denormalize(num_res, node_res);
    num_add_uint_offset(num_res, node_res->next, HIGH(u));
    num_mul_uint_offset(num_res, node_res->next, node_2->next, value);
    return node_res;
}

/* NUM_RES can be NULL, preserves NUM */
num_p num_mul_uint(num_p num_res, num_p num, uint64_t value)
{
    DBG_CHECK_PTR(num_res);
    DBG_CHECK_PTR(num);

    if(num_res == NULL)
        num_res = num_create(NULL, NULL);

    if(value)
        num_mul_uint_offset(num_res, num_res->head, num->head, value);

    return num_res;
}



int64_t node_cmp(node_p node_1, node_p node_2)
{
    DBG_CHECK_PTR(node_1);
    DBG_CHECK_PTR(node_2);

    if(node_1 == NULL)
        return node_2 ? -1 : 0;

    if(node_2 == NULL)
        return 1;

    uint64_t later = node_cmp(node_1->next, node_2->next);
    if(later != 0)
        return later;

    if(node_1->value < node_2->value)
        return -1;

    return node_1->value > node_2->value ? 1 : 0;
}



bool num_is_zero(num_p num) // TODO test
{
    DBG_CHECK_PTR(num);
    return num->head == NULL;
}

int64_t num_cmp(num_p num_1, num_p num_2)
{
    DBG_CHECK_PTR(num_1);
    DBG_CHECK_PTR(num_2);
    return node_cmp(num_1->head, num_2->head);
}



void num_add_offset(num_p num_1, node_p node_1, node_p node_2, node_p node_tail)
{
    DBG_CHECK_PTR(num_1);
    DBG_CHECK_PTR(node_1);
    DBG_CHECK_PTR(node_2);
    DBG_CHECK_PTR(node_tail);

    if(node_1 == NULL)
    {
        num_insert_list(num_1, node_2, node_tail);
        return;
    }

    if(node_2 == 0)
        return;

    num_add_uint_offset(num_1, node_1, node_2->value);

    node_2 = node_consume(node_2);
    num_add_offset(num_1, node_1->next, node_2, node_tail);
}

num_p num_add(num_p num_1, num_p num_2)
{
    DBG_CHECK_PTR(num_1);
    DBG_CHECK_PTR(num_2)
    num_add_offset(num_1, num_1->head, num_2->head, num_2->tail);

    free(num_2);
    return num_1;
}

node_p num_sub_offset(num_p num_1, node_p node_1, node_p node_2)
{
    DBG_CHECK_PTR(num_1);
    DBG_CHECK_PTR(node_1);
    DBG_CHECK_PTR(node_2);

    if(node_2 == NULL)
        return node_1;

    if(num_sub_uint_offset(num_1, node_1, node_2->value))
    {
        free(node_2);
        return NULL;
    }

    node_2 = node_consume(node_2);
    num_sub_offset(num_1, node_1->next, node_2);

    return num_normalize(num_1) ? NULL : node_1;
}

num_p num_sub(num_p num_1, num_p num_2)
{
    DBG_CHECK_PTR(num_1);
    DBG_CHECK_PTR(num_2);
    num_sub_offset(num_1, num_1->head, num_2->head);

    free(num_2);
    return num_1;
}

void num_mul_rec(num_p num_res, node_p node_res, node_p node_1, node_p node_2)
{
    DBG_CHECK_PTR(num_res);
    DBG_CHECK_PTR(node_res);
    DBG_CHECK_PTR(node_1);
    DBG_CHECK_PTR(node_2);

    if(node_2 == NULL) return;

    node_res = num_mul_uint_offset(num_res, node_res, node_1, node_2->value);
    node_res = num_denormalize(num_res, node_res);
    node_2 = node_consume(node_2);
    num_mul_rec(num_res, node_res->next, node_1, node_2);
}

num_p num_mul(num_p num_1, num_p num_2)
{
    DBG_CHECK_PTR(num_1);
    DBG_CHECK_PTR(num_2);

    if(num_1->head == NULL)
    {
        num_free(num_1);
        num_free(num_2);
        return num_create(NULL, NULL);
    }

    num_p num_res = num_create(NULL, NULL);
    num_mul_rec(num_res, num_res->head, num_1->head, num_2->head);

    num_free(num_1);
    free(num_2);
    return num_res;
}

void num_div_mod_rec(num_p num_q, num_p num_r, node_p node_r, num_p num_2)
{
    DBG_CHECK_PTR(num_q);
    DBG_CHECK_PTR(num_r);
    DBG_CHECK_PTR(node_r);
    DBG_CHECK_PTR(num_2);
    printf("\nentering");

    if(node_r == NULL)
        return;


    if(num_normalize(num_r))
    {
        num_insert_head(num_q, 0);
        num_div_mod_rec(num_q, num_r, num_r->tail, num_2);
        return;
    }

    uint64_t r =  0;
    while(node_cmp(node_r, num_2->head) >= 0)
    {
printf("\n\t%lu %lu", num_r->tail->value, num_2->tail->value);
num_display_tag("num_r", num_r);

        uint64_t r_max, r_min;
        if(num_r->tail->value < num_2->tail->value)
        {
            uint128_t val_1 = U128_IMMED(num_r->tail->value, num_r->tail->prev->value);
            r_max = val_1 / num_2->tail->value;
            r_min = val_1 / (U128(num_2->tail->value) + 1);
        }
        else
        {
            uint64_t val_1 = num_r->tail->value;
            r_max = val_1 / num_2->tail->value;
            r_min = val_1 / (U128(num_2->tail->value) + 1);
        }
printf("\nr_min: %lu\tr_max: %lu", r_min, r_max);

        num_p num_aux = num_mul_uint(NULL, num_2, r_max);
        if(node_cmp(node_r, num_aux->head) < 0)
        {
printf("\nadjusting");
            num_free(num_aux);
            num_aux = num_mul_uint(NULL, num_2, r_min);
            r_max = r_min;
        }
num_display_tag("num_aux", num_aux);

// assert(node_r->prev->value == 0x39b4c936f7c040ed);

        node_r = num_sub_offset(num_r, node_r, num_aux->head);
        r += r_max;
        free(num_aux);
    }

    num_insert_head(num_q, r);
    node_r = node_r ? node_r->prev : num_r->tail;
    num_div_mod_rec(num_q, num_r, node_r, num_2);
}

void num_div_mod(num_p *out_num_q, num_p *out_num_r, num_p num_1, num_p num_2)
{
    DBG_CHECK_PTR(num_1);
    DBG_CHECK_PTR(num_2);

    num_p num_q = num_create(NULL, NULL);

    if(num_cmp(num_1, num_2) < 0)
    {
        *out_num_q = num_q;
        *out_num_r = num_1;
        num_free(num_2);
        return;
    }

    uint64_t cnt_2 = num_count(num_2);

    node_p node_r = num_1->tail;
    for(uint64_t i=1; i<cnt_2; i++)
        node_r = node_r->prev;

    num_div_mod_rec(num_q, num_1, node_r, num_2);
    num_normalize(num_q);

    *out_num_q = num_q;
    *out_num_r = num_1;
    num_free(num_2);
}

num_p num_div(num_p num_1, num_p num_2)
{
    DBG_CHECK_PTR(num_1);
    DBG_CHECK_PTR(num_2);

    num_p num_q, num_r;
    num_div_mod(&num_q, &num_r, num_1, num_2);
    num_free(num_r);
    return num_q;
}

num_p num_mod(num_p num_1, num_p num_2)
{
    DBG_CHECK_PTR(num_1);
    DBG_CHECK_PTR(num_2);

    num_p num_q, num_r;
    num_div_mod(&num_q, &num_r, num_1, num_2);
    num_free(num_q);
    return num_r;
}
