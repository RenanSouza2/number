#include <stdio.h>
#include <stdlib.h>

#include "debug.h"

#ifdef DEBUG

#include "../../utils/clu/bin/header.h"

#include "../num/debug.h"

bool mod_immed(mod_t mod, uint64_t n, ...)
{
    va_list args;
    va_start(args, n);
    num_p num = num_create_variadic(n, args);
    bool res = num_str(mod.num, num);
    num_free(num);
    return res;
}

#endif

mod_t mod_create(num_p num, num_p p)
{
    return (mod_t)
    {
        .num = num,
        .p = p
    };
}

mod_t mod_wrap(uint64_t value, num_p p)
{
    return (mod_t)
    {
        .num = num_wrap(value),
        .p = p
    };
}

void mod_free(mod_t mod)
{
    num_free(mod.num);
}



mod_t mod_add(mod_t mod_1, mod_t mod_2)
{
    num_p num = num_add(mod_1.num, mod_2.num, false);
    num = num_mod(num, mod_1.p, true);
    return mod_create(num, mod_1.p);
}
