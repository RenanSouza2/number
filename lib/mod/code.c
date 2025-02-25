#include <stdio.h>
#include <stdlib.h>

#include "debug.h"
#include "../../utils/assert.h"

#ifdef DEBUG

#include "../../utils/clu/bin/header.h"

#include "../num/debug.h"

bool mod_immed(mod_p mod, uint64_t n, ...)
{
    va_list args;
    va_start(args, n);
    num_p num = num_create_variadic(n, args);
    bool res = num_str(mod->num, num);
    num_free(num);
    mod_free(mod);
    return res;
}

#endif



void mod_display(char tag[], mod_p mod)
{
    printf("\n%s: (", tag);
    num_display(mod->num);
    printf(") / (");
    num_display(mod->p);
    printf(")");
}



mod_p mod_create(num_p num, num_p p)
{
    DBG_CHECK_PTR(num);
    DBG_CHECK_PTR(p);

    mod_p mod = malloc(sizeof(mod_t));
    assert(mod);

    *mod = (mod_t)
    {
        .num = num,
        .p = p
    };
    return mod;
}

void mod_free(mod_p mod)
{
    DBG_CHECK_PTR(mod);

    num_free(mod->num);
    free(mod);
}



mod_p mod_wrap(uint64_t value, num_p p)
{
    DBG_CHECK_PTR(p);

    num_p num = num_wrap(value);
    return mod_create(num, p);
}



mod_p mod_add(mod_p mod_1, mod_p mod_2)
{
    DBG_CHECK_PTR(mod_1);
    DBG_CHECK_PTR(mod_2);

    num_p num = num_add(mod_1->num, mod_2->num);
    mod_1->num = num_mod(num, num_copy(mod_1->p));

    free(mod_2);
    return mod_1;
}

mod_p mod_sub(mod_p mod_1, mod_p mod_2)
{
    DBG_CHECK_PTR(mod_1);
    DBG_CHECK_PTR(mod_2);
    
    if(num_cmp(mod_1->num, mod_2->num) >= 0)
    {
        mod_1->num = num_sub(mod_1->num, mod_2->num);

        free(mod_2);
        return mod_1;
    }

    num_p num = num_add(mod_1->num, num_copy(mod_1->p));
    mod_1->num = num_sub(num, mod_2->num);

    free(mod_2);
    return mod_1;
}

mod_p mod_mul(mod_p mod_1, mod_p mod_2)
{
    DBG_CHECK_PTR(mod_1);
    DBG_CHECK_PTR(mod_2);

    num_p num = num_mul(mod_1->num, mod_2->num);
    mod_1->num = num_mod(num, num_copy(mod_1->p));

    free(mod_2);
    return mod_1;
}

mod_p mod_div(mod_p mod_1, mod_p mod_2)
{
    DBG_CHECK_PTR(mod_1);
    DBG_CHECK_PTR(mod_2);

    assert(!num_is_zero(mod_2->num));

    num_p num_q, num_r;
    num_div_mod(&num_q, &num_r, num_copy(mod_1->num), num_copy(mod_2->num));
    if(num_is_zero(num_r))
    {
        num_free(num_r);

        num_p num_cut = mod_1->p;
        mod_free(mod_1);
        mod_free(mod_2);
        return mod_create(num_q, num_cut);
    }
    num_free(num_q);

    mod_p mod_1_next = mod_create(num_r, mod_2->num);

    num_p num_2 = num_mod(num_copy(mod_1->p), num_copy(mod_2->num));
    num_2 = num_sub(num_copy(mod_2->num), num_2);

    mod_p mod_2_next = mod_create(num_2, mod_2->num);

    mod_p mod_n = mod_div(mod_1_next, mod_2_next);

    num_p num = num_mul(mod_n->num, num_copy(mod_1->p));
    num = num_add(num, mod_1->num);
    num = num_div(num, mod_2->num);

    num_p num_cut = mod_1->p;
    free(mod_1);
    free(mod_2);
    free(mod_n);
    return mod_create(num, num_cut);
}
