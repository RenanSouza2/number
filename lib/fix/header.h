#ifndef __FIX_H__
#define __FIX_H__

#include "struct.h"

void fix_num_display_dec(char tag[], fix_num_t fix);
void fix_num_display(fix_num_t fix);
void fix_num_display_tag(char tag[], fix_num_t fix);
void fix_num_display_full(char tag[], fix_num_t fix);

fix_num_t fix_num_reposition(fix_num_t fix, uint64_t pos);

fix_num_t fix_num_wrap(int64_t value, uint64_t pos);
fix_num_t fix_num_copy(fix_num_t fix);
void fix_num_free(fix_num_t fix);

fix_num_t fix_num_base_to(fix_num_t fix, uint64_t base);

int64_t fix_num_cmp(fix_num_t fix_1, fix_num_t fix_2);

fix_num_t fix_num_shl(fix_num_t fix, uint64_t value);
fix_num_t fix_num_shr(fix_num_t fix, uint64_t value);

fix_num_t fix_num_add(fix_num_t fix_1, fix_num_t fix_2);
fix_num_t fix_num_sub(fix_num_t fix_1, fix_num_t fix_2);
fix_num_t fix_num_mul(fix_num_t fix_1, fix_num_t fix_2);
fix_num_t fix_num_sqr(fix_num_t fix);
fix_num_t fix_num_div(fix_num_t fix_1, fix_num_t fix_2);

#endif
