#ifndef __NUMBER_DEBUG_H__
#define __NUMBER_DEBUG_H__

#include "struct.h"

#ifdef DEBUG

#include <stdbool.h>

bool number_immed(number_p num, uint64_t n, ...);

#endif

number_p number_create(uint64_t value);
void number_free(number_p num);

number_p number_add_uint(number_p n, uint64_t value);

#endif
