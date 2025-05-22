#ifndef __SIG_H__
#define __SIG_H__

#include <stdbool.h>

#include "struct.h"

void sig_num_display(sig_num_t sig, bool full);
void sig_num_display_tag(char tag[], sig_num_t sig);
void sig_num_display_full(char tag[], sig_num_t sig);

sig_num_t sig_num_wrap(int64_t value);
sig_num_t sig_num_wrap_str(char str[]);
sig_num_t sig_num_copy(sig_num_t sig);
void sig_num_free(sig_num_t sig);

bool sig_num_is_zero(sig_num_t sig);
int64_t sig_num_cmp(sig_num_t sig_1, sig_num_t sig_2);

sig_num_t sig_num_shl(sig_num_t sig, uint64_t bits);
sig_num_t sig_num_shr(sig_num_t sig, uint64_t bits);

sig_num_t sig_num_opposite(sig_num_t sig);
sig_num_t sig_num_add(sig_num_t sig_1, sig_num_t sig_2);
sig_num_t sig_num_sub(sig_num_t sig_1, sig_num_t sig_2);
sig_num_t sig_num_mul(sig_num_t sig_1, sig_num_t sig_2);
sig_num_t sig_num_sqr(sig_num_t sig);
sig_num_t sig_num_div(sig_num_t sig_1, sig_num_t sig_2);

#endif
