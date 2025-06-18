#include <stdio.h>
#include <stdlib.h>

#include "debug.h"
#include "../../mods/clu/header.h"

#include "../../mods/macros/assert.h"
#include "../../mods/macros/U64.h"

#include "../num/header.h"
#include "../sig/header.h"



#ifdef DEBUG

#include "../sig/debug.h"

fix_num_t fix_num_create_immed(uint64_t pos, uint64_t signal, uint64_t n, ...)
{
    va_list args;
    va_start(args, n);
    sig_num_t sig = sig_num_create_variadic(signal, n, &args);
    return fix_num_create(sig, pos);
}

#endif



void fix_num_display_dec(fix_num_t fix)
{
    CLU_HANDLER_IS_SAFE(fix.sig.num.head);

    printf("%c ", fix.sig.signal == NEGATIVE ? '-' : '+');

    num_t num_hi, num_lo;
    num_break(&num_hi, &num_lo, num_copy(fix.sig.num), fix.pos);

    num_display_dec(num_hi);
    num_free(num_hi);

    printf(".");

    if(num_lo.count == 0)
    {
        printf("0");
        return;
    }

    uint64_t pos = fix.pos;
    num_t num_u = num_wrap(1);
    for(;;)
    {
        num_lo = num_mul(num_lo, num_wrap(1000000000000000000));
        num_u = num_mul(num_u, num_wrap(1000000000000000000));

        if(num_u.count > 2)
        {
            num_lo = num_shr(num_lo, 64);
            num_u = num_shr(num_u, 64);
            pos--;
        }

        if(num_u.count > pos)
            break;

        num_t num_aux;
        num_break(&num_aux, &num_lo, num_lo, pos);
        printf(U64P(018), num_unwrap(num_aux));
    }
    num_free(num_lo);
    num_free(num_u);
}

void fix_num_display(fix_num_t fix)
{
    printf("" U64P() "\t", fix.pos);
    sig_num_display(fix.sig, false);
}

void fix_num_display_tag(char tag[], fix_num_t fix)
{
    sig_num_display_tag(tag, fix.sig);
}

void fix_num_display_full(char tag[], fix_num_t fix)
{
    sig_num_display_full(tag, fix.sig);
}



fix_num_t fix_num_create(sig_num_t sig, uint64_t pos) // TODO test
{
    return (fix_num_t)
    {
        .sig = sig,
        .pos = pos
    };
}

fix_num_t fix_num_wrap(int64_t value, uint64_t pos) // TODO test
{
    sig_num_t sig = sig_num_wrap(value);
    if(value) sig = sig_num_head_grow(sig, pos);
    return fix_num_create(sig, pos);
}

fix_num_t fix_num_copy(fix_num_t fix) // TODO test
{
    sig_num_t sig = sig_num_copy(fix.sig);
    return fix_num_create(sig, fix.pos);
}

void fix_num_free(fix_num_t fix)
{
    sig_num_free(fix.sig);
}



fix_num_t fix_num_reposition(fix_num_t fix, uint64_t pos) // TODO test
{
    if(fix.sig.num.count == 0)
        return (fix_num_t)
        {
            .sig = fix.sig,
            .pos = pos
        };

    if(pos > fix.pos)
        return (fix_num_t)
        {
            .sig = sig_num_head_grow(fix.sig, pos - fix.pos),
            .pos = pos
        };

    return (fix_num_t)
    {
        .sig = sig_num_head_trim(fix.sig, fix.pos - pos),
        .pos = pos
    };
}



fix_num_t fix_num_base_to(fix_num_t fix, uint64_t base) // TODO test
{
    num_t num_hi, num_lo;
    num_break(&num_hi, &num_lo, fix.sig.num, fix.pos);

    num_hi = num_base_to(num_hi, base);

    uint64_t pos = fix.pos;
    fix.pos = 0;
    num_t num_u = num_wrap(1);
    for(;;)
    {
        num_lo = num_mul(num_lo, num_wrap(base));
        num_u = num_mul(num_u, num_wrap(base));

        if(num_u.count > pos)
            break;

        num_t num_aux;
        num_break(&num_aux, &num_lo, num_lo, pos);
        num_insert_head(&num_hi, num_unwrap(num_aux));
        fix.pos++;
    }

    fix.sig.num = num_hi;
    return fix;
}



int64_t fix_num_cmp(fix_num_t fix_1, fix_num_t fix_2) // TODO test
{
    return sig_num_cmp(fix_1.sig, fix_2.sig);
}

bool fix_num_is_zero(fix_num_t fix)
{
    return sig_num_is_zero(fix.sig);
}



fix_num_t fix_num_shl(fix_num_t fix, uint64_t value) // TODO test
{
    fix.sig = sig_num_shl(fix.sig, value);
    return fix;
}

fix_num_t fix_num_shr(fix_num_t fix, uint64_t value) // TODO test
{
    fix.sig = sig_num_shr(fix.sig, value);
    return fix;
}



fix_num_t fix_num_add(fix_num_t fix_1, fix_num_t fix_2) // TODO test
{
    fix_1.sig = sig_num_add(fix_1.sig, fix_2.sig);
    return fix_1;
}

fix_num_t fix_num_sub(fix_num_t fix_1, fix_num_t fix_2) // TODO test
{
    fix_1.sig = sig_num_sub(fix_1.sig, fix_2.sig);
    return fix_1;
}

fix_num_t fix_num_mul(fix_num_t fix_1, fix_num_t fix_2) // TODO test
{
    fix_1.sig = sig_num_mul(fix_1.sig, fix_2.sig);
    fix_1.sig = sig_num_head_trim(fix_1.sig, fix_1.pos);
    return fix_1;
}

fix_num_t fix_num_sqr(fix_num_t fix) // TODO test
{
    fix.sig = sig_num_sqr(fix.sig);
    fix.sig = sig_num_head_trim(fix.sig, fix.pos);
    return fix;
}

fix_num_t fix_num_div(fix_num_t fix_1, fix_num_t fix_2) // TODO test
{
    fix_1.sig = sig_num_head_grow(fix_1.sig, fix_1.pos);
    fix_1.sig = sig_num_div(fix_1.sig, fix_2.sig);
    return fix_1;
}
