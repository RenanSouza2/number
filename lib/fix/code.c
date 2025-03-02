#include <stdio.h>
#include <stdlib.h>

#include "debug.h"

#ifdef DEBUG

#include "../../utils/clu/header.h"

#endif



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
    fix_1->snum = snum_shl(fix_1->snum, 64 * fix_1->size);
    return fix_1;
}

fix_p fix_div(fix_p fix_1, fix_p fix_2) // TODO test
{
    fix_1->snum = snum_shr(fix_1->snum, 64 * fix_1->size);
    fix_1->snum = snum_sub(fix_1->snum, fix_2->snum);
    return fix_1;
}
