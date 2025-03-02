#include <stdio.h>
#include <stdlib.h>

#include "debug.h"
#include "../../utils/assert.h"
#include "../../utils/clu/header.h"



#ifdef DEBUG

#include "../num/debug.h"

snum_p snum_create_variadic(uint64_t snumnal, uint64_t n, va_list args)
{
    num_p num = num_create_variadic(n, args);
    return snum_create(snumnal, num);
}

snum_p snum_create_immed(uint64_t snumnal, uint64_t n, ...)
{
    va_list args;
    va_start(args, n);
    return snum_create_variadic(snumnal, n, args);
}



bool snum_inner(snum_p snum_1, snum_p snum_2)
{
    if(!uint64(snum_1->snumnal, snum_2->snumnal))
    {
        printf("\n\tSIG ASSERT ERROR\t| DIFFERENT SIGNAL");
        return false;
    }

    if(!num_str(snum_1->num, snum_2->num))
    {
        printf("\n\tSIG ASSERT ERROR\t| DIFFERENT NUMBER");
        return false;
    }

    return true;
}

bool snum_str(snum_p snum_1, snum_p snum_2)
{
    if(!snum_inner(snum_1, snum_2))
    {
        printf("\n");
        snum_display_tag("\tsnum_1", snum_1);
        snum_display_tag("\tsnum_2", snum_2);
        return false;
    }

    return true;
}

bool snum_immed(snum_p snum, uint64_t snumnal, uint64_t count, ...)
{
    va_list args;
    va_start(args, count);
    snum_p snum_2 = snum_create_variadic(snumnal, count, args);

    bool res = snum_str(snum, snum_2);

    snum_free(snum);
    snum_free(snum_2);
    return res;
}

#endif



void snum_display(snum_p snum)
{
    if(snum->snumnal == ZERO)
    {
        printf("  0");
        return;
    }

    if(snum->snumnal & POSITIVE)
        printf("+ ");
    else
        printf("- ");

    num_display(snum->num);
}

void snum_display_tag(char tag[], snum_p snum)
{
    printf("\n%s: ", tag);
    snum_display(snum);
}



snum_p snum_create(uint64_t snumnal, num_p num)
{
    snum_p snum = malloc(sizeof(snum_t));
    assert(snum);

    if(num_is_zero(num))
        snumnal = ZERO;
    else
        assert(snumnal != ZERO)

    *snum = (snum_t)
    {
        .num = num,
        .snumnal = snumnal
    };
    return snum;
}



snum_p snum_wrap(int64_t value)
{
    if(value == 0)
    {
        num_p num = num_wrap(0);
        return snum_create(ZERO, num);
    }

    if(value < 0)
    {
        num_p num = num_wrap(-value);
        return snum_create(NEGATIVE, num);
    }

    num_p num = num_wrap(value);
    return snum_create(POSITIVE, num);
}

snum_p snum_wrap_str(char str[])
{
    uint64_t snumnal = str[0] == '-' ? NEGATIVE : POSITIVE;
    uint64_t offset = str[0] == '-' || str[0] == '+' ? 1 : 0;
    num_p num = num_wrap_str(&str[offset]);
    return snum_create(snumnal, num);
}

snum_p snum_copy(snum_p snum)
{
    num_p num = num_copy(snum->num);
    return snum_create(snum->snumnal, num);
}

void snum_free(snum_p snum)
{
    DBG_CHECK_PTR(snum);

    num_free(snum->num);
    free(snum);
}


bool snum_is_zero(snum_p snum)
{
    return snum->snumnal == ZERO;
}

int64_t snum_cmp(snum_p snum_1, snum_p snum_2)
{
    if(snum_1->snumnal & POSITIVE)
    {
        if(snum_2->snumnal & POSITIVE)
            return num_cmp(snum_1->num, snum_2->num);

        return 1;
    }

    if(snum_2->snumnal & POSITIVE)
        return -1;

    return -num_cmp(snum_1->num, snum_2->num);
}



snum_p snum_shl(snum_p snum, uint64_t bits) // TODO test
{
    num_shl(snum->num, bits);
    return snum;
}

snum_p snum_shr(snum_p snum, uint64_t bits) // TODO test
{
    num_shr(snum->num, bits);
    if(num_is_zero(snum->num))
        snum->snumnal = ZERO;

    return snum;
}



snum_p snum_opposite(snum_p snum)
{
    DBG_CHECK_PTR(snum);

    snum->snumnal = snum->snumnal >> 1 | (snum->snumnal & POSITIVE) << 1;
    return snum;
}

snum_p snum_add(snum_p snum_1, snum_p snum_2)
{
    DBG_CHECK_PTR(snum_1);
    DBG_CHECK_PTR(snum_2);

    uint64_t snumnal_res = snum_1->snumnal & snum_2->snumnal;
    if(snumnal_res)
    {
        num_p num_res = num_add(snum_1->num, snum_2->num);
        free(snum_1);
        free(snum_2);
        return snum_create(snumnal_res, num_res);
    }

    if(num_cmp(snum_1->num, snum_2->num) > 0)
    {
        num_p num_res = num_sub(snum_1->num, snum_2->num);
        snumnal_res = snum_1->snumnal;
        free(snum_1);
        free(snum_2);
        return snum_create(snumnal_res, num_res);
    }

    num_p num_res = num_sub(snum_2->num, snum_1->num);
    snumnal_res = snum_2->snumnal;
    free(snum_1);
    free(snum_2);
    return snum_create(snumnal_res, num_res);
}

snum_p snum_sub(snum_p snum_1, snum_p snum_2)
{
    snum_2 = snum_opposite(snum_2);
    return snum_add(snum_1, snum_2);
}

snum_p snum_mul(snum_p snum_1, snum_p snum_2)
{
    uint64_t snumnal_res = snum_1->snumnal & snum_2->snumnal ?
        POSITIVE : NEGATIVE;

    num_p num_res = num_mul(snum_1->num, snum_2->num);
    free(snum_1);
    free(snum_2);
    return snum_create(snumnal_res, num_res);
}

snum_p snum_div(snum_p snum_1, snum_p snum_2)
{
    uint64_t snumnal_res = snum_1->snumnal & snum_2->snumnal ?
        POSITIVE : NEGATIVE;

    num_p num_res = num_div(snum_1->num, snum_2->num);
    free(snum_1);
    free(snum_2);
    return snum_create(snumnal_res, num_res);
}
