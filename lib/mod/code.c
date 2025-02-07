#include <stdio.h>
#include <stdlib.h>

#include "debug.h"

#ifdef DEBUG

#include "../../utils/clu/bin/header.h"

#endif

mod_t mod_create(num_p num, num_p p)
{
    return (mod_t)
    {
        .num = num,
        .p = p
    };
}

mod_t mod_add(mod_t m1, mod_t m2)
{
    num_p num = num_add(m1.num, m2.num, false);
    num = num_mod(num, m1.p);
    return mod_create(num, m1.p);
}
