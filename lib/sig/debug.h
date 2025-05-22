#ifndef __SIG_DEBUG_H__
#define __SIG_DEBUG_H__

#include "header.h"

#ifdef DEBUG

#include <stdarg.h>

sig_num_t sig_num_create_variadic(uint64_t signal, uint64_t n, va_list *args);
sig_num_t sig_num_create_immed(uint64_t signal, uint64_t n, ...);

bool sig_num_immed(sig_num_t sig, uint64_t signal, uint64_t n, ...);

#endif

sig_num_t sig_num_create(uint64_t signal, num_t num);

#endif
