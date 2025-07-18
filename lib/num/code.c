#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbit.h>

#include "debug.h"
#include "fft.c"
#include "../../mods/macros/assert.h"
#include "../../mods/macros/uint.h"
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
        value = value * base + aux;
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

uint64_t uint_inv(uint64_t value, uint64_t q) // TODO TEST
{
    uint64_t res = 0;
    for(uint64_t i=0; i<q; i++)
    {
        res <<= 1;
        res |= value & 1;
        value >>= 1;
    }
    return res;
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
    for(uint64_t i=num->count-2; i!=UINT64_MAX; i--)
        printf(U64P(018), num->chunk[i]);

    num_free(num);
}

void num_display_opts(num_p num, char *tag, bool length, bool full)
{
    CLU_HANDLER_IS_SAFE(num);
    assert(num);

    if(tag)
        printf("\n%s\t: ", tag);

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

    if(target < num->size)
        return num;

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

    uint64_t count_res = num->count + count;
    num = num_expand_to(num, count_res);
    for(uint64_t i=num->count-1; i!=UINT64_MAX; i--)
        num->chunk[i+count] = num->chunk[i];
    

    memset(num->chunk, 0, count * sizeof(uint64_t));
    num->count = count_res;
    return num;
}

num_p num_head_trim(num_p num, uint64_t count) // TODO test
{
    if(num->count == 0)
        return num;

    if(count == 0)
        return num;

    if(count >= num->count)
    {
        memset(num->chunk, 0, num->count * sizeof(uint64_t));
        num->count = 0;
        return num;
    }

    uint64_t count_res = num->count - count;
    for(uint64_t i=count; i<num->count; i++)
        num->chunk[i-count] = num->chunk[i];

    memset(&num->chunk[num->count - count], 0, count * sizeof(uint64_t));
    num->count = count_res;
    return num;
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

num_p num_join(num_p num_hi, num_p num_lo, uint64_t count) // TODO TEST
{
    CLU_HANDLER_IS_SAFE(num_hi);
    CLU_HANDLER_IS_SAFE(num_lo);
    assert(num_hi);
    assert(num_lo);

    if(num_hi->count == 0)
    {
        num_free(num_hi);
        return num_lo;
    }

    if(num_lo->count == 0 && count == 0)
    {
        num_free(num_lo);
        return num_hi;
    }

    count = count > num_lo->count ? count : num_lo->count;
    uint64_t count_res = count + num_hi->count;
    num_lo = num_expand_to(num_lo, count_res);
    memcpy(&num_lo->chunk[count], num_hi->chunk, num_hi->count * sizeof(uint64_t));
    num_lo->count = count_res;

    num_free(num_hi);
    return num_lo;
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

    while(num_normalize(num));
    return num;
}

/* keeps NUM */
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

num_p num_shl_inner(num_p num, uint64_t bits) // TODO test
{
    CLU_HANDLER_IS_SAFE(num);
    assert(num);
    assert(bits < 64); // TODO test

    if(bits == 0)
        return num;

    uint64_t carry = 0;
    for(uint64_t i=0; i<num->count; i++)
    {
        uint64_t value = num->chunk[i];
        num->chunk[i] = (value << bits) | carry;
        carry = value >> (64 - bits);
    }

    if(carry)
        num = num_chunk_set(num, num->count, carry);

    return num;
}

num_p num_shr_inner(num_p num, uint64_t bits) // TODO test
{
    CLU_HANDLER_IS_SAFE(num);
    assert(num);
    assert(bits < 64); // TODO test

    if(bits == 0)
        return num;

    uint64_t carry = 0;
    for(uint64_t i=num->count-1; i!=UINT64_MAX; i--)
    {
        uint64_t value = num->chunk[i];
        num->chunk[i] = (value >> bits) | carry;
        carry = value << (64 - bits);
    }
    num_normalize(num);

    return num;
}

/* preserves NUM */
void num_mul_uint_inner(num_p num_res, num_p num, uint64_t value) // TODO TEST
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




int64_t num_cmp_offset(num_p num_1, uint64_t pos_1, num_p num_2) // TODO TEST
{
    CLU_HANDLER_IS_SAFE(num_1);
    CLU_HANDLER_IS_SAFE(num_2);
    assert(num_1);
    assert(num_2);

    if(num_1->count == 0)
    {
        if(num_2->count == 0)
            return 0;

        if(num_2->count != 0)
            return -1;
    }

    if(num_2->count == 0)
        return 1;

    if(num_1->count > num_2->count + pos_1)
        return 1;

    if(num_1->count < num_2->count + pos_1)
        return -1;

    for(uint64_t i = num_2->count-1; i != UINT64_MAX; i--)
    {
        uint64_t value_1 = num_1->chunk[pos_1 + i];
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

/* keeps NUM2 */
num_p num_sub_offset(num_p num_1, uint64_t pos_1, num_p num_2)
{
    CLU_HANDLER_IS_SAFE(num_1);
    CLU_HANDLER_IS_SAFE(num_2);
    assert(num_1);
    assert(num_2);

    for(uint64_t i = num_2->count-1; i!=UINT64_MAX; i--)
        num_1 = num_sub_uint_offset(num_1, pos_1 + i, num_2->chunk[i]);

    return num_1;
}



num_p num_mul_simple(num_p num_1, num_p num_2)
{
    CLU_HANDLER_IS_SAFE(num_1);
    CLU_HANDLER_IS_SAFE(num_2);
    assert(num_1);
    assert(num_2);

    return num_mul_high(num_1, num_2, 0);
}

num_p num_sqr_simple(num_p num)
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



num_p num_pad(num_p num) // TODO TEST
{
    uint64_t count = num->count;
    for(uint64_t i=count-1; i!=UINT64_MAX; i--)
    {
        uint64_t value = num->chunk[i];
        for(uint64_t j=0; j<4; j++)
        {
            num = num_chunk_set(num, 4 * i + j, value & 0xffff);
            value >>= 16;
        }
    }
    return num;
}

num_p num_depad(num_p num) // TODO TEST
{
    uint64_t count = num->count;
    for(uint64_t i=1; i<count; i++)
    {
        uint64_t value = num->chunk[i];
        num->chunk[i] = 0;

        uint128_t tmp = U128(value) << (16 * (i%4));
        uint64_t value_1 = LOW(tmp);
        uint64_t value_2 = HIGH(tmp);

        num = num_add_uint_offset(num, i/4, value_1);
        num = num_add_uint_offset(num, i/4 + 1, value_2);
    }

    num->count = (num->count + 3) / 4;
    while(num_normalize(num));
    return num;
}

num_p num_shuffle(num_p num, uint64_t n) // TODO TEST
{
    num = num_expand_to(num, n);
    num->count = n;
    
    uint64_t q = stdc_trailing_zeros(n);
    for(uint64_t i=0; i<n; i++)
    {
        uint64_t j = uint_inv(i, q);
        if(j < i)
            continue;

        uint64_t value = num->chunk[i];
        num->chunk[i] = num->chunk[j];
        num->chunk[j] = value;
    }
    return num;
}

void fft_rec(uint64_t vec[], uint64_t n, uint64_t w)
{
    if(n > 2)
    {
        uint64_t w_next = uint_mod_mul(w, w, PRIME_1);
        fft_rec( vec     , n/2, w_next);
        fft_rec(&vec[n/2], n/2, w_next);
    }

    uint64_t wi = 1;
    for(uint64_t i=0; i<n/2; i++)
    {
        uint64_t i_a = i; 
        uint64_t i_b = i + n/2;

        uint64_t a = vec[i_a];
        uint64_t b = uint_mod_mul(wi, vec[i_b], PRIME_1);

        vec[i_a] = uint_mod_add(a, b, PRIME_1);
        vec[i_b] = uint_mod_sub(a, b, PRIME_1);

        wi = uint_mod_mul(wi, w, PRIME_1);
    }
}

num_p num_fft(num_p num, uint64_t n) // TODO TEST
{
    if(n == 1)
        return num;

    uint64_t q = (1UL << 57) / n;
    uint64_t w = uint_mod_pow(ROOT_W_1, q, PRIME_1);
    fft_rec(num->chunk, n, w);

    return num;
}

num_p num_fft_inv(num_p num, uint64_t n) // TODO TEST
{
    if(n == 1)
        return num;

    num = num_expand_to(num, n);

    uint64_t q = (1UL << 57) / n;
    uint64_t w = uint_mod_pow(ROOT_W_INV_1, q, PRIME_1);
    fft_rec(num->chunk, n, w);

    uint64_t n_inv = uint_mod_div(1, n, PRIME_1);
    for(uint64_t i=0; i<n; i++)
        num->chunk[i] = uint_mod_mul(num->chunk[i], n_inv, PRIME_1);

    return num;
}

num_p num_mul_fft(num_p num_1, num_p num_2)
{
    CLU_HANDLER_IS_SAFE(num_1);
    CLU_HANDLER_IS_SAFE(num_2);
    assert(num_1);
    assert(num_2);

    num_1 = num_pad(num_1);
    num_2 = num_pad(num_2);
    
    uint64_t n = stdc_bit_ceil(num_1->count + num_2->count);
    num_1 = num_shuffle(num_1, n);
    num_2 = num_shuffle(num_2, n);

    num_1 = num_fft(num_1, n);
    num_2 = num_fft(num_2, n);

    for(uint64_t i=0; i<n; i++)
        num_1->chunk[i] = uint_mod_mul(num_1->chunk[i], num_2->chunk[i], PRIME_1);

    num_free(num_2);

    num_1 = num_shuffle(num_1, n);
    num_1 = num_fft_inv(num_1, n);
    num_1 = num_depad(num_1);

    return num_1;
}

num_p num_sqr_fft(num_p num)
{
    CLU_HANDLER_IS_SAFE(num);
    assert(num);

    num = num_pad(num);

    uint64_t n = stdc_bit_ceil(2 * num->count);
    num = num_shuffle(num, n);
    num = num_fft(num, n);

    for(uint64_t i=0; i<n; i++)
        num->chunk[i] = uint_mod_mul(num->chunk[i], num->chunk[i], PRIME_1);

    num = num_shuffle(num, n);
    num = num_fft_inv(num, n);
    num = num_depad(num);

    return num;
}



num_p num_pseudo_float_num(num_p num_1, num_p num_2, uint64_t count)
{
    num_1 = num_mul(num_1, num_2);
    // num_display_tag("res", num_1);
    num_1 = num_head_trim(num_1, count);
    return num_1;
}

num_p num_newton_inv(num_p num, uint64_t count)
{
    num_p num_r = num_wrap(1);
    num_r = num_head_grow(num_r, count);

    num_p num_2 = num_wrap(2);
    num_2 = num_head_grow(num_2, count);

    uint64_t i=0;
    for(i=0; i < 64 * num->count; i++)
    {
        num_p num_last = num_copy(num_r);

        num_p num_aux = num_pseudo_float_num(num_copy(num), num_copy(num_r), num->count);
        num_aux = num_sub(num_copy(num_2), num_aux);
        num_r = num_pseudo_float_num(num_r, num_aux, count);

        if(num_cmp(num_r, num_last) == 0)
        {
            num_free(num_last);
            break;
        }
        num_free(num_last);
    }
    num_free(num);
    num_free(num_2);
    return num_r;
}

num_p num_div_newton(num_p num_1, num_p num_2)
{
    assert(num_1->count >= num_2->count);
    uint64_t count = num_1->count - num_2->count +
        (num_cmp_offset(num_1, num_1->count - num_2->count, num_2) >= 0);

    num_div_normalize(&num_1, &num_2);
    num_p num_res = num_copy(num_2);
    num_res = num_newton_inv(num_res, count > num_res->count ? count : num_res->count);
    num_res = num_mul(num_res, num_copy(num_1));
    num_res = num_head_trim(num_res, num_res->count - count);

    num_p num_adjust = num_mul(num_copy(num_2), num_copy(num_res));
    if(num_cmp(num_1, num_adjust) < 0)
    {
        num_res = num_sub_uint(num_res, 1);
        num_free(num_1);
    }
    else
    {   
        num_adjust = num_sub(num_1, num_adjust);
        if(num_cmp(num_adjust, num_2) >= 0)
            num_res = num_add_uint(num_res, 1);
    }

    num_free(num_adjust);
    num_free(num_2);
    return num_res;
}



uint64_t num_div_mod_uint(num_p *out_num_q, num_p num, uint64_t value)
{
    CLU_HANDLER_IS_SAFE(num);
    assert(num);
    assert(value);

    num_p num_q = num_create(num->count, num->count);
    for(uint64_t i = num->count - 1; i != UINT64_MAX; i--)
    {
        if((num->count < i) || ((num->count - 1 == i) && (num->chunk[i] < value)))
        {
            num_q->chunk[i] = 0;
            continue;
        }

        if(num->count - 1 == i)
        {
            uint64_t r = num->chunk[i] / value;
            num_q->chunk[i] = r;
            num = num_sub_uint_offset(num, i, r * value);
            continue;
        }

        uint128_t value_1 = U128_IMMED(num->chunk[i + 1], num->chunk[i]);
        uint64_t r = value_1 / value;
        value_1 = MUL(r, value);
        num = num_sub_uint_offset(num, i+1, HIGH(value_1));
        num = num_sub_uint_offset(num, i  , LOW(value_1));
        num_q->chunk[i] = r;
    }
    num_normalize(num_q);

    *out_num_q = num_q;
    return num_unwrap(num);
}

num_p num_div_mod_classic(num_p num_1, num_p num_2)
{
    CLU_HANDLER_IS_SAFE(num_1);
    CLU_HANDLER_IS_SAFE(num_2);
    assert(num_1);
    assert(num_2);

    uint64_t count = num_1->count - num_2->count + 1;
    num_p num_q = num_create(count, count);
    num_p num_aux = num_create(num_2->count + 1, 0);
    uint64_t val_2 = num_2->chunk[num_2->count-1];
    for(uint64_t i = count - 1; i != UINT64_MAX; i--)
    {
        if(num_cmp_offset(num_1, i, num_2) < 0)
        {
            num_q->chunk[i] = 0;
            continue;
        }

        if(num_1->count == num_2->count + i)
        {
            num_q->chunk[i] = 1;
            num_1 = num_sub_offset(num_1, i, num_2);
            continue;
        }

        if(num_1->chunk[num_1->count-1] == val_2)
        {
            num_q->chunk[i] = UINT64_MAX;
            num_mul_uint_inner(num_aux, num_2, UINT64_MAX);
            num_1 = num_sub_offset(num_1, i, num_aux);
            continue;
        }

        uint128_t val_1 = U128_IMMED(num_1->chunk[num_1->count-1], num_1->chunk[num_1->count-2]);
        uint64_t r = val_1 / val_2;
        num_mul_uint_inner(num_aux, num_2, r);
        while(num_cmp_offset(num_1, i, num_aux) < 0)
        {
            r--;
            num_aux = num_sub_offset(num_aux, 0, num_2);
        }

        num_q->chunk[i] = r;
        num_1 = num_sub_offset(num_1, i, num_aux);
    }
    num_free(num_2);
    num_free(num_aux);

    num_normalize(num_q);
    return num_q;
}

uint64_t num_div_normalize(num_p *num_1, num_p *num_2) // TODO TEST
{
    CLU_HANDLER_IS_SAFE(*num_1);
    CLU_HANDLER_IS_SAFE(*num_2);
    assert(*num_1);
    assert(*num_2);

    uint64_t bits = 64 - stdc_bit_width((*num_2)->chunk[(*num_2)->count-1]);
    *num_1 = num_shl_inner((*num_1), bits);
    *num_2 = num_shl_inner((*num_2), bits);
    return bits;
}

void num_div_mod_rec(num_p *out_num_q, num_p *out_num_r, num_p num_1, num_p num_2)
{
    CLU_HANDLER_IS_SAFE(num_1);
    CLU_HANDLER_IS_SAFE(num_2);
    assert(num_1);
    assert(num_2);

    if(num_1->count < num_2->count + 2 || num_2->count == 1)
    {
        num_div_mod_fallback(out_num_q, out_num_r, num_1, num_2);
        return;
    }

    uint64_t k = (num_1->count - num_2->count) / 2;
    assert(k < num_2->count);
    num_p num_2_1, num_2_0;
    num_break(&num_2_1, &num_2_0, num_copy(num_2), k);

    num_p num_q[2];
    for(uint64_t i=1; i!=UINT64_MAX; i--)
    {
        num_p num_1_1, num_1_0, num_q_tmp;
        num_break(&num_1_1, &num_1_0, num_1, k * (i + 1));
        num_div_mod_rec(&num_q_tmp, &num_1_1, num_1_1, num_copy(num_2_1));
        num_p num_aux = num_mul(num_copy(num_q_tmp), num_copy(num_2_0));

        num_1 = num_join(num_1_1, num_1_0, k * (i + 1));
        while(num_cmp_offset(num_1, k * i, num_aux) < 0)
        {
            num_q_tmp = num_sub_uint(num_q_tmp, 1);
            num_1 = num_add_offset(num_1, k * i, num_2, 0);
        }
        num_1 = num_sub_offset(num_1, k * i, num_aux);
        num_free(num_aux);

        num_q[i] = num_q_tmp;
    }

    num_free(num_2);
    num_free(num_2_1);
    num_free(num_2_0);

    *out_num_q = num_join(num_q[1], num_q[0], k);
    *out_num_r = num_1;
}

void num_div_mod_unbalanced(num_p *out_num_q, num_p *out_num_r, num_p num_1, num_p num_2)
{
    CLU_HANDLER_IS_SAFE(num_1);
    CLU_HANDLER_IS_SAFE(num_2);
    assert(num_1);
    assert(num_2);

    uint64_t n_2 = num_2->count;
    uint64_t n_1 = num_1->count;

    num_p num_q = num_create(num_1->count - num_2->count + 1, 0);
    for(uint64_t i=0; n_1 > 2 * n_2; i++)
    {
        num_p num_1_1, num_1_0;
        uint64_t k_q = num_2->count;
        uint64_t k_1 = num_1->count - 2 * k_q;
        num_break(&num_1_1, &num_1_0, num_1, k_1);

        num_p num_q_tmp;
        num_div_mod_rec(&num_q_tmp, &num_1_1, num_1_1, num_copy(num_2));
        num_q = num_join(num_q, num_q_tmp, k_q);
        num_1 = num_join(num_1_1, num_1_0, k_1);

        n_1 -= n_2;
    }

    num_div_mod_rec(&num_2, out_num_r, num_1, num_2);
    *out_num_q = num_join(num_q, num_2, n_1 - n_2);
}

uint64_t num_div_mod_inner(
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
        uint64_t value = num_unwrap(num_2);
        value = num_div_mod_uint(out_num_q, num_1, value);
        *out_num_r = num_wrap(value);
        return 0;
    }

    uint64_t bits = num_div_normalize(&num_1, &num_2);
    // *out_num_q = num_div_mod_classic(num_1, num_2);
    // *out_num_r = num_1;
    num_div_mod_unbalanced(out_num_q, out_num_r, num_1, num_2);
    return bits;
}

void num_div_mod_fallback(
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
        return;
    }

    if(num_2->count == 1)
    {
        uint64_t value = num_unwrap(num_2);
        value = num_div_mod_uint(out_num_q, num_1, value);
        *out_num_r = num_wrap(value);
        return;
    }
    
    *out_num_q = num_div_mod_classic(num_1, num_2);
    *out_num_r = num_1;
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

    return num_cmp_offset(num_1, 0, num_2);
}



num_p num_shl(num_p num, uint64_t bits) // TODO TEST
{
    CLU_HANDLER_IS_SAFE(num);
    assert(num);

    if(num->count == 0)
        return num;

    num = num_shl_inner(num, bits & 0x3f);
    return num_head_grow(num, bits >> 6);
}

num_p num_shr(num_p num, uint64_t bits) // TODO TEST
{
    CLU_HANDLER_IS_SAFE(num);
    assert(num);

    if(num->count == 0)
        return num;

    num = num_head_trim(num, bits >> 6);
    return num_shr_inner(num, bits & 0x3f);
}

num_p num_add_uint(num_p num, uint64_t value)
{
    CLU_HANDLER_IS_SAFE(num)
    assert(num)

    return num_add_uint_offset(num, 0, value);
}

num_p num_sub_uint(num_p num, uint64_t value)
{
    CLU_HANDLER_IS_SAFE(num)
    assert(num)

    return num_sub_uint_offset(num, 0, value);
}

num_p num_mul_uint(num_p num, uint64_t value)
{
    CLU_HANDLER_IS_SAFE(num)
    assert(num)

    num_p num_res = num_create(num->count + 1, 0);
    num_res = num_add_mul_uint_offset(num_res, 0, num, 0, value);
    num_free(num);
    return num_res;
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

    num_1 = num_sub_offset(num_1, 0, num_2);
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

    if(num_1->count == 0)
    {
        num_free(num_2);
        return num_1;
    }

    if(num_2->count == 0)
    {
        num_free(num_1);
        return num_2;
    }

    if(num_1->count < 1500 || num_2->count < 1500)
        return num_mul_simple(num_1, num_2);

    return num_mul_fft(num_1, num_2);
}

num_p num_sqr(num_p num)
{
    if(num->count == 0)
        return num;

    if(num->count < 1500)
        return num_sqr_simple(num);

    return num_sqr_fft(num);
}

void num_div_mod(num_p *out_num_q, num_p *out_num_r, num_p num_1, num_p num_2)
{
    CLU_HANDLER_IS_SAFE(num_1);
    CLU_HANDLER_IS_SAFE(num_2);
    assert(num_1);
    assert(num_2);

    num_p num_q, num_r;
    uint64_t bits = num_div_mod_inner(&num_q, &num_r, num_1, num_2);

    *out_num_q = num_q;
    *out_num_r = num_shr_inner(num_r, bits);
}

num_p num_div(num_p num_1, num_p num_2)
{
    CLU_HANDLER_IS_SAFE(num_1);
    CLU_HANDLER_IS_SAFE(num_2);
    assert(num_1);
    assert(num_2);

    num_p num_q, num_r;
    num_div_mod_inner(&num_q, &num_r, num_1, num_2);
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
    uint64_t bits = num_div_mod_inner(&num_q, &num_r, num_1, num_2);
    num_free(num_q);

    return num_shr_inner(num_r, bits);
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
    assert(base > 1);

    num_p num_res = num_create(num->count, 0);
    for(uint64_t i=0; num->count; i++)
    {
        uint64_t value = num_div_mod_uint(&num, num, base);
        num_res = num_chunk_set(num_res, i, value);
    }
    num_free(num);
    return num_res;
}

num_p num_base_from(num_p num, uint64_t base)
{
    CLU_HANDLER_IS_SAFE(num);
    assert(num);
    assert(base > 1);

    num_p num_res = num_create(2 * num->count, 0);
    for(uint64_t i=num->count-1; i!=UINT64_MAX; i--)
    {
        assert(num->chunk[i] < base);
        num_res = num_mul_uint(num_res, base);
        num_res = num_add_uint(num_res, num->chunk[i]);
    }
    num_free(num);
    return num_res;
}
