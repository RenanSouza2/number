#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "debug.h"
#include "../../mods/macros/assert.h"
#include "../../mods/macros/U64.h"
#include "../../mods/clu/header.h"



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



bool int64(int64_t i1, int64_t i2)
{
    if(i1 != i2)
    {
        printf("\n\n\tINT64 ASSERT ERROR\t| (" D64P() ") (" D64P() ")", i1, i2);
        return false;
    }

    return true;
}

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
    CLU_HANDLER_IS_SAFE(num_1);
    CLU_HANDLER_IS_SAFE(num_2);
    assert(num_1);
    assert(num_2);

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
            printf("\n\tNUMBER ASSERT ERROR\t| DIFFERENCE IN VALUE | " U64P() "", i);
            return false;
        }
    }

    return true;
}

bool num_eq_dbg(num_p num_1, num_p num_2)
{
    CLU_HANDLER_IS_SAFE(num_1);
    CLU_HANDLER_IS_SAFE(num_2);
    assert(num_1);
    assert(num_2);

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
    exit(EXIT_FAILURE);
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



void num_display_dec(num_p num)
{
    CLU_HANDLER_IS_SAFE(num);
    assert(num);

    if(num->count == 0)
    {
        printf("0");
        return;
    }

    num = num_base_to(num_copy(num), 1000000000000000000);
    printf(U64P(), num->chunk[num->count-1]);
    for(uint64_t pos=num->count-2; pos!=UINT64_MAX; pos--)
        printf(U64P(018), num->chunk[pos]);

    num_free(num);
}

void num_display_opts(num_p num, char *tag, bool length, bool full)
{
    CLU_HANDLER_IS_SAFE(num);
    assert(num);

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
    CLU_HANDLER_IS_SAFE(num);
    assert(num);

    num_display_opts(num, NULL, true, false);
}

void num_display_tag(char *tag, num_p num)
{
    CLU_HANDLER_IS_SAFE(num);
    assert(num);

    num_display_opts(num, tag, true, false);
}

void num_display_full(char *tag, num_p num)
{
    CLU_HANDLER_IS_SAFE(num);
    assert(num);

    num_display_opts(num, tag, true, true);
}



num_p num_create(uint64_t size, uint64_t count)
{
    assert(size >= count);
    size = size ? size : 1;
    num_p num = calloc(1, sizeof(num_t) + size * sizeof(uint64_t));
    assert(num);

    *num = (num_t)
    {
        .size = size,
        .count = count,
        .chunk = (chunk_p)&num[1]
    };
    return num;
}

num_p num_expand_to(num_p num, uint64_t target)
{
    CLU_HANDLER_IS_SAFE(num);
    assert(num);

    assert(target >= num->size);
    uint64_t size = target * 2;
    num = realloc(num, sizeof(num_t) + size * sizeof(uint64_t));
    assert(num);
    uint64_t size_old = num->size;
    *num = (num_t)
    {
        .size = size,
        .count = num->count,
        .chunk = (chunk_p)&num[1]
    };

    memset(&num->chunk[size_old], 0, (size - size_old) * sizeof(uint64_t));
    return num;
}

num_p num_expand(num_p num)
{
    CLU_HANDLER_IS_SAFE(num);
    assert(num);

    return num_expand_to(num, num->size);
}

uint64_t num_chunk_get(num_p num, uint64_t pos)
{
    CLU_HANDLER_IS_SAFE(num);
    assert(num);

    return pos < num->count ? num->chunk[pos] : 0;
}

num_p num_chunk_set(num_p num, uint64_t pos, uint64_t value)
{
    CLU_HANDLER_IS_SAFE(num);
    assert(num);

    if(pos >= num->count && value == 0)
        return num;

    if(pos >= num->size)
        num = num_expand_to(num, pos);

    num->chunk[pos] = value;
    if(pos >= num->count)
        num->count = pos + 1;

    return num;
}

// returns true if NUM had to be corrected
bool num_normalize(num_p num)
{
    CLU_HANDLER_IS_SAFE(num);
    assert(num);

    if(num->count == 0)
        return false;

    if(num->chunk[num->count-1] != 0)
        return false;

    num->count--;
    return true;
}

num_p num_head_grow(num_p num, uint64_t count) // TODO test
{
    CLU_HANDLER_IS_SAFE(num);
    assert(num);

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

void num_break(num_p *out_num_hi, num_p *out_num_lo, num_p num, uint64_t count)
{
    CLU_HANDLER_IS_SAFE(num);
    assert(num);

    if(count == 0)
    {
        *out_num_hi = num;
        *out_num_lo = num_create(0, 0);
        return;
    }

    if(num->count <= count)
    {
        *out_num_hi = num_create(0, 0);
        *out_num_lo = num;
        return;
    }

    uint64_t size = num->count - count;
    num_p num_hi = num_create(size, size);
    memcpy(num_hi->chunk, &num->chunk[count], size * sizeof(uint64_t));

    memset(&num->chunk[count], 0, (num->size - count) * sizeof(uint64_t));
    num->count = count;
    while(num_normalize(num));

    *out_num_hi = num_hi;
    *out_num_lo = num;
}



num_p num_wrap(uint64_t value)
{
    if(value == 0)
        return num_create(0, 0);

    num_p num = num_create(1, 1);
    num->chunk[0] = value;
    return num;
}

num_p num_wrap_dec(char str[])
{
    uint64_t len = strlen(str);
    uint64_t pos = len / 18;
    uint64_t extra = len %18 != 0;
    num_p num = num_create(pos + extra, 0);

    if(extra)
    {
        uint64_t value = uint_from_str(str, len % 18, 10);
        num = num_chunk_set(num, pos, value);
    }

    for(uint64_t i=len % 18; i<len; i+=18)
    {
        uint64_t value = uint_from_str(&str[i], 18, 10);
        pos--;
        num = num_chunk_set(num, pos, value);
    }

    while(num_normalize(num));
    return num_base_from(num, 1000000000000000000);
}

num_p num_wrap_hex(char str[])
{
    uint64_t len = strlen(str);
    assert(len > 1 && str[0] == '0' && str[1] == 'x');

    uint64_t pos = (len - 2) / 16;
    uint64_t extra = (len - 2) % 16 != 0;
    num_p num = num_create(pos + extra, 0);

    if(extra)
    {
        uint64_t value = uint_from_str(&str[2], (len - 2) % 16, 16);
        num = num_chunk_set(num, pos, value);
    }

    for(uint64_t i = 2 + (len - 2) % 16; i < len; i += 16)
    {
        uint64_t value = uint_from_str(&str[i], 16, 16);
        pos--;
        num = num_chunk_set(num, pos, value);
    }

    while(num_normalize(num));
    return num;
}

num_p num_wrap_str(char str[])
{
    return str[0] == '0' && str[1] == 'x' ?
        num_wrap_hex(str) : num_wrap_dec(str);
}

uint64_t get_ftell(FILE* fp)
{
    int64_t res = ftell(fp);
    assert(res >= 0);
    return res;
}

num_p num_read_dec(char file_name[])
{
    FILE *fp = fopen(file_name, "r");
    assert(fp);

    assert(!fseek(fp, 0, SEEK_END));
    uint64_t size = get_ftell(fp);
    assert(!fseek(fp, 0, SEEK_SET));

    uint64_t pos = size / 18;
    uint64_t extra = size % 18;

    num_p num = num_create(pos + extra, 0);
    if(extra)
    {
        uint64_t value = uint_read(fp, size % 18, 10);
        num = num_chunk_set(num, pos, value);
    }

    while((uint64_t)get_ftell(fp) < size)
    {
        uint64_t value = uint_read(fp, 18, 10);
        pos--;
        num = num_chunk_set(num, pos, value);
    }
    fclose(fp);

    while(num_normalize(num));
    return num_base_from(num, 1000000000000000000);
}

uint64_t num_unwrap(num_p num)
{
    CLU_HANDLER_IS_SAFE(num);
    assert(num);

    assert(num->count < 2);
    uint64_t value = num->count ? num->chunk[0] : 0;
    num_free(num);

    return value;
}

num_p num_copy(num_p num) // TODO TEST
{
    CLU_HANDLER_IS_SAFE(num);
    assert(num);

    num_p num_res = num_create(num->count, num->count);
    memcpy(num_res->chunk, num->chunk, num->count * sizeof(uint64_t));

    return num_res;
}

void num_free(num_p num)
{
    CLU_HANDLER_IS_SAFE(num);
    assert(num);

    free(num);
}



num_p num_add_uint_offset(num_p num, uint64_t pos, uint64_t value)
{
    CLU_HANDLER_IS_SAFE(num);
    assert(num);

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
    CLU_HANDLER_IS_SAFE(num);
    assert(num);

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
num_p num_add_mul_uint_offset(
    num_p num_res,
    uint64_t pos_res,
    num_p num,
    uint64_t pos,
    uint64_t value
)
{
    CLU_HANDLER_IS_SAFE(num_res);
    CLU_HANDLER_IS_SAFE(num);
    assert(num_res);
    assert(num);

    if(value == 0)
        return num_res;

    for(uint64_t i=pos; i<num->count; i++)
    {
        uint128_t u = MUL(num->chunk[i], value);
        num_res = num_add_uint_offset(num_res, pos_res + i - pos, LOW(u));
        num_res = num_add_uint_offset(num_res, pos_res + i - pos + 1, HIGH(u));
    }

    return num_res;
}



num_p num_shl_uint(num_p num, uint64_t bits) // TODO test
{
    CLU_HANDLER_IS_SAFE(num);
    assert(num);
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
    CLU_HANDLER_IS_SAFE(num);
    assert(num);
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
    CLU_HANDLER_IS_SAFE(num_res);
    CLU_HANDLER_IS_SAFE(num);
    assert(num_res);
    assert(num);

    return num_add_mul_uint_offset(num_res, 0, num, 0, value);
}

/* preserves NUM */
void num_mul_uint(num_p num_res, num_p num, uint64_t value) // TODO TEST
{
    CLU_HANDLER_IS_SAFE(num);
    assert(num);

    if(value == 0 || num->count == 0)
    {
        memset(num_res->chunk, 0, num_res->size * sizeof(uint64_t));
        num_res->count = 0;
        return;
    }

    num_res->count = num->count + 1;
    num_res->chunk[0] = 0;
    for(uint64_t i=0; i<num->count; i++)
    {
        uint128_t u = MUL(num->chunk[i], value);
        num_res->chunk[i+1] = HIGH(u);
        num_res = num_add_uint_offset(num_res, i, LOW(u));
    }
    num_normalize(num_res);
}



int64_t num_cmp_offset(num_p num_1, uint64_t pos_1, num_p num_2, uint64_t pos_2) // TODO TEST
{
    CLU_HANDLER_IS_SAFE(num_1);
    CLU_HANDLER_IS_SAFE(num_2);
    assert(num_1);
    assert(num_2);

    if(num_1->count - pos_1 > num_2->count - pos_2)
        return 1;

    if(num_1->count - pos_1 < num_2->count - pos_2)
        return -1;

    for(uint64_t i = num_2->count-1; i != pos_2-1; i--)
    {
        uint64_t value_1 = num_1->chunk[pos_1 - pos_2 + i];
        uint64_t value_2 = num_2->chunk[i];

        if(value_1 > value_2)
            return 1;

        if(value_1 < value_2)
            return -1;
    }

    return 0;
}

/* keeps NUM_2 */
num_p num_add_offset(num_p num_1, uint64_t pos_1, num_p num_2, uint64_t pos_2) // TODO TEST
{
    CLU_HANDLER_IS_SAFE(num_1);
    CLU_HANDLER_IS_SAFE(num_2);
    assert(num_1);
    assert(num_2);

    for(uint64_t i=pos_2; i<num_2->count; i++)
        num_1 = num_add_uint_offset(num_1, pos_1 + i - pos_2, num_2->chunk[i]);

    return num_1;
}

/*
keeps NUM2,
NUM_RES may not be normalized
*/
num_p num_sub_offset(num_p num_1, uint64_t pos_1, num_p num_2)
{
    CLU_HANDLER_IS_SAFE(num_1);
    CLU_HANDLER_IS_SAFE(num_2);
    assert(num_1);
    assert(num_2);

    for(uint64_t pos_2 = 0; pos_2<num_2->count; pos_2++)
        num_1 = num_sub_uint_offset(num_1, pos_1 + pos_2, num_2->chunk[pos_2]);

    while(num_1->count > pos_1 && num_normalize(num_1));

    return num_1;
}

/* RES is quocient NUM_1 is remainder */
num_p num_div_mod_sigle(num_p num_1, num_p num_2)
{
    CLU_HANDLER_IS_SAFE(num_1);
    CLU_HANDLER_IS_SAFE(num_2);
    assert(num_1);
    assert(num_2);

    uint64_t value_2 = num_2->chunk[0];
    num_p num_q = num_create(num_1->count, 0);
    num_p num_aux = num_create(num_2->count + 1, 0);
    for(uint64_t i = num_1->count - 1; i != UINT64_MAX; i--)
    {
        if(num_normalize(num_1))
        {
            num_q = num_chunk_set(num_q, i, 0);
            continue;
        }

        uint128_t value_1 = num_1->count > 1 + i ?
            U128_IMMED(num_1->chunk[num_1->count - 1], num_1->chunk[num_1->count - 2]) :
            num_1->chunk[num_1->count - 1];

        uint64_t r = value_1 / value_2;
        num_mul_uint(num_aux, num_2, r);
        num_1 = num_sub_offset(num_1, i, num_aux);

        num_q = num_chunk_set(num_q, i, r);
    }
    num_free(num_2);
    num_free(num_aux);

    return num_q;
}

/* RES is quocient NUM_1 is remainder */
num_p num_div_mod_general(num_p num_1, num_p num_2)
{
    CLU_HANDLER_IS_SAFE(num_1);
    CLU_HANDLER_IS_SAFE(num_2);
    assert(num_1);
    assert(num_2);

    num_p num_q = num_create(num_1->count - num_2->count + 1, 0);
    num_p num_aux = num_create(num_2->count + 1, 0);
    uint128_t val_2 = num_2->chunk[num_2->count-1];
    for(uint64_t i = num_1->count - num_2->count; i != UINT64_MAX; i--)
    {
        if(num_normalize(num_1))
        {
            num_q = num_chunk_set(num_q, i, 0);
            continue;
        }

        uint64_t r = 0;
        while(num_cmp_offset(num_1, i, num_2, 0) >= 0)
        {
            uint128_t val_1 = num_1->count > num_2->count + i ?
            U128_IMMED(num_1->chunk[num_1->count-1], num_1->chunk[num_1->count-2]) :
            num_1->chunk[num_1->count-1];

            uint64_t r_aux = (val_1 == val_2) ? 1 : (val_1 / (val_2 + 1));
            r += r_aux;
            num_1 = num_sub_offset(num_1, i, num_aux);
        }
        num_q = num_chunk_set(num_q, i, r);
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
    CLU_HANDLER_IS_SAFE(num_1);
    CLU_HANDLER_IS_SAFE(num_2);
    assert(num_1);
    assert(num_2);

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
    CLU_HANDLER_IS_SAFE(num);
    assert(num);

    return num->count == 0;
}

int64_t num_cmp(num_p num_1, num_p num_2) // TODO TEST
{
    CLU_HANDLER_IS_SAFE(num_1);
    CLU_HANDLER_IS_SAFE(num_2);

    return num_cmp_offset(num_1, 0, num_2, 0);
}



num_p num_shl(num_p num, uint64_t bits) // TODO TEST
{
    CLU_HANDLER_IS_SAFE(num);
    assert(num);

    if(num->count == 0)
        return num;

    num = num_shl_uint(num, bits & 0x3f);
    return num_head_grow(num, bits >> 6);
}

num_p num_shr(num_p num, uint64_t bits) // TODO TEST
{
    CLU_HANDLER_IS_SAFE(num);
    assert(num);

    if(num->count == 0)
        return num;

    num = num_head_trim(num, bits >> 6);
    return num_shr_uint(num, bits & 0x3f);
}



num_p num_add(num_p num_1, num_p num_2)
{
    CLU_HANDLER_IS_SAFE(num_1);
    CLU_HANDLER_IS_SAFE(num_2);
    assert(num_1);
    assert(num_2);

    num_1 = num_add_offset(num_1, 0, num_2, 0);
    num_free(num_2);
    return num_1;
}

num_p num_sub(num_p num_1, num_p num_2)
{
    CLU_HANDLER_IS_SAFE(num_1);
    CLU_HANDLER_IS_SAFE(num_2);
    assert(num_1);
    assert(num_2);

    num_1 =  num_sub_offset(num_1, 0, num_2);
    num_free(num_2);
    return num_1;
}

num_p num_mul_high(num_p num_1, num_p num_2, uint64_t pos) // TODO TEST
{
    CLU_HANDLER_IS_SAFE(num_1);
    CLU_HANDLER_IS_SAFE(num_2);
    assert(num_1);
    assert(num_2);

    if(pos >= num_1->count + num_2->count)
    {
        num_free(num_1);
        num_free(num_2);
        return num_create(0, 0);
    }

    num_p num_res = num_create(num_1->count + num_2->count - pos, 0);
    uint64_t max = pos > num_2->count ? num_2->count : pos;
    for(uint64_t i=0; i<max; i++)
        num_res = num_add_mul_uint_offset(num_res, 0, num_1, pos-i, num_2->chunk[i]);

    for(uint64_t i=pos; i<num_2->count; i++)
        num_res = num_add_mul_uint_offset(num_res, i-pos, num_1, 0, num_2->chunk[i]);

    num_free(num_1);
    num_free(num_2);
    return num_res;
}

num_p num_mul(num_p num_1, num_p num_2)
{
    CLU_HANDLER_IS_SAFE(num_1);
    CLU_HANDLER_IS_SAFE(num_2);
    assert(num_1);
    assert(num_2);

    return num_mul_high(num_1, num_2, 0);
}

num_p num_sqr(num_p num)
{
    CLU_HANDLER_IS_SAFE(num);
    assert(num);

    if(num->count == 0)
        return num;

    num_p num_res = num_create(2 * num->count + 1, 0);
    for(uint64_t i=0; i<num->count; i++)
    {
        uint64_t value = num->chunk[i];

        uint128_t u = MUL(value, value);
        num_res = num_add_uint_offset(num_res, 2 * i, LOW(u));
        num_res = num_add_uint_offset(num_res, 2 * i + 1, HIGH(u));

        num_res = num_add_mul_uint_offset(num_res, 2 * i + 1, num, i + 1, value << 1);

        if(value >= 0x8000000000000000)
            num_res = num_add_offset(num_res, 2 * i + 2, num, i + 1);
    }

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
    CLU_HANDLER_IS_SAFE(num_1);
    CLU_HANDLER_IS_SAFE(num_2);
    assert(num_1);
    assert(num_2);

    num_p num_q, num_r;
    uint64_t bits = num_div_mod_unajusted(&num_q, &num_r, num_1, num_2);

    *out_num_q = num_q;
    *out_num_r = num_shr_uint(num_r, bits);
}

num_p num_div(num_p num_1, num_p num_2)
{
    CLU_HANDLER_IS_SAFE(num_1);
    CLU_HANDLER_IS_SAFE(num_2);
    assert(num_1);
    assert(num_2);

    num_p num_q, num_r;
    num_div_mod_unajusted(&num_q, &num_r, num_1, num_2);
    num_free(num_r);

    return num_q;
}

num_p num_mod(num_p num_1, num_p num_2)
{
    CLU_HANDLER_IS_SAFE(num_1);
    CLU_HANDLER_IS_SAFE(num_2);
    assert(num_1);
    assert(num_2);

    num_p num_q, num_r;
    uint64_t bits = num_div_mod_unajusted(&num_q, &num_r, num_1, num_2);
    num_free(num_q);

    return num_shr_uint(num_r, bits);
}

num_p num_gcd(num_p num_1, num_p num_2)
{
    CLU_HANDLER_IS_SAFE(num_1);
    CLU_HANDLER_IS_SAFE(num_2);
    assert(num_1);
    assert(num_2);

    while(num_2->count != 0)
    {
        num_p num_aux = num_mod(num_1, num_copy(num_2));
        num_1 = num_2;
        num_2 = num_aux;
    }
    num_free(num_2);
    return(num_1);
}



num_p num_base_to(num_p num, uint64_t base)
{
    CLU_HANDLER_IS_SAFE(num);
    assert(num);

    num_p num_res = num_create(num->count, 0);
    for(uint64_t pos=0; num->count; pos++)
    {
        num_p num_q, num_r;
        num_div_mod(&num_q, &num_r, num, num_wrap(base));
        uint64_t value = num_unwrap(num_r);
        num_res = num_chunk_set(num_res, pos, value);
        num = num_q;
    }
    num_free(num);
    return num_res;
}

num_p num_base_from(num_p num, uint64_t base)
{
    CLU_HANDLER_IS_SAFE(num);
    assert(num);

    num_p num_res = num_create(2 * num->count, 0);
    for(uint64_t pos=num->count-1; pos!=UINT64_MAX; pos--)
    {
        assert(num->chunk[pos] < base);
        num_p num_aux = num_add_mul_uint(num_wrap(num->chunk[pos]), num_res, base);
        num_free(num_res);
        num_res = num_aux;
    }
    num_free(num);
    return num_res;
}
