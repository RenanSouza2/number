#include <stdio.h>
#include <stdlib.h>

#include "debug.h"
#include "../../utils/assert.h"

#ifdef DEBUG

#include <stdarg.h>

#include "../../utils/clu/bin/header.h"

number_p number_create_variadic(uint64_t n, va_list args)
{
    number_p num = NULL;
    for(uint64_t i=0; i<n; i++)
    {
        uint64_t value = va_arg(args, uint64_t);
        num = number_create(value, num);
    }
    return num;
}

number_p number_create_immed(uint64_t n, ...)
{
    va_list args;
    va_start(args, n);
    return number_create_variadic(n, args);
}



bool number(number_p num_1, number_p num_2)
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

bool number_immed(number_p num, uint64_t n, ...)
{
    va_list args;
    va_start(args, n);
    number_p num_2 = number_create_variadic(n, args);
    bool res = number(num, num_2);
    number_free(num_2);
    return res;
}

#endif


void number_display(number_p num)
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



number_p number_create(uint64_t value, number_p next)
{
    number_p num = malloc(sizeof(number_t));
    assert(num);
    
    *num = (number_t){value, next};
    return num;
}

void number_free(number_p num)
{
    while(num)
    {
        number_p aux = num->next;
        free(num);
        num = aux;
    }
}



number_p number_add_uint(number_p num, uint64_t value)
{
    if(value == 0)
        return num;

    if(num == NULL)
        return number_create(value, NULL);

    num->value += value;
    if(num->value < value)
        num->next = number_add_uint(num->next, 1);

    return num;
}



number_p number_add(number_p num_1, number_p num_2)
{
    if(num_1 == NULL) return num_2;
    if(num_2 == NULL) return num_1;

    number_add_uint(num_1, num_2->value);
    
    number_p aux = num_2->next;
    free(num_2);
    number_add(num_1->next, aux);
    return num_1;
}
