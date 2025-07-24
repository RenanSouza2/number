#include "../../mods/macros/uint.h"



#define PRIME_1         4179340454199820289UL
#define ROOT_W_1        68630377364883UL
#define ROOT_W_INV_1    182099332568824125UL

static uint64_t uint_mod_add(uint64_t a, uint64_t b, uint64_t p)
{
    return (((uint128_t)a) + b) % p;
}

static uint64_t uint_mod_sub(uint64_t a, uint64_t b, uint64_t p)
{
    uint64_t c = a - b;
    if(a < b) c += p;  
    return c % p;
}

static uint64_t uint_mod_mul(uint64_t a, uint64_t b, uint64_t p)
{
    return (((uint128_t)a) * b) % p;
}

static uint64_t uint_mod_pow(uint64_t a, uint64_t b, uint64_t p)
{
    uint64_t res = 1;
    for(uint64_t mask = stdc_bit_ceil(b); mask; mask >>= 1)
    {
        res = uint_mod_mul(res, res, p);
        if(b & mask)
            res = uint_mod_mul(res, a, p);
    }
    return res;
}

static uint64_t uint_mod_div(uint64_t a, uint64_t b, uint64_t p)
{
    if(a % b == 0)
        return a / b;

    uint64_t n = b > (p / 2) ?
        uint_mod_div(b - a%b, p%b, b) :
        uint_mod_div(a%b, b - p%b, b);

    return (((uint128_t)n) * p + a) / b;
}
