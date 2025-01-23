#include <stdio.h>
#include <stdlib.h>

#include "debug.h"
#include "../../utils/assert.h"

#ifdef DEBUG

#include <stdarg.h>

#include "../../utils/clu/bin/header.h"

bool number_immed(number_p num, uint64_t n, ...)
{
    va_list args;
    va_start(args, n);

    uint64_t i;
    for(i=0; i<n && num; i++, num = num->next)
    {
        uint64_t value = va_arg(args, uint64_t);
        if(num->value == value)
            continue;

        printf("\n\tNUMBER ASSET ERROR | DIFFERENCE IN VALUE %ld | %ld %ld", i, num->value, value);
        return false;
    }

    if(num != NULL)
    {
        printf("\n\tNUMBER ASSET ERROR | NUMBER LONGER THAN EXPECTED");
        return false;
    }

    if(i < n)
    {
        printf("\n\tNUMBER ASSET ERROR | NUMBER SHORTER THAN EXPECTED");
        return false;
    }

    return true;
}

#endif

number_p number_create(uint64_t value)
{
    assert(value != 0);

    number_p num = malloc(sizeof(number_t));
    assert(num);
    
    *num = (number_t){value, NULL};
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
        return number_create(value);

    num->value += value;
    if(num->value < value)
        num->next = number_add_uint(num->next, 1);

    return num;
}
