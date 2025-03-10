#ifndef __MOD_H__
#define __MOD_H__

#include "../num/struct.h"
#include "../../utils/struct.h"

PLACEHOLDER(mod);

void mod_display(char tag[], mod_p mod);

mod_p mod_wrap(uint64_t value, num_t p);
mod_p mod_copy(mod_p mod);
void mod_free(mod_p mod);

bool mod_cmp(mod_p mod_a, mod_p mod_b);

mod_p mod_add(mod_p mod_1, mod_p mod_2);
mod_p mod_sub(mod_p mod_1, mod_p mod_2);
mod_p mod_mul(mod_p mod_1, mod_p mod_2);
mod_p mod_div(mod_p mod_1, mod_p mod_2);

#endif
