#ifndef __SIG_DEBUG_H__
#define __SIG_DEBUG_H__

#include "struct.h"

#ifdef DEBUG

snum_p snum_create_immed(uint64_t signal, uint64_t n, ...);

bool snum_immed(snum_p snum, uint64_t signal, uint64_t n, ...);

#endif

#define POSITIVE 1
#define NEGATIVE 2
#define ZERO 3

snum_p snum_create(uint64_t signal, num_p num);

#endif
