#include <stdio.h>
#include <stdlib.h>

#include "debug.h"
#include "../../utils/assert.h"
#include "../../utils/clu/header.h"

#ifdef DEBUG

#include "../num/debug.h"

bool mod_immed(mod_t mod, uint64_t n, ...)
{
    va_list args;
    va_start(args, n);
    num_t num = num_create_variadic(n, &args);
    return num_str(mod.num, num);
}

#endif



void mod_display(char tag[], mod_t mod)
{
    printf("\n%s: (", tag);
    num_display_opts(mod.num, NULL, false, true);
    printf(") / (");
    num_display_opts(mod.max, NULL, false, true);
    printf(")");
}



mod_t mod_create(num_t num, num_t max)
{
    return (mod_t)
    {
        .num = num,
        .max = max
    };
}



mod_t mod_wrap(uint64_t value, num_t max)
{
    num_t num = num_wrap(value);
    return mod_create(num, max);
}

mod_t mod_copy(mod_t mod) // TODO test
{
    num_t num = num_copy(mod.num);
    return mod_create(num, mod.max);
}

void mod_free(mod_t mod)
{
    num_free(mod.num);
}

mod_t mod_adjust(mod_t mod) // TODO test
{
    mod.num = num_mod(mod.num, num_copy(mod.max));
    return mod;
}


bool mod_cmp(mod_t mod_1, mod_t mod_2) // TODO test
{
    return num_cmp(mod_1.num, mod_2.num) == 0;
}



mod_t mod_add(mod_t mod_1, mod_t mod_2)
{
    mod_1.num = num_add(mod_1.num, mod_2.num);
    return mod_adjust(mod_1);
}

mod_t mod_sub(mod_t mod_1, mod_t mod_2)
{
    if(num_cmp(mod_1.num, mod_2.num) < 0)
        mod_1.num = num_add(mod_1.num, num_copy(mod_1.max));

    mod_1.num = num_sub(mod_1.num, mod_2.num);
    return mod_1;
}

mod_t mod_mul(mod_t mod_1, mod_t mod_2)
{
    mod_1.num = num_mul(mod_1.num, mod_2.num);
    return mod_adjust(mod_1);
}

mod_t mod_sqr(mod_t mod)
{
    mod.num = num_sqr(mod.num);
    return mod_adjust(mod);
}

mod_t mod_exp(mod_t mod, uint64_t value) // TODO test
{
    DBG_CHECK_PTR(mod.num.head);

    if(mod.num.count == 0)
    {
        assert(value);
        return mod;
    }

    mod_t mod_res = mod_wrap(1, mod.max);
    for(uint64_t mask = (uint64_t)1 << 63; mask; mask >>= 1)
    {
        mod_res = mod_sqr(mod_res);
        if(value & mask)
            mod_res = mod_mul(mod_res, mod_copy(mod));
    }
    mod_free(mod);
    return mod_res;
}
mod_t mod_div(mod_t mod_1, mod_t mod_2)
{
    assert(!num_is_zero(mod_2.num));

    num_t num_q, num_r;
    num_div_mod(&num_q, &num_r, num_copy(mod_1.num), num_copy(mod_2.num));
    if(num_is_zero(num_r))
    {
        num_free(num_r);

        num_t num_t = mod_1.max;
        mod_free(mod_1);
        mod_free(mod_2);
        return mod_create(num_q, num_t);
    }
    num_free(num_q);

    mod_t mod_1_next = mod_create(num_r, mod_2.num);

    num_t num_2 = num_mod(num_copy(mod_1.max), num_copy(mod_2.num));
    num_2 = num_sub(num_copy(mod_2.num), num_2);

    mod_t mod_2_next = mod_create(num_2, mod_2.num);

    mod_t mod_n = mod_div(mod_1_next, mod_2_next);

    num_t num = num_mul(mod_n.num, num_copy(mod_1.max));
    num = num_add(num, mod_1.num);
    num = num_div(num, mod_2.num);

    num_t num_cut = mod_1.max;
    return mod_create(num, num_cut);
}
