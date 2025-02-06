#include <stdio.h>
#include <stdlib.h>

#include "debug.h"
#include "../../utils/assert.h"

#ifdef DEBUG

#include <stdarg.h>

#include "../../utils/clu/bin/header.h"

num_p num_create_variadic(uint64_t n, va_list args)
{
    num_p num = NULL;
    for(uint64_t i=0; i<n; i++)
    {
        uint64_t value = va_arg(args, uint64_t);
        num = num_create(value, num);
    }
    return num;
}

num_p num_create_immed(uint64_t n, ...)
{
    va_list args;
    va_start(args, n);
    return num_create_variadic(n, args);
}



bool num_str(num_p num_1, num_p num_2)
{
    for(uint64_t i=0; num_1 && num_2; i++, num_1 = num_1->next, num_2 = num_2->next)
    {
        if(num_1->value == num_2->value)
            continue;

        printf("\n\n\tNUMBER ASSET ERROR | DIFFERENCE IN VALUE %lu | %lx %lx", i, num_1->value, num_2->value);
        return false;
    }

    if(num_1 != NULL)
    {
        printf("\n\n\tNUMBER ASSET ERROR | NUMBER LONGER THAN EXPECTED");
        return false;
    }

    if(num_2 != NULL)
    {
        printf("\n\n\tNUMBER ASSET ERROR | NUMBER SHORTER THAN EXPECTED");
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

#endif



void num_display_rec(num_p num)
{
    if(num == NULL)
        return;

    num_display_rec(num->next);
    printf("%lx ", num->value);
}

void num_display(num_p num)
{
    if(num == NULL)
    {
        printf("0");
        return;
    }

    num_display_rec(num);
}

void num_display_immed(char *tag, num_p num)
{
    printf("\n%s: ", tag);num_display(num);
}



typedef __int128_t uint128_t;
#define U128(V) ((uint128_t)(V))
#define MUL(V1, V2) U128(V1) * U128(V2)
#define LOW(V) ((uint64_t)(V))
#define HIGH(V) LOW((V) >> 64)



num_p num_create(uint64_t value, num_p next)
{
    num_p num = malloc(sizeof(num_t));
    assert(num);
    
    *num = (num_t){value, next};
    return num;
}

num_p num_wrap(uint64_t value)
{
    return num_create(value, NULL);
}

/* free NUM struct and return next */
num_p num_consume(num_p num, bool keep)
{
    if(num == NULL)
        return NULL;

    num_p num_next = num->next;
    if(!keep) free(num);
    return num_next;
}

/* free NUM list */
void num_free(num_p num)
{
    while(num)
        num = num_consume(num, false);
}

/* creates a num struct with value 0 if NUM is null */
num_p num_denormalize(num_p num)
{
    if(num) return num;

    return num_create(0, NULL);
}

/* creates a num struct with value 0 if NUM is null */
num_p num_normalize(num_p num)
{
    if(num == NULL || num->value != 0 || num->next != NULL)
        return num;
        
    free(num);
    return NULL;
}

num_p num_copy(num_p num)
{
    if(num == NULL)
        return NULL;

    num_p num_2 = num_copy(num->next);
    return num_create(num->value, num_2);
}



num_p num_add_uint(num_p num, uint64_t value)
{
    if(value == 0)
        return num;

    if(num == NULL)
        return num_create(value, NULL);

    num->value += value;
    if(num->value < value)
        num->next = num_add_uint(num->next, 1);

    return num;
}

num_p num_sub_uint(num_p num, uint64_t value)
{
    if(value == 0)
        return num;

    assert(num);

    bool do_next = num->value < value;
    num->value -= value;
    if(do_next)
        num->next = num_sub_uint(num->next, 1);

    return num_normalize(num);
}

num_p num_mul_uint_rec(num_p num_res, num_p num, uint64_t value)
{
    if(num == NULL)
        return num_res;

    uint128_t u = MUL(num->value, value);
    num_res = num_add_uint(num_res, LOW(u));
    num_res = num_denormalize(num_res);
    num_res->next = num_add_uint(num_res->next, HIGH(u));
    num_res->next = num_mul_uint_rec(num_res->next, num->next, value);
    return num_res;
}

/* preserves NUM */
num_p num_mul_uint(num_p num_res, num_p num, uint64_t value)
{
    if(value == 0)
        return num_res;

    return num_mul_uint_rec(NULL, num, value);
}

num_p num_shl_rec(num_p num, bool carry)
{
    if(num == NULL)
        return carry ? num_create(1, NULL) : NULL;

    bool carry_next = num->value >> 63;
    num->value = num->value << 1 | carry;
    num->next = num_shl_rec(num->next, carry_next);
    return num;
}

num_p num_shl(num_p num)
{
    return num_shl_rec(num, false);
}

num_p num_shr(num_p num)
{
    if(num == NULL)
        return NULL;

    uint64_t carry = num->next ? num->next->value & 1 : 0;
    num->value = num->value >> 1 | carry << 63;
    num->next = num_shr(num->next);
    return num_normalize(num);
}



int64_t num_cmp(num_p num_1, num_p num_2)
{
    if(num_1 == NULL)
        return num_2 ? -1 : 0;

    if(num_2 == NULL)
        return 1;

    int64_t next = num_cmp(num_1->next, num_2->next);
    if(next != 0) return next;

    if(num_1->value > num_2->value) return 1;
    return num_1->value < num_2->value ? -1 : 0;
}



num_p num_add(num_p num_1, num_p num_2, bool keep)
{
    if(num_1 == NULL) 
        return keep ? num_copy(num_2) : num_2;

    if(num_2 == NULL) return num_1;

    num_add_uint(num_1, num_2->value);
    
    num_2 = num_consume(num_2, keep);
    num_1->next = num_add(num_1->next, num_2, keep);
    return num_1;
}

num_p num_sub(num_p num_1, num_p num_2, bool keep)
{
    if(num_2 == NULL) return num_1;
    assert(num_1);

    num_1 = num_sub_uint(num_1, num_2->value);
    
    num_1 = num_denormalize(num_1);
    num_2 = num_consume(num_2, keep);
    num_1->next = num_sub(num_1->next, num_2, keep);
    return num_normalize(num_1);
}

num_p num_mul_rec(num_p num_res, num_p num_1, num_p num_2)
{
    if(num_2 == NULL)
    {
        num_free(num_1);
        return num_res;
    }

    num_res = num_mul_uint_rec(num_res, num_1, num_2->value);

    num_res = num_denormalize(num_res);
    num_2 = num_consume(num_2, false);
    num_res->next = num_mul_rec(num_res->next, num_1, num_2);
    return num_res;
}

num_p num_mul(num_p num_1, num_p num_2)
{
    if(num_1 == NULL)
    {
        num_free(num_2);
        return NULL;
    }

    return num_mul_rec(NULL, num_1, num_2);
}

num_p num_div(num_p num_1, num_p num_2)
{
    assert(num_2);

    if(num_1 == NULL)
    {
        num_free(num_2);
        return NULL;
    }

    num_p num_base = num_create(1, NULL);
    while(num_cmp(num_2, num_1) <= 0)
    {
        num_2 = num_shl(num_2);
        num_base = num_shl(num_base);
    }

    num_p num_res = NULL;
    while(num_base)
    {
        num_2 = num_shr(num_2);
        num_base = num_shr(num_base);
        if(num_cmp(num_1, num_2) < 0)
            continue;

        num_res = num_add(num_res, num_base, true);
        num_1 = num_sub(num_1, num_2, true);
    }
    num_free(num_1);
    num_free(num_2);
    return num_res;
}
