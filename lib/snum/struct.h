#ifndef __SIG_STRUCT_H__
#define __SIG_STRUCT_H__

#include "../num/struct.h"

#include "header.h"

#define POSITIVE 1
#define NEGATIVE 2
#define ZERO 3

STRUCT(snum)
{
    uint64_t signal;
    num_t num;
};

void snum_display(snum_t snum, bool full);
void snum_display_tag(char tag[], snum_t snum);
void snum_display_full(char tag[], snum_t snum);

snum_t snum_wrap(int64_t value);
snum_t snum_wrap_str(char str[]);
snum_t snum_copy(snum_t snum);
void snum_free(snum_t snum);

bool snum_is_zero(snum_t snum);
int64_t snum_cmp(snum_t snum_1, snum_t snum_2);

snum_t snum_shl(snum_t snum, uint64_t bits);
snum_t snum_shr(snum_t snum, uint64_t bits);

snum_t snum_opposite(snum_t snum);
snum_t snum_add(snum_t snum_1, snum_t snum_2);
snum_t snum_sub(snum_t snum_1, snum_t snum_2);
snum_t snum_mul(snum_t snum_1, snum_t snum_2);
snum_t snum_sqr(snum_t snum);
snum_t snum_div(snum_t snum_1, snum_t snum_2);

#endif
