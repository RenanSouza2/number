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

        printf("\n\n\tNUMBER ASSET ERROR | DIFFERENCE IN VALUE %ld | %ld %ld", i, num_1->value, num_2->value);
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



void num_display(num_p num)
{
    printf("\n");

    if(num == NULL)
    {
        printf("0");
        return;
    }

    for(; num; num = num->next)
        printf("%ld ", num->value);
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

/* creates a num struct with value 0 if NUM is null */
num_p num_normalize(num_p num)
{
    if(num) return num;

    return num_create(0, NULL);
}

/* free NUM struct and return next */
num_p num_consume(num_p num)
{
    if(num == NULL)
        return NULL;

    num_p num_next = num->next;
    free(num);
    return num_next;
}

/* free NUM list */
void num_free(num_p num)
{
    while(num)
        num = num_consume(num);
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

num_p num_mult_uint_rec(num_p num_res, num_p num, uint64_t value)
{
    if(num == NULL)
        return num_res;

    uint128_t u = MUL(num->value, value);
    num_res = num_add_uint(num_res, LOW(u));
    num_res = num_normalize(num);
    num_res->next = num_add_uint(num_res->next, HIGH(u));
    num_res->next = num_mult_uint_rec(num_res->next, num->next, value);
    return num_res;
}

/* preserves NUM */
num_p num_mult_uint(num_p num, uint64_t value)
{
    return num_mult_uint_rec(NULL, num, value);
}



num_p num_add(num_p num_1, num_p num_2)
{
    if(num_1 == NULL) return num_2;
    if(num_2 == NULL) return num_1;

    num_add_uint(num_1, num_2->value);
    
    num_p num_next = num_consume(num_2);
    num_add(num_1->next, num_next);
    return num_1;
}

// num_p num_mul(num_p num_1, num_p num_2)
// {
//     if(num_1 == NULL)
//     {
//         num_free(num_2);
//         return NULL;
//     }

//     if(num_2 == NULL)
//     {
//         num_free(num_1);
//         return NULL;
//     }

//     num_p num_res = NULL;
//     for(num_p num_c = num_1; num_c; num_c = num_c->next)
//     {
//         if(num_c->value == 0)
//             continue;
        
//         num_res = num_add(num_res)
//     }
// }