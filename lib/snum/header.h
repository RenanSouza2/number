#ifndef __SIG_H__
#define __SIG_H__

#include <stdint.h>
#include <stdbool.h>

#include "../../utils/struct.h"

PLACEHOLDER(snum);

void snum_display(snum_p snum);
void snum_display_tag(char tag[], snum_p snum);

snum_p snum_wrap(int64_t value);
snum_p snum_wrap_str(char str[]);
snum_p snum_copy(snum_p snum);
void snum_free(snum_p snum);

bool snum_is_zero(snum_p snum);
int64_t snum_cmp(snum_p snum_1, snum_p snum_2);

snum_p snum_shl(snum_p snum, uint64_t bits);
snum_p snum_shr(snum_p snum, uint64_t bits);

snum_p snum_opposite(snum_p snum);
snum_p snum_add(snum_p snum_1, snum_p snum_2);
snum_p snum_sub(snum_p snum_1, snum_p snum_2);
snum_p snum_mul(snum_p snum_1, snum_p snum_2);
snum_p snum_div(snum_p snum_1, snum_p snum_2);

#endif
