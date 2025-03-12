#include <stdio.h>
#include <stdlib.h>

#include "debug.h"

#include "../../utils/assert.h"
#include "../../utils/U64.h"



#ifdef DEBUG

#include "../../utils/clu/header.h"

#endif



void fix_display_dec(fix_t fix)
{
    fix = fix_copy(fix);

    num_t num_h, num_l;
    num_break(&num_h, &num_l, fix.snum.num, fix.pos);

    if(fix.snum.signal == NEGATIVE)
        printf("-");
    else
        printf("+");

    num_display_dec(num_h);
    num_free(num_h);

    printf(".");

    for(uint64_t i=0; !num_is_zero(num_l) && i < fix.pos; i++ )
    {
        num_l = num_mul(num_l, num_wrap(1000000000000000000));
        num_break(&num_h, &num_l, num_l, fix.pos);
        printf(U64P(018), num_unwrap(num_h));
    }
    num_free(num_l);
}

void fix_display(fix_t fix)
{
    snum_display(fix.snum);
}

void fix_display_tag(char tag[], fix_t fix)
{
    snum_display_tag(tag, fix.snum);
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
    snum = snum_shl(snum, pos << 6);
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

fix_t fix_div(fix_t fix_1, fix_t fix_2) // TODO test
{
    fix_1.snum = snum_shl(fix_1.snum, fix_1.pos << 6);
    fix_1.snum = snum_div(fix_1.snum, fix_2.snum);
    return fix_1;
}
