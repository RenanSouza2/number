#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "debug.h"
#include "../../utils/assert.h"
#include "../../utils/U64.h"
#include "../../utils/clu/header.h"



#ifdef DEBUG

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
        printf("\n\n\tUINT64 ASSERT ERROR\t| (" U64PX ") (" U64PX ")", u1, u2);
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
        printf("\n\tNUMBER VALIDITY ERROR\t| COUNT IS " U64P() " BUT IT HAS NO HEAD", num_1->count);
        return false;
    }

    if(num_1->head->prev != NULL)
    {
        printf("\n\tNUMBER VALIDITY ERROR\t| HEAD HAS PREV");
        return false;
    }

    if(num_1->tail == NULL)
    {
        printf("\n\tNUMBER VALIDITY ERROR\t| COUNT IS " U64P() " BUT IT HAS NO TAIL", num_1->count);
        return false;
    }

    if(num_1->tail->next != NULL)
    {
        printf("\n\tNUMBER VALIDITY ERROR\t| TAIL HAS NEXT");
        return false;
    }

    node_p node_1 = num_1->tail;
    node_p node_2 = num_2->tail;
    for(uint64_t count = num_1->count; count > 0; count--)
    {
        if(count > 1)
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
            if(node_1 != num_1->head)
            {
                printf("\n\n\tNODE VALIDITY ERROR\t| INVALID HEAD");
                return false;
            }
        }

        if(!uint64(node_1->value, node_2->value))
        {
            printf("\n\tNUMBER ASSERT ERROR\t| DIFFERENCE IN VALUE " U64P() "", count - 1);
            return false;
        }

        node_1 = node_1->prev;
        node_2 = node_2->prev;
    }

    if(node_1 != NULL)
    {
        printf("\n\tNODE VALIDITY ERROR\t| NUMBER LONGER THAN EXPECTED");
        return false;
    }

    return true;
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
    num_free(num);
    return res;
}



node_p num_get_node(num_p num, uint64_t count)
{
    node_p node = num->head;
    for(uint64_t i=0; i<count && node; i++, node = node->next);
    return node;
}

#endif



#define COALESCE(PTR_A, PTR_B) (PTR_A) ? PTR_A : PTR_B;



uint64_t uint_from_char(char c)
{
    switch (c)
    {
        case '0' ... '9': return c - '0';
        case 'a' ... 'f': return c - 'a' + 10;
        case 'A' ... 'F': return c - 'A' + 10;
    }

    assert(false);
}



void num_display_opts(num_p num, bool length, bool full)
{
    DBG_CHECK_PTR(num);

    if(length)
    {
        if(num->count == 0)
        {
            printf("(0)\t");
        }
        else
        {
            printf("(" U64P() ")\t| ", num->count);
        }
    }
    
    if(num->count == 0)
    {
        printf("0");
        return;
    }

    uint64_t max = full ? UINT64_MAX : 4;
    node_p node = num->tail;
    for(uint64_t i=0; i<max && node != NULL; i++, node = node->prev)
        printf("" U64PX " ", node->value);

    if(!full && num->count > 4)
        printf("...");
}

void num_display(num_p num)
{
    num_display_opts(num, true, false);
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
    num_display_opts(num, true, true);
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

node_p node_consume(node_p node)
{
    DBG_CHECK_PTR(node);

    if(node == NULL)
        return NULL;

    node_p node_next = node->next;
    free(node);

    if(node_next) node_next->prev = NULL;
    return node_next;
}

/* free NODE list */
void node_free(node_p node)
{
    DBG_CHECK_PTR(node);

    while(node)
        node = node_consume(node);
}



num_p num_create(uint64_t count, node_p head, node_p tail)
{
    DBG_CHECK_PTR(head);
    DBG_CHECK_PTR(tail);

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

node_p num_insert(num_p num, uint64_t value)
{
    DBG_CHECK_PTR(num);

    num->tail = node_create(value, NULL, num->tail);

    num->count++;
    num->head = COALESCE(num->head, num->tail);

    return num->tail;
}

node_p num_insert_head(num_p num, uint64_t value)
{
    DBG_CHECK_PTR(num);

    num->head = node_create(value, num->head, NULL);

    num->count++;
    num->tail = COALESCE(num->tail, num->head);

    return num->head;
}

void num_remove_head(num_p num)
{
    DBG_CHECK_PTR(num);

    if(num->count == 0)
        return;

    node_p node_head = num->head;
    num->head = node_head->next;
    free(node_head);

    num->count--;
    if(num->count == 0)
        num->tail = NULL;
    else
        num->head->prev = NULL;
}

void num_insert_list(num_p num, node_p head, node_p tail, uint64_t cnt)
{
    DBG_CHECK_PTR(num);
    DBG_CHECK_PTR(head);
    DBG_CHECK_PTR(tail);

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

    num->tail->next = head;
    head->prev = num->tail;

    num->count += cnt;
    num->tail = tail;
}

node_p num_denormalize(num_p num, node_p node)
{
    DBG_CHECK_PTR(num);
    DBG_CHECK_PTR(node);

    return COALESCE(node, num_insert(num, 0));
}

/* removes last element if zero, returns TRUE if so */
bool num_normalize(num_p num)
{
    DBG_CHECK_PTR(num);

    if(num->count == 0)
        return false;

    node_p node_tail = num->tail;
    if(node_tail->value != 0)
        return false;

    num->tail = node_tail->prev;
    free(node_tail);

    num->count--;
    if(num->count == 0)
        num->head = NULL;
    else
        num->tail->next = NULL;

    return true;
}



/* returns empty list if value is 0, list with one value otherwise */
num_p num_wrap(uint64_t value)
{
    if(value == 0)
        return num_create(0, NULL, NULL);

    node_p node = node_create(value, NULL, NULL);
    return num_create(1, node,  node);
}

num_p num_wrap_dec(char str[])
{
    uint64_t len = strlen(str);
    num_p num = num_create(0, NULL, NULL);
    for(uint64_t i=0; i<len; i++)
    {
        uint64_t d = uint_from_char(str[i]);
        assert(d < 10);

        num_p num_aux = num_mul_uint(num_wrap(d), num, 10);

        num_free(num);
        num = num_aux;
    }
    return num;
}

num_p num_wrap_hex(char str[])
{
    assert(str[0] == '0');
    assert(str[1] == 'x');

    uint64_t len = strlen(str);
    num_p num = num_create(0, NULL, NULL);
    for(uint64_t i=2; i<len; i++)
    {
        uint64_t d = uint_from_char(str[i]);
        num_p num_aux = num_mul_uint(num_wrap(d), num, 16);

        num_free(num);
        num = num_aux;
    }
    return num;
}

num_p num_wrap_str(char str[])
{
    return str[0] == '0' && str[1] == 'x' ?
        num_wrap_hex(str) : num_wrap_dec(str);
}

num_p num_copy(num_p num)
{
    DBG_CHECK_PTR(num);

    num_p num_res = num_create(0, NULL, NULL);
    for(node_p node = num->head; node; node = node->next)
        num_insert(num_res, node->value);

    return num_res;
}

void num_free(num_p num)
{
    DBG_CHECK_PTR(num);

    node_free(num->head);
    free(num);
}



node_p num_add_uint_offset(num_p num, node_p node, uint64_t value)
{
    DBG_CHECK_PTR(num);
    DBG_CHECK_PTR(node);

    node_p node_0 = node;
    for(;value && node; node = node->next)
    {
        node->value += value;
        value = node->value < value;
    }

    if(value)
        num_insert(num, value);

    return node_0 ? node_0 : value ? num->tail : NULL;
}

void num_add_uint(num_p num, uint64_t value)
{
    DBG_CHECK_PTR(num);

    num_add_uint_offset(num, num->head, value);
}



/* returns TRUE if passed offset is TAIL and ELIMINATED */
bool num_sub_uint_offset(num_p num, node_p node, uint64_t value)
{
    DBG_CHECK_PTR(num);
    DBG_CHECK_PTR(node);

    bool is_tail = node == num->tail;

    for(; value; node = node->next)
    {
        assert(node);

        uint64_t next = node->value < value;
        node->value -= value;
        value = next;
    }

    return num_normalize(num) && is_tail;
}

void num_sub_uint(num_p num, uint64_t value)
{
    DBG_CHECK_PTR(num);

    num_sub_uint_offset(num, num->head, value);
}

/* num_res CANNOT be NULL */
node_p num_mul_uint_offset(num_p num_res, node_p node_res, node_p node_2, uint64_t value)
{
    DBG_CHECK_PTR(num_res);
    DBG_CHECK_PTR(node_res);
    DBG_CHECK_PTR(node_2);

    if(node_2 == NULL)
        return node_res;

    node_p node_0 = node_res = num_denormalize(num_res, node_res);
    for(; node_2; )
    {
        uint128_t u = MUL(node_2->value, value);
        node_res = num_add_uint_offset(num_res, node_res, LOW(u));
        node_res = num_denormalize(num_res, node_res);
        num_add_uint_offset(num_res, node_res->next, HIGH(u));

        node_res = node_res->next;
        node_2 = node_2->next;
    }

    return node_0;
}

/* NUM_RES can be NULL, preserves NUM */
num_p num_mul_uint(num_p num_res, num_p num, uint64_t value)
{
    DBG_CHECK_PTR(num_res);
    DBG_CHECK_PTR(num);

    if(num_res == NULL)
        num_res = num_create(0, NULL, NULL);

    if(value)
        num_mul_uint_offset(num_res, num_res->head, num->head, value);

    return num_res;
}

num_p num_shl_uint(num_p num, uint64_t bits) // TODO test
{
    DBG_CHECK_PTR(num);

    uint64_t carry = 0;
    for(node_p node = num->head; node; node = node->next)
    {
        uint64_t value = node->value;
        node->value = (value << bits) | carry;
        carry = value >> (64 - bits);
    }

    if(carry)
        num_insert(num, carry);

    return num;
}

num_p num_shr_uint(num_p num, uint64_t bits) // TODO test
{
    DBG_CHECK_PTR(num);

    uint64_t carry = 0;
    for(node_p node = num->tail; node; node = node->prev)
    {
        uint64_t value = node->value;
        node->value = (value >> bits) | carry;
        carry = value << (64 - bits);
    }
    num_normalize(num);

    return num;
}



bool num_is_zero(num_p num) // TODO test
{
    DBG_CHECK_PTR(num);

    return num->count == 0;
}

int64_t num_cmp_offset(num_p num_1, num_p num_2, uint64_t offset)
{
    DBG_CHECK_PTR(num_1);
    DBG_CHECK_PTR(num_2);

    if(num_1->count > num_2->count + offset)
        return 1;

    if(num_1->count < num_2->count + offset)
        return -1;

    node_p node_1 = num_1->tail;
    node_p node_2 = num_2->tail;
    while(node_2)
    {
        if(node_1->value > node_2->value)
            return 1;

        if(node_1->value < node_2->value)
            return -1;

        node_1 = node_1->prev;
        node_2 = node_2->prev;
    }

    return 0;
}

int64_t num_cmp(num_p num_1, num_p num_2)
{
    DBG_CHECK_PTR(num_1);
    DBG_CHECK_PTR(num_2);

    return num_cmp_offset(num_1, num_2, 0);
}



num_p num_add(num_p num_1, num_p num_2)
{
    DBG_CHECK_PTR(num_1);
    DBG_CHECK_PTR(num_2)

    node_p node_1 = num_1->head;
    node_p node_2 = num_2->head;
    uint64_t count = num_2->count;
    for(; node_1 && node_2; count--)
    {
        num_add_uint_offset(num_1, node_1, node_2->value);

        node_1 = node_1->next;
        node_2 = node_consume(node_2);
    }

    if(node_1 == NULL && node_2)
        num_insert_list(num_1, node_2, num_2->tail, count);

    free(num_2);
    return num_1;
}

/* may leave num denormalized */
node_p num_sub_offset(num_p num_1, node_p node_1, node_p node_2) // TODO test
{
    DBG_CHECK_PTR(num_1);
    DBG_CHECK_PTR(node_1);
    DBG_CHECK_PTR(node_2);

    if(node_2 == NULL)
        return node_1;

    node_p node_0 = node_1;
    node_p prev_0 = node_0->prev;

    for(; node_2; )
    {
        if(num_sub_uint_offset(num_1, node_1, node_2->value))
        {
            assert(node_2->next == NULL);
            free(node_2);

            while(num_1->tail != prev_0)
                if(!num_normalize(num_1))
                    break;

            return num_1->tail == prev_0 ? NULL : node_0;
        }

        node_1 = node_1->next;
        node_2 = node_consume(node_2);
    }

    return node_0;
}

num_p num_sub(num_p num_1, num_p num_2)
{
    DBG_CHECK_PTR(num_1);
    DBG_CHECK_PTR(num_2);

    num_sub_offset(num_1, num_1->head, num_2->head);

    free(num_2);
    return num_1;
}

num_p num_mul(num_p num_1, num_p num_2)
{
    DBG_CHECK_PTR(num_1);
    DBG_CHECK_PTR(num_2);

    if(num_1->count == 0)
    {
        num_free(num_1);
        num_free(num_2);
        return num_create(0, NULL, NULL);
    }

    num_p num_res = num_create(0, NULL, NULL);
    node_p node_1 = num_1->head;
    node_p node_2 = num_2->head;

    // printf(" | count: %lu |", num_2->count/1000);
    uint64_t i=0;
    for(node_p node_res = NULL; node_2; node_res = node_res->next, i++)
    {
        // if(i%1000 == 0) printf("\t%lu", i/1000);

        node_res = num_mul_uint_offset(num_res, node_res, node_1, node_2->value);
        node_res = num_denormalize(num_res, node_res);

        node_2 = node_consume(node_2);
    }

    num_free(num_1);
    free(num_2);
    return num_res;
}

/*
* returns NUM_B * R if less then num_a, returns NULL otherwise
*  keeps NUM_a ans NUM_B
*/
num_p num_cmp_mul_uint(num_p num_a, num_p num_b, uint64_t r, uint64_t offset)
{
    num_p num_aux = num_create(0, NULL, NULL);
    node_p node = num_b->tail;
    for(offset += num_b->count-1; node; node = node->prev, offset--)
    {
        uint128_t u = MUL(node->value, r);
        num_add_uint(num_aux, HIGH(u));
        num_insert_head(num_aux, LOW(u));

        if(num_cmp_offset(num_a, num_aux, offset) < 0)
        {
            num_free(num_aux);
            return NULL;
        }
    }

    return num_aux;
}

void num_div_mod_loop(
    num_p num_q,
    num_p num_r,
    node_p node_r,
    uint64_t offset_r,
    num_p num_2
)
{
    DBG_CHECK_PTR(num_q);
    DBG_CHECK_PTR(num_r);
    DBG_CHECK_PTR(node_r);
    DBG_CHECK_PTR(num_2);

    for(; node_r;)
    {
        if(num_normalize(num_r))
        {
            num_insert_head(num_q, 0);
            node_r = num_r->tail;
            offset_r--;
            continue;
        }

        uint64_t r =  0;
        while(num_cmp_offset(num_r, num_2, offset_r) >= 0)
        {
            bool bool_1 = num_r->count > num_2->count + offset_r;
            bool bool_2 = num_2->count == 1 || num_r->tail->value == UINT64_MAX;

            uint128_t val_1 = bool_1 || !bool_2 ?
                U128_IMMED(num_r->tail->value, num_r->tail->prev->value) :
                num_r->tail->value;

            uint128_t val_2 = bool_1 || bool_2 ?
                num_2->tail->value :
                U128_IMMED(num_2->tail->value, num_2->tail->prev->value);

            uint128_t aux = val_1 / val_2;
            uint64_t r_max = aux > UINT64_MAX ? UINT64_MAX : aux;
            uint64_t r_min = val_1 / (val_2 + 1);

            num_p num_aux = num_cmp_mul_uint(num_r, num_2, r_max, offset_r);
            if(num_aux == NULL)
            {
                num_aux = num_mul_uint(NULL, num_2, r_min);
                r_max = r_min;
            }

            node_r = num_sub_offset(num_r, node_r, num_aux->head);

            r += r_max;
            free(num_aux);
        }

        num_insert_head(num_q, r);
        node_r = node_r ? node_r->prev : num_r->tail;
        offset_r--;
    }
}

uint64_t num_div_mod_inner(num_p *out_num_q, num_p *out_num_r, num_p num_1, num_p num_2)
{
    DBG_CHECK_PTR(num_1);
    DBG_CHECK_PTR(num_2);

    assert(num_2->count);

    num_p num_q = num_create(0, NULL, NULL);

    if(num_cmp(num_1, num_2) < 0)
    {
        *out_num_q = num_q;
        *out_num_r = num_1;
        num_free(num_2);
        return 0;
    }

    if(num_2->count == 1)
    {
        num_div_mod_loop(
            num_q,
            num_1,
            num_1->tail,
            num_1->count - num_2->count,
            num_2
        );

        *out_num_q = num_q;
        *out_num_r = num_1;
        num_free(num_2);
        return 0;
    }

    uint64_t bits = 0;
    for(uint64_t l = num_2->tail->value; l >> 63 == 0; l <<= 1)
        bits++;

    num_1 = num_shl_uint(num_1, bits);
    num_2 = num_shl_uint(num_2, bits);

    node_p node_r = num_1->tail;
    for(uint64_t i=1; i<num_2->count; i++)
        node_r = node_r->prev;

    num_div_mod_loop(
        num_q,
        num_1,
        node_r,
        num_1->count - num_2->count,
        num_2
    );
    num_free(num_2);

    *out_num_q = num_q;
    *out_num_r = num_1;
    return bits;
}

void num_div_mod(num_p *out_num_q, num_p *out_num_r, num_p num_1, num_p num_2)
{
    uint64_t bits = num_div_mod_inner(out_num_q, out_num_r, num_1, num_2);
    num_normalize(*out_num_q);
    num_shr_uint(*out_num_r, bits);
}

num_p num_div(num_p num_1, num_p num_2)
{
    DBG_CHECK_PTR(num_1);
    DBG_CHECK_PTR(num_2);

    num_p num_q, num_r;
    num_div_mod_inner(&num_q, &num_r, num_1, num_2);
    num_normalize(num_q);
    num_free(num_r);
    return num_q;
}

num_p num_mod(num_p num_1, num_p num_2)
{
    DBG_CHECK_PTR(num_1);
    DBG_CHECK_PTR(num_2);

    num_p num_q, num_r;
    uint64_t bits = num_div_mod_inner(&num_q, &num_r, num_1, num_2);
    num_shr_uint(num_r, bits);
    num_free(num_q);
    return num_r;
}

num_p num_shl(num_p num, uint64_t bits) // TODO test
{
    for(; bits > 63; bits -= 64)
        num_insert_head(num, 0);

    if(bits)
        num_shl_uint(num, bits);

    return num;
}

num_p num_shr(num_p num, uint64_t bits) // TODO test
{
    for(; bits > 63 && num->count; bits -= 64)
        num_remove_head(num);

    if(bits && num->count)
        num_shr_uint(num, bits);

    return num;
}
