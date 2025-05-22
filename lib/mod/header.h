#ifndef __MOD_H__
#define __MOD_H__

#include <stdbool.h>

#include "struct.h"

void mod_display(char tag[], mod_t mod);

mod_t mod_wrap(uint64_t value, num_t p);
mod_t mod_copy(mod_t mod);
void mod_free(mod_t mod);

bool mod_cmp(mod_t mod_a, mod_t mod_b);

mod_t mod_add(mod_t mod_1, mod_t mod_2);
mod_t mod_sub(mod_t mod_1, mod_t mod_2);
mod_t mod_mul(mod_t mod_1, mod_t mod_2);
mod_t mod_div(mod_t mod_1, mod_t mod_2);

#endif
