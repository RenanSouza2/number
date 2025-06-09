#include <stdio.h>
#include <stdlib.h>

#include "debug.h"
#include "../../mods/clu/header.h"
#include "../../mods/macros/assert.h"

#include "../num/header.h"



#ifdef DEBUG

#include "../num/debug.h"

sig_num_t sig_num_create_variadic(uint64_t signal, uint64_t n, va_list *args)
{
    num_t num = num_create_variadic(n, args);
    return sig_num_create(signal, num);
}

sig_num_t sig_num_create_immed(uint64_t signal, uint64_t n, ...)
{
    va_list args;
    va_start(args, n);
    return sig_num_create_variadic(signal, n, &args);
}



bool sig_num_inner(sig_num_t sig_1, sig_num_t sig_2)
{
    if(!uint64(sig_1.signal, sig_2.signal))
    {
        printf("\n\tSIG ASSERT ERROR\t| DIFFERENT SIGNAL");
        return false;
    }

    if(!num_eq_dbg(sig_1.num, sig_2.num))
    {
        printf("\n\tSIG ASSERT ERROR\t| DIFFERENT NUMBER");
        return false;
    }

    return true;
}

bool sig_num_eq_dbg(sig_num_t sig_1, sig_num_t sig_2)
{
    if(!sig_num_inner(sig_1, sig_2))
    {
        printf("\n");
        sig_num_display_tag("\tsig_1", sig_1);
        sig_num_display_tag("\tsig_2", sig_2);
        return false;
    }

    return true;
}

bool sig_num_immed(sig_num_t sig, uint64_t signal, uint64_t n, ...)
{
    va_list args;
    va_start(args, n);
    sig_num_t sig_2 = sig_num_create_variadic(signal, n, &args);

    return sig_num_eq_dbg(sig, sig_2);
}

#endif



void sig_num_display(sig_num_t sig, bool full)
{
    if(sig.signal == ZERO)
    {
        printf("  0");
        return;
    }

    printf("%c ", sig.signal & POSITIVE ? '+': '-');

    printf("[");
    num_display_opts(sig.num, NULL, true, full);
    printf("]");
}

void sig_num_display_tag(char tag[], sig_num_t sig)
{
    printf("\n%s:\t", tag);
    sig_num_display(sig, false);
}

void sig_num_display_full(char tag[], sig_num_t sig)
{
    printf("\n%s:\t", tag);
    sig_num_display(sig, true);
}



sig_num_t sig_num_create(uint64_t signal, num_t num)
{
    if(num_is_zero(num))
        signal = ZERO;
    else
        assert(signal != ZERO)

    return (sig_num_t)
    {
        .num = num,
        .signal = signal
    };
}

void sig_num_free(sig_num_t sig)
{
    num_free(sig.num);
}



sig_num_t sig_num_wrap(int64_t value)
{
    if(value == 0)
    {
        num_t num = num_wrap(0);
        return sig_num_create(ZERO, num);
    }

    if(value < 0)
    {
        num_t num = num_wrap(-value);
        return sig_num_create(NEGATIVE, num);
    }

    num_t num = num_wrap(value);
    return sig_num_create(POSITIVE, num);
}

sig_num_t sig_num_wrap_str(char str[])
{
    uint64_t signal = str[0] == '-' ? NEGATIVE : POSITIVE;
    uint64_t offset = str[0] == '-' || str[0] == '+' ? 1 : 0;
    num_t num = num_wrap_str(&str[offset]);
    return sig_num_create(signal, num);
}

sig_num_t sig_num_copy(sig_num_t sig)
{
    num_t num = num_copy(sig.num);
    return sig_num_create(sig.signal, num);
}

sig_num_t sig_num_head_grow(sig_num_t sig, uint64_t count) // TODO test
{
    sig.num = num_head_grow(sig.num, count);
    return sig;
}
sig_num_t sig_num_head_trim(sig_num_t sig, uint64_t count) // TODO test
{
    sig.num = num_head_trim(sig.num, count);
    return sig;
}



bool sig_num_is_zero(sig_num_t sig)
{
    return sig.signal == ZERO;
}

int64_t sig_num_cmp(sig_num_t sig_1, sig_num_t sig_2)
{
    if(sig_1.signal & POSITIVE)
    {
        if(sig_2.signal & POSITIVE)
            return num_cmp(sig_1.num, sig_2.num);

        return 1;
    }

    if(sig_2.signal & POSITIVE)
        return -1;

    return -num_cmp(sig_1.num, sig_2.num);
}



sig_num_t sig_num_shl(sig_num_t sig, uint64_t bits)
{
    sig.num = num_shl(sig.num, bits);
    return sig;
}

sig_num_t sig_num_shr(sig_num_t sig, uint64_t bits)
{
    num_t num = num_shr(sig.num, bits);
    return sig_num_create(sig.signal, num);
}



sig_num_t sig_num_opposite(sig_num_t sig)
{
    sig.signal = sig.signal >> 1 | (sig.signal & POSITIVE) << 1;
    return sig;
}

sig_num_t sig_num_add(sig_num_t sig_1, sig_num_t sig_2)
{
    uint64_t signal_res = sig_1.signal & sig_2.signal;
    if(signal_res)
    {
        num_t num_res = num_add(sig_1.num, sig_2.num);
        return sig_num_create(signal_res, num_res);
    }

    if(num_cmp(sig_1.num, sig_2.num) > 0)
    {
        num_t num_res = num_sub(sig_1.num, sig_2.num);
        signal_res = sig_1.signal;
        return sig_num_create(signal_res, num_res);
    }

    num_t num_res = num_sub(sig_2.num, sig_1.num);
    signal_res = sig_2.signal;
    return sig_num_create(signal_res, num_res);
}

sig_num_t sig_num_sub(sig_num_t sig_1, sig_num_t sig_2)
{
    sig_2 = sig_num_opposite(sig_2);
    return sig_num_add(sig_1, sig_2);
}

sig_num_t sig_num_mul(sig_num_t sig_1, sig_num_t sig_2)
{
    uint64_t signal_res = sig_1.signal & sig_2.signal ?
        POSITIVE : NEGATIVE;

    num_t num_res = num_mul(sig_1.num, sig_2.num);
    return sig_num_create(signal_res, num_res);
}

sig_num_t sig_num_sqr(sig_num_t sig) // TODO test
{
    num_t num = num_sqr(sig.num);
    return sig_num_create(POSITIVE, num);
}

sig_num_t sig_num_div(sig_num_t sig_1, sig_num_t sig_2)
{
    uint64_t signal_res = sig_1.signal & sig_2.signal ?
        POSITIVE : NEGATIVE;

    num_t num_res = num_div(sig_1.num, sig_2.num);
    return sig_num_create(signal_res, num_res);
}
