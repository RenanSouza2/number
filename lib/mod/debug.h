#ifndef __MOD_DEBUG_H__
#define __MOD_DEBUG_H__

#include "struct.h"

#ifdef DEBUG

bool mod_immed(mod_t mod, uint64_t n, ...);

#endif

mod_t mod_create(num_p num, num_p p);
mod_t mod_wrap(uint64_t value, num_p p);

mod_t mod_add(mod_t m1, mod_t m2);

#endif
