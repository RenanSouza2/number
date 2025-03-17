#ifndef __SIG_DEBUG_H__
#define __SIG_DEBUG_H__

#include "struct.h"

#ifdef DEBUG

snum_t snum_create_immed(uint64_t signal, ...);

bool snum_immed(snum_t snum, uint64_t signal, ...);

#endif

snum_t snum_create(uint64_t signal, num_t num);

#endif
