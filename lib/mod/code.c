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



void mod_display(char tag[], mod_t mod)
{
    printf("\n%s: ", tag);
    num_display(mod.num);
    printf(" / ");
    num_display(mod.p);
}



mod_t mod_add(mod_t mod_1, mod_t mod_2)
{
    num_p num = num_add(mod_1.num, mod_2.num);
    num = num_mod(num, num_copy(mod_1.p));
    return mod_create(num, mod_1.p);
}

mod_t mod_sub(mod_t mod_1, mod_t mod_2)
{
    if(num_cmp(mod_1.num, mod_2.num) >= 0)
    {
        num_p num = num_sub(mod_1.num, mod_2.num);
        return mod_create(num, mod_1.p);
    }
    
    num_p num = num_add(mod_1.num, num_copy(mod_1.p));
    num = num_sub(num, mod_2.num);
    return mod_create(num, mod_1.p);
}

mod_t mod_mul(mod_t mod_1, mod_t mod_2)
{
    num_p num = num_mul(mod_1.num, mod_2.num);
    num = num_mod(num, num_copy(mod_1.p));
    return mod_create(num, mod_1.p);
}

mod_t mod_div_rec(mod_t mod_1, mod_t mod_2)
{
    assert(mod_2.num);

    num_p num_q, num_r;
    num_div_mod(&num_q, &num_r, num_copy(mod_1.num), num_copy(mod_2.num));
    if(num_r == NULL)
        return mod_create(num_q, mod_1.p);
        
    num_free(num_q);

    mod_t mod_1_next = mod_create(num_r, mod_2.num);

    num_p num_2 = num_mod(num_copy(mod_1.p), num_copy(mod_2.num));
    num_2 = num_sub(num_copy(mod_2.num), num_2);
    mod_t mod_2_next = mod_create(num_2, mod_2.num);

    mod_t mod_n = mod_div_rec(mod_1_next, mod_2_next);
    mod_free(mod_1_next);
    mod_free(mod_2_next);

    num_p num = num_mul(mod_n.num, num_copy(mod_1.p));
    num = num_add(num, num_copy(mod_1.num));
    num = num_div(num, num_copy(mod_2.num));
    return mod_create(num, mod_1.p);
}

mod_t mod_div(mod_t mod_1, mod_t mod_2)
{
    mod_t mod = mod_div_rec(mod_1, mod_2);
    mod_free(mod_1);
    mod_free(mod_2);
    return mod;
}
