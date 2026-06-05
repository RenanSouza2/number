#ifndef MOD_H
#define MOD_H

#include <stdbool.h>

#include "../../mods/macros/uint.h" // IWYU pragma: keep

#include "struct.h"

void mod_num_display(char tag[], mod_num_t mod);

mod_num_t mod_num_create(num_p num, num_p p);
mod_num_t mod_num_wrap(uint64_t value, num_p p);
mod_num_t mod_num_copy(mod_num_t mod);
void mod_num_free(mod_num_t mod);

bool mod_num_cmp(mod_num_t mod_1, mod_num_t mod_2);

mod_num_t mod_num_add(mod_num_t mod_1, mod_num_t mod_2);
mod_num_t mod_num_sub(mod_num_t mod_1, mod_num_t mod_2);
mod_num_t mod_num_mul(mod_num_t mod_1, mod_num_t mod_2);
mod_num_t mod_num_sqr(mod_num_t mod);
mod_num_t mod_num_pow(mod_num_t mod, uint64_t value);
mod_num_t mod_num_div(mod_num_t mod_1, mod_num_t mod_2);

#endif
