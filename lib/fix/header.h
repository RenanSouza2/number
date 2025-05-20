#ifndef __FIX_H__
#define __FIX_H__

#include "struct.h"

void fix_display_dec(char tag[], fix_t fix);
void fix_display(fix_t fix);
void fix_display_tag(char tag[], fix_t fix);
void fix_display_full(char tag[], fix_t fix);

fix_t fix_reposition(fix_t fix, uint64_t pos);

fix_t fix_wrap(int64_t value, uint64_t pos);
fix_t fix_copy(fix_t fix);
void fix_free(fix_t fix);

fix_t fix_base_to(fix_t fix, uint64_t base);

int64_t fix_cmp(fix_t fix_1, fix_t fix_2);

fix_t fix_shl(fix_t fix, uint64_t value);
fix_t fix_shr(fix_t fix, uint64_t value);

fix_t fix_add(fix_t fix_1, fix_t fix_2);
fix_t fix_sub(fix_t fix_1, fix_t fix_2);
fix_t fix_mul(fix_t fix_1, fix_t fix_2);
fix_t fix_sqr(fix_t fix);
fix_t fix_div(fix_t fix_1, fix_t fix_2);

#endif
