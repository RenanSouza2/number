#include <stdio.h>
#include <stdlib.h>

#include "debug.h"
#include "../../mods/clu/header.h"
#include "../../mods/macros/assert.h"

#include "../fix/header.h"
#include "../sig/header.h"
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
        printf("\n\tFLOAT NUM ASSERT ERROR\t| MISMATCH SIG");
        return false;
    }

    return true;
}

bool float_num_eq_dbg(float_num_t flt_1, float_num_t flt_2)
{
    CLU_FLOAT_IS_SAFE(flt_1);
    CLU_FLOAT_IS_SAFE(flt_2);

    if(!float_num_inner(flt_1, flt_2))
    {
        printf("\n");
        float_num_display(flt_1);
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

void float_num_display_dec(float_num_t flt) // TODO TEST
{
    CLU_FLOAT_IS_SAFE(flt);

    flt = float_num_copy(flt);
    flt = float_num_set_size(flt, flt.size + 2);

    if(float_num_is_zero(flt))
    {
        printf("0");
        return;
    }

    uint64_t signal = flt.sig.signal;
    flt.sig.signal = POSITIVE;

    float_num_t flt_one = float_num_wrap(1, flt.size);
    float_num_t flt_ten = float_num_wrap(10, flt.size);

    int64_t base = 1;
    float_num_t flt_base;
    if(float_num_cmp(flt, flt_one) < 0)
    {
        flt_base = float_num_div(
            float_num_copy(flt_one),
            float_num_copy(flt_ten)
        );
        base = -1;
        while(float_num_cmp(flt_base, flt) > 0)
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
        float_num_t flt_tmp = float_num_div(
            float_num_copy(flt),
            float_num_copy(flt_base)
        );
        if(float_num_cmp(flt_tmp, flt_ten) < 0)
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

            if(float_num_cmp(flt_tmp, flt) > 0)
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

    flt = float_num_div(flt, flt_base);
    flt = float_num_set_size(flt, flt.size - 1);

    fix_num_t fix = (fix_num_t)
    {
        .pos = flt.size - 1,
        .sig = (sig_num_t)
        {
            .signal = signal,
            .num = flt.sig.num
        }
    };
    fix_num_display_dec(fix);
    printf(" * 10 ^ " D64P() "", base);
    float_num_free(flt);
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
    CLU_FLOAT_IS_SAFE(flt);

    if(sig_num_is_zero(flt.sig))
        return flt;

    if(flt.sig.num->count < flt.size)
    {
        uint64_t diff = flt.size - flt.sig.num->count;
        flt.exponent = int64_add(flt.exponent, -diff);
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

void float_num_free(float_num_t flt)
{
    CLU_FLOAT_IS_SAFE(flt);

    sig_num_free(flt.sig);
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

    uint64_t exponent = flt_1.exponent > flt_2.exponent ?
        flt_1.exponent : flt_2.exponent;

    flt_1 = float_num_set_exponent(float_num_copy(flt_1), exponent - 1);
    flt_2 = float_num_set_exponent(float_num_copy(flt_2), exponent - 1);

    if(float_num_is_zero(flt_1))
    {
        float_num_free(flt_1);
        float_num_free(flt_2);
        return false;
    }

    if(float_num_is_zero(flt_2))
    {
        float_num_free(flt_1);
        float_num_free(flt_2);
        return false;
    }

    float_num_free(flt_1);
    float_num_free(flt_2);
    return true;
}

float_num_t float_num_opposite(float_num_t flt) // TODO TEST
{
    CLU_FLOAT_IS_SAFE(flt);

    flt.sig = sig_num_opposite(flt.sig);
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

    int64_t exponent = int64_add(flt_1.exponent, flt_2.exponent);
    sig_num_t sig = sig_num_mul(flt_1.sig, flt_2.sig);
    return float_num_create(exponent, flt_1.size, sig);
}

float_num_t float_num_sqr(float_num_t flt) // TODO TEST
{
    CLU_FLOAT_IS_SAFE(flt);

    int64_t exponent = int64_add(flt.exponent, flt.exponent);
    sig_num_t sig = sig_num_sqr(flt.sig);
    return float_num_create(exponent, flt.size, sig);
}

float_num_t float_num_div(float_num_t flt_1, float_num_t flt_2) // TODO TEST
{
    CLU_FLOAT_IS_SAFE(flt_1);
    CLU_FLOAT_IS_SAFE(flt_2);

    int64_t exponent = int64_add(flt_1.exponent, -(int64_t)flt_1.size);
    flt_1 = float_num_set_exponent(flt_1, exponent);
    exponent = int64_add(flt_1.exponent, -flt_2.exponent);
    sig_num_t sig = sig_num_div(flt_1.sig, flt_2.sig);
    return float_num_create(exponent, flt_1.size, sig);
}
