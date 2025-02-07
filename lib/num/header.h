#ifndef __NUM_H__
#define __NUM_H__

#include <stdint.h>
#include <stdbool.h>

#include "../../utils/struct.h"

PLACEHOLDER(num);

num_p num_wrap(uint64_t value);
num_p num_copy(num_p num);

void num_display(num_p num);
void num_display_immed(char *tag, num_p num);

num_p num_add(num_p num_1, num_p num_2, bool keep);
num_p num_sub(num_p num_1, num_p num_2, bool keep);
num_p num_mul(num_p num_1, num_p num_2);
num_p num_div(num_p num_1, num_p num_2, bool keep);
num_p num_mod(num_p num_1, num_p num_2, bool keep);

#endif
