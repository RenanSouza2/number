#include <stdio.h>
#include <stdlib.h>

#include "debug.h"
#include "../../utils/assert.h"
#include "../../utils/clu/header.h"



#ifdef DEBUG

#include "../num/debug.h"

sig_p sig_create_variadic(uint64_t signal, uint64_t n, va_list args)
{
    num_p num = num_create_variadic(n, args);
    return sig_create(signal, num);
}

sig_p sig_create_immed(uint64_t signal, uint64_t n, ...)
{
    va_list args;
    va_start(args, n);
    return sig_create_variadic(signal, n, args);
}



bool sig_inner(sig_p sig_1, sig_p sig_2)
{
    if(!uint64(sig_1->signal, sig_2->signal))
    {
        printf("\n\tSIG ASSERT ERROR\t| DIFFERENT SIGNAL");
        return false;
    }

    if(!num_str(sig_1->num, sig_2->num))
    {
        printf("\n\tSIG ASSERT ERROR\t| DIFFERENT NUMBER");
        return false;
    }

    return true;
}

bool sig_str(sig_p sig_1, sig_p sig_2)
{
    if(!sig_inner(sig_1, sig_2))
    {
        printf("\n");
        sig_display_tag("\tsig_1", sig_1);
        sig_display_tag("\tsig_2", sig_2);
        return false;
    }

    return true;
}

bool sig_immed(sig_p sig, uint64_t signal, uint64_t count, ...)
{
    va_list args;
    va_start(args, count);
    sig_p sig_2 = sig_create_variadic(signal, count, args);

    bool res = sig_str(sig, sig_2);

    sig_free(sig);
    sig_free(sig_2);
    return res;
}

#endif



void sig_display(sig_p sig)
{
    if(sig->signal == ZERO)
    {
        printf("  0");
        return;
    }

    if(sig->signal & POSITIVE)
        printf("+ ");
    else
        printf("- ");

    num_display(sig->num);
}

void sig_display_tag(char tag[], sig_p sig)
{
    printf("\n%s: ", tag);
    sig_display(sig);
}



sig_p sig_create(uint64_t signal, num_p num)
{
    sig_p sig = malloc(sizeof(sig_t));
    assert(sig);

    if(num_is_zero(num))
        signal = ZERO;
    else
        assert(signal != ZERO)

    *sig = (sig_t)
    {
        .num = num,
        .signal = signal
    };
    return sig;
}



sig_p sig_wrap(int64_t value)
{
    if(value == 0)
    {
        num_p num = num_wrap(0);
        return sig_create(ZERO, num);
    }

    if(value < 0)
    {
        num_p num = num_wrap(-value);
        return sig_create(NEGATIVE, num);
    }

    num_p num = num_wrap(value);
    return sig_create(POSITIVE, num);
}

sig_p sig_wrap_str(char str[])
{
    uint64_t signal = str[0] == '-' ? NEGATIVE : POSITIVE;
    uint64_t offset = str[0] == '-' || str[0] == '+' ? 1 : 0;
    num_p num = num_wrap_str(&str[offset]);
    return sig_create(signal, num);
}

sig_p sig_copy(sig_p sig)
{
    num_p num = num_copy(sig->num);
    return sig_create(sig->signal, num);
}

void sig_free(sig_p sig)
{
    DBG_CHECK_PTR(sig);

    num_free(sig->num);
    free(sig);
}



sig_p sig_shl(sig_p sig, uint64_t bits) // TODO test
{
    num_shl(sig->num, bits);
    return sig;
}

sig_p sig_shr(sig_p sig, uint64_t bits) // TODO test
{
    num_shr(sig->num, bits);
    if(num_is_zero(sig->num))
        sig->signal = ZERO;

    return sig;
}



sig_p sig_opposite(sig_p sig)
{
    DBG_CHECK_PTR(sig);

    sig->signal = sig->signal >> 1 | (sig->signal & POSITIVE) << 1;
    return sig;
}

sig_p sig_add(sig_p sig_1, sig_p sig_2)
{
    DBG_CHECK_PTR(sig_1);
    DBG_CHECK_PTR(sig_2);

    uint64_t signal_res = sig_1->signal & sig_2->signal;
    if(signal_res)
    {
        num_p num_res = num_add(sig_1->num, sig_2->num);
        free(sig_1);
        free(sig_2);
        return sig_create(signal_res, num_res);
    }

    if(num_cmp(sig_1->num, sig_2->num) > 0)
    {
        num_p num_res = num_sub(sig_1->num, sig_2->num);
        signal_res = sig_1->signal;
        free(sig_1);
        free(sig_2);
        return sig_create(signal_res, num_res);
    }

    num_p num_res = num_sub(sig_2->num, sig_1->num);
    signal_res = sig_2->signal;
    free(sig_1);
    free(sig_2);
    return sig_create(signal_res, num_res);
}

sig_p sig_sub(sig_p sig_1, sig_p sig_2)
{
    sig_2 = sig_opposite(sig_2);
    return sig_add(sig_1, sig_2);
}

sig_p sig_mul(sig_p sig_1, sig_p sig_2)
{
    uint64_t signal_res = sig_1->signal & sig_2->signal ?
        POSITIVE : NEGATIVE;

    num_p num_res = num_mul(sig_1->num, sig_2->num);
    free(sig_1);
    free(sig_2);
    return sig_create(signal_res, num_res);
}

sig_p sig_div(sig_p sig_1, sig_p sig_2)
{
    uint64_t signal_res = sig_1->signal & sig_2->signal ?
        POSITIVE : NEGATIVE;

    num_p num_res = num_div(sig_1->num, sig_2->num);
    free(sig_1);
    free(sig_2);
    return sig_create(signal_res, num_res);
}
