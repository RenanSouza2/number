#include <stdio.h>
#include <stdlib.h>

#include "debug.h"

#include "../../utils/assert.h"
#include "../../utils/U64.h"
#include "../../utils/clu/header.h"



#ifdef DEBUG

#include "../snum/debug.h"

fix_t fix_create_immed(uint64_t pos, ...)
{
    va_list args;
    va_start(args, pos);
    snum_t snum = snum_create_variadic(&args);
    return fix_create(snum, pos);
}

#endif



void fix_display_dec(char tag[], fix_t fix)
{
    CLU_IS_SAFE(fix.snum.num.head);
    
    printf("\n%s: %c\t", tag, fix.snum.signal == NEGATIVE ? '-' : '+');

    num_t num_hi, num_lo;
    num_break(&num_hi, &num_lo, num_copy(fix.snum.num), fix.pos);
    
    if(num_hi.count == 0)
    {
        printf("0");
    }
    else
    {    
        num_hi = num_base_to(num_hi, 1000000000000000000);
        printf(U64P(), num_hi.tail->value);
        for(chunk_p chunk = num_hi.tail->prev; chunk; chunk = chunk->prev)
            printf(U64P(018), chunk->value);
    }

    printf(".");

    if(num_lo.count == 0)
    {
        printf("0");
    }
    else
    {    
        uint64_t pos = fix.pos;
        num_t num_u = num_wrap(1);
        for(uint64_t i=0; ; i++)
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
    }
}

void fix_display(fix_t fix)
{
    printf("%lu\t", fix.pos);
    snum_display(fix.snum, false);
}

void fix_display_tag(char tag[], fix_t fix)
{
    snum_display_tag(tag, fix.snum);
}

void fix_display_full(char tag[], fix_t fix)
{
    snum_display_full(tag, fix.snum);
}



fix_t fix_create(snum_t snum, uint64_t pos) // TODO test
{
    return (fix_t)
    {
        .snum = snum,
        .pos = pos
    };
}

fix_t fix_wrap(int64_t value, uint64_t pos) // TODO test
{
    snum_t snum = snum_wrap(value);
    if(value) snum = snum_shl(snum, pos << 6);
    return fix_create(snum, pos);
}

fix_t fix_copy(fix_t fix) // TODO test
{
    snum_t snum = snum_copy(fix.snum);
    return fix_create(snum, fix.pos);
}

void fix_free(fix_t fix)
{
    snum_free(fix.snum);
}



fix_t fix_reposition(fix_t fix, uint64_t pos) // TODO test
{
    if(fix.snum.num.count == 0)
        return (fix_t)
        {
            .snum = fix.snum,
            .pos = pos
        };

    if(pos > fix.pos)
        return (fix_t)
        {
            .snum = snum_shl(fix.snum, (pos - fix.pos) << 6),
            .pos = pos
        };

    return (fix_t)
    {
        .snum = snum_shr(fix.snum, (fix.pos - pos) << 6),
        .pos = pos
    };
}



fix_t fix_base_to(fix_t fix, uint64_t base) // TODO test
{
    num_t num_hi, num_lo;
    num_break(&num_hi, &num_lo, fix.snum.num, fix.pos);

    num_hi = num_base_to(num_hi, base);
    
    uint64_t pos = fix.pos;
    fix.pos = 0;
    num_t num_u = num_wrap(1);
    for(uint64_t i=0; ; i++)
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

    fix.snum.num = num_hi;
    return fix;
}



int64_t fix_cmp(fix_t fix_1, fix_t fix_2) // TODO test
{
    return snum_cmp(fix_1.snum, fix_2.snum);
}



fix_t fix_shl(fix_t fix, uint64_t value) // TODO test
{
    fix.snum = snum_shl(fix.snum, value);
    return fix;
}

fix_t fix_shr(fix_t fix, uint64_t value) // TODO test
{
    fix.snum = snum_shr(fix.snum, value);
    return fix;
}



fix_t fix_add(fix_t fix_1, fix_t fix_2) // TODO test
{
    fix_1.snum = snum_add(fix_1.snum, fix_2.snum);
    return fix_1;
}

fix_t fix_sub(fix_t fix_1, fix_t fix_2) // TODO test
{
    fix_1.snum = snum_sub(fix_1.snum, fix_2.snum);
    return fix_1;
}

fix_t fix_mul(fix_t fix_1, fix_t fix_2) // TODO test
{
    fix_1.snum = snum_mul(fix_1.snum, fix_2.snum);
    fix_1.snum = snum_shr(fix_1.snum, fix_1.pos << 6);
    return fix_1;
}

fix_t fix_sqr(fix_t fix) // TODO test
{
    fix.snum = snum_sqr(fix.snum);
    fix.snum = snum_shr(fix.snum, fix.pos << 6);
    return fix;
}

fix_t fix_div(fix_t fix_1, fix_t fix_2) // TODO test
{
    fix_1.snum = snum_shl(fix_1.snum, fix_1.pos << 6);
    fix_1.snum = snum_div(fix_1.snum, fix_2.snum);
    return fix_1;
}
