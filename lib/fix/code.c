#include <stdio.h>
#include <stdlib.h>

#include "debug.h"

#include "../../utils/assert.h"

#ifdef DEBUG

#include "../../utils/clu/header.h"

#endif



void fix_display(fix_p fix)
{
    snum_display(fix->snum);
}

void fix_display_tag(char tag[], fix_p fix)
{
    snum_display_tag(tag, fix->snum);
}



fix_p fix_create(snum_p snum, uint64_t size) // TODO test
{
    fix_p fix = malloc(sizeof(fix_t));
    assert(fix);

    *fix = (fix_t)
    {
        .snum = snum,
        .size = size
    };
    return fix;
}

fix_p fix_wrap(int64_t value, uint64_t size) // TODO test
{
    snum_p snum = snum_wrap(value);
    snum = snum_shl(snum, 64 * size);
    return fix_create(snum, size);
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
    fix_1->snum = snum_shr(fix_1->snum, fix_1->size << 6);
    return fix_1;
}

fix_p fix_div(fix_p fix_1, fix_p fix_2) // TODO test
{
    fix_1->snum = snum_shl(fix_1->snum, fix_1->size << 6);
    fix_1->snum = snum_div(fix_1->snum, fix_2->snum);
    return fix_1;
}
