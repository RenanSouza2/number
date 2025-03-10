#ifndef __FIX_STRUCT_H__
#define __FIX_STRUCT_H__

#include <stdint.h>

#include "header.h"

#include "../snum/struct.h"

STRUCT(fix)
{
    snum_t snum;
    uint64_t pos;
};

PLACEHOLDER(fix);

void fix_display_dec(fix_t fix);
void fix_display(fix_t fix);
void fix_display_tag(char tag[], fix_t fix);

fix_t fix_wrap(int64_t value, uint64_t pos);
fix_t fix_copy(fix_t fix);
void fix_free(fix_t fix);

fix_t fix_add(fix_t fix_1, fix_t fix_2);
fix_t fix_sub(fix_t fix_1, fix_t fix_2);
fix_t fix_mul(fix_t fix_1, fix_t fix_2);
fix_t fix_div(fix_t fix_1, fix_t fix_2);

#endif
