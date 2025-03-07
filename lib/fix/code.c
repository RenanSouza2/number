#include <stdio.h>
#include <stdlib.h>

#include "debug.h"

#include "../snum/struct.h"
#include "../../utils/assert.h"
#include "../../utils/U64.h"

#ifdef DEBUG

#include "../../utils/clu/header.h"

#endif



void fix_display_dec(fix_p fix)
{
    fix = fix_copy(fix);

    num_p num_h, num_l;
    num_break(&num_h, &num_l, fix->snum->num, fix->pos);

    if(fix->snum->signal == NEGATIVE)
        printf("-");
    else
        printf("+");

    num_display_dec(num_h);
    num_free(num_h);

    printf(".");

    for(uint64_t i=0; !num_is_zero(num_l) && i < fix->pos; i++ )
    {
        num_l = num_mul(num_l, num_wrap(1000000000000000000));
        num_break(&num_h, &num_l, num_l, fix->pos);
        printf(U64P(018), num_unwrap(num_h));
    }
}

void fix_display(fix_p fix)
{
    snum_display(fix->snum);
}

void fix_display_tag(char tag[], fix_p fix)
{
    snum_display_tag(tag, fix->snum);
}



fix_p fix_create(snum_p snum, uint64_t pos) // TODO test
{
    fix_p fix = malloc(sizeof(fix_t));
    assert(fix);

    *fix = (fix_t)
    {
        .snum = snum,
        .pos = pos
    };
    return fix;
}

fix_p fix_wrap(int64_t value, uint64_t pos) // TODO test
{
    snum_p snum = snum_wrap(value);
    snum = snum_shl(snum, pos << 6);
    return fix_create(snum, pos);
}

fix_p fix_copy(fix_p fix) // TODO test
{
    snum_p snum = snum_copy(fix->snum);
    return fix_create(snum, fix->pos);
}

void fix_free(fix_p fix)
{
    snum_free(fix->snum);
    free(fix);
}



fix_p fix_add(fix_p fix_1, fix_p fix_2) // TODO test
{
    fix_1->snum = snum_add(fix_1->snum, fix_2->snum);
    return fix_1;
}

fix_p fix_sub(fix_p fix_1, fix_p fix_2) // TODO test
{
    fix_1->snum = snum_sub(fix_1->snum, fix_2->snum);
    return fix_1;
}

fix_p fix_mul(fix_p fix_1, fix_p fix_2) // TODO test
{
    fix_1->snum = snum_mul(fix_1->snum, fix_2->snum);
    fix_1->snum = snum_shr(fix_1->snum, fix_1->pos << 6);
    return fix_1;
}

fix_p fix_div(fix_p fix_1, fix_p fix_2) // TODO test
{
    fix_1->snum = snum_shl(fix_1->snum, fix_1->pos << 6);
    fix_1->snum = snum_div(fix_1->snum, fix_2->snum);
    return fix_1;
}
