#ifndef __NUM_STRUCT_H__
#define __NUM_STRUCT_H__

#include "header.h"

STRUCT(chunk)
{
    uint64_t value;
    chunk_p next, prev;
};

STRUCT(num)
{
    uint64_t count;
    chunk_p head, tail;
};

void num_display_dec(num_t num);
void num_display_opts(num_t num, char *tag, bool length, bool full);
void num_display(num_t num);
void num_display_tag(char *tag, num_t num);
void num_display_full(char *tag, num_t num);

void num_break(num_p out_num_h, num_p out_num_l, num_t num, uint64_t count);

num_t num_wrap(uint64_t value);
num_t num_wrap_str(char str[]);
num_t num_read_dec(char file_name[]);
uint64_t num_unwrap(num_t num);
num_t num_copy(num_t num);
void num_free(num_t num);

bool num_is_zero(num_t num);
int64_t num_cmp(num_t num_1, num_t num_2);

num_t num_shl(num_t num, uint64_t bits);
num_t num_shr(num_t num, uint64_t bits);
num_t num_exp(num_t num, uint64_t value);

num_t num_add(num_t num_1, num_t num_2);
num_t num_sub(num_t num_1, num_t num_2);
num_t num_mul(num_t num_1, num_t num_2);
void num_div_mod(num_p out_num_q, num_p out_num_r, num_t num_1, num_t num_2);
num_t num_div(num_t num_1, num_t num_2);
num_t num_mod(num_t num_1, num_t num_2);

num_t num_sqr(num_t num);

#endif
