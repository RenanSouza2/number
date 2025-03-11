#include <stdio.h>
#include <stdlib.h>

#include "debug.h"
#include "../../utils/assert.h"
#include "../../utils/clu/header.h"



#ifdef DEBUG

#include "../num/debug.h"

snum_t snum_create_variadic(uint64_t signal, uint64_t n, va_list *args)
{
    num_t num = num_create_variadic(n, args);
    return snum_create(signal, num);
}

snum_t snum_create_immed(uint64_t signal, uint64_t n, ...)
{
    va_list args;
    va_start(args, n);
    return snum_create_variadic(signal, n, &args);
}



bool snum_inner(snum_t snum_1, snum_t snum_2)
{
    if(!uint64(snum_1.signal, snum_2.signal))
    {
        printf("\n\tSIG ASSERT ERROR\t| DIFFERENT SIGNAL");
        return false;
    }

    if(!num_str(snum_1.num, snum_2.num))
    {
        printf("\n\tSIG ASSERT ERROR\t| DIFFERENT NUMBER");
        return false;
    }

    return true;
}

bool snum_str(snum_t snum_1, snum_t snum_2)
{
    if(!snum_inner(snum_1, snum_2))
    {
        printf("\n");
        snum_display_tag("\tsnum_1", snum_1);
        snum_display_tag("\tsnum_2", snum_2);
        return false;
    }

    return true;
}

bool snum_immed(snum_t snum, uint64_t signal, uint64_t count, ...)
{
    va_list args;
    va_start(args, count);
    snum_t snum_2 = snum_create_variadic(signal, count, &args);

    return snum_str(snum, snum_2);
}

#endif



void snum_display(snum_t snum)
{
    if(snum.signal == ZERO)
    {
        printf("  0");
        return;
    }

    printf("%c ", snum.signal & POSITIVE ? '+': '-');

    printf("[");
    num_display(snum.num);
    printf("]");
}

void snum_display_tag(char tag[], snum_t snum)
{
    printf("\n%s:\t", tag);
    snum_display(snum);
}



snum_t snum_create(uint64_t signal, num_t num)
{
    if(num_is_zero(num))
        signal = ZERO;
    else
        assert(signal != ZERO)

    return (snum_t)
    {
        .num = num,
        .signal = signal
    };
}



snum_t snum_wrap(int64_t value)
{
    if(value == 0)
    {
        num_t num = num_wrap(0);
        return snum_create(ZERO, num);
    }

    if(value < 0)
    {
        num_t num = num_wrap(-value);
        return snum_create(NEGATIVE, num);
    }

    num_t num = num_wrap(value);
    return snum_create(POSITIVE, num);
}

snum_t snum_wrap_str(char str[])
{
    uint64_t signal = str[0] == '-' ? NEGATIVE : POSITIVE;
    uint64_t offset = str[0] == '-' || str[0] == '+' ? 1 : 0;
    num_t num = num_wrap_str(&str[offset]);
    return snum_create(signal, num);
}

snum_t snum_copy(snum_t snum)
{
    num_t num = num_copy(snum.num);
    return snum_create(snum.signal, num);
}

void snum_free(snum_t snum)
{
    num_free(snum.num);
}



bool snum_is_zero(snum_t snum)
{
    return snum.signal == ZERO;
}

int64_t snum_cmp(snum_t snum_1, snum_t snum_2)
{
    if(snum_1.signal & POSITIVE)
    {
        if(snum_2.signal & POSITIVE)
            return num_cmp(snum_1.num, snum_2.num);

        return 1;
    }

    if(snum_2.signal & POSITIVE)
        return -1;

    return -num_cmp(snum_1.num, snum_2.num);
}



snum_t snum_shl(snum_t snum, uint64_t bits)
{
    snum.num = num_shl(snum.num, bits);
    return snum;
}

snum_t snum_shr(snum_t snum, uint64_t bits)
{
    snum.num = num_shr(snum.num, bits);
    if(num_is_zero(snum.num))
        snum.signal = ZERO;

    return snum;
}



snum_t snum_opposite(snum_t snum)
{
    snum.signal = snum.signal >> 1 | (snum.signal & POSITIVE) << 1;
    return snum;
}

snum_t snum_add(snum_t snum_1, snum_t snum_2)
{
    uint64_t signal_res = snum_1.signal & snum_2.signal;
    if(signal_res)
    {
        num_t num_res = num_add(snum_1.num, snum_2.num);
        return snum_create(signal_res, num_res);
    }

    if(num_cmp(snum_1.num, snum_2.num) > 0)
    {
        num_t num_res = num_sub(snum_1.num, snum_2.num);
        signal_res = snum_1.signal;
        return snum_create(signal_res, num_res);
    }

    num_t num_res = num_sub(snum_2.num, snum_1.num);
    signal_res = snum_2.signal;
    return snum_create(signal_res, num_res);
}

snum_t snum_sub(snum_t snum_1, snum_t snum_2)
{
    snum_2 = snum_opposite(snum_2);
    return snum_add(snum_1, snum_2);
}

snum_t snum_mul(snum_t snum_1, snum_t snum_2)
{
    uint64_t signal_res = snum_1.signal & snum_2.signal ?
        POSITIVE : NEGATIVE;

    num_t num_res = num_mul(snum_1.num, snum_2.num);
    return snum_create(signal_res, num_res);
}

snum_t snum_div(snum_t snum_1, snum_t snum_2)
{
    uint64_t signal_res = snum_1.signal & snum_2.signal ?
        POSITIVE : NEGATIVE;

    num_t num_res = num_div(snum_1.num, snum_2.num);
    return snum_create(signal_res, num_res);
}
