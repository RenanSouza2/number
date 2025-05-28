#include <stdio.h>
#include <stdlib.h>

#include "debug.h"
#include "../../mods/clu/header.h"

#include "../sig/header.h"



#ifdef DEBUG

#include "../sig/debug.h"



float_num_t float_num_create_immed(
    uint64_t exponent,
    uint64_t size_cap,
    uint64_t signal,
    uint64_t n,
    ...
){
    va_list args;
    va_start(args, n);
    sig_num_t sig = sig_num_create_variadic(signal, n, &args);
    return float_num_create(exponent, size_cap, sig);
}

#endif



float_num_t float_num_create(uint64_t exponent, uint64_t size_cap, sig_num_t sig)
{
    return (float_num_t)
    {
        .exponent = exponent,
        .size_cap = size_cap,
        .sig = sig
    }; 
}

void float_num_free(float_num_t flt)
{
    sig_num_free(flt.sig);
}



bool float_num_is_zero(float_num_t flt) // TODO TEST
{
    return sig_num_is_zero(flt.sig);
}




float_num_t float_num_set_exponent(float_num_t flt, uint64_t exponent) // TODO TEST
{
    if(flt.exponent == exponent)
        return flt;

    if(flt.exponent < exponent)
    {
        uint64_t shift = (exponent - flt.exponent) << 6;
        sig_num_t sig = sig_num_shr(flt.sig, shift);
        return float_num_create(exponent, flt.size_cap, sig);
    }
    
    uint64_t shift = (flt.exponent - exponent) << 6;
    sig_num_t sig = sig_num_shl(flt.sig, shift);
    return float_num_create(exponent, flt.size_cap, sig);
}

float_num_t float_num_normalize(float_num_t flt) // TODO TEST
{
    if(flt.sig.num.count <= flt.size_cap)
        return flt;

    return float_num_set_exponent(flt, flt.sig.num.count - flt.size_cap);
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
