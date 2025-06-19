#ifndef __MOD_H__
#define __MOD_H__

#include <stdbool.h>

#include "../../mods/macros/U64.h"

#include "struct.h"

void mod_num_display(char tag[], mod_num_t mod);

mod_num_t mod_num_wrap(uint64_t value, num_p p);
mod_num_t mod_num_copy(mod_num_t mod);
void mod_num_free(mod_num_t mod);

bool mod_num_cmp(mod_num_t mod_a, mod_num_t mod_b);

mod_num_t mod_num_add(mod_num_t mod_1, mod_num_t mod_2);
mod_num_t mod_num_sub(mod_num_t mod_1, mod_num_t mod_2);
mod_num_t mod_num_mul(mod_num_t mod_1, mod_num_t mod_2);
mod_num_t mod_num_div(mod_num_t mod_1, mod_num_t mod_2);

#endif
