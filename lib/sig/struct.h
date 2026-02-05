#ifndef __SIG_STRUCT_H__
#define __SIG_STRUCT_H__

#include <stdio.h>

#include "../../mods/macros/struct.h"
#include "../../mods/macros/uint.h"

PLACEHOLDER(num)

#define POSITIVE 1
#define NEGATIVE 2
#define ZERO 3

STRUCT(file)
{
    FILE *fp;
    uint64_t amount;
    uint64_t count;
    uint64_t pos;
};

STRUCT(sig_num)
{
    uint64_t signal;
    num_p num;
};

#endif
