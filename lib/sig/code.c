#include <stdio.h>
#include <stdlib.h>

#include "../../utils/assert.h"
#include "debug.h"

#ifdef DEBUG

#include "../../utils/clu/bin/header.h"

#endif



#define POSITIVE 1
#define NEGATIVE 2


sig_p sig_create(num_p num, uint64_t signal) // TODO test
{
    sig_p sig = malloc(sizeof(sig_t));
    assert(sig);

    if(num_is_zero(num))
        signal = POSITIVE | NEGATIVE;

    *sig = (sig_t)
    {
        .num = num,
        .signal = signal
    };
    return sig;
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
        return sig_create(num_res, signal_res);
    }

    if(num_cmp(sig_1->num, sig_2->num) > 0)
    {
        num_p num_res = num_sub(sig_1->num, sig_2->num);
        return sig_create(num_res, sig_1->signal);
    }

    num_p num_res = num_sub(sig_2->num, sig_1->num);
    return sig_create(num_res, sig_2->signal);
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
    return sig_create(num_res, signal_res);
}

sig_p sig_div(sig_p sig_1, sig_p sig_2) // TODO test
{
    uint64_t signal_res = sig_1->signal & sig_2->signal ?
        POSITIVE : NEGATIVE;

    num_p num_res = num_div(sig_1->num, sig_2->num);
    return sig_create(num_res, signal_res);
}
