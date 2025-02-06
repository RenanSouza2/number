#ifndef __NUM_DEBUG_H__
#define __NUM_DEBUG_H__

#include "struct.h"

#ifdef DEBUG

#include <stdbool.h>

num_p num_create_immed(uint64_t n, ...);
void num_display_immed(char *tag, num_p num);
bool num_immed(num_p num, uint64_t n, ...);

#endif

void num_display(num_p num);

num_p num_create(uint64_t value, num_p next);
num_p num_consume(num_p num, bool keep);
void num_free(num_p num);
num_p num_denormalize(num_p num);
num_p num_normalize(num_p num);
num_p num_copy(num_p num);

num_p num_add_uint(num_p n, uint64_t value);
num_p num_sub_uint(num_p num, uint64_t value);
num_p num_mul_uint(num_p num_res, num_p num, uint64_t value);
num_p num_shl(num_p num);
num_p num_shr(num_p num);

num_p num_add(num_p num_1, num_p num_2, bool keep);
num_p num_sub(num_p num_1, num_p num_2, bool keep);
num_p num_mul(num_p num_1, num_p num_2);

int64_t num_cmp(num_p num_1, num_p num_2);

#endif
