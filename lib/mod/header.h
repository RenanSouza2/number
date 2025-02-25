#ifndef __MOD_H__
#define __MOD_H__

#include <stdint.h>

#include "../num/header.h"
#include "../../utils/struct.h"

PLACEHOLDER(mod);

void mod_display(char tag[], mod_p mod);

mod_p mod_wrap(uint64_t value, num_p p);

#endif
