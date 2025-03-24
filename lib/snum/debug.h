#ifndef __SIG_DEBUG_H__
#define __SIG_DEBUG_H__

#include "struct.h"

#ifdef DEBUG

#include <stdarg.h>

snum_t snum_create_variadic(va_list *args);
snum_t snum_create_immed(uint64_t signal, ...);
void snum_create_vec_immed(snum_t snum[], uint64_t n, ...);

bool snum_str(snum_t snum_1, snum_t snum_2);
bool snum_immed(snum_t snum, uint64_t signal, ...);

#endif

snum_t snum_create(uint64_t signal, num_t num);

#endif
