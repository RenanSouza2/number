#ifndef __NUM_H__
#define __NUM_H__

#include <stdbool.h>

#include "../../mods/macros/struct.h"
#include "../../mods/macros/uint.h"

PLACEHOLDER(num)

void num_display_dec(num_p num);
void num_display_opts(num_p num, char *tag, bool length, bool full);
void num_display(num_p num);
void num_display_tag(char *tag, num_p num);
void num_display_full(char *tag, num_p num);

num_p num_head_grow(num_p num, uint64_t count);
void num_head_trim(num_p num, uint64_t count);
void num_break(num_p *out_num_ih, num_p *out_num_lo,num_p num, uint64_t count);

num_p num_wrap(uint64_t value);
num_p num_wrap_str(char str[]);
num_p num_read_dec(char file_name[]);
uint64_t num_unwrap(num_p num);
num_p num_copy(num_p num);
void num_free(num_p num);

bool num_is_zero(num_p num);
int64_t num_cmp(num_p num_1, num_p num_2);

num_p num_shl(num_p num, uint64_t bits);
num_p num_shr(num_p num, uint64_t bits);
num_p num_add_uint(num_p num, uint64_t value);
num_p num_sub_uint(num_p num, uint64_t value);
num_p num_mul_uint(num_p num, uint64_t value);
num_p num_div_mod_uint(num_p num, uint64_t value);

num_p num_add(num_p num_1, num_p num_2);
num_p num_sub(num_p num_1, num_p num_2);
num_p num_mul_high(num_p num_1, num_p num_2, uint64_t pos);
num_p num_mul(num_p num_1, num_p num_2);
num_p num_pow(num_p num, uint64_t value);
num_p num_sqr(num_p num);
void num_div_mod(num_p *out_num_q, num_p *out_num_r, num_p num_1, num_p num_2);
num_p num_div(num_p num_1, num_p num_2);
num_p num_mod(num_p num_1, num_p num_2);
num_p num_gcd(num_p num_1, num_p num_2);

num_p num_base_to(num_p num, uint64_t base);
num_p num_base_from(num_p num, uint64_t base);

num_p num_div_mod_unbalanced(num_p num_1, num_p num_2);

num_p num_mul_ssm(num_p num_1, num_p num_2);
num_p num_mul_classic(num_p num_1, num_p num_2);

void ssm_get_params_no_wrap(uint64_t res[4], uint64_t count_1, uint64_t count_2);
uint64_t ssm_round_count(uint64_t count);

#endif
