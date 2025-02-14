#include <stdio.h>
#include <stdlib.h>

#include "debug.h"
#include "../../utils/assert.h"
#include "../../utils/U64.h"

#ifdef DEBUG

#include "../../utils/clu/bin/header.h"

num_p num_create_variadic(uint64_t n, va_list args)
{
    num_p num = NULL;
    for(uint64_t i=0; i<n; i++)
    {
        uint64_t value = va_arg(args, uint64_t);
        num = num_create(value, num);
    }
    return num;
}

num_p num_create_immed(uint64_t n, ...)
{
    va_list args;
    va_start(args, n);
    return num_create_variadic(n, args);
}



bool uint64(uint64_t u1, uint64_t u2)
{
    if(u1 == u2)
        return true;

    printf("\n\n\tUINT64 ASSERT ERROR\t| " U64PX " " U64PX "", u1, u2);
    return false;
}

bool uint128_immed(uint128_t u1, uint64_t v2h, uint64_t v2l)
{
    if(!uint64(HIGH(u1), v2h))
    {
        printf("\n\tUINT128 ASSERT ERROR\t| HIGH");
        return false;
    }

    if(!uint64(LOW(u1), v2l))
    {
        printf("\n\tUINT128 ASSERT ERROR\t| LOW");
        return false;
    }

    return true;
}



bool num_str_rec(num_p num_1, num_p num_2, uint64_t index)
{
    if(num_1 == NULL)
    {
        if(num_2)
        {
            printf("\n\n\tNUMBER ASSERT ERROR\t| NUMBER SHORTER THAN EXPECTED");
            return false;
        }

        return true;
    }

    if(num_2 == NULL)
    {
        printf("\n\n\tNUMBER ASSERT ERROR\t| NUMBER LONGER THAN EXPECTED");
        return false;
    }

    if(!uint64(num_1->value, num_2->value))
    {
        printf("\n\tNUMBER ASSERT ERROR\t| DIFFERENCE IN VALUE " U64P "", index);
        return false;
    }

    return num_str_rec(num_1->next, num_2->next, index+1);
}

bool num_str(num_p num_1, num_p num_2)
{
    if(num_str_rec(num_1, num_2, 0))
        return true;

    num_display_immed("\n\tnum_1", num_1);
    num_display_immed("\tnum_2", num_2);
    return false;
}

bool num_immed(num_p num, uint64_t n, ...)
{
    va_list args;
    va_start(args, n);
    num_p num_2 = num_create_variadic(n, args);
    bool res = num_str(num, num_2);
    num_free(num_2);
    return res;
}

#endif


uint64_t num_count(num_p num);

void num_display_rec(num_p num)
{
    if(num == NULL)
        return;

    num_display_rec(num->next);
    printf("" U64PX " ", num->value);
}

void num_display_rec_2(num_p num, uint64_t count)
{
    bool bigger = count > 4;
    for(; count > 4; count--)
        num = num->next;

    num_display_rec(num);
    if(bigger) printf(" ...");
}

void num_display(num_p num)
{
    if(num == NULL)
    {
        printf("0 | 0");
        return;
    }

    uint64_t count = num_count(num);
    printf("" U64P " | ", count);
    num_display_rec_2(num, count);
}

void num_display_immed(char *tag, num_p num)
{
    printf("\n%s: ", tag);num_display(num);
}



num_p num_create(uint64_t value, num_p next)
{
    num_p num = malloc(sizeof(num_t));
    assert(num);
    
    *num = (num_t){value, next};
    return num;
}

num_p num_wrap(uint64_t value)
{
    return value ? num_create(value, NULL) : NULL;
}

/* free NUM struct and return next */
num_p num_consume(num_p num)
{
    if(num == NULL)
        return NULL;

    num_p num_next = num->next;
    free(num);
    return num_next;
}

/* free NUM list */
void num_free(num_p num)
{
    while(num)
        num = num_consume(num);
}

/* creates a num struct with value 0 if NUM is null */
num_p num_denormalize(num_p num)
{
    if(num) return num;

    return num_create(0, NULL);
}

/* frees num if NUm->value is zero and NUM->next is NULL */
num_p num_normalize(num_p num)
{
    if(num == NULL || num->value != 0 || num->next != NULL)
        return num;
        
    free(num);
    return NULL;
}

num_p num_copy(num_p num)
{
    if(num == NULL)
        return NULL;

    num_p num_2 = num_copy(num->next);
    return num_create(num->value, num_2);
}



uint64_t num_count(num_p num) // TODO test
{
    uint64_t i;
    for(i=0; num; num = num->next, i++);
    return i;
}

uint64_t num_get_last(num_p num) // TODO test
{
    if(num == NULL)
        return 0;

    while(num->next)
        num = num->next;

    return num->value;
}

uint128_t num_get_last_2(num_p num) // TODO test
{
    assert(num && num->next);

    while(num->next->next)
        num = num->next;

    return U128_IMMED(num->next->value, num->value);
}



num_p num_add_uint(num_p num, uint64_t value)
{
    if(value == 0)
        return num;

    if(num == NULL)
        return num_create(value, NULL);

    num->value += value;
    if(num->value < value)
        num->next = num_add_uint(num->next, 1);

    return num;
}

num_p num_sub_uint(num_p num, uint64_t value)
{
    if(value == 0)
        return num;

    assert(num);

    bool do_next = num->value < value;
    num->value -= value;
    if(do_next)
        num->next = num_sub_uint(num->next, 1);

    return num_normalize(num);
}

num_p num_mul_uint_rec(num_p num_res, num_p num, uint64_t value)
{
    if(num == NULL)
        return num_res;

    uint128_t u = MUL(num->value, value);
    num_res = num_add_uint(num_res, LOW(u));
    num_res = num_denormalize(num_res);
    num_res->next = num_add_uint(num_res->next, HIGH(u));
    num_res->next = num_mul_uint_rec(num_res->next, num->next, value);
    return num_res;
}

/* preserves NUM */
num_p num_mul_uint(num_p num_res, num_p num, uint64_t value)
{
    if(value == 0)
        return num_res;

    return num_mul_uint_rec(NULL, num, value);
}

num_p num_shl_rec(num_p num, bool carry)
{
    if(num == NULL)
        return carry ? num_create(1, NULL) : NULL;

    bool carry_next = num->value >> 63;
    num->value = num->value << 1 | carry;
    num->next = num_shl_rec(num->next, carry_next);
    return num;
}

num_p num_shl(num_p num)
{
    return num_shl_rec(num, false);
}

num_p num_shr(num_p num)
{
    if(num == NULL)
        return NULL;

    uint64_t carry = num->next ? num->next->value & 1 : 0;
    num->value = num->value >> 1 | carry << 63;
    num->next = num_shr(num->next);
    return num_normalize(num);
}



int64_t num_cmp(num_p num_1, num_p num_2)
{
    if(num_1 == NULL)
        return num_2 ? -1 : 0;

    if(num_2 == NULL)
        return 1;

    int64_t next = num_cmp(num_1->next, num_2->next);
    if(next != 0) return next;

    if(num_1->value > num_2->value) return 1;
    return num_1->value < num_2->value ? -1 : 0;
}



num_p num_add(num_p num_1, num_p num_2)
{
    if(num_1 == NULL) return num_2;
    if(num_2 == NULL) return num_1;

    num_add_uint(num_1, num_2->value);
    
    num_2 = num_consume(num_2);
    num_1->next = num_add(num_1->next, num_2);
    return num_1;
}

num_p num_sub(num_p num_1, num_p num_2)
{
    if(num_2 == NULL) return num_1;
    assert(num_1);

    num_1 = num_sub_uint(num_1, num_2->value);
    
    num_1 = num_denormalize(num_1);
    num_2 = num_consume(num_2);
    num_1->next = num_sub(num_1->next, num_2);
    return num_normalize(num_1);
}

num_p num_mul_rec(num_p num_res, num_p num_1, num_p num_2)
{
    if(num_2 == NULL)
    {
        num_free(num_1);
        return num_res;
    }

    num_res = num_mul_uint_rec(num_res, num_1, num_2->value);

    num_res = num_denormalize(num_res);
    num_2 = num_consume(num_2);
    num_res->next = num_mul_rec(num_res->next, num_1, num_2);
    return num_res;
}

num_p num_mul(num_p num_1, num_p num_2)
{
    if(num_1 == NULL)
    {
        num_free(num_2);
        return NULL;
    }

    return num_mul_rec(NULL, num_1, num_2);
}

num_p num_div_mod_rec(
    num_p *out_num_q, 
    num_p num_1,
    num_p num_2, 
    uint64_t cnt_1,
    uint64_t cnt_2, 
    uint64_t val_2
)
{
    if(cnt_1 <= cnt_2+1)
    if(num_cmp(num_1, num_2) < 0)
    {
        *out_num_q = NULL;
        return num_1;
    }

    num_p num_q;
    num_1->next = num_div_mod_rec(&num_q, num_1->next, num_2, cnt_1-1, cnt_2, val_2);
    num_1 = num_normalize(num_1);

    uint64_t r =  0;
    while(num_cmp(num_1, num_2) >= 0)
    {
        cnt_1 = num_count(num_1);
        uint64_t r_max, r_min;
        if(cnt_1 > cnt_2)
        {
            uint128_t val_1 = num_get_last_2(num_1);
            r_max = val_1 / val_2;
            r_min = val_1 / (U128(val_2) + 1);
        }
        else
        {
            uint64_t val_1 = num_get_last(num_1);
            r_max = val_1 / val_2;
            r_min = val_1 / (U128(val_2) + 1);
        }

        num_p num_aux = num_mul_uint(NULL, num_2, r_max);
        if(num_cmp(num_aux, num_1) > 0)
        {
            num_free(num_aux);
            num_aux = num_mul_uint(NULL, num_2, r_min);
            r_max = r_min;
        }
        num_1 = num_sub(num_1, num_aux);
        r += r_max;
    }

    *out_num_q = num_create(r, num_q);
    return num_1;
}

void num_div_mod(num_p *out_num_q, num_p *out_num_r, num_p num_1, num_p num_2)
{
    assert(num_2);

    uint64_t cnt_1 = num_count(num_1);
    uint64_t cnt_2 = num_count(num_2);
    uint64_t val_2 = num_get_last(num_2);
    
    num_p num_q;
    num_p num_r = num_div_mod_rec(&num_q, num_1, num_2, cnt_1, cnt_2, val_2);
    num_free(num_2);
    
    *out_num_q = num_q;
    *out_num_r = num_r;
}

num_p num_div(num_p num_1, num_p num_2)
{
    num_p num_q, num_r;
    num_div_mod(&num_q, &num_r, num_1, num_2);
    num_free(num_r);
    return num_q;
}

num_p num_mod(num_p num_1, num_p num_2)
{
    num_p num_q, num_r;
    num_div_mod(&num_q, &num_r, num_1, num_2);
    num_free(num_q);
    return num_r;
}
