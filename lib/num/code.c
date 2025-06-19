#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "debug.h"
#include "../../mods/macros/assert.h"
#include "../../mods/macros/U64.h"
#include "../../mods/clu/header.h"

#define CLU_NUM_IS_SAFE(NUM) CLU_HANDLER_IS_SAFE((NUM))



#ifdef DEBUG

num_p num_create_variadic(uint64_t n, va_list *args)
{
    num_p num = num_create(n, n);
    for(uint64_t i=0; i<n; i++)
        num->chunk[n-1-i] = va_arg(*args, uint64_t);

    return num;
}

num_p num_create_immed(uint64_t n, ...)
{
    va_list args;
    va_start(args, n);
    return num_create_variadic(n, &args);
}



// bool int64(int64_t i1, int64_t i2)
// {
//     if(i1 != i2)
//     {
//         printf("\n\n\tINT64 ASSERT ERROR\t| (" D64P() ") (" D64P() ")", i1, i2);
//         return false;
//     }
//
//     return true;
// }

bool uint64(uint64_t u1, uint64_t u2)
{
    if(u1 != u2)
    {
        printf("\n\n\tUINT64 ASSERT ERROR\t| (" U64PX ") (" U64PX ")", u1, u2);
        return false;
    }

    return true;
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



bool num_inner(num_p num_1, num_p num_2)
{
    CLU_NUM_IS_SAFE(num_1);
    CLU_NUM_IS_SAFE(num_2);

    if(num_1->count > num_1->size)
    {
        printf("\n\n\tNUMBER ASSERT ERROR\t| COUNT BIGGER THAN SIZE | " U64P() " " U64P() "", num_1->count, num_1->size);
        return false;
    }

    if(num_1->chunk == NULL)
    {
        printf("\n\n\tNUMBER ASSERT ERROR\t| CHUNK IS NULL");
        return false;
    }

    if(!uint64(num_1->count, num_2->count))
    {
        printf("\n\tNUMBER ASSERT ERROR\t| DIFFERENCE IN LENGTH");
        return false;
    }

    for(uint64_t i=0; i<num_1->count; i++)
    {
        if(!uint64(num_1->chunk[i], num_2->chunk[i]))
        {
            printf("\n\tNUMBER ASSERT ERROR\t| DIFFERENCE IN VALUE " U64P() "", i);
            return false;
        }
    }

    return true;
}

bool num_eq_dbg(num_p num_1, num_p num_2)
{
    CLU_NUM_IS_SAFE(num_1);
    CLU_NUM_IS_SAFE(num_2);

    if(!num_inner(num_1, num_2))
    {
        printf("\n");
        num_display_full("\tnum_1", num_1);
        num_display_full("\tnum_2", num_2);
        return false;
    }

    num_free(num_1);
    num_free(num_2);
    return true;
}

bool num_immed(num_p num, uint64_t n, ...)
{
    va_list args;
    va_start(args, n);
    num_p num_2 = num_create_variadic(n, &args);
    return num_eq_dbg(num, num_2);
}

#endif



uint64_t uint_from_char(char c)
{
    switch (c)
    {
        case '0' ... '9': return c - '0';
        case 'a' ... 'f': return c - 'a' + 10;
        case 'A' ... 'F': return c - 'A' + 10;
    }
    assert(false);
}

uint64_t uint_from_str(char str[], uint64_t size, uint64_t base) // TODO test
{
    uint64_t value = 0;
    for(uint64_t i=0; i<size; i++)
    {
        uint64_t aux = uint_from_char(str[i]);
        assert(aux < base);
        value = value * base + uint_from_char(str[i]);
    }

    return value;
}

uint64_t uint_read(FILE *fp, uint64_t size, uint64_t base)
{
    char str[size];
    for(uint64_t i=0; i<size; i++)
        str[i] = fgetc(fp);

    return uint_from_str(str, size, base);
}



// void num_display_dec(num_t num)
// {
//     CLU_HANDLER_IS_SAFE(num.head);
//
//     if(num.count == 0)
//     {
//         printf("0");
//         return;
//     }
//
//     num = num_base_to(num_copy(num), 1000000000000000000);
//     printf(U64P(), num.tail->value);
//     for(chunk_p chunk = num.tail->prev; chunk; chunk = chunk->prev)
//         printf(U64P(018), chunk->value);
//
//     num_free(num);
// }

void num_display_opts(num_p num, char *tag, bool length, bool full)
{
    CLU_NUM_IS_SAFE(num);

    if(tag)
        printf("\n%s: ", tag);

    if(length)
    {
        if(num->count == 0)
        {
            printf("(    0) | ");
        }
        else
        {
            printf("(" U64P(5) ") | ", num->count);
        }
    }

    if(num->count == 0)
    {
        printf("0");
        return;
    }

    uint64_t max = full ?
        num->count :
        num->count > 4 ?
            4 : num->count;

    for(uint64_t i=0; i<max; i++)
        printf("" U64PX " ", num->chunk[num->count-1-i]);

    if(!full && num->count > 4)
        printf("...");
}

void num_display(num_p num)
{
    CLU_NUM_IS_SAFE(num);

    num_display_opts(num, NULL, true, false);
}

// void num_display_tag(char *tag, num_t num)
// {
//     CLU_HANDLER_IS_SAFE(num.head);
//
//     num_display_opts(num, tag, true, false);
// }

void num_display_full(char *tag, num_p num)
{
    CLU_NUM_IS_SAFE(num);

    num_display_opts(num, tag, true, true);
}



num_p num_create(uint64_t size, uint64_t count)
{
    assert(size >= count);
    size = size ? size : 1;
    chunk_p chunk = calloc(size, sizeof(uint64_t));
    assert(chunk);

    num_p num = malloc(sizeof(num_t));
    *num = (num_t)
    {
        .size = size,
        .count = count,
        .chunk = chunk
    };
    return num;
}

num_p num_expand_to(num_p num, uint64_t target)
{
    CLU_NUM_IS_SAFE(num);

    assert(target >= num->size);

    uint64_t size = target * 2;
    chunk_p chunk = realloc(num->chunk, size * sizeof(uint64_t));
    assert(chunk);

    memset(&chunk[num->size], 0, (size - num->size) * sizeof(uint64_t));
    *num = (num_t)
    {
        .size = size,
        .count = num->count,
        .chunk = chunk
    };
    return num;
}

num_p num_expand(num_p num)
{
    CLU_NUM_IS_SAFE(num);

    return num_expand_to(num, num->size);
}

uint64_t num_chunk_get(num_p num, uint64_t pos)
{
    CLU_NUM_IS_SAFE(num);

    return pos < num->count ? num->chunk[pos] : 0;
}

num_p num_chunk_set(num_p num, uint64_t pos, uint64_t value)
{
    CLU_NUM_IS_SAFE(num);

    if(pos >= num->count && value == 0)
        return num;

    if(pos >= num->size)
        num = num_expand_to(num, pos);

    num->chunk[pos] = value;
    num->count = pos >= num->count ? pos + 1 : num->count;
    return num;
}

num_p num_cut(num_p num, uint64_t pos)  // TODO TEST
{
    assert(num->count >= pos);

    num_p num_res = malloc(sizeof(num_t));
    *num_res = (num_t)
    {
        .size = num->size - pos,
        .count = num->count - pos,
        .chunk = &num->chunk[pos]
    };
    return num_res;
}

// num_t num_insert_tail(num_t num, uint64_t value)
// {
//     CLU_NUM_IS_SAFE(num);
//
//     if(num.count == num.size)
//         num = num_expand(num);
//
//     num.chunk[num.count] = value;
//     num.count++;
//     return num;
// }

// chunk_p num_insert_head(num_p num, uint64_t value)
// {
//     CLU_HANDLER_IS_SAFE(num->head);
//
//     num->count++;
//     num->head = chunk_create(value, num->head, NULL);
//     num->tail = COALESCE(num->tail, num->head);
//
//     return num->head;
// }

// num_t num_remove_head(num_t num)
// {
//     CLU_HANDLER_IS_SAFE(num.head);
//
//     if(num.count == 0)
//         return num;
//
//     chunk_p chunk_head = num.head;
//     num.head = chunk_head->next;
//     chunk_free(chunk_head);
//
//     num.count--;
//     if(num.count == 0)
//         num.tail = NULL;
//     else
//         num.head->prev = NULL;
//
//     return num;
// }

// chunk_p num_denormalize(num_p num, chunk_p chunk)
// {
//     CLU_HANDLER_IS_SAFE(num->head);
//     CLU_HANDLER_IS_SAFE(chunk);
//
//     return COALESCE(chunk, num_insert_tail(num, 0));
// }

// returns true if NUM was not normalized
bool num_normalize(num_p num)
{
    CLU_NUM_IS_SAFE(num);

    if(num->count == 0)
        return false;

    if(num->chunk[num->count-1] != 0)
        return false;

    num->count--;
    return true;
}

num_p num_head_grow(num_p num, uint64_t count) // TODO test
{
    if(num->count == 0)
        return num;

    if(count == 0)
        return num;

    uint64_t size = num->count + count;
    num_p num_res = num_create(size, size);
    memcpy(&num_res->chunk[count], num->chunk, num->count * sizeof(uint64_t));

    num_free(num);
    return num_res;
}

num_p num_head_trim(num_p num, uint64_t count) // TODO test
{
    if(num->count == 0)
        return num;

    if(count == 0)
        return num;

    if(count >= num->count)
    {
        num_free(num);
        return num_create(0, 0);
    }

    uint64_t size = num->count - count;
    num_p num_res = num_create(size, size);
    memcpy(num_res->chunk, &num->chunk[count], size * sizeof(uint64_t));

    num_free(num);
    return num_res;
}

// void num_break(num_p out_num_hi, num_p out_num_lo, num_t num, uint64_t count)
// {
//     CLU_HANDLER_IS_SAFE(num.head);
//
//     if(count == 0)
//     {
//         *out_num_hi = num;
//         *out_num_lo = num_create(0, NULL, NULL);
//         return;
//     }
//
//     if(num.count <= count)
//     {
//         *out_num_hi = num_create(0, NULL, NULL);
//         *out_num_lo = num;
//         return;
//     }
//
//     chunk_p chunk = num.head;
//     for(uint64_t i=0; i<count; i++)
//         chunk = chunk->next;
//
//     num_t num_hi = num_create(num.count - count, chunk, num.tail);
//     num.count = count;
//     num.tail = chunk->prev;
//
//     chunk->prev = NULL;
//     num.tail->next = NULL;
//
//     while(num_normalize(&num));
//
//     *out_num_hi = num_hi;
//     *out_num_lo = num;
// }



num_p num_wrap(uint64_t value)
{
    if(value == 0)
        return num_create(0, 0);

    num_p num = num_create(1, 1);
    return num_chunk_set(num, 0, value);
}

// num_t num_wrap_dec(char str[])
// {
//     uint64_t len = strlen(str);
//
//     uint64_t value = uint_from_str(str, len % 18, 10);
//     num_t num = num_wrap(value);
//     for(uint64_t i=len % 18; i<len; i+=18)
//     {
//         value = uint_from_str(&str[i], 18, 10);
//         num_insert_head(&num, value);
//     }
//     return num_base_from(num, 1000000000000000000);
// }

// num_t num_wrap_hex(char str[])
// {
//     uint64_t len = strlen(str);
//     assert(len > 1 && str[0] == '0' && str[1] == 'x');
//
//     uint64_t size = (len - 2) % 16;
//     uint64_t value = uint_from_str(&str[2], size, 16);
//     num_t num = num_wrap(value);
//     for(uint64_t i = 2 + size; i < len; i += 16)
//     {
//         value = uint_from_str(&str[i], 16, 16);
//         num_insert_head(&num, value);
//     }
//     return num;
// }

// num_t num_wrap_str(char str[])
// {
//     return str[0] == '0' && str[1] == 'x' ?
//         num_wrap_hex(str) : num_wrap_dec(str);
// }

// uint64_t get_ftell(FILE* fp)
// {
//     int64_t res = ftell(fp);
//     assert(res >= 0);
//     return res;
// }

// num_t num_read_dec(char file_name[])
// {
//     FILE *fp = fopen(file_name, "r");
//     assert(fp);
//
//     assert(!fseek(fp, 0, SEEK_END));
//     uint64_t size = get_ftell(fp);
//     assert(!fseek(fp, 0, SEEK_SET));
//
//     uint64_t value = uint_read(fp, size % 18, 10);
//     num_t num = num_wrap(value);
//
//     while((uint64_t)get_ftell(fp) < size)
//     {
//         uint64_t value = uint_read(fp, 18, 10);
//         num_insert_head(&num, value);
//     }
//
//     fclose(fp);
//     return num_base_from(num, 1000000000000000000);
// }

// uint64_t num_unwrap(num_t num)
// {
//     CLU_HANDLER_IS_SAFE(num.head);
//
//     assert(num.count < 2);
//
//     uint64_t value = num.count ? num.head->value : 0;
//     num_free(num);
//
//     return value;
// }

num_p num_copy(num_p num) // TODO TEST
{
    CLU_NUM_IS_SAFE(num);

    num_p num_res = num_create(num->count, num->count);
    memcpy(num_res->chunk, num->chunk, num->count * sizeof(uint64_t));

    return num_res;
}

void num_free(num_p num)
{
    CLU_NUM_IS_SAFE(num);

    free(num->chunk);
    free(num);
}




num_p num_add_uint_offset(num_p num, uint64_t pos, uint64_t value)
{
    CLU_NUM_IS_SAFE(num);

    for(; value; pos++)
    {
        uint64_t cur = num_chunk_get(num, pos);
        cur += value;
        num = num_chunk_set(num, pos, cur);
        value = cur < value;
    }

    return num;
}

num_p num_sub_uint_offset(num_p num, uint64_t pos, uint64_t value)
{
    CLU_NUM_IS_SAFE(num);

    for(; value; pos++)
    {
        assert(pos < num->count);

        uint64_t next = num->chunk[pos] < value;
        num->chunk[pos] -= value;
        value = next;
    }

    return num;
}

/* keeps NUM_1 */
num_p num_add_mul_uint_offset(num_p num_res, uint64_t pos_res, num_p num, uint64_t value)
{
    CLU_NUM_IS_SAFE(num_res);
    CLU_NUM_IS_SAFE(num);

    if(value == 0)
        return num_res;

    for(uint64_t pos=0; pos<num->count; pos++)
    {
        uint128_t u = MUL(num->chunk[pos], value);
        num_res = num_add_uint_offset(num_res, pos_res + pos, LOW(u));
        num_res = num_add_uint_offset(num_res, pos_res + pos + 1, HIGH(u));
    }

    return num_res;
}



num_p num_shl_uint(num_p num, uint64_t bits) // TODO test
{
    CLU_NUM_IS_SAFE(num);
    assert(bits < 64); // TODO test

    if(bits == 0)
        return num;

    uint64_t carry = 0;
    for(uint64_t pos=0; pos<num->count; pos++)
    {
        uint64_t value = num->chunk[pos];
        num->chunk[pos] = (value << bits) | carry;
        carry = value >> (64 - bits);
    }

    if(carry)
        num = num_chunk_set(num, num->count, carry);

    return num;
}

num_p num_shr_uint(num_p num, uint64_t bits) // TODO test
{
    CLU_NUM_IS_SAFE(num);
    assert(bits < 64); // TODO test

    if(bits == 0)
        return num;

    uint64_t carry = 0;
    for(uint64_t pos=num->count-1; pos!=UINT64_MAX; pos--)
    {
        uint64_t value = num->chunk[pos];
        num->chunk[pos] = (value >> bits) | carry;
        carry = value << (64 - bits);
    }
    num_normalize(num);

    return num;
}

/* preserves NUM */
num_p num_add_mul_uint(num_p num_res, num_p num, uint64_t value)
{
    CLU_NUM_IS_SAFE(num_res);
    CLU_NUM_IS_SAFE(num);

    return num_add_mul_uint_offset(num_res, 0, num, value);
}

/* preserves NUM */
num_p num_mul_uint(num_p num, uint64_t value)
{
    CLU_NUM_IS_SAFE(num);

    num_p num_res = num_create(num->count, 0);
    return num_add_mul_uint(num_res, num, value);
}



int64_t num_cmp_offset(num_p num_1, uint64_t pos_1, num_p num_2) // TODO TEST
{
    CLU_NUM_IS_SAFE(num_1);
    CLU_NUM_IS_SAFE(num_2);

    if(num_1->count > num_2->count + pos_1)
        return 1;

    if(num_1->count < num_2->count + pos_1)
        return -1;

    for(uint64_t pos_2=num_2->count-1; pos_2!=UINT64_MAX; pos_2--)
    {
        uint64_t value_1 = num_1->chunk[pos_1 + pos_2];
        uint64_t value_2 = num_2->chunk[pos_2];

        if(value_1 > value_2)
            return 1;

        if(value_1 < value_2)
            return -1;
    }

    return 0;
}

/* keeps NUM_2 */
num_p num_add_offset(num_p num_1, uint64_t pos_1, num_p num_2) // TODO TEST
{
    CLU_NUM_IS_SAFE(num_1);
    CLU_NUM_IS_SAFE(num_2);

    for(uint64_t pos_2=0; pos_2<num_2->count; pos_2++)
        num_1 = num_add_uint_offset(num_1, pos_1 + pos_2, num_2->chunk[pos_2]);

    return num_1;
}

/*
keeps NUM2,
NUM_RES may not be normalized
*/
num_p num_sub_offset(num_p num_1, uint64_t pos_1, num_p num_2)
{
    CLU_NUM_IS_SAFE(num_1);
    CLU_NUM_IS_SAFE(num_2);

    for(uint64_t pos_2 = 0; pos_2<num_2->count; pos_2++)
        num_1 = num_sub_uint_offset(num_1, pos_1 + pos_2, num_2->chunk[pos_2]);

    while(num_1->count > pos_1 && num_normalize(num_1));

    return num_1;
}

/*
returns NUM_2 * R if less then NUM_1,
returns 0 otherwise
R cannot be zero
keeps NUM_1 and NUM_2
*/
num_p num_cmp_mul_uint_offset(
    num_p num_res,
    num_p num_1,
    uint64_t pos_1,
    num_p num_2,
    uint64_t r
)
{
    CLU_NUM_IS_SAFE(num_res);
    CLU_NUM_IS_SAFE(num_1);
    CLU_NUM_IS_SAFE(num_2);

    num_res->count = num_2->count;
    for(uint64_t pos_2=num_2->count-1; pos_2!=UINT64_MAX; pos_2--)
    {
        uint128_t u = MUL(num_2->chunk[pos_2], r);

        num_res = num_add_uint_offset(num_res, pos_2 + 1, HIGH(u));
        num_res = num_add_uint_offset(num_res, pos_2, LOW(u));

        if(num_cmp_offset(num_1, pos_1, num_res) < 0)
        {
            num_res->count = 0;
            return num_res;
        }
    }

    return num_res;
}

/* RES is quocient NUM_1 is remainder */
num_p num_div_mod_sigle(num_p num_1, num_p num_2)
{
    CLU_NUM_IS_SAFE(num_1);
    CLU_NUM_IS_SAFE(num_2);

    uint64_t value_2 = num_2->chunk[0];
    num_p num_q = num_create(num_1->count, 0);
    for(uint64_t pos_q = num_1->count - 1; pos_q != UINT64_MAX; pos_q--)
    {
        if(num_normalize(num_1))
        {
            num_q = num_chunk_set(num_q, pos_q, 0);
            continue;
        }

        uint128_t value_1 = num_1->count > 1 + pos_q ?
            U128_IMMED(num_1->chunk[num_1->count - 1], num_1->chunk[num_1->count - 2]) :
            num_1->chunk[num_1->count - 1];

        uint64_t r = value_1 / value_2;
        num_p num_aux = num_mul_uint(num_2, r);
        num_1 = num_sub_offset(num_1, pos_q, num_aux);
        num_free(num_aux);

        num_q = num_chunk_set(num_q, pos_q, r);
    }
    num_free(num_2);

    return num_q;
}

/* RES is quocient NUM_1 is remainder */
num_p num_div_mod_general(num_p num_1, num_p num_2)
{
    CLU_NUM_IS_SAFE(num_1);
    CLU_NUM_IS_SAFE(num_2);

    num_p num_q = num_create(num_1->count - num_2->count + 1, 0);
    num_p num_aux = num_create(num_2->count+1, 0);
    uint128_t val_2 = num_2->chunk[num_2->count-1];
    for(uint64_t pos_q = num_1->count - num_2->count; pos_q != UINT64_MAX; pos_q--)
    {
        if(num_normalize(num_1))
        {
            num_q = num_chunk_set(num_q, pos_q, 0);
            continue;
        }

        uint64_t r = 0;
        while(num_cmp_offset(num_1, pos_q, num_2) >= 0)
        {
            uint128_t val_1 = num_1->count > num_2->count + pos_q ?
                U128_IMMED(num_1->chunk[num_1->count-1], num_1->chunk[num_1->count-2]) :
                num_1->chunk[num_1->count-1];

            uint128_t tmp = val_1 / val_2;
            uint64_t r_aux = UINT64_MAX < tmp ? UINT64_MAX : tmp;

            num_aux = num_cmp_mul_uint_offset(num_aux, num_1, pos_q, num_2, r_aux);
            if(num_aux->count == 0)
            {
                num_free(num_aux);

                r_aux = val_1 / (val_2 + 1);
                num_aux = num_mul_uint(num_2, r_aux);
            }
            r += r_aux;
            num_1 = num_sub_offset(num_1, pos_q, num_aux);
        }

        num_q = num_chunk_set(num_q, pos_q, r);
    }
    num_free(num_2);
    num_free(num_aux);

    return num_q;
}

/* NUM_R has to be shifted RES bites to the right */
uint64_t num_div_mod_unajusted(
    num_p *out_num_q,
    num_p *out_num_r,
    num_p num_1,
    num_p num_2
)
{
    CLU_NUM_IS_SAFE(num_1);
    CLU_NUM_IS_SAFE(num_2);

    assert(num_2->count);

    if(num_cmp(num_1, num_2) < 0)
    {
        num_free(num_2);
        *out_num_q = num_create(0, 0);
        *out_num_r = num_1;
        return 0;
    }

    if(num_2->count == 1)
    {
        *out_num_q = num_div_mod_sigle(num_1, num_2);
        *out_num_r = num_1;
        return 0;
    }

    uint64_t bits = 0;
    for(uint64_t l = num_2->chunk[num_2->count-1]; l < 0x8000000000000000; l <<= 1)
        bits++;

    num_1 = num_shl_uint(num_1, bits);
    num_2 = num_shl_uint(num_2, bits);

    *out_num_q = num_div_mod_general(num_1, num_2);
    *out_num_r = num_1;
    return bits;
}



bool num_is_zero(num_p num)
{
    CLU_NUM_IS_SAFE(num);

    return num->count == 0;
}

int64_t num_cmp(num_p num_1, num_p num_2) // TODO TEST
{
    CLU_NUM_IS_SAFE(num_1);
    CLU_NUM_IS_SAFE(num_2);

    return num_cmp_offset(num_1, 0, num_2);
}



num_p num_shl(num_p num, uint64_t bits) // TODO TEST
{
    CLU_NUM_IS_SAFE(num);

    if(num->count == 0)
        return num;

    num = num_shl_uint(num, bits & 0x3f);
    return num_head_grow(num, bits >> 6);
}

num_p num_shr(num_p num, uint64_t bits) // TODO TEST
{
    CLU_NUM_IS_SAFE(num);

    if(num->count == 0)
        return num;

    num = num_head_trim(num, bits >> 6);
    return num_shr_uint(num, bits & 0x3f);
}



num_p num_add(num_p num_1, num_p num_2)
{
    CLU_NUM_IS_SAFE(num_1);
    CLU_NUM_IS_SAFE(num_2);

    num_1 = num_add_offset(num_1, 0, num_2);
    num_free(num_2);
    return num_1;
}

num_p num_sub(num_p num_1, num_p num_2)
{
    CLU_NUM_IS_SAFE(num_1);
    CLU_NUM_IS_SAFE(num_2);

    num_1 =  num_sub_offset(num_1, 0, num_2);
    num_free(num_2);
    return num_1;
}

num_p num_mul(num_p num_1, num_p num_2)
{
    CLU_NUM_IS_SAFE(num_1);
    CLU_NUM_IS_SAFE(num_2);

    num_p num_res = num_create(num_1->count + num_2->count, 0);
    for(uint64_t pos_2=0; pos_2<num_2->count; pos_2++)
        num_res = num_add_mul_uint_offset(num_res, pos_2, num_1, num_2->chunk[pos_2]);

    num_free(num_1);
    num_free(num_2);
    return num_res;
}

num_p num_sqr(num_p num)
{
    num_p num_res = num_create(2 * num->count, 0);
    if(num->count == 0)
    {
        num_free(num);
        return num_res;
    }

    for(uint64_t pos=0; pos<num->count; pos++)
    {
        uint64_t value = num->chunk[pos];

        uint128_t u = MUL(value, value);
        num_res = num_add_uint_offset(num_res, 2 * pos, LOW(u));
        num_res = num_add_uint_offset(num_res, 2 * pos + 1, HIGH(u));

        num_p num_tmp = num_cut(num, pos + 1);
        num_res = num_add_mul_uint_offset(num_res, 2 * pos + 1, num_tmp, value << 1);

        if(value >= 0x8000000000000000)
            num_res = num_add_offset(num_res, 2 * pos + 2, num_tmp);

        free(num_tmp);
    }
    num_normalize(num_res);

    num_free(num);
    return num_res;
}

// num_t num_exp(num_t num, uint64_t value) // TODO TEST
// {
//     CLU_HANDLER_IS_SAFE(num.head);
//
//     if(num.count == 0)
//     {
//         assert(value);
//         return num;
//     }
//
//     num_t num_res = num_wrap(1);
//     for(uint64_t mask = 0x8000000000000000; mask; mask >>= 1)
//     {
//         num_res = num_sqr(num_res);
//         if(value & mask)
//             num_res = num_mul(num_res, num_copy(num));
//     }
//     num_free(num);
//     return num_res;
// }

void num_div_mod(num_p *out_num_q, num_p *out_num_r, num_p num_1, num_p num_2)
{
    CLU_NUM_IS_SAFE(num_1);
    CLU_NUM_IS_SAFE(num_2);

    num_p num_q, num_r;
    uint64_t bits = num_div_mod_unajusted(&num_q, &num_r, num_1, num_2);

    *out_num_q = num_q;
    *out_num_r = num_shr_uint(num_r, bits);
}

num_p num_div(num_p num_1, num_p num_2)
{
    CLU_NUM_IS_SAFE(num_1);
    CLU_NUM_IS_SAFE(num_2);

    num_p num_q, num_r;
    num_div_mod_unajusted(&num_q, &num_r, num_1, num_2);
    num_free(num_r);

    return num_q;
}

num_p num_mod(num_p num_1, num_p num_2)
{
    CLU_NUM_IS_SAFE(num_1);
    CLU_NUM_IS_SAFE(num_2);

    num_p num_q, num_r;
    uint64_t bits = num_div_mod_unajusted(&num_q, &num_r, num_1, num_2);
    num_free(num_q);

    return num_shr_uint(num_r, bits);
}

num_p num_gcd(num_p num_1, num_p num_2) // TODO TEST
{
    while(num_2->count == 0)
    {
        num_p num_aux = num_mod(num_1, num_copy(num_2));
        num_1 = num_2;
        num_2 = num_aux;
    }
    return(num_1);
}



// num_t num_base_to(num_t num, uint64_t base)
// {
//     CLU_HANDLER_IS_SAFE(num.head);
//
//     num_t num_res = num_create(0, NULL, NULL);
//     while (num.count)
//     {
//         num_t num_q, num_r;
//         num_div_mod(&num_q, &num_r, num, num_wrap(base));
//         num_insert_tail(&num_res, num_unwrap(num_r));
//         num = num_q;
//     }
//     num_free(num);
//     return num_res;
// }

// num_t num_base_from(num_t num, uint64_t base)
// {
//     CLU_HANDLER_IS_SAFE(num.head);
//
//     num_t num_res = num_create(0, NULL, NULL);
//     for(chunk_p chunk = num.tail; chunk; chunk = chunk->prev)
//     {
//         assert(chunk->value < base);
//         num_t num_aux = num_add_mul_uint(num_wrap(chunk->value), num_res, base);
//         num_free(num_res);
//         num_res = num_aux;
//     }
//     num_free(num);
//     return num_res;
// }
