#ifndef __FIX_H__
#define __FIX_H__

#include <stdint.h>

#include "../../utils/struct.h"

#endif

PLACEHOLDER(fix);

void fix_display_dec(fix_p fix);
void fix_display(fix_p fix);
void fix_display_tag(char tag[], fix_p fix);

fix_p fix_wrap(int64_t value, uint64_t pos);
fix_p fix_copy(fix_p fix);
void fix_free(fix_p fix);

fix_p fix_add(fix_p fix_1, fix_p fix_2);
fix_p fix_sub(fix_p fix_1, fix_p fix_2);
fix_p fix_mul(fix_p fix_1, fix_p fix_2);
fix_p fix_div(fix_p fix_1, fix_p fix_2);
