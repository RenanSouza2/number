#ifndef __MOD_DEBUG_H__
#define __MOD_DEBUG_H__

#include "struct.h"

#ifdef DEBUG

bool mod_immed(mod_p mod, uint64_t n, ...);

#endif

mod_p mod_create(num_t num, num_t p);

#endif
