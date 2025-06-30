#ifndef __MOD_DEBUG_H__
#define __MOD_DEBUG_H__

#include "header.h"

#ifdef DEBUG

bool mod_num_immed(mod_num_t mod, uint64_t n, ...);

#endif

mod_num_t mod_num_create(num_p num, num_p p);

#endif
