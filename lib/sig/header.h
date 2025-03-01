#ifndef __SIG_H__
#define __SIG_H__

#include <stdint.h>

#include "../../utils/struct.h"

PLACEHOLDER(sig);

void sig_display(sig_p sig);
void sig_display_tag(char tag[], sig_p sig);

sig_p sig_wrap(int64_t value);
sig_p sig_wrap_str(char str[]);
sig_p sig_copy(sig_p sig);
void sig_free(sig_p sig);

bool sig_is_zero(sig_p sig);
int64_t sig_cmp(sig_p sig_1, sig_p sig_2);

sig_p sig_shl(sig_p sig, uint64_t bits);
sig_p sig_shr(sig_p sig, uint64_t bits);

sig_p sig_opposite(sig_p sig);
sig_p sig_add(sig_p sig_1, sig_p sig_2);
sig_p sig_sub(sig_p sig_1, sig_p sig_2);
sig_p sig_mul(sig_p sig_1, sig_p sig_2);
sig_p sig_div(sig_p sig_1, sig_p sig_2);

#endif
