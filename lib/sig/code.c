#include <stdio.h>
#include <stdlib.h>

#include "debug.h"
#include "../../utils/assert.h"
#include "../../utils/clu/bin/header.h"

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
        printf("\n\tSIG ASSERT ERROR\t| WRONG SIGNAL");
        return false;
    }

    if(!num_str(sig_1->num, sig_2->num))
    {
        printf("\n\tSIG ASSERT ERROR\t| WRONG NUMBER");
        return false;
    }

    return true;
}

bool sig_str(sig_p sig_1, sig_p sig_2)
{
    if(!sig_inner(sig_1, sig_2))
    {
        // TODO print
        printf("\n\tSIG ASSERT ERROR\t| WRONG NUMBER");
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



void sig_free(sig_p sig)
{
    DBG_CHECK_PTR(sig);

    num_free(sig->num);
    free(sig);
}



sig_p sig_opposite(sig_p sig) // TODO test
{
    sig->signal = sig->signal >> 1 | (sig->signal & POSITIVE) << 1;
    return sig;
}

sig_p sig_add(sig_p sig_1, sig_p sig_2) // TODO test
{
    uint64_t signal_res = sig_1->signal & sig_2->signal;
    if(signal_res)
    {
        num_p num_res = num_add(sig_1->num, sig_2->num);
        return sig_create(signal_res, num_res);
    }

    if(num_cmp(sig_1->num, sig_2->num) > 0)
    {
        num_p num_res = num_sub(sig_1->num, sig_2->num);
        return sig_create(sig_1->signal, num_res);
    }

    num_p num_res = num_sub(sig_2->num, sig_1->num);
    return sig_create(sig_2->signal, num_res);
}

sig_p sig_sub(sig_p sig_1, sig_p sig_2) // TODO test
{
    sig_2 = sig_opposite(sig_2);
    return sig_add(sig_1, sig_2);
}

sig_p sig_mul(sig_p sig_1, sig_p sig_2) // TODO test
{
    uint64_t signal_res = sig_1->signal & sig_2->signal ?
        POSITIVE : NEGATIVE;

    num_p num_res = num_mul(sig_1->num, sig_2->num);
    return sig_create(signal_res, num_res);
}

sig_p sig_div(sig_p sig_1, sig_p sig_2) // TODO test
{
    uint64_t signal_res = sig_1->signal & sig_2->signal ?
        POSITIVE : NEGATIVE;

    num_p num_res = num_div(sig_1->num, sig_2->num);
    return sig_create(signal_res, num_res);
}
