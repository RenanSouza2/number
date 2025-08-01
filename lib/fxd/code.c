#include <stdio.h>
#include <stdlib.h>

#include "debug.h"
#include "../../mods/clu/header.h"

#include "../../mods/macros/assert.h"
#include "../../mods/macros/uint.h"

#include "../num/header.h"
#include "../num/struct.h"
#include "../sig/header.h"

#define CLU_FXD_IS_SAFE(FXD) CLU_HANDLER_IS_SAFE((FXD).sig.num)



#ifdef DEBUG

#include "../sig/debug.h"

fxd_num_t fxd_num_create_immed(uint64_t pos, uint64_t signal, uint64_t n, ...)
{
    va_list args;
    va_start(args, n);
    sig_num_t sig = sig_num_create_variadic(signal, n, &args);
    return fxd_num_create(sig, pos);
}

#endif


void fxd_num_display_dec(fxd_num_t fxd)
{
    CLU_FXD_IS_SAFE(fxd)

    printf("%c ", fxd.sig.signal == NEGATIVE ? '-' : '+');

    num_p num_hi, num_lo;
    num_break(&num_hi, &num_lo, num_copy(fxd.sig.num), fxd.pos);

    num_display_dec(num_hi);
    num_free(num_hi);

    printf(".");

    if(num_lo->count == 0)
    {
        printf("0");
        num_free(num_lo);
        return;
    }

    uint64_t t = 0;
    num_p num_u = num_wrap(1);
    for(uint64_t pos = fxd.pos; num_u->count < pos + 1; t++)
    {
        num_u = num_mul_uint(num_u, 1000000000000000000);

        if(num_u->count > 2)
        {
            num_head_trim(num_u, 1);
            pos--;
        }
    }
    num_free(num_u);

    num_p num = num_pow(num_wrap(1000000000000000000), t);
    num_lo = num_mul(num_lo, num);
    num_break(&num_lo, &num_hi, num_lo, fxd.pos);

    num_lo = num_base_to(num_lo, 1000000000000000000);
    for(uint64_t i=num_lo->count-1; i!=0; i--)
        printf("" U64P(018) "", num_lo->chunk[i]);

    num_free(num_lo);
}

void fxd_num_display(fxd_num_t fxd)
{
    CLU_FXD_IS_SAFE(fxd);

    printf("" U64P() "\t", fxd.pos);
    sig_num_display(fxd.sig, false);
}

void fxd_num_display_tag(char tag[], fxd_num_t fxd)
{
    CLU_FXD_IS_SAFE(fxd);

    sig_num_display_tag(tag, fxd.sig);
}

void fxd_num_display_full(char tag[], fxd_num_t fxd)
{
    CLU_FXD_IS_SAFE(fxd);

    sig_num_display_full(tag, fxd.sig);
}



fxd_num_t fxd_num_create(sig_num_t sig, uint64_t pos) // TODO test
{
    CLU_HANDLER_IS_SAFE(sig.num);

    return (fxd_num_t)
    {
        .sig = sig,
        .pos = pos
    };
}

fxd_num_t fxd_num_wrap(int64_t value, uint64_t pos) // TODO test
{
    sig_num_t sig = sig_num_wrap(value);
    sig = sig_num_head_grow(sig, pos);
    return fxd_num_create(sig, pos);
}

fxd_num_t fxd_num_copy(fxd_num_t fxd) // TODO test
{
    CLU_FXD_IS_SAFE(fxd);

    sig_num_t sig = sig_num_copy(fxd.sig);
    return fxd_num_create(sig, fxd.pos);
}

void fxd_num_free(fxd_num_t fxd)
{
    CLU_FXD_IS_SAFE(fxd);

    sig_num_free(fxd.sig);
}



fxd_num_t fxd_num_reposition(fxd_num_t fxd, uint64_t pos) // TODO test
{
    CLU_FXD_IS_SAFE(fxd);

    if(sig_num_is_zero(fxd.sig) == 0)
        return (fxd_num_t)
        {
            .sig = fxd.sig,
            .pos = pos
        };

    if(pos > fxd.pos)
        return (fxd_num_t)
        {
            .sig = sig_num_head_grow(fxd.sig, pos - fxd.pos),
            .pos = pos
        };

    return (fxd_num_t)
    {
        .sig = sig_num_head_trim(fxd.sig, fxd.pos - pos),
        .pos = pos
    };
}



fxd_num_t fxd_num_base_to(fxd_num_t fxd, uint64_t base) // TODO test
{
    CLU_FXD_IS_SAFE(fxd);

    num_p num_hi, num_lo;
    num_break(&num_hi, &num_lo, fxd.sig.num, fxd.pos);

    num_hi = num_base_to(num_hi, base);

    uint64_t pos = fxd.pos;
    fxd.pos = 0;
    num_p num_u = num_wrap(1);
    for(;;)
    {
        num_lo = num_mul(num_lo, num_wrap(base));
        num_u = num_mul(num_u, num_wrap(base));

        if(num_u->count > pos)
            break;

        num_p num_aux;
        num_break(&num_aux, &num_lo, num_lo, pos);
        num_hi = num_head_grow(num_hi, 1);
        num_hi->chunk[0] = num_unwrap(num_aux);
        fxd.pos++;
    }

    fxd.sig.num = num_hi;
    return fxd;
}



int64_t fxd_num_cmp(fxd_num_t fxd_1, fxd_num_t fxd_2) // TODO test
{
    CLU_FXD_IS_SAFE(fxd_1);
    CLU_FXD_IS_SAFE(fxd_2);

    return sig_num_cmp(fxd_1.sig, fxd_2.sig);
}

bool fxd_num_is_zero(fxd_num_t fxd)
{
    CLU_FXD_IS_SAFE(fxd);

    return sig_num_is_zero(fxd.sig);
}



fxd_num_t fxd_num_shl(fxd_num_t fxd, uint64_t value) // TODO test
{
    CLU_FXD_IS_SAFE(fxd);

    fxd.sig = sig_num_shl(fxd.sig, value);
    return fxd;
}

fxd_num_t fxd_num_shr(fxd_num_t fxd, uint64_t value) // TODO test
{
    CLU_FXD_IS_SAFE(fxd);

    fxd.sig = sig_num_shr(fxd.sig, value);
    return fxd;
}



fxd_num_t fxd_num_add(fxd_num_t fxd_1, fxd_num_t fxd_2) // TODO test
{
    CLU_FXD_IS_SAFE(fxd_1);
    CLU_FXD_IS_SAFE(fxd_2);

    fxd_1.sig = sig_num_add(fxd_1.sig, fxd_2.sig);
    return fxd_1;
}

fxd_num_t fxd_num_sub(fxd_num_t fxd_1, fxd_num_t fxd_2) // TODO test
{
    CLU_FXD_IS_SAFE(fxd_1);
    CLU_FXD_IS_SAFE(fxd_2);

    fxd_1.sig = sig_num_sub(fxd_1.sig, fxd_2.sig);
    return fxd_1;
}

fxd_num_t fxd_num_mul(fxd_num_t fxd_1, fxd_num_t fxd_2) // TODO test
{
    CLU_FXD_IS_SAFE(fxd_1);
    CLU_FXD_IS_SAFE(fxd_2);

    fxd_1.sig = sig_num_mul(fxd_1.sig, fxd_2.sig);
    fxd_1.sig = sig_num_head_trim(fxd_1.sig, fxd_1.pos);
    return fxd_1;
}

fxd_num_t fxd_num_sqr(fxd_num_t fxd) // TODO test
{
    CLU_FXD_IS_SAFE(fxd);

    fxd.sig = sig_num_sqr(fxd.sig);
    fxd.sig = sig_num_head_trim(fxd.sig, fxd.pos);
    return fxd;
}

fxd_num_t fxd_num_div(fxd_num_t fxd_1, fxd_num_t fxd_2) // TODO test
{
    CLU_FXD_IS_SAFE(fxd_1);
    CLU_FXD_IS_SAFE(fxd_2);

    fxd_1.sig = sig_num_head_grow(fxd_1.sig, fxd_1.pos);
    fxd_1.sig = sig_num_div(fxd_1.sig, fxd_2.sig);
    return fxd_1;
}



fxd_num_t fxd_num_mul_sig(fxd_num_t fxd, sig_num_t sig) // TODO test
{
    CLU_FXD_IS_SAFE(fxd);
    CLU_HANDLER_IS_SAFE(sig.num);

    fxd.sig = sig_num_mul(fxd.sig, sig);
    return fxd;
}

fxd_num_t fxd_num_div_sig(fxd_num_t fxd, sig_num_t sig) // TODO test
{
    CLU_FXD_IS_SAFE(fxd);
    CLU_HANDLER_IS_SAFE(sig.num);

    fxd.sig = sig_num_div(fxd.sig, sig);
    return fxd;
}
