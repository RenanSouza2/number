#ifndef __SIG_DEBUG_H__
#define __SIG_DEBUG_H__

#include "header.h"

#ifdef DEBUG

#include <stdarg.h>

sig_num_t sig_num_create_variadic(uint64_t signal, uint64_t n, va_list *args);
sig_num_t sig_num_create_immed(uint64_t signal, uint64_t n, ...);
sig_num_t sig_num_create_rand(uint64_t count);

bool sig_num_keep(sig_num_t sig_1, sig_num_t sig_2);
bool sig_num_eq_dbg(sig_num_t sig_1, sig_num_t sig_2);
bool sig_num_immed(sig_num_t sig, uint64_t signal, uint64_t n, ...);

#endif

#endif
