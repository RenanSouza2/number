#include <stdio.h>
#include <stdlib.h>

#include "debug.h"
#include "../../mods/clu/header.h"
#include "../../mods/macros/assert.h"

#include "../num/header.h"

#define CLU_SIG_IS_SAFE(SIG) CLU_HANDLER_IS_SAFE((SIG).num);



#ifdef DEBUG

#include "../num/debug.h"

sig_num_t sig_num_create_variadic(uint64_t signal, uint64_t n, va_list *args)
{
    num_p num = num_create_variadic(n, args);
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
    CLU_SIG_IS_SAFE(sig_1);
    CLU_SIG_IS_SAFE(sig_2);

    if(!uint64(sig_1.signal, sig_2.signal))
    {
        printf("\n\tSIG ASSERT ERROR\t| DIFFERENT SIGNAL");
        return false;
    }

    if(!num_inner(sig_1.num, sig_2.num))
    {
        printf("\n\tSIG ASSERT ERROR\t| DIFFERENT NUMBER");
        return false;
    }

    return true;
}

bool sig_num_eq_dbg(sig_num_t sig_1, sig_num_t sig_2)
{
    CLU_SIG_IS_SAFE(sig_1);
    CLU_SIG_IS_SAFE(sig_2);

    if(!sig_num_inner(sig_1, sig_2))
    {
        printf("\n");
        sig_num_display_tag("\tsig_1", sig_1);
        sig_num_display_tag("\tsig_2", sig_2);
        return false;
    }

    sig_num_free(sig_1);
    sig_num_free(sig_2);
    return true;
}

bool sig_num_immed(sig_num_t sig, uint64_t signal, uint64_t n, ...)
{
    CLU_SIG_IS_SAFE(sig);

    va_list args;
    va_start(args, n);
    sig_num_t sig_2 = sig_num_create_variadic(signal, n, &args);

    return sig_num_eq_dbg(sig, sig_2);
}

#endif



void sig_num_display(sig_num_t sig, bool full)
{
    CLU_SIG_IS_SAFE(sig);
    assert(sig.num);

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
    CLU_SIG_IS_SAFE(sig);
    assert(sig.num);

    printf("\n%s:\t", tag);
    sig_num_display(sig, false);
}

void sig_num_display_full(char tag[], sig_num_t sig)
{
    CLU_SIG_IS_SAFE(sig);
    assert(sig.num);

    printf("\n%s:\t", tag);
    sig_num_display(sig, true);
}

void sig_num_display_dec(sig_num_t sig)
{
    CLU_SIG_IS_SAFE(sig);
    assert(sig.num);

    if(sig.signal == ZERO)
    {
        printf("  0");
        return;
    }

    printf("%c ", sig.signal & POSITIVE ? '+': '-');
    num_display_dec(sig.num);
}



sig_num_t sig_num_create(uint64_t signal, num_p num)
{
    CLU_HANDLER_IS_SAFE(num);
    assert(num);

    if(num_is_zero(num))
    {
        signal = ZERO;
    }
    else
    {
        assert(signal != ZERO)
    }

    return (sig_num_t)
    {
        .signal = signal,
        .num = num
    };
}

void sig_num_free(sig_num_t sig)
{
    CLU_SIG_IS_SAFE(sig);

    num_free(sig.num);
}



sig_num_t sig_num_wrap(int64_t value)
{
    if(value < 0)
    {
        num_p num = num_wrap(-value);
        return sig_num_create(NEGATIVE, num);
    }

    num_p num = num_wrap(value);
    return sig_num_create(POSITIVE, num);
}

sig_num_t sig_num_wrap_num(num_p num)   // TODO TEST
{
    return sig_num_create(POSITIVE, num);
}

sig_num_t sig_num_wrap_str(char str[])
{
    uint64_t signal = str[0] == '-' ? NEGATIVE : POSITIVE;
    uint64_t offset = str[0] == '-' || str[0] == '+' ? 1 : 0;
    num_p num = num_wrap_str(&str[offset]);
    return sig_num_create(signal, num);
}

sig_num_t sig_num_copy(sig_num_t sig)
{
    CLU_SIG_IS_SAFE(sig);
    assert(sig.num);

    num_p num = num_copy(sig.num);
    return sig_num_create(sig.signal, num);
}

sig_num_t sig_num_head_grow(sig_num_t sig, uint64_t count) // TODO test
{
    CLU_SIG_IS_SAFE(sig);
    assert(sig.num);

    sig.num = num_head_grow(sig.num, count);
    return sig;
}

sig_num_t sig_num_head_trim(sig_num_t sig, uint64_t count) // TODO test
{
    CLU_SIG_IS_SAFE(sig);
    assert(sig.num);

    num_head_trim(sig.num, count);
    return sig_num_create(sig.signal, sig.num);
}



bool sig_num_is_zero(sig_num_t sig)
{
    CLU_SIG_IS_SAFE(sig);
    assert(sig.num);

    return sig.signal == ZERO;
}

int64_t sig_num_cmp(sig_num_t sig_1, sig_num_t sig_2)
{
    CLU_SIG_IS_SAFE(sig_1);
    CLU_SIG_IS_SAFE(sig_2);

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
    CLU_SIG_IS_SAFE(sig);
    assert(sig.num);

    sig.num = num_shl(sig.num, bits);
    return sig;
}

sig_num_t sig_num_shr(sig_num_t sig, uint64_t bits)
{
    CLU_SIG_IS_SAFE(sig);
    assert(sig.num);

    num_p num = num_shr(sig.num, bits);
    return sig_num_create(sig.signal, num);
}



sig_num_t sig_num_opposite(sig_num_t sig)
{
    CLU_SIG_IS_SAFE(sig);

    sig.signal = sig.signal >> 1 | (sig.signal & POSITIVE) << 1;
    return sig;
}

sig_num_t sig_num_add(sig_num_t sig_1, sig_num_t sig_2)
{
    CLU_SIG_IS_SAFE(sig_1);
    CLU_SIG_IS_SAFE(sig_2);

    uint64_t signal_res = sig_1.signal & sig_2.signal;
    if(signal_res)
    {
        num_p num_res = num_add(sig_1.num, sig_2.num);
        return sig_num_create(signal_res, num_res);
    }

    if(num_cmp(sig_1.num, sig_2.num) > 0)
    {
        num_p num_res = num_sub(sig_1.num, sig_2.num);
        signal_res = sig_1.signal;
        return sig_num_create(signal_res, num_res);
    }

    num_p num_res = num_sub(sig_2.num, sig_1.num);
    signal_res = sig_2.signal;
    return sig_num_create(signal_res, num_res);
}

sig_num_t sig_num_sub(sig_num_t sig_1, sig_num_t sig_2)
{
    CLU_SIG_IS_SAFE(sig_1);
    CLU_SIG_IS_SAFE(sig_2);

    sig_2 = sig_num_opposite(sig_2);
    return sig_num_add(sig_1, sig_2);
}

sig_num_t sig_num_mul_high(sig_num_t sig_1, sig_num_t sig_2, uint64_t pos) // TODO TEST
{
    CLU_SIG_IS_SAFE(sig_1);
    CLU_SIG_IS_SAFE(sig_2);

    uint64_t signal_res = sig_1.signal & sig_2.signal ?
        POSITIVE : NEGATIVE;

    num_p num_res = num_mul_high(sig_1.num, sig_2.num, pos);
    return sig_num_create(signal_res, num_res);
}

sig_num_t sig_num_mul(sig_num_t sig_1, sig_num_t sig_2)
{
    CLU_SIG_IS_SAFE(sig_1);
    CLU_SIG_IS_SAFE(sig_2);

    uint64_t signal_res = sig_1.signal & sig_2.signal ?
        POSITIVE : NEGATIVE;

    num_p num_res = num_mul(sig_1.num, sig_2.num);
    return sig_num_create(signal_res, num_res);
}

sig_num_t sig_num_sqr(sig_num_t sig) // TODO test
{
    CLU_SIG_IS_SAFE(sig);

    num_p num = num_sqr(sig.num);
    return sig_num_create(POSITIVE, num);
}

sig_num_t sig_num_div(sig_num_t sig_1, sig_num_t sig_2)
{
    CLU_SIG_IS_SAFE(sig_1);
    CLU_SIG_IS_SAFE(sig_2);

    uint64_t signal_res = sig_1.signal & sig_2.signal ?
        POSITIVE : NEGATIVE;

    num_p num_res = num_div(sig_1.num, sig_2.num);
    return sig_num_create(signal_res, num_res);
}
