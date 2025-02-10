#include <stdio.h>
#include <stdlib.h>

#include "debug.h"
#include "../../utils/assert.h"

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



void mod_display(mod_t mod)
{
    num_display(mod.num);printf(" / ");num_display(mod.p);
}



mod_t mod_add(mod_t mod_1, mod_t mod_2)
{
    num_p num = num_add(mod_1.num, mod_2.num, false);
    num = num_mod(num, false, mod_1.p, true);
    return mod_create(num, mod_1.p);
}

mod_t mod_sub(mod_t mod_1, mod_t mod_2)
{
    if(num_cmp(mod_1.num, mod_2.num) >= 0)
    {
        num_p num = num_sub(mod_1.num, false, mod_2.num, false);
        return mod_create(num, mod_1.p);
    }
    
    num_p num = num_add(mod_1.num, mod_1.p, true);
    num = num_sub(num, false, mod_2.num, false);
    return mod_create(num, mod_1.p);
}

mod_t mod_mul(mod_t mod_1, mod_t mod_2)
{
    num_p num = num_mul(mod_1.num, mod_2.num);
    num = num_mod(num, false, mod_1.p, true);
    return mod_create(num, mod_1.p);
}

// mod_t mod_div(mod_t mod_1, mod_t mod_2)
// {
//     assert(mod_2.num);
//
//     num_p num_mod_res = num_mod(mod_1.num, true, mod_2.num, true);
//     if(num_mod_res == NULL)
//     {
//         num_p num = num_div(mod_1.num, false, mod_2.num, false);
//         return mod_create(num, mod_1.p);
//     }
//
//     num_p num_1 = num_mod(mod_1.num, true, mod_2.num, true);
//     num_p num_2 = num_mod(mod_1.p, true, mod_2.num, true);
//     num_2 = num_sub(mod_2.num, true, num_2, false);
// }
