#ifndef __NUMBER_DEBUG_H__
#define __NUMBER_DEBUG_H__

#include "struct.h"

#ifdef DEBUG

#include <stdbool.h>

number_p number_create_immed(uint64_t n, ...);
bool number_immed(number_p num, uint64_t n, ...);

#endif

void number_display(number_p num);

number_p number_create(uint64_t value, number_p next);
void number_free(number_p num);

number_p number_add_uint(number_p n, uint64_t value);

number_p number_add(number_p num_1, number_p num_2);

#endif
