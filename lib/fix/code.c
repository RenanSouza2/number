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

fix_t fix_create_immed(uint64_t pos, uint64_t signal, uint64_t n, ...)
{
    va_list args;
    va_start(args, n);
    sig_num_t sig = sig_num_create_variadic(signal, n, &args);
    return fix_create(sig, pos);
}

#endif



void fix_display_dec(char tag[], fix_t fix)
{
    CLU_HANDLER_IS_SAFE(fix.sig.num.head);

    printf("\n%s: %c\t", tag, fix.sig.signal == NEGATIVE ? '-' : '+');

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

void fix_display(fix_t fix)
{
    printf("" U64P() "\t", fix.pos);
    sig_num_display(fix.sig, false);
}

void fix_display_tag(char tag[], fix_t fix)
{
    sig_num_display_tag(tag, fix.sig);
}

void fix_display_full(char tag[], fix_t fix)
{
    sig_num_display_full(tag, fix.sig);
}



fix_t fix_create(sig_num_t sig, uint64_t pos) // TODO test
{
    return (fix_t)
    {
        .sig = sig,
        .pos = pos
    };
}

fix_t fix_wrap(int64_t value, uint64_t pos) // TODO test
{
    sig_num_t sig = sig_num_wrap(value);
    if(value) sig = sig_num_shl(sig, pos << 6);
    return fix_create(sig, pos);
}

fix_t fix_copy(fix_t fix) // TODO test
{
    sig_num_t sig = sig_num_copy(fix.sig);
    return fix_create(sig, fix.pos);
}

void fix_free(fix_t fix)
{
    sig_num_free(fix.sig);
}



fix_t fix_reposition(fix_t fix, uint64_t pos) // TODO test
{
    if(fix.sig.num.count == 0)
        return (fix_t)
        {
            .sig = fix.sig,
            .pos = pos
        };

    if(pos > fix.pos)
        return (fix_t)
        {
            .sig = sig_num_shl(fix.sig, (pos - fix.pos) << 6),
            .pos = pos
        };

    return (fix_t)
    {
        .sig = sig_num_shr(fix.sig, (fix.pos - pos) << 6),
        .pos = pos
    };
}



fix_t fix_base_to(fix_t fix, uint64_t base) // TODO test
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



int64_t fix_cmp(fix_t fix_1, fix_t fix_2) // TODO test
{
    return sig_num_cmp(fix_1.sig, fix_2.sig);
}



fix_t fix_shl(fix_t fix, uint64_t value) // TODO test
{
    fix.sig = sig_num_shl(fix.sig, value);
    return fix;
}

fix_t fix_shr(fix_t fix, uint64_t value) // TODO test
{
    fix.sig = sig_num_shr(fix.sig, value);
    return fix;
}



fix_t fix_add(fix_t fix_1, fix_t fix_2) // TODO test
{
    fix_1.sig = sig_num_add(fix_1.sig, fix_2.sig);
    return fix_1;
}

fix_t fix_sub(fix_t fix_1, fix_t fix_2) // TODO test
{
    fix_1.sig = sig_num_sub(fix_1.sig, fix_2.sig);
    return fix_1;
}

fix_t fix_mul(fix_t fix_1, fix_t fix_2) // TODO test
{
    fix_1.sig = sig_num_mul(fix_1.sig, fix_2.sig);
    fix_1.sig = sig_num_shr(fix_1.sig, fix_1.pos << 6);
    return fix_1;
}

fix_t fix_sqr(fix_t fix) // TODO test
{
    fix.sig = sig_num_sqr(fix.sig);
    fix.sig = sig_num_shr(fix.sig, fix.pos << 6);
    return fix;
}

fix_t fix_div(fix_t fix_1, fix_t fix_2) // TODO test
{
    fix_1.sig = sig_num_shl(fix_1.sig, fix_1.pos << 6);
    fix_1.sig = sig_num_div(fix_1.sig, fix_2.sig);
    return fix_1;
}
