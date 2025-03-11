#ifndef __MOD_DEBUG_H__
#define __MOD_DEBUG_H__

#include "struct.h"

#ifdef DEBUG

bool mod_immed(mod_t mod, uint64_t n, ...);

#endif

mod_t mod_create(num_t num, num_t p);

#endif
