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
        return num_create(0, NULL, NULL);

    uint64_t value = va_arg(args, uint64_t);
    node_p tail = node_create(value, NULL, NULL);

    node_p node = tail;
    for(uint64_t i=1; i<n; i++)
    {
        uint64_t value = va_arg(args, uint64_t);
        node = node_create(value, node, NULL);
    }
    return num_create(n, node, tail);
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



bool node_rec(node_p node_1, node_p node_2, uint64_t index, node_p node_head)
{
    
    if(index == 0)
    {
        if(node_1 != NULL)
        {

            printf("\n\tNODE VALIDITY ERROR\t| NUMBER LONGER THAN EXPECTED");
            return false;
        }

        return true;
    }

    if(index > 1)
    {
        if(node_1->prev == NULL)
        {
            printf("\n\n\tNODE VALIDITY ERROR\t| NUMBER SHORTER THAN EXPECTED");
            return false;
        }

        if(node_1->prev->next != node_1)
        {
            printf("\n\n\tNODE VALIDITY ERROR\t| INVALID LINKAGE");
            return false;
        }
    }
    else
    {
        if(node_1 != node_head)
        {
            printf("\n\n\tNODE VALIDITY ERROR\t| INVALID HEAD");
            return false;
        }
    }

    if(!uint64(node_1->value, node_2->value))
    {
        printf("\n\tNUMBER ASSERT ERROR\t| DIFFERENCE IN VALUE " U64P "", index);
        return false;
    }

    return node_rec(node_1->prev, node_2->prev, index - 1, node_head);
}

bool num_str_inner(num_p num_1, num_p num_2)
{
    if(!uint64(num_1->count, num_2->count))
    {
        printf("\n\tNUMBER ASSERT ERROR\t| DIFFERENCE IN LENGTH");
        return false;
    }

    if(num_1->count == 0)
    {
        if(num_1->head != NULL)
        {
            printf("\n\tNUMBER VALIDITY ERROR\t| COUNT IS ZERO BUT IT HAS HEAD");
            return false;
        }
        
        if(num_1->tail != NULL)
        {
            printf("\n\tNUMBER VALIDITY ERROR\t| COUNT IS ZERO BUT IT HAS TAIL");
            return false;
        }

        return true;
    }

    if(num_1->head == NULL)
    {
        printf("\n\tNUMBER VALIDITY ERROR\t| COUNT IS " U64P " BUT IT HAS NO HEAD", num_1->count);
        return false;
    }

    if(num_1->head->prev != NULL)
    {
        printf("\n\tNUMBER VALIDITY ERROR\t| HEAD HAS PREV");
        return false;
    }

    if(num_1->tail == NULL)
    {
        printf("\n\tNUMBER VALIDITY ERROR\t| COUNT IS " U64P " BUT IT HAS NO TAIL", num_1->count);
        return false;
    }

    if(num_1->tail->next != NULL)
    {
        printf("\n\tNUMBER VALIDITY ERROR\t| TAIL HAS NEXT");
        return false;
    }

    return node_rec(num_1->tail, num_2->tail, num_1->count, num_1->head);
}

bool num_str(num_p num_1, num_p num_2)
{
    if(num_str_inner(num_1, num_2))
        return true;

    num_display("\n\tnum_1", num_1);
    num_display("\tnum_2", num_2);
    return false;
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

#endif



void node_display_rec(node_p node, uint64_t index)
{
    if(node == NULL || index == 0)
        return;

    printf("" U64PX " ", node->value);
    node_display_rec(node->prev, index - 1);
}

void num_display_inner(char *tag, num_p num, uint64_t index)
{
    printf("\n%s: ", tag);
    
    if(num->count == 0)
    {
        printf("0 | 0");
        return;
    }

    printf("" U64P " | ", num->count);
    
    node_display_rec(num->tail, index);
}

void num_display(char *tag, num_p num)
{
    num_display_inner(tag, num, 4);
    if(num->count > 4) printf(" ...");
}

void num_display_full(char *tag, num_p num)
{
    num_display_inner(tag, num, num->count);
}



node_p node_create(uint64_t value, node_p next, node_p prev)
{
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
    if(node == NULL)
        return NULL;

    node_p node_next = node->next;
    assert(node->prev == NULL);
    free(node);

    if(node_next) node_next->prev = NULL;
    return node_next;
}

/* free NODE list */
void node_free(node_p node)
{
    while(node)
        node = node_consume(node);
}

/* creates a NODE struct with value 0 if NUM is null */
node_p node_denormalize(node_p node)
{
    return node ? node : node_create(0, NULL, NULL);
}

/* frees NODE if NODE->value is 0 and NODE->next is NULL */
node_p node_normalize(node_p node)
{
    if(node == NULL || node->value != 0 || node->next != NULL)
        return node;
    
    node_p node_prev = node->prev;
    if(node_prev) node_prev->next = NULL;

    free(node);
    return node_prev;
}



num_p num_create(uint64_t count, node_p head, node_p tail)
{
    num_p num = malloc(sizeof(num_t));
    assert(num);

    *num = (num_t)
    {
        .count = count,
        .head = head,
        .tail = tail
    };
    return num;
}

void num_free(num_p num)
{
    node_free(num->head);
    free(num);
}

/* returns empty list if value is 0, list with one value otherwise */
num_p num_wrap(uint64_t value)
{
    if(value == 0)
        return num_create(0, NULL, NULL);

    node_p node = node_create(value, NULL, NULL);
    return num_create(1, node,  node);
}

node_p num_insert(num_p num, uint64_t value) // TODO test
{
    num->tail = node_create(value, NULL, num->tail);
    num->count++;
    if(num->count == 1)
        num->head = num->tail;

    return num->tail;
}

node_p num_insert_head(num_p num, uint64_t value) // TODO test
{
    num->head = node_create(value, num->head, NULL);
    num->count++;
    if(num->count == 1)
        num->tail = num->head;

    return num->head;
}

void num_insert_list(num_p num, node_p head, node_p tail, uint64_t cnt) // TODO test
{
    if(cnt == 0)
        return;

    if(num->count == 0)
    {
        *num = (num_t)
        {
            .count = cnt,
            .head = head,
            .tail = tail
        };
        return;
    }

    num->count += cnt;
    num->tail->next = head;
    head->prev = num->tail;
    num->tail = tail;
}

node_p num_denormalize(num_p num, node_p node)
{
    if(node)
        return node;

    return num_insert(num, 0);
}

bool num_normalize(num_p num) // TODO test
{
    if(num->count == 0)
        return false;

    node_p node_tail = num->tail;
    num->tail = node_normalize(num->tail);
    if(num->tail == node_tail)
        return false;

    num->count--;
    if(num->count == 0)
        num->head = NULL;

    return true;
}

num_p num_copy(num_p num) //  TODO test
{
    num_p num_res = num_create(0, NULL, NULL);
    for(node_p node = num->head; node; node = node->next)
        num_insert(num_res, node->value);
        
    return num_res;
}



node_p num_add_uint_rec(num_p num, node_p node, uint64_t value)
{
    if(node == NULL)
        return value ? num_insert(num, value) : NULL;

    node->value += value;
    if(node->value < value)
        num_add_uint_rec(num, node->next, 1);

    return node;
}

void num_add_uint(num_p num, uint64_t value)
{
    num_add_uint_rec(num, num->head, value);
}

void num_sub_uint_rec(num_p num, node_p node, uint64_t value)
{
    if(value == 0)
        return;

    assert(node);

    bool do_next = node->value < value;
    node->value -= value;
    if(do_next)
        num_sub_uint_rec(num, node->next, 1);

    num_normalize(num);
}

void num_sub_uint(num_p num, uint64_t value)
{
    num_sub_uint_rec(num, num->head, value);
}

node_p num_mul_uint_rec(num_p num_res, node_p node_res, node_p node, uint64_t value)
{
    if(node == NULL)
        return node_res;

    uint128_t u = MUL(node->value, value);
    node_res = num_add_uint_rec(num_res, node_res, LOW(u));
    node_res = num_denormalize(num_res, node_res);
    num_add_uint_rec(num_res, node_res->next, HIGH(u));
    num_mul_uint_rec(num_res, node_res->next, node->next, value);
    return node_res;
}

/* NUM_RES can be NULL, preserves NUM */
num_p num_mul_uint(num_p num_res, num_p num, uint64_t value)
{
    assert(num);

    if(num_res == NULL)
        num_res = num_create(0, NULL, NULL);

    if(value)
        num_mul_uint_rec(num_res, num_res->head, num->head, value);

    return num_res;
}



int64_t node_cmp(node_p node_1, node_p node_2, uint64_t cnt)
{
    for(uint64_t i=0; i<cnt; i++)
    {
        if(node_1 == NULL)
            return node_2 ? -1 : 0;

        if(node_2 == NULL)
            return 1;

        if(node_1->value > node_2->value)
            return 1;

        if(node_1->value < node_2->value)
            return -1;

        node_1 = node_1->prev;
        node_2 = node_2->prev;
    }

    return 0;
}

int64_t num_cmp_offset(num_p num_1, num_p num_2, uint64_t offset)
{
    assert(num_1);
    assert(num_2);

    if(num_1->count > num_2->count + offset)
        return 1;

    if(num_1->count < num_2->count + offset)
        return -1;

    return node_cmp(num_1->tail, num_2->tail, num_2->count);
}

int64_t num_cmp(num_p num_1, num_p num_2)
{
    return num_cmp_offset(num_1, num_2, 0);
}



void num_add_rec(num_p num_1, node_p node_1, node_p node_2, node_p node_tail, uint64_t cnt)
{
    if(node_1 == NULL)
    {
        num_insert_list(num_1, node_2, node_tail, cnt);
        return;
    }

    if(node_2 == 0)
        return;
    
    num_add_uint_rec(num_1, node_1, node_2->value);

    node_2 = node_consume(node_2);
    num_add_rec(num_1, node_1->next, node_2, node_tail, cnt - 1);
}

num_p num_add(num_p num_1, num_p num_2)
{
   assert(num_1);
   assert(num_2);

   num_add_rec(num_1, num_1->head, num_2->head, num_2->tail, num_2->count);
   free(num_2);
   return num_1;
}

// TODO: num_2 keep
node_p num_sub_rec(num_p num_1, node_p node_1, node_p node_2)
{
    if(node_2 == NULL)
        return node_1;

    assert(node_1);

    num_sub_uint_rec(num_1, node_1, node_2->value);

    node_2 = node_consume(node_2);
    num_sub_rec(num_1, node_1->next, node_2);
    return num_normalize(num_1) ? NULL : node_1;
}

num_p num_sub(num_p num_1, num_p num_2)
{
    assert(num_1);
    assert(num_2);

    num_sub_rec(num_1, num_1->head, num_2->head);
    free(num_2);
    return num_1;
}

void num_mul_rec(num_p num_res, node_p node_res, node_p node_1, node_p node_2)
{
    if(node_2 == NULL)
        return;

    node_res = num_mul_uint_rec(num_res, node_res, node_1, node_2->value);
    node_res = num_denormalize(num_res, node_res);
    node_2 = node_consume(node_2);
    num_mul_rec(num_res, node_res->next, node_1, node_2);
}

num_p num_mul(num_p num_1, num_p num_2)
{
    assert(num_1);
    assert(num_2);

    if(num_1->count == 0)
    {
        num_free(num_1);
        num_free(num_2);
        return num_create(0, NULL, NULL);
    }

    num_p num_res = num_create(0, NULL, NULL);
    num_mul_rec(num_res, num_res->head, num_1->head, num_2->head);

    num_free(num_1);
    free(num_2);
    return num_res;
}

void num_div_mod_rec(
    num_p num_q,
    num_p num_r,
    node_p node_r,
    uint64_t cnt_r,
    num_p num_2
)
{
    if(cnt_r == UINT64_MAX)
        return;

    uint64_t r =  0;
    while(num_cmp_offset(num_r, num_2, cnt_r) >= 0)
    {
        uint64_t r_max, r_min;
        if(num_r->count - cnt_r > num_2->count)
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

        num_p num_aux = num_mul_uint(NULL, num_2, r_max);
        if(num_cmp_offset(num_r, num_aux, cnt_r) < 0)
        {
            num_free(num_aux);
            num_aux = num_mul_uint(NULL, num_2, r_min);
            r_max = r_min;
        }
        node_r = num_sub_rec(num_r, node_r, num_aux->head);

        free(num_aux);
        r += r_max;
    }

    num_insert_head(num_q, r);
    num_div_mod_rec(num_q, num_r, node_r ? node_r->prev : num_r->tail, cnt_r - 1, num_2);
}

void num_div_mod(num_p *out_num_q, num_p *out_num_r, num_p num_1, num_p num_2)
{
    assert(num_1);
    assert(num_2);
    assert(num_2->count);

    num_p num_q = num_create(0, NULL, NULL);

    if(num_cmp(num_1, num_2) < 0)
    {
        *out_num_q = num_q;
        *out_num_r = num_1;
        num_free(num_2);
        return;
    }

    node_p node_r = num_1->tail;
    for(uint64_t i=1; i<num_2->count; i++)
        node_r = node_r->prev;

    num_div_mod_rec(num_q, num_1, node_r, num_1->count - num_2->count, num_2);
    num_normalize(num_q);

    *out_num_q = num_q;
    *out_num_r = num_1;
    num_free(num_2);
}

// num_p num_div(num_p num_1, num_p num_2)
// {
//     num_p num_q, num_r;
//     num_div_mod(&num_q, &num_r, num_1, num_2);
//     num_free(num_r);
//     return num_q;
// }

// num_p num_mod(num_p num_1, num_p num_2)
// {
//     num_p num_q, num_r;
//     num_div_mod(&num_q, &num_r, num_1, num_2);
//     num_free(num_q);
//     return num_r;
// }
