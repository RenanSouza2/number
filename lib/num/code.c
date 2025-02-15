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

    node_p node = NULL;
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

    num_display("\n\tnum_1", num_1, false);
    num_display("\tnum_2", num_2, false);
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


void num_display(char *tag, num_p num, bool full)
{
    printf("\n%s: ", tag);
    
    if(num->count == 0)
    {
        printf("0 | 0");
        return;
    }

    printf("" U64P " | ", num->count);
    
    node_display_rec(num->tail, full ? num->count : 4);
    if(!full && num->count > 4) printf(" ...");
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
    if(node) return node;

    return node_create(0, NULL, NULL);
}

/* frees NODE if NODE->value is 0 and NODE->next is NULL */
node_p node_normalize(node_p node)
{
    if(node == NULL || node->value != 0 || node->next != NULL)
        return node;
    
    if(node->prev) node->prev->next = NULL;
    free(node);
    return NULL;
}

node2_t node_copy(node_p node)
{
    if(node == NULL)
        return (node2_t){NULL, NULL};

    node2_t n2 = node_copy(node->next);
    node_p node_2 = node_create(node->value, n2.node_1, NULL);
    return (node2_t){node_2, n2.node_2 ? n2.node_2 : node_2};
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

/* returns NULL if value is 0, num(VALUE) else */
num_p num_wrap(uint64_t value) // TODO test
{
    if(value == 0)
        return num_create(0, NULL, NULL);

    node_p node = node_create(value, NULL, NULL);
    return num_create(1, node,  node);
}

num_p num_copy(num_p num) //  TODO test
{
    node2_t n2 = node_copy(num->head);
    return num_create(num->count, n2.node_1, n2.node_2);
}



// num_p num_add_uint(num_p num, uint64_t value)
// {
//     if(value == 0)
//         return num;
//
//     if(num == NULL)
//         return num_create(value, NULL);
//
//     num->value += value;
//     if(num->value < value)
//         num->next = num_add_uint(num->next, 1);
//
//     return num;
// }

// num_p num_sub_uint(num_p num, uint64_t value)
// {
//     if(value == 0)
//         return num;
//
//     assert(num);
//
//     bool do_next = num->value < value;
//     num->value -= value;
//     if(do_next)
//         num->next = num_sub_uint(num->next, 1);
//
//     return num_normalize(num);
// }

// num_p num_mul_uint_rec(num_p num_res, num_p num, uint64_t value)
// {
//     if(num == NULL)
//         return num_res;
//
//     uint128_t u = MUL(num->value, value);
//     num_res = num_add_uint(num_res, LOW(u));
//     num_res = num_denormalize(num_res);
//     num_res->next = num_add_uint(num_res->next, HIGH(u));
//     num_res->next = num_mul_uint_rec(num_res->next, num->next, value);
//     return num_res;
// }

// /* preserves NUM */
// num_p num_mul_uint(num_p num_res, num_p num, uint64_t value)
// {
//     if(value == 0)
//         return num_res;
//
//     return num_mul_uint_rec(num_res, num, value);
// }

// num_p num_shl_rec(num_p num, bool carry)
// {
//     if(num == NULL)
//         return carry ? num_create(1, NULL) : NULL;
//
//     bool carry_next = num->value >> 63;
//     num->value = num->value << 1 | carry;
//     num->next = num_shl_rec(num->next, carry_next);
//     return num;
// }

// num_p num_shl(num_p num)
// {
//     return num_shl_rec(num, false);
// }

// num_p num_shr(num_p num)
// {
//     if(num == NULL)
//         return NULL;
//
//     uint64_t carry = num->next ? num->next->value & 1 : 0;
//     num->value = num->value >> 1 | carry << 63;
//     num->next = num_shr(num->next);
//     return num_normalize(num);
// }



// int64_t num_cmp(num_p num_1, num_p num_2)
// {
//     if(num_1 == NULL)
//         return num_2 ? -1 : 0;
//
//     if(num_2 == NULL)
//         return 1;
//
//     int64_t next = num_cmp(num_1->next, num_2->next);
//     if(next != 0) return next;
//
//     if(num_1->value > num_2->value) return 1;
//     return num_1->value < num_2->value ? -1 : 0;
// }



// num_p num_add(num_p num_1, num_p num_2)
// {
//     if(num_1 == NULL) return num_2;
//     if(num_2 == NULL) return num_1;
//
//     num_add_uint(num_1, num_2->value);
//
//     num_2 = num_consume(num_2);
//     num_1->next = num_add(num_1->next, num_2);
//     return num_1;
// }

// num_p num_sub(num_p num_1, num_p num_2)
// {
//     if(num_2 == NULL) return num_1;
//     assert(num_1);
//
//     num_1 = num_sub_uint(num_1, num_2->value);
//
//     num_1 = num_denormalize(num_1);
//     num_2 = num_consume(num_2);
//     num_1->next = num_sub(num_1->next, num_2);
//     return num_normalize(num_1);
// }

// num_p num_mul_rec(num_p num_res, num_p num_1, num_p num_2)
// {
//     if(num_2 == NULL)
//     {
//         num_free(num_1);
//         return num_res;
//     }
//
//     num_res = num_mul_uint(num_res, num_1, num_2->value);
//
//     num_res = num_denormalize(num_res);
//     num_2 = num_consume(num_2);
//     num_res->next = num_mul_rec(num_res->next, num_1, num_2);
//     return num_res;
// }

// num_p num_mul(num_p num_1, num_p num_2)
// {
//     if(num_1 == NULL)
//     {
//         num_free(num_2);
//         return NULL;
//     }
//
//     return num_mul_rec(NULL, num_1, num_2);
// }

// num_p num_div_mod_rec(
//     num_p *out_num_q, 
//     num_p num_1,
//     num_p num_2, 
//     uint64_t cnt_1,
//     uint64_t cnt_2, 
//     uint64_t val_2
// )
// {
//     if(cnt_1 <= cnt_2+1)
//     if(num_cmp(num_1, num_2) < 0)
//     {
//         *out_num_q = NULL;
//         return num_1;
//     }
//
//     num_p num_q;
//     num_1->next = num_div_mod_rec(&num_q, num_1->next, num_2, cnt_1-1, cnt_2, val_2);
//     num_1 = num_normalize(num_1);
//
//     uint64_t r =  0;
//     while(num_cmp(num_1, num_2) >= 0)
//     {
//         cnt_1 = num_count(num_1);
//         uint64_t r_max, r_min;
//         if(cnt_1 > cnt_2)
//         {
//             uint128_t val_1 = num_get_last_2(num_1);
//             r_max = val_1 / val_2;
//             r_min = val_1 / (U128(val_2) + 1);
//         }
//         else
//         {
//             uint64_t val_1 = num_get_last(num_1);
//             r_max = val_1 / val_2;
//             r_min = val_1 / (U128(val_2) + 1);
//         }
//
//         num_p num_aux = num_mul_uint(NULL, num_2, r_max);
//         if(num_cmp(num_aux, num_1) > 0)
//         {
//             num_free(num_aux);
//             num_aux = num_mul_uint(NULL, num_2, r_min);
//             r_max = r_min;
//         }
//         num_1 = num_sub(num_1, num_aux);
//         r += r_max;
//     }
//
//     *out_num_q = num_create(r, num_q);
//     return num_1;
// }
//
// void num_div_mod(num_p *out_num_q, num_p *out_num_r, num_p num_1, num_p num_2)
// {
//     assert(num_2);
//
//     uint64_t cnt_1 = num_count(num_1);
//     uint64_t cnt_2 = num_count(num_2);
//     uint64_t val_2 = num_get_last(num_2);
//
//     num_p num_q;
//     num_p num_r = num_div_mod_rec(&num_q, num_1, num_2, cnt_1, cnt_2, val_2);
//     num_free(num_2);
//
//     *out_num_q = num_q;
//     *out_num_r = num_r;
// }

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
