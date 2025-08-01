#include <stdio.h>
#include <stdlib.h>

#include "debug.h"
#include "../../mods/clu/header.h"
#include "../../mods/macros/assert.h"

#include "../sig/header.h"
#include "../num/header.h"
#include "../num/struct.h"

#define CLU_FLT_IS_SAFE(FLT) CLU_HANDLER_IS_SAFE((FLT).sig.num)



#ifdef DEBUG

#include "../num/debug.h"
#include "../sig/debug.h"


flt_num_t flt_num_create_variadic(
    int64_t exponent,
    uint64_t size,
    uint64_t signal,
    uint64_t n,
    va_list *args
){
    sig_num_t sig = sig_num_create_variadic(signal, n, args);

    assert(size > 1);
    return (flt_num_t)
    {
        .exponent = exponent,
        .size = size,
        .sig = sig
    };
}

flt_num_t flt_num_create_immed(
    int64_t exponent,
    uint64_t size,
    uint64_t signal,
    uint64_t n,
    ...
){
    va_list args;
    va_start(args, n);
    return flt_num_create_variadic(exponent, size, signal, n, &args);
}



bool flt_num_inner(flt_num_t flt_1, flt_num_t flt_2)
{
    CLU_FLT_IS_SAFE(flt_1);
    CLU_FLT_IS_SAFE(flt_2);

    if(!int64(flt_1.exponent, flt_2.exponent))
    {
        printf("\n\tFLT NUM ASSERT ERROR\t| MISMATCH EXPONENT");
        return false;
    }

    if(!uint64(flt_1.size, flt_2.size))
    {
        printf("\n\tFLT NUM ASSERT ERROR\t| MISMATCH SIZE");
        return false;
    }

    if(!sig_num_inner(flt_1.sig, flt_2.sig))
    {
        printf("\n\tFLT NUM ASSERT ERROR\t| MISMATCH SIG NUM");
        return false;
    }

    // TODO
    // if(!sig_num_is_zero(flt_1.sig))
    // {
    //     if(!uint64(flt_1.size, flt_1.sig.num->count))
    //     {
    //         printf("\n\tFLT NUM ASSERT ERROR\t| NUM COUNT IS NOT SIZE");
    //         return false;
    //     }
    // }

    return true;
}

bool flt_num_eq_dbg(flt_num_t flt_1, flt_num_t flt_2)
{
    CLU_FLT_IS_SAFE(flt_1);
    CLU_FLT_IS_SAFE(flt_2);

    if(!flt_num_inner(flt_1, flt_2))
    {
        printf("\n");
        printf("\n");
        flt_num_display(flt_1);
        printf("\n");
        flt_num_display(flt_2);
        return false;
    }

    flt_num_free(flt_1);
    flt_num_free(flt_2);
    return true;
}

bool flt_num_immed(
    flt_num_t flt,
    int64_t exponent,
    uint64_t size,
    uint64_t signal,
    uint64_t n,
    ...
){
    CLU_FLT_IS_SAFE(flt);

    va_list args;
    va_start(args, n);
    flt_num_t flt_2 = flt_num_create_variadic(exponent, size, signal, n, &args);
    return flt_num_eq_dbg(flt, flt_2);
}

#endif



void flt_num_display(flt_num_t flt)
{
    CLU_FLT_IS_SAFE(flt);

    sig_num_display(flt.sig, false);
    printf(" | exponent: " D64P() "", flt.exponent);
}

void flt_num_display_dec(flt_num_t flt_0) // TODO TEST
{
    CLU_FLT_IS_SAFE(flt_0);

    if(flt_num_is_zero(flt_0))
    {
        printf("0");
        return;
    }

    flt_num_t flt_1 = flt_num_copy(flt_0);
    flt_1 = flt_num_set_size(flt_1, 2);

    uint64_t signal = flt_1.sig.signal;
    flt_1.sig.signal = POSITIVE;

    flt_num_t flt_one = flt_num_wrap(1, 2);
    flt_num_t flt_ten = flt_num_wrap(10, 2);

    int64_t base = 1;
    flt_num_t flt_base;
    if(flt_num_cmp(flt_1, flt_one) < 0)
    {
        flt_base = flt_num_div(
            flt_num_copy(flt_one),
            flt_num_copy(flt_ten)
        );
        base = -1;
        while(flt_num_cmp(flt_base, flt_1) > 0)
        {
            flt_base = flt_num_sqr(flt_base);
            base *= 2;
        }
    }
    else
    {
        flt_base = flt_num_copy(flt_one);
        base = 0;
    }
    flt_num_free(flt_one);

    while(true)
    {
        flt_num_t flt_tmp = flt_num_mul(
            flt_num_copy(flt_ten),
            flt_num_copy(flt_base)
        );
        if(flt_num_cmp(flt_1, flt_tmp) < 0)
        {
            flt_num_free(flt_tmp);
            break;
        }
        flt_num_free(flt_tmp);

        flt_num_t flt_add = flt_num_copy(flt_ten);
        int64_t add = 1;
        while(true)
        {
            flt_num_t flt_add_2 = flt_num_sqr(flt_num_copy(flt_add));
            flt_num_t flt_tmp = flt_num_mul(
                flt_num_copy(flt_base),
                flt_num_copy(flt_add_2)
            );

            if(flt_num_cmp(flt_tmp, flt_1) > 0)
            {
                flt_num_free(flt_tmp);
                flt_num_free(flt_add_2);
                break;
            }
            flt_num_free(flt_tmp);

            flt_num_free(flt_add);
            flt_add = flt_add_2;
            add *= 2;
        }

        flt_base = flt_num_mul(flt_base, flt_add);
        base += add;
    }
    flt_num_free(flt_ten);

    flt_num_free(flt_base);
    flt_base = flt_num_pow(flt_num_wrap(10, flt_0.size), base);
    flt_0 = flt_num_div(flt_num_copy(flt_0), flt_base);
    // flt_0 = flt_num_set_size(flt_0, flt_0.size - 2);

    fxd_num_t fxd = (fxd_num_t)
    {
        .pos = flt_0.size - 1,
        .sig = (sig_num_t)
        {
            .signal = signal,
            .num = flt_0.sig.num
        }
    };
    fxd_num_display_dec(fxd);
    printf(" * 10 ^ " D64P() "", base);
    flt_num_free(flt_0);
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



flt_num_t flt_num_normalize(flt_num_t flt)
{
    CLU_FLT_IS_SAFE(flt);
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

flt_num_t flt_num_create(int64_t exponent, uint64_t size, sig_num_t sig) // TODO TEST
{
    CLU_HANDLER_IS_SAFE(sig.num);

    assert(size > 1);
    flt_num_t flt = (flt_num_t)
    {
        .exponent = exponent,
        .size = size,
        .sig = sig
    };

    return flt_num_normalize(flt);
}

flt_num_t flt_num_copy(flt_num_t flt) // TODO TEST
{
    CLU_FLT_IS_SAFE(flt);

    sig_num_t sig = sig_num_copy(flt.sig);
    return flt_num_create(flt.exponent, flt.size, sig);
}

flt_num_t flt_num_wrap(int64_t value, uint64_t size) // TODO TEST
{
    sig_num_t sig = sig_num_wrap(value);
    return flt_num_create(0, size, sig);
}

flt_num_t flt_num_wrap_num(num_p num, uint64_t size) // TODO TEST
{
    sig_num_t sig = sig_num_wrap_num(num);
    return flt_num_create(0, size, sig);
}

flt_num_t flt_num_wrap_sig(sig_num_t sig, uint64_t size) // TODO TEST
{
    return flt_num_create(0, size, sig);
}

void flt_num_free(flt_num_t flt)
{
    CLU_FLT_IS_SAFE(flt);

    sig_num_free(flt.sig);
}



void flt_num_file_write(FILE *fp, flt_num_t flt)
{
    fprintf(fp, " %ld ", flt.exponent);
    fprintf(fp, " %lu", flt.size);
    sig_num_file_write(fp, flt.sig);
}

void flt_num_save(char file_path[], flt_num_t flt)
{
    FILE *fp = fopen(file_path, "w");
    assert(fp);

    flt_num_file_write(fp, flt);

    fclose(fp);
}

flt_num_t flt_num_file_read(FILE *fp)
{
    int64_t exponent;
    uint64_t size;
    assert(fscanf(fp, " %ld %lu", &exponent, &size) == 2);

    sig_num_t sig = sig_num_file_read(fp);

    return (flt_num_t)
    {
        .exponent = exponent,
        .size = size,
        .sig = sig
    };
}

flt_num_t flt_num_load(char file_path[])
{
    FILE *fp = fopen(file_path, "r");
    assert(fp);

    flt_num_t flt = flt_num_file_read(fp);

    fclose(fp);
    // remove(file_path);

    return flt;
}



fxd_num_t fxd_num_wrap_flt(flt_num_t flt, uint64_t pos) // TODO test
{
    flt = flt_num_set_exponent(flt, 1 - pos);
    return (fxd_num_t)
    {
        .pos = pos - 1,
        .sig = flt.sig
    };
}



flt_num_t flt_num_set_exponent(flt_num_t flt, int64_t exponent)
{
    CLU_FLT_IS_SAFE(flt);

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

flt_num_t flt_num_set_size(flt_num_t flt, uint64_t size) // TODO TEST
{
    CLU_FLT_IS_SAFE(flt);

    flt.size = size;
    return flt_num_normalize(flt);
}



bool flt_num_is_zero(flt_num_t flt)
{
    CLU_FLT_IS_SAFE(flt);

    return sig_num_is_zero(flt.sig);
}

int64_t flt_num_cmp(flt_num_t flt_1, flt_num_t flt_2) // TODO TEST
{
    CLU_FLT_IS_SAFE(flt_1);
    CLU_FLT_IS_SAFE(flt_2);

    flt_1 = flt_num_copy(flt_1);
    flt_2 = flt_num_copy(flt_2);

    flt_1 = flt_num_sub(flt_1, flt_2);
    uint64_t signal = flt_1.sig.signal;
    flt_num_free(flt_1);

    switch (signal)
    {
        case NEGATIVE: return -1;
        case ZERO: return 0;
        case POSITIVE: return 1;
    }

    exit(EXIT_FAILURE);
}

bool flt_num_safe_add(flt_num_t flt_1, flt_num_t flt_2) // TODO TEST
{
    CLU_FLT_IS_SAFE(flt_1);
    CLU_FLT_IS_SAFE(flt_2);

    if(flt_num_is_zero(flt_1))
        return true;

    if(flt_num_is_zero(flt_2))
        return true;

    uint64_t diff = flt_1.exponent > flt_2.exponent ?
        flt_1.exponent - flt_2.exponent:
        flt_2.exponent > flt_1.exponent;

    return flt_1.size > diff;
}

flt_num_t flt_num_opposite(flt_num_t flt) // TODO TEST
{
    CLU_FLT_IS_SAFE(flt);

    flt.sig = sig_num_opposite(flt.sig);
    return flt;
}



flt_num_t flt_num_shr(flt_num_t flt, uint64_t bits) // TODO TEST
{
    CLU_FLT_IS_SAFE(flt);

    if(bits == 0)
        return flt;

    if(flt_num_is_zero(flt))
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



flt_num_t flt_num_add(flt_num_t flt_1, flt_num_t flt_2)
{
    CLU_FLT_IS_SAFE(flt_1);
    CLU_FLT_IS_SAFE(flt_2);

    if(flt_num_is_zero(flt_1))
    {
        flt_num_free(flt_1);
        return flt_2;
    }

    if(flt_num_is_zero(flt_2))
    {
        flt_num_free(flt_2);
        return flt_1;
    }

    uint64_t exponent = flt_1.exponent > flt_2.exponent ?
        flt_1.exponent : flt_2.exponent;

    flt_1 = flt_num_set_exponent(flt_1, exponent - 1);
    flt_2 = flt_num_set_exponent(flt_2, exponent - 1);

    flt_1.sig = sig_num_add(flt_1.sig, flt_2.sig);
    return flt_num_normalize(flt_1);
}

flt_num_t flt_num_sub(flt_num_t flt_1, flt_num_t flt_2) // TODO TEST
{
    CLU_FLT_IS_SAFE(flt_1);
    CLU_FLT_IS_SAFE(flt_2);

    flt_2 = flt_num_opposite(flt_2);
    return flt_num_add(flt_1, flt_2);
}

flt_num_t flt_num_mul(flt_num_t flt_1, flt_num_t flt_2) // TODO TEST
{
    CLU_FLT_IS_SAFE(flt_1);
    CLU_FLT_IS_SAFE(flt_2);

    uint64_t pos = 0;
    flt_1.exponent = int64_add(flt_1.exponent, flt_2.exponent);
    flt_1.exponent = int64_add(flt_1.exponent, pos);
    flt_1.sig = sig_num_mul(flt_1.sig, flt_2.sig);
    return flt_num_normalize(flt_1);
}

flt_num_t flt_num_sqr(flt_num_t flt) // TODO TEST
{
    CLU_FLT_IS_SAFE(flt);

    flt.exponent = int64_add(flt.exponent, flt.exponent);
    flt.sig = sig_num_sqr(flt.sig);
    return flt_num_normalize(flt);
}

flt_num_t flt_num_pow(flt_num_t flt, int64_t value) // TODO TEST
{
    CLU_FLT_IS_SAFE(flt);

    bool negative = value < 0;
    if(negative)
        value = -value;

    flt_num_t flt_res = flt_num_wrap(1, flt.size);
    for(uint64_t mask = B(63); mask; mask >>= 1)
    {
        flt_res = flt_num_sqr(flt_res);
        if(value & mask)
            flt_res = flt_num_mul(flt_res, flt_num_copy(flt));
    }
    flt_num_free(flt);

    if(negative)
    {
        flt_res = flt_num_div(flt_num_wrap(1, flt_res.size), flt_res);
    }

    return flt_res;
}

flt_num_t flt_num_div(flt_num_t flt_1, flt_num_t flt_2) // TODO TEST
{
    CLU_FLT_IS_SAFE(flt_1);
    CLU_FLT_IS_SAFE(flt_2);

    int64_t exponent = int64_add(flt_1.exponent, -(int64_t)flt_1.size);
    flt_1 = flt_num_set_exponent(flt_1, exponent);
    flt_1.exponent = int64_sub(flt_1.exponent, flt_2.exponent);
    flt_1.sig = sig_num_div(flt_1.sig, flt_2.sig);
    return flt_num_normalize(flt_1);
}



flt_num_t flt_num_mul_sig(flt_num_t flt, sig_num_t sig) // TODO TEST
{
    CLU_FLT_IS_SAFE(flt);
    CLU_HANDLER_IS_SAFE(sig.num);

    flt.sig = sig_num_mul(flt.sig, sig);
    return flt_num_normalize(flt);
}

flt_num_t flt_num_div_sig(flt_num_t flt, sig_num_t sig) // TODO TEST
{
    CLU_FLT_IS_SAFE(flt);
    CLU_HANDLER_IS_SAFE(sig.num);

    int64_t exponent = int64_add(flt.exponent, -(int64_t)sig.num->count);
    flt = flt_num_set_exponent(flt, exponent);
    flt.sig = sig_num_div(flt.sig, sig);
    return flt_num_normalize(flt);
}
