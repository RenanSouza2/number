#include <stdio.h>
#include <stdlib.h>

#include "debug.h"
#include "../../mods/clu/header.h"
#include "../../mods/macros/assert.h"

#include "../sig/header.h"
#include "../num/header.h"
#include "../num/struct.h"

#define CLU_FLOAT_IS_SAFE(FLOAT) CLU_HANDLER_IS_SAFE((FLOAT).sig.num)



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

    assert(size > 1);
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
    CLU_FLOAT_IS_SAFE(flt_1);
    CLU_FLOAT_IS_SAFE(flt_2);

    if(!int64(flt_1.exponent, flt_2.exponent))
    {
        printf("\n\tFLOAT NUM ASSERT ERROR\t| MISMATCH EXPONENT");
        return false;
    }

    if(!uint64(flt_1.size, flt_2.size))
    {
        printf("\n\tFLOAT NUM ASSERT ERROR\t| MISMATCH SIZE");
        return false;
    }

    if(!sig_num_inner(flt_1.sig, flt_2.sig))
    {
        printf("\n\tFLOAT NUM ASSERT ERROR\t| MISMATCH SIG NUM");
        return false;
    }

    // TODO
    // if(!sig_num_is_zero(flt_1.sig))
    // {
    //     if(!uint64(flt_1.size, flt_1.sig.num->count))
    //     {
    //         printf("\n\tFLOAT NUM ASSERT ERROR\t| NUM COUNT IS NOT SIZE");
    //         return false;
    //     }
    // }

    return true;
}

bool float_num_eq_dbg(float_num_t flt_1, float_num_t flt_2)
{
    CLU_FLOAT_IS_SAFE(flt_1);
    CLU_FLOAT_IS_SAFE(flt_2);

    if(!float_num_inner(flt_1, flt_2))
    {
        printf("\n");
        printf("\n");
        float_num_display(flt_1);
        printf("\n");
        float_num_display(flt_2);
        return false;
    }

    float_num_free(flt_1);
    float_num_free(flt_2);
    return true;
}

bool float_num_immed(
    float_num_t flt,
    int64_t exponent,
    uint64_t size,
    uint64_t signal,
    uint64_t n,
    ...
){
    CLU_FLOAT_IS_SAFE(flt);

    va_list args;
    va_start(args, n);
    float_num_t flt_2 = float_num_create_variadic(exponent, size, signal, n, &args);
    return float_num_eq_dbg(flt, flt_2);
}

#endif



void float_num_display(float_num_t flt)
{
    CLU_FLOAT_IS_SAFE(flt);

    sig_num_display(flt.sig, false);
    printf(" | exponent: " D64P() "", flt.exponent);
}

void float_num_display_dec(float_num_t flt_0) // TODO TEST
{
    CLU_FLOAT_IS_SAFE(flt_0);

    if(float_num_is_zero(flt_0))
    {
        printf("0");
        return;
    }

    float_num_t flt_1 = float_num_copy(flt_0);
    flt_1 = float_num_set_size(flt_1, 2);

    uint64_t signal = flt_1.sig.signal;
    flt_1.sig.signal = POSITIVE;

    float_num_t flt_one = float_num_wrap(1, 2);
    float_num_t flt_ten = float_num_wrap(10, 2);

    int64_t base = 1;
    float_num_t flt_base;
    if(float_num_cmp(flt_1, flt_one) < 0)
    {
        flt_base = float_num_div(
            float_num_copy(flt_one),
            float_num_copy(flt_ten)
        );
        base = -1;
        while(float_num_cmp(flt_base, flt_1) > 0)
        {
            flt_base = float_num_sqr(flt_base);
            base *= 2;
        }
    }
    else
    {
        flt_base = float_num_copy(flt_one);
        base = 0;
    }
    float_num_free(flt_one);

    while(true)
    {
        float_num_t flt_tmp = float_num_mul(
            float_num_copy(flt_ten),
            float_num_copy(flt_base)
        );
        if(float_num_cmp(flt_1, flt_tmp) < 0)
        {
            float_num_free(flt_tmp);
            break;
        }
        float_num_free(flt_tmp);

        float_num_t flt_add = float_num_copy(flt_ten);
        int64_t add = 1;
        while(true)
        {
            float_num_t flt_add_2 = float_num_sqr(float_num_copy(flt_add));
            float_num_t flt_tmp = float_num_mul(
                float_num_copy(flt_base),
                float_num_copy(flt_add_2)
            );

            if(float_num_cmp(flt_tmp, flt_1) > 0)
            {
                float_num_free(flt_tmp);
                float_num_free(flt_add_2);
                break;
            }
            float_num_free(flt_tmp);

            float_num_free(flt_add);
            flt_add = flt_add_2;
            add *= 2;
        }

        flt_base = float_num_mul(flt_base, flt_add);
        base += add;
    }
    float_num_free(flt_ten);

    float_num_free(flt_base);
    flt_base = float_num_pow(float_num_wrap(10, flt_0.size), base);
    flt_0 = float_num_div(float_num_copy(flt_0), flt_base);
    // flt_0 = float_num_set_size(flt_0, flt_0.size - 2);

    fix_num_t fix = (fix_num_t)
    {
        .pos = flt_0.size - 1,
        .sig = (sig_num_t)
        {
            .signal = signal,
            .num = flt_0.sig.num
        }
    };
    fix_num_display_dec(fix);
    printf(" * 10 ^ " D64P() "", base);
    float_num_free(flt_0);
}



uint64_t int64_get_sign(int64_t i)
{
    if(i == (int64_t)0) return ZERO;
    return i > (int64_t)0 ? POSITIVE : NEGATIVE;
}

int64_t int64_add(int64_t a, int64_t b)
{
    int64_t res = a + b;
    if(a != (int64_t)0 && b != (int64_t)0)
    {
        uint64_t sign_a = int64_get_sign(a);
        if(sign_a == int64_get_sign(b))
            assert(sign_a == int64_get_sign(res));
    }
    return res;
}

int64_t int64_sub(int64_t a, int64_t b) // TODO test
{
    assert(b != INT64_MIN);
    return int64_add(a, -b);
}



float_num_t float_num_normalize(float_num_t flt)
{
    CLU_FLOAT_IS_SAFE(flt);
    assert(flt.sig.num);

    if(sig_num_is_zero(flt.sig))
        return flt;

    if(flt.sig.num->count < flt.size)
    {
        uint64_t diff = flt.size - flt.sig.num->count;
        flt.exponent = int64_sub(flt.exponent, diff);
        flt.sig = sig_num_head_grow(flt.sig, diff);
    }

    if(flt.sig.num->count > flt.size)
    {
        uint64_t diff = flt.sig.num->count - flt.size;
        flt.exponent = int64_add(flt.exponent, diff);
        flt.sig = sig_num_head_trim(flt.sig, diff);
    }

    return flt;
}

float_num_t float_num_create(int64_t exponent, uint64_t size, sig_num_t sig) // TODO TEST
{
    CLU_HANDLER_IS_SAFE(sig.num);

    assert(size > 1);
    float_num_t flt = (float_num_t)
    {
        .exponent = exponent,
        .size = size,
        .sig = sig
    };

    return float_num_normalize(flt);
}

float_num_t float_num_copy(float_num_t flt) // TODO TEST
{
    CLU_FLOAT_IS_SAFE(flt);

    sig_num_t sig = sig_num_copy(flt.sig);
    return float_num_create(flt.exponent, flt.size, sig);
}

float_num_t float_num_wrap(int64_t value, uint64_t size) // TODO TEST
{
    sig_num_t sig = sig_num_wrap(value);
    return float_num_create(0, size, sig);
}

float_num_t float_num_wrap_num(num_p num, uint64_t size) // TODO TEST
{
    sig_num_t sig = sig_num_wrap_num(num);
    return float_num_create(0, size, sig);
}

float_num_t float_num_wrap_sig(sig_num_t sig, uint64_t size) // TODO TEST
{
    return float_num_create(0, size, sig);
}

void float_num_free(float_num_t flt)
{
    CLU_FLOAT_IS_SAFE(flt);

    sig_num_free(flt.sig);
}



fix_num_t fix_num_wrap_float(float_num_t flt, uint64_t pos) // TODO test
{
    flt = float_num_set_exponent(flt, 1 - pos);
    return (fix_num_t)
    {
        .pos = pos - 1,
        .sig = flt.sig
    };
}



float_num_t float_num_set_exponent(float_num_t flt, int64_t exponent)
{
    CLU_FLOAT_IS_SAFE(flt);

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

float_num_t float_num_set_size(float_num_t flt, uint64_t size) // TODO TEST
{
    CLU_FLOAT_IS_SAFE(flt);

    flt.size = size;
    return float_num_normalize(flt);
}



bool float_num_is_zero(float_num_t flt)
{
    CLU_FLOAT_IS_SAFE(flt);

    return sig_num_is_zero(flt.sig);
}

int64_t float_num_cmp(float_num_t flt_1, float_num_t flt_2) // TODO TEST
{
    CLU_FLOAT_IS_SAFE(flt_1);
    CLU_FLOAT_IS_SAFE(flt_2);

    flt_1 = float_num_copy(flt_1);
    flt_2 = float_num_copy(flt_2);

    flt_1 = float_num_sub(flt_1, flt_2);
    uint64_t signal = flt_1.sig.signal;
    float_num_free(flt_1);

    switch (signal)
    {
        case NEGATIVE: return -1;
        case ZERO: return 0;
        case POSITIVE: return 1;
    }

    exit(EXIT_FAILURE);
}

bool float_num_safe_add(float_num_t flt_1, float_num_t flt_2) // TODO TEST
{
    CLU_FLOAT_IS_SAFE(flt_1);
    CLU_FLOAT_IS_SAFE(flt_2);

    if(float_num_is_zero(flt_1))
        return true;

    if(float_num_is_zero(flt_2))
        return true;

    uint64_t diff = flt_1.exponent > flt_2.exponent ?
        flt_1.exponent - flt_2.exponent:
        flt_2.exponent > flt_1.exponent;

    return flt_1.size > diff;
}

float_num_t float_num_opposite(float_num_t flt) // TODO TEST
{
    CLU_FLOAT_IS_SAFE(flt);

    flt.sig = sig_num_opposite(flt.sig);
    return flt;
}



float_num_t float_num_shr(float_num_t flt, uint64_t bits) // TODO TEST
{
    CLU_FLOAT_IS_SAFE(flt);

    if(bits == 0)
        return flt;

    if(float_num_is_zero(flt))
        return flt;

    uint64_t rem = bits & 0x3f;
    if(flt.sig.num->chunk[flt.size - 1] >> rem)
    {
        flt.exponent = int64_sub(flt.exponent, bits >> 6);
        flt.sig.num = num_shr(flt.sig.num, rem);
        return flt;
    }

    flt.exponent = int64_sub(flt.exponent, 1 + (bits >> 6));
    flt.sig.num = num_shl(flt.sig.num, 64 - rem);
    return flt;
}



float_num_t float_num_add(float_num_t flt_1, float_num_t flt_2)
{
    CLU_FLOAT_IS_SAFE(flt_1);
    CLU_FLOAT_IS_SAFE(flt_2);

    if(float_num_is_zero(flt_1))
    {
        float_num_free(flt_1);
        return flt_2;
    }

    if(float_num_is_zero(flt_2))
    {
        float_num_free(flt_2);
        return flt_1;
    }

    uint64_t exponent = flt_1.exponent > flt_2.exponent ?
        flt_1.exponent : flt_2.exponent;

    flt_1 = float_num_set_exponent(flt_1, exponent - 1);
    flt_2 = float_num_set_exponent(flt_2, exponent - 1);

    flt_1.sig = sig_num_add(flt_1.sig, flt_2.sig);
    return float_num_normalize(flt_1);
}

float_num_t float_num_sub(float_num_t flt_1, float_num_t flt_2) // TODO TEST
{
    CLU_FLOAT_IS_SAFE(flt_1);
    CLU_FLOAT_IS_SAFE(flt_2);

    flt_2 = float_num_opposite(flt_2);
    return float_num_add(flt_1, flt_2);
}

float_num_t float_num_mul(float_num_t flt_1, float_num_t flt_2) // TODO TEST
{
    CLU_FLOAT_IS_SAFE(flt_1);
    CLU_FLOAT_IS_SAFE(flt_2);

    uint64_t pos = 0;
    flt_1.exponent = int64_add(flt_1.exponent, flt_2.exponent);
    flt_1.exponent = int64_add(flt_1.exponent, pos);
    flt_1.sig = sig_num_mul(flt_1.sig, flt_2.sig);
    return float_num_normalize(flt_1);
}

float_num_t float_num_sqr(float_num_t flt) // TODO TEST
{
    CLU_FLOAT_IS_SAFE(flt);

    flt.exponent = int64_add(flt.exponent, flt.exponent);
    flt.sig = sig_num_sqr(flt.sig);
    return float_num_normalize(flt);
}

float_num_t float_num_pow(float_num_t flt, int64_t value) // TODO TEST
{
    CLU_FLOAT_IS_SAFE(flt);

    bool negative = value < 0;
    if(negative)
        value = -value;

    float_num_t flt_res = float_num_wrap(1, flt.size);
    for(uint64_t mask = B(63); mask; mask >>= 1)
    {
        flt_res = float_num_sqr(flt_res);
        if(value & mask)
            flt_res = float_num_mul(flt_res, float_num_copy(flt));
    }
    float_num_free(flt);

    if(negative)
    {
        flt_res = float_num_div(float_num_wrap(1, flt_res.size), flt_res);
    }

    return flt_res;
}

float_num_t float_num_div(float_num_t flt_1, float_num_t flt_2) // TODO TEST
{
    CLU_FLOAT_IS_SAFE(flt_1);
    CLU_FLOAT_IS_SAFE(flt_2);

    int64_t exponent = int64_add(flt_1.exponent, -(int64_t)flt_1.size);
    flt_1 = float_num_set_exponent(flt_1, exponent);
    flt_1.exponent = int64_sub(flt_1.exponent, flt_2.exponent);
    flt_1.sig = sig_num_div(flt_1.sig, flt_2.sig);
    return float_num_normalize(flt_1);
}



float_num_t float_num_mul_sig(float_num_t flt, sig_num_t sig) // TODO TEST
{
    CLU_FLOAT_IS_SAFE(flt);
    CLU_HANDLER_IS_SAFE(sig.num);

    flt.sig = sig_num_mul(flt.sig, sig);
    return float_num_normalize(flt);
}

float_num_t float_num_div_sig(float_num_t flt, sig_num_t sig) // TODO TEST
{
    CLU_FLOAT_IS_SAFE(flt);
    CLU_HANDLER_IS_SAFE(sig.num);

    int64_t exponent = int64_add(flt.exponent, -(int64_t)sig.num->count);
    flt = float_num_set_exponent(flt, exponent);
    flt.sig = sig_num_div(flt.sig, sig);
    return float_num_normalize(flt);
}
