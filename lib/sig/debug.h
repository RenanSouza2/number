#ifndef __SIG_DEBUG_H__
#define __SIG_DEBUG_H__

#include "struct.h"

#ifdef DEBUG

bool sig_immed(sig_p sig, uint64_t signal, uint64_t count, ...);

#endif

#define POSITIVE 1
#define NEGATIVE 2
#define ZERO 3

sig_p sig_create(uint64_t signal, num_p num);

void sig_free(sig_p sig);

#endif
