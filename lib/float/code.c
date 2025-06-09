#include <stdio.h>
#include <stdlib.h>

#include "debug.h"
#include "../../mods/clu/header.h"
#include "../../mods/macros/assert.h"

#include "../sig/header.h"



#ifdef DEBUG

#include "../num/debug.h"
#include "../sig/debug.h"


float_num_t float_num_create_variadic(
    int64_t exponent,
    uint64_t size,
    uint64_t signal,
    uint64_t n,
    va_list *args
){
    sig_num_t sig = sig_num_create_variadic(signal, n, args);
    return (float_num_t)
    {
        .exponent = exponent,
        .size = size,
        .sig = sig
    };
}

float_num_t float_num_create_immed(
    int64_t exponent,
    uint64_t size,
    uint64_t signal,
    uint64_t n,
    ...
){
    va_list args;
    va_start(args, n);
    return float_num_create_variadic(exponent, size, signal, n, &args);
}



bool float_num_inner(float_num_t flt_1, float_num_t flt_2)
{
    assert(int64(flt_1.exponent, flt_2.exponent))
    {
        printf("\n\tFLOAT NUM ASSERT ERROR\t| MISMATCH EXPONENT");
        return false;
    }
}

bool float_num_immed(
    float_num_t flt,
    int64_t exponent,
    uint64_t size,
    uint64_t signal,
    uint64_t n,
    ...
){
    va_list args;
    va_start(args, n);
    float_num_t flt_2 = float_num_create_variadic(exponent, size, signal, n, &args);
    return float_num_inner(flt, flt_2);
}

#endif



void float_num_display(float_num_t flt)
{
    printf("\nexponent: " U64P() "\n", flt.exponent);
    sig_num_display(flt.sig, false);
}



uint64_t int64_get_sign(int64_t i)
{
    if(i == 0)  return ZERO;
    return i > 0 ? POSITIVE : NEGATIVE;
}

int64_t int64_add(int64_t a, int64_t b)
{
    int64_t res = a + b;
    if(a != 0 && b != 0)
    {
        uint64_t sign_a = int64_get_sign(a);
        if(sign_a == int64_get_sign(b))
            assert(sign_a == int64_get_sign(res));
    }
    return res;
}



float_num_t float_num_normalize(float_num_t flt)
{
    if(sig_num_is_zero(flt.sig))
        return flt;

    if(flt.sig.num.count < flt.size)
    {
        uint64_t diff = flt.size - flt.sig.num.count;
        flt.exponent = int64_add(flt.exponent, -diff);
        flt.sig = sig_num_head_grow(flt.sig, diff);
    }

    if(flt.sig.num.count > flt.size)
    {
        uint64_t diff = flt.sig.num.count - flt.size;
        flt.exponent = int64_add(flt.exponent, diff);
        flt.sig = sig_num_head_trim(flt.sig, diff);
    }

    return flt;
}

float_num_t float_num_create(int64_t exponent, uint64_t size, sig_num_t sig)
{
    float_num_t flt = (float_num_t)
    {
        .exponent = exponent,
        .size = size,
        .sig = sig
    };

    return float_num_normalize(flt);
}

void float_num_free(float_num_t flt)
{
    sig_num_free(flt.sig);
}



float_num_t float_num_set_exponent(float_num_t flt, int64_t exponent) // TODO TEST
{
    if(flt.exponent < exponent)
    {
        flt.sig = sig_num_head_trim(flt.sig, exponent - flt.exponent);
        flt.exponent = exponent;
    }
    
    if(flt.exponent > exponent)
    {
        flt.sig = sig_num_head_grow(flt.sig, flt.exponent - exponent);
        flt.exponent = exponent;
    }

    return flt;
}



bool float_num_is_zero(float_num_t flt)
{
    return sig_num_is_zero(flt.sig);
}

float_num_t float_num_opposite(float_num_t flt)
{
    flt.sig = sig_num_opposite(flt.sig);
    return flt;
}



float_num_t float_num_add(float_num_t flt_1, float_num_t flt_2) // TODO TEST
{
    if(float_num_is_zero(flt_1))
        return flt_2;

    if(float_num_is_zero(flt_2))
        return flt_1;

    uint64_t exponent = flt_1.exponent > flt_2.exponent ?
        flt_1.exponent : flt_2.exponent;

    flt_1 = float_num_set_exponent(flt_1, exponent);
    flt_2 = float_num_set_exponent(flt_2, exponent);
    
    flt_1.sig = sig_num_add(flt_1.sig, flt_2.sig);
    return float_num_normalize(flt_1);
}

float_num_t float_num_sub(float_num_t flt_1, float_num_t flt_2) // TODO TEST
{
    flt_2 = float_num_opposite(flt_2);
    return float_num_add(flt_1, flt_2);
}
