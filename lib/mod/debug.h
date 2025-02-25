#ifndef __MOD_DEBUG_H__
#define __MOD_DEBUG_H__

#include "struct.h"

#ifdef DEBUG

bool mod_immed(mod_p mod, uint64_t n, ...);

#endif

mod_p mod_create(num_p num, num_p p);
void mod_free(mod_p mod);

mod_p mod_add(mod_p mod_1, mod_p mod_2);
mod_p mod_sub(mod_p mod_1, mod_p mod_2);
mod_p mod_mul(mod_p mod_1, mod_p mod_2);
mod_p mod_div(mod_p mod_1, mod_p mod_2);

#endif
