#ifndef __NUM_H__
#define __NUM_H__

#include <stdint.h>
#include <stdbool.h>

#include "../../utils/struct.h"

PLACEHOLDER(num);

void num_display_opts(num_p num, bool length, bool full);
void num_display(num_p num);
void num_display_tag(char *tag, num_p num);
void num_display_full(char *tag, num_p num);

num_p num_wrap(uint64_t value);
num_p num_wrap_dec(char str[]);
num_p num_wrap_hex(char str[]);
num_p num_wrap_str(char str[]);
num_p num_copy(num_p num);
void num_free(num_p num);

bool num_is_zero(num_p num);
int64_t num_cmp(num_p num_1, num_p num_2);

num_p num_add(num_p num_1, num_p num_2);
num_p num_sub(num_p num_1, num_p num_2);
num_p num_mul(num_p num_1, num_p num_2);
void num_div_mod(num_p *out_num_q, num_p *out_num_r, num_p num_1, num_p num_2);
num_p num_div(num_p num_1, num_p num_2);
num_p num_mod(num_p num_1, num_p num_2);
num_p num_shl(num_p num, uint64_t bits);
num_p num_shr(num_p num, uint64_t bits);

#endif
