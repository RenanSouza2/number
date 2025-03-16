#include "../debug.h"
#include "../../../utils/U64.h"
#include "../../../utils/assert.h"
#include "../../../utils/test_revert.h"
#include "../../../utils/clu/header.h"



void test_uint_from_char(bool show)
{
    printf("\n\t%s", __func__);

    #define TEST_UINT_FROM_CHAR(TAG, CHAR, UINT)            \
    {                                                       \
        if(show) printf("\n\t\t%s %2d\t\t", __func__, TAG); \
        uint64_t res = uint_from_char(CHAR);                \
        assert(uint64(res, UINT));                          \
    }

    TEST_UINT_FROM_CHAR( 1, '0',  0);
    TEST_UINT_FROM_CHAR( 2, '1',  1);
    TEST_UINT_FROM_CHAR( 3, '9',  9);
    TEST_UINT_FROM_CHAR( 4, 'a', 10);
    TEST_UINT_FROM_CHAR( 5, 'b', 11);
    TEST_UINT_FROM_CHAR( 6, 'f', 15);
    TEST_UINT_FROM_CHAR( 7, 'A', 10);
    TEST_UINT_FROM_CHAR( 8, 'B', 11);
    TEST_UINT_FROM_CHAR( 9, 'F', 15);

    #undef TEST_UINT_FROM_CHAR

    if(show) printf("\n\t\t%s 10\t\t", __func__);
    TEST_REVERT_OPEN
    uint_from_char('g');
    TEST_REVERT_CLOSE

    chunk_pool_clean();
    assert(clu_mem_empty());
}

void test_uint128(bool show)
{
    printf("\n\t%s", __func__);

    if(show) printf("\n\t\t%s  1", __func__);
    uint128_t u = 1;
    assert(uint128_immed(u, 0, 1));

    if(show) printf("\n\t\t%s  2", __func__);
    u = UINT64_MAX;
    assert(uint128_immed(u, 0, UINT64_MAX));

    if(show) printf("\n\t\t%s  3", __func__);
    u = U128_IMMED(1, 0);
    assert(uint128_immed(u, 1, 0));

    if(show) printf("\n\t\t%s  4", __func__);
    u = U128_IMMED(1, 2);
    assert(uint128_immed(u, 1, 2));

    if(show) printf("\n\t\t%s  5", __func__);
    u = U128_IMMED(UINT64_MAX, UINT64_MAX);
    assert(uint128_immed(u, UINT64_MAX, UINT64_MAX));

    if(show) printf("\n\t\t%s  6", __func__);
    u = UINT64_MAX;
    u += 1;
    assert(uint128_immed(u, 1, 0));

    if(show) printf("\n\t\t%s  7", __func__);
    u = U128_IMMED(1, 0);
    u -= 1;
    assert(uint128_immed(u, 0, UINT64_MAX));

    if(show) printf("\n\t\t%s  8", __func__);
    u = UINT64_MAX;
    u *= UINT64_MAX;
    assert(uint128_immed(u, UINT64_MAX - 1, 1));

    if(show) printf("\n\t\t%s  9", __func__);
    u = U128_IMMED(1, 0);
    u <<= 1;
    assert(uint128_immed(u, 2, 0));

    if(show) printf("\n\t\t%s 10", __func__);
    u = U128_IMMED(1, 0);
    u >>= 1;
    assert(uint128_immed(u, 0, 0x8000000000000000));

    if(show) printf("\n\t\t%s 11", __func__);
    u = U128_IMMED(1, 0);
    u /= 1;
    assert(uint128_immed(u, 1, 0));

    if(show) printf("\n\t\t%s 12", __func__);
    u = U128_IMMED(1, 0);
    u /= 2;
    assert(uint128_immed(u, 0, 0x8000000000000000));

    if(show) printf("\n\t\t%s 13", __func__);
    u = U128_IMMED(UINT64_MAX, UINT64_MAX);
    u /= 1;
    assert(uint128_immed(u, UINT64_MAX, UINT64_MAX));

    if(show) printf("\n\t\t%s 14", __func__);
    u = U128_IMMED(UINT64_MAX / 2, UINT64_MAX);
    u /= 2;
    assert(uint128_immed(u, UINT64_MAX >> 2, UINT64_MAX));

    if(show) printf("\n\t\t%s 15", __func__);
    u = U128_IMMED(UINT64_MAX, UINT64_MAX - 1);
    u /= 2;
    assert(uint128_immed(u, UINT64_MAX >> 1, UINT64_MAX));

    chunk_pool_clean();
    assert(clu_mem_empty());
}



void test_chunk_create(bool show)
{
    printf("\n\t%s", __func__);

    if(show) printf("\n\t\t%s 1", __func__);
    chunk_p chunk = chunk_create(0, NULL, NULL);
    assert(chunk->value == 0);
    assert(chunk->next == NULL);
    assert(chunk->prev == NULL);
    free(chunk);

    if(show) printf("\n\t\t%s 2", __func__);
    chunk = chunk_create(1, NULL, NULL);
    assert(chunk->value == 1);
    assert(chunk->next == NULL);
    assert(chunk->prev == NULL);
    free(chunk);

    if(show) printf("\n\t\t%s 3", __func__);
    chunk_p chunk_next = chunk_create(0, NULL, NULL);
    chunk = chunk_create(1, chunk_next, NULL);
    assert(chunk->value == 1);
    assert(chunk->next == chunk_next);
    assert(chunk->next->prev == chunk);
    assert(chunk->prev  == NULL);
    free(chunk);
    free(chunk_next);

    if(show) printf("\n\t\t%s 4", __func__);
    chunk_p chunk_prev = chunk_create(0, NULL, NULL);
    chunk = chunk_create(1, NULL, chunk_prev);
    assert(chunk->value == 1);
    assert(chunk->next == NULL);
    assert(chunk->prev == chunk_prev);
    assert(chunk->prev->next == chunk);
    free(chunk);
    free(chunk_prev);

    if(show) printf("\n\t\t%s 5", __func__);
    chunk_next = chunk_create(0, NULL, NULL);
    chunk_prev = chunk_create(0, NULL, NULL);
    chunk = chunk_create(1, chunk_next, chunk_prev);
    assert(chunk->value == 1);
    assert(chunk->next == chunk_next);
    assert(chunk->next->prev == chunk);
    assert(chunk->prev == chunk_prev);
    assert(chunk->prev->next == chunk);
    free(chunk);
    free(chunk_next);
    free(chunk_prev);

    chunk_pool_clean();
    assert(clu_mem_empty());
}



void test_num_create_immed(bool show)
{
    printf("\n\t%s\t\t", __func__);

    if(show) printf("\n\t\t%s 1\t\t", __func__);
    num_t num = num_create_immed(0);
    assert(num.count == 0);
    assert(num.head == NULL);
    assert(num.tail == NULL);
    num_free(num);

    if(show) printf("\n\t\t%s 2\t\t", __func__);
    num = num_create_immed(1, 2);
    assert(num.count == 1);
    assert(num.head != NULL);
    assert(num.head->value == 2);
    assert(num.head->next == NULL);
    assert(num.head->prev == NULL);
    assert(num.tail == num.head);
    num_free(num);

    if(show) printf("\n\t\t%s 3\t\t", __func__);
    num = num_create_immed(2, 2, 1);
    assert(num.count == 2);
    assert(num.head != NULL)
    assert(num.head->value == 1);
    assert(num.head->next != NULL);
    assert(num.head->prev == NULL);
    assert(num.head->next->value == 2);
    assert(num.head->next->next == NULL);
    assert(num.head->next->prev == num.head);
    assert(num.tail == num.head->next);
    num_free(num);

    if(show) printf("\n\t\t%s 4\t\t", __func__);
    num = num_create_immed(2, 2, 0);
    assert(num.count == 2);
    assert(num.head != NULL)
    assert(num.head->value == 0);
    assert(num.head->next != NULL);
    assert(num.head->prev == NULL);
    assert(num.head->next->value == 2);
    assert(num.head->next->next == NULL);
    assert(num.head->next->prev == num.head);
    assert(num.tail == num.head->next);
    num_free(num);

    chunk_pool_clean();
    assert(clu_mem_empty());
}

void test_num_create_immed_vec(bool show)
{
    printf("\n\t%s", __func__);

    if(show) printf("\n\t\t%s 1", __func__);
    num_t num[2];
    num_create_immed_vec(num, 1, 0);
    assert(num_immed(num[0], 0));

    if(show) printf("\n\t\t%s 2", __func__);
    num_create_immed_vec(num, 1, 1, 1);
    assert(num_immed(num[0], 1, 1));

    if(show) printf("\n\t\t%s 3", __func__);
    num_create_immed_vec(num, 2, 1, 1, 1, 2);
    assert(num_immed(num[0], 1, 1));
    assert(num_immed(num[1], 1, 2));

    if(show) printf("\n\t\t%s 4", __func__);
    num_create_immed_vec(num, 2, 0, 1, 2);
    assert(num_immed(num[0], 0));
    assert(num_immed(num[1], 1, 2));


    if(show) printf("\n\t\t%s 3", __func__);
    num_create_immed_vec(num, 2, 1, 1, 0);
    assert(num_immed(num[0], 1, 1));
    assert(num_immed(num[1], 0));

    chunk_pool_clean();
    assert(clu_mem_empty());
}



void test_num_insert(bool show)
{
    printf("\n\t%s", __func__);

    #define TEST_NUM_INSERT(TAG, VALUE, IS_HEAD, ...)   \
    {                                                   \
        num_t num[2];                                   \
        if(show) printf("\n\t\t%s %d", __func__, TAG);  \
        num_create_immed_vec(num, 2, __VA_ARGS__);      \
        chunk_p chunk = num_insert_tail(&num[0], VALUE);     \
        assert(uint64(chunk->value, VALUE));            \
        assert((num[0].head == chunk) == IS_HEAD);      \
        assert(num[0].tail == chunk);                   \
        assert(num_str(num[0], num[1]));                \
    }

    TEST_NUM_INSERT(1, 0, true,
        0,
        1, 0
    );
    TEST_NUM_INSERT(2, 1, true,
        0,
        1, 1
    );
    TEST_NUM_INSERT(3, 0, false,
        1, 2,
        2, 0, 2
    );
    TEST_NUM_INSERT(4, 1, false,
        1, 2,
        2, 1, 2
    );

    #undef TEST_NUM_INSERT

    chunk_pool_clean();
    assert(clu_mem_empty());
}

void test_num_insert_head(bool show)
{
    printf("\n\t%s\t\t", __func__);

    #define TEST_NUM_INSERT_HEAD(TAG, VALUE, IS_TAIL, ...)  \
    {                                                       \
        num_t num[2];                                       \
        if(show) printf("\n\t\t%s %d\t\t", __func__, TAG);  \
        num_create_immed_vec(num, 2, __VA_ARGS__);          \
        chunk_p chunk = num_insert_head(&num[0], VALUE);    \
        assert(uint64(chunk->value, VALUE));                \
        assert(num[0].head == chunk);                       \
        assert((num[0].tail == chunk) == IS_TAIL);          \
        assert(num_str(num[0], num[1]));                    \
    }

    TEST_NUM_INSERT_HEAD(1, 0, true,
        0,
        1, 0
    );
    TEST_NUM_INSERT_HEAD(2, 1, true,
        0,
        1, 1
    );
    TEST_NUM_INSERT_HEAD(3, 0, false,
        1, 2,
        2, 2, 0
    );
    TEST_NUM_INSERT_HEAD(4, 1, false,
        1, 2,
        2, 2, 1
    );

    #undef TEST_NUM_INSERT_HEAD

    chunk_pool_clean();
    assert(clu_mem_empty());
}

void test_num_remove_head(bool show)
{
    printf("\n\t%s\t\t", __func__);

    #define TEST_NUM_REMOVE_HEAD(TAG, ...)                  \
    {                                                       \
        num_t num[2];                                       \
        if(show) printf("\n\t\t%s %d\t\t", __func__, TAG);  \
        num_create_immed_vec(num, 2, __VA_ARGS__);          \
        num[0] = num_remove_head(num[0]);                   \
        assert(num_str(num[0], num[1]));                    \
    }

    TEST_NUM_REMOVE_HEAD(1,
        0,
        0
    );
    TEST_NUM_REMOVE_HEAD(2,
        1, 1,
        0
    );
    TEST_NUM_REMOVE_HEAD(3,
        2, 1, 2,
        1, 1
    );

    #undef TEST_NUM_REMOVE_HEAD

    chunk_pool_clean();
    assert(clu_mem_empty());
}

void test_num_denormalize(bool show)
{
    printf("\n\t%s", __func__);

    #define TEST_NUM_DENORMALIZE(TAG, ...)                  \
    {                                                       \
        num_t num[2];                                       \
        if(show) printf("\n\t\t%s " #TAG "\t\t", __func__); \
        num_create_immed_vec(num, 2, __VA_ARGS__);          \
        chunk_p chunk = num_denormalize(&num[0], NULL);     \
        assert(num[0].tail == chunk);                       \
        assert(num_str(num[0], num[1]));                    \
    }

    TEST_NUM_DENORMALIZE(1,
        0,
        1, 0
    );
    TEST_NUM_DENORMALIZE(2,
        1, 1,
        2, 0, 1
    );

    #undef TEST_NUM_DENORMALIZE

    if(show) printf("\n\t\t%s 3\t\t", __func__);
    num_t num =  num_create_immed(1, 1);
    chunk_p chunk = num_denormalize(&num, num.head);
    assert(num.head == chunk);
    assert(num_immed(num, 1, 1));

    chunk_pool_clean();
    assert(clu_mem_empty());
}

void test_num_normalize(bool show)
{
    printf("\n\t%s", __func__);

    #define TEST_NUM_NORMALIZE(TAG, RES, ...)               \
    {                                                       \
        num_t num[2];                                       \
        if(show) printf("\n\t\t%s %d\t\t", __func__, TAG);  \
        num_create_immed_vec(num, 2, __VA_ARGS__);          \
        bool res = num_normalize(&num[0]);                  \
        assert(res == RES);                                 \
        assert(num_str(num[0], num[1]));                    \
    }

    TEST_NUM_NORMALIZE(1, false,
        0,
        0
    );
    TEST_NUM_NORMALIZE(2, false,
        1, 1,
        1, 1
    );
    TEST_NUM_NORMALIZE(3, true,
        1, 0,
        0
    );
    TEST_NUM_NORMALIZE(4, true,
        2, 0, 1,
        1, 1
    );
    TEST_NUM_NORMALIZE(5, true,
        2, 0, 0,
        1, 0
    );

    #undef TEST_NUM_NORMALIZE

    chunk_pool_clean();
    assert(clu_mem_empty());
}

void test_num_break(bool show)
{
    printf("\n\t%s", __func__);

    #define TEST_NUM_BREAK(TAG, COUNT, ...)                 \
    {                                                       \
        num_t num[3], num_h, num_l;                         \
        if(show) printf("\n\t\t%s %2d\t\t", __func__, TAG); \
        num_create_immed_vec(num, 3, __VA_ARGS__);          \
        num_break(&num_h, &num_l, num[0], COUNT);           \
        assert(num_str(num_h, num[1]));                     \
        assert(num_str(num_l, num[2]));                     \
    }

    TEST_NUM_BREAK( 1, 0,
        0,
        0,
        0
    );
    TEST_NUM_BREAK( 2, 1,
        0,
        0,
        0
    );
    TEST_NUM_BREAK( 3, 0,
        1, 1,
        1, 1,
        0
    );
    TEST_NUM_BREAK( 4, 1,
        1, 1,
        0,
        1, 1
    );
    TEST_NUM_BREAK( 5, 2,
        1, 1,
        0,
        1, 1
    );
    TEST_NUM_BREAK( 6, 0,
        2, 1, 2,
        2, 1, 2,
        0
    );
    TEST_NUM_BREAK( 7, 1,
        2, 1, 2,
        1, 1,
        1, 2
    );
    TEST_NUM_BREAK( 8, 2,
        2, 1, 2,
        0,
        2, 1, 2
    );
    TEST_NUM_BREAK( 9, 3,
        2, 1, 2,
        0,
        2, 1, 2
    );
    TEST_NUM_BREAK(10, 2,
        3, 1, 0, 2,
        1, 1,
        1, 2
    );
    TEST_NUM_BREAK(11, 2,
        3, 1, 0, 0,
        1, 1,
        0
    );

    #undef TEST_NUM_BREAK

    chunk_pool_clean();
    assert(clu_mem_empty());
}



void test_num_wrap(bool show)
{
    printf("\n\t%s", __func__);

    #define TEST_NUM_WRAP(TAG, NUM, ...)                \
    {                                                   \
        if(show) printf("\n\t\t%s %d", __func__, TAG);  \
        num_t num = num_wrap(NUM);                      \
        assert(num_immed(num, __VA_ARGS__));            \
    }

    TEST_NUM_WRAP(1, 0, 0);
    TEST_NUM_WRAP(2, 2, 1, 2);
    TEST_NUM_WRAP(3, UINT64_MAX, 1, UINT64_MAX);

    #undef TEST_NUM_WRAP

    chunk_pool_clean();
    assert(clu_mem_empty());
}

void test_num_wrap_dec(bool show)
{
    printf("\n\t%s", __func__);

    #define TEST_NUM_WRAP_DEC(TAG, STR, ...)            \
    {                                                   \
        if(show) printf("\n\t\t%s " #TAG "", __func__); \
        num_t num = num_wrap_dec(STR);                  \
        assert(num_immed(num, __VA_ARGS__));            \
    }

    TEST_NUM_WRAP_DEC(1, "0", 0);
    TEST_NUM_WRAP_DEC(2, "1", 1, 1);
    TEST_NUM_WRAP_DEC(3, "9", 1, 9);
    TEST_NUM_WRAP_DEC(4, "10", 1, 10);
    TEST_NUM_WRAP_DEC(5, "18446744073709551615", 1, UINT64_MAX);
    TEST_NUM_WRAP_DEC(6, "18446744073709551616", 2, 1, 0);
    TEST_NUM_WRAP_DEC(7, "0000", 0);
    TEST_NUM_WRAP_DEC(8, "00001", 1, 1);
    TEST_NUM_WRAP_DEC(9, "", 0);

    #undef TEST_NUM_WRAP_DEC

    chunk_pool_clean();
    assert(clu_mem_empty());
}

void test_num_wrap_hex(bool show)
{
    printf("\n\t%s", __func__);

    #define TEST_NUM_WRAP_HEX(TAG, STR, ...)            \
    {                                                   \
        if(show) printf("\n\t\t%s %2d", __func__, TAG); \
        num_t num = num_wrap_hex(STR);                  \
        assert(num_immed(num, __VA_ARGS__));            \
    }

    TEST_NUM_WRAP_HEX( 1, "0x0", 0);
    TEST_NUM_WRAP_HEX( 2, "0x1", 1, 1);
    TEST_NUM_WRAP_HEX( 3, "0x9", 1, 9);
    TEST_NUM_WRAP_HEX( 4, "0xa", 1, 10);
    TEST_NUM_WRAP_HEX( 5, "0xA", 1, 10);
    TEST_NUM_WRAP_HEX( 6, "0x10", 1, 16);
    TEST_NUM_WRAP_HEX( 7, "0xffffffffffffffff", 1, UINT64_MAX);
    TEST_NUM_WRAP_HEX( 8, "0x10000000000000000", 2, 1, 0);
    TEST_NUM_WRAP_HEX( 9, "0x0000", 0);
    TEST_NUM_WRAP_HEX(10, "0x00001", 1, 1);
    TEST_NUM_WRAP_HEX(11, "0x", 0);

    #undef TEST_NUM_WRAP_HEX

    #define TEST_NUM_WRAP_HEX(TAG, STR)                 \
    {                                                   \
        if(show) printf("\n\t\t%s %2d", __func__, TAG); \
        TEST_REVERT_OPEN                                \
        num_wrap_hex(STR);                              \
        TEST_REVERT_CLOSE                               \
    }

    TEST_NUM_WRAP_HEX(12, "");
    TEST_NUM_WRAP_HEX(13, "0");
    TEST_NUM_WRAP_HEX(14, "ab");
    TEST_NUM_WRAP_HEX(14, "0b");

    #undef TEST_NUM_WRAP_HEX

    chunk_pool_clean();
    assert(clu_mem_empty());
}

void test_num_wrap_str(bool show)
{
    printf("\n\t%s", __func__);

    #define TEST_NUM_WRAP_STR(TAG, STR, ...)            \
    {                                                   \
        if(show) printf("\n\t\t%s %2d", __func__, TAG); \
        num_t num = num_wrap_str(STR);                  \
        assert(num_immed(num, __VA_ARGS__));            \
    }

    TEST_NUM_WRAP_STR( 1, "0", 0);
    TEST_NUM_WRAP_STR( 2, "1", 1, 1);
    TEST_NUM_WRAP_STR( 3, "9", 1, 9);
    TEST_NUM_WRAP_STR( 4, "10", 1, 10);
    TEST_NUM_WRAP_STR( 5, "18446744073709551615", 1, UINT64_MAX);
    TEST_NUM_WRAP_STR( 6, "18446744073709551616", 2, 1, 0);
    TEST_NUM_WRAP_STR( 7, "0000", 0);
    TEST_NUM_WRAP_STR( 8, "00001", 1, 1);
    TEST_NUM_WRAP_STR( 9, "0x0", 0);
    TEST_NUM_WRAP_STR(10, "0x1", 1, 1);
    TEST_NUM_WRAP_STR(11, "0x9", 1, 9);
    TEST_NUM_WRAP_STR(12, "0xa", 1, 10);
    TEST_NUM_WRAP_STR(13, "0xA", 1, 10);
    TEST_NUM_WRAP_STR(14, "0x10", 1, 16);
    TEST_NUM_WRAP_STR(15, "0xffffffffffffffff", 1, UINT64_MAX);
    TEST_NUM_WRAP_STR(16, "0x10000000000000000", 2, 1, 0);
    TEST_NUM_WRAP_STR(17, "0x0000", 0);
    TEST_NUM_WRAP_STR(18, "0x00001", 1, 1);

    #undef TEST_NUM_WRAP_STR

    chunk_pool_clean();
    assert(clu_mem_empty());
}

void test_num_read_dec(bool show)
{
    printf("\n\t%s", __func__);

    #define TEST_NUM_READ_DEC(TAG, ...)                         \
    {                                                           \
        if(show) printf("\n\t\t%s %d", __func__, TAG);         \
        num_t num = num_read_dec("numbers/num_" #TAG ".txt");   \
        assert(num_immed(num, __VA_ARGS__));                    \
    }

    TEST_NUM_READ_DEC(1, 0);
    TEST_NUM_READ_DEC(2, 1, 1);
    TEST_NUM_READ_DEC(3, 1, 10);
    TEST_NUM_READ_DEC(4, 1, 12345678901234567);
    TEST_NUM_READ_DEC(5, 1, 123456789012345678);
    TEST_NUM_READ_DEC(6, 1, 1234567890123456789);
    TEST_NUM_READ_DEC(7, 2, 6, 0xb14e9f812f366c35);

    #undef TEST_NUM_READ_DEC

    if(show) printf("\n\t\t%s 8", __func__);
    TEST_REVERT_OPEN
    num_read_dec("numbers/num_8.txt");
    TEST_REVERT_CLOSE

    chunk_pool_clean();
    assert(clu_mem_empty());
}

void test_num_unwrap(bool show)
{
    printf("\n\t%s", __func__);

    if(show) printf("\n\t\t%s 1", __func__);
    num_t num = num_create_immed(0);
    uint64_t res = num_unwrap(num);
    assert(res == 0);

    if(show) printf("\n\t\t%s 2", __func__);
    num = num_create_immed(1, 1);
    res = num_unwrap(num);
    assert(res == 1);

    if(show) printf("\n\t\t%s 3", __func__);
    num = num_create_immed(2, 3, 4);
    TEST_REVERT_OPEN
    num_unwrap(num);
    TEST_REVERT_CLOSE
    num_free(num);

    chunk_pool_clean();
    assert(clu_mem_empty());
}

void test_num_copy(bool show)
{
    printf("\n\t%s", __func__);

    #define TEST_NUM_COPY(TAG, ...)                     \
    {                                                   \
        if(show) printf("\n\t\t%s %d", __func__, TAG);  \
        num_t num = num_create_immed(__VA_ARGS__);      \
        num_t num_res = num_copy(num);                  \
        assert(num_str(num_res, num));                  \
    }

    TEST_NUM_COPY(1, 0);
    TEST_NUM_COPY(2, 1, 1);
    TEST_NUM_COPY(3, 2, 1, 2);

    #undef TEST_NUM_COPY

    chunk_pool_clean();
    assert(clu_mem_empty());
}



void test_num_base_to(bool show)
{
    printf("\n\t%s", __func__);

    #define TEST_NUM_BASE_TO(TAG, ...)                      \
    {                                                       \
        num_t num[2];                                       \
        if(show) printf("\n\t\t%s %d\t\t", __func__, TAG);  \
        num_create_immed_vec(num, 2, __VA_ARGS__);          \
        num[0] = num_base_to(num[0], 10);                   \
        assert(num_str(num[0], num[1]));                    \
    }

    TEST_NUM_BASE_TO(1,
        0,
        0
    );
    TEST_NUM_BASE_TO(2,
        1, 1,
        1, 1
    );
    TEST_NUM_BASE_TO(3,
        1, 9,
        1, 9
    );
    TEST_NUM_BASE_TO(4,
        1, 10,
        2, 1, 0
    );
    TEST_NUM_BASE_TO(5,
        1, 100,
        3, 1, 0, 0
    );
    TEST_NUM_BASE_TO(6,
        1, 99,
        2, 9, 9
    );
    TEST_NUM_BASE_TO(7,
        2, 5, 0x6bc75e2d63100000,
        21, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    );

    #undef TEST_NUM_BASE_TO

    chunk_pool_clean();
    assert(clu_mem_empty());
}

void test_num_base_from(bool show)
{
    printf("\n\t%s", __func__);

    #define TEST_NUM_BASE_FROM(TAG, ...)                    \
    {                                                       \
        num_t num[2];                                       \
        if(show) printf("\n\t\t%s %d\t\t", __func__, TAG);  \
        num_create_immed_vec(num, 2, __VA_ARGS__);          \
        num[0] = num_base_from(num[0], 10);                 \
        assert(num_str(num[0], num[1]));                    \
    }

    TEST_NUM_BASE_FROM(1,
        0,
        0
    );
    TEST_NUM_BASE_FROM(2,
        1, 1,
        1, 1
    );
    TEST_NUM_BASE_FROM(3,
        1, 9,
        1, 9
    );
    TEST_NUM_BASE_FROM(4,
        2, 1, 0,
        1, 10
    );
    TEST_NUM_BASE_FROM(5,
        21, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        2, 5, 0x6bc75e2d63100000
    );

    #undef TEST_NUM_BASE_FROM

    chunk_pool_clean();
    assert(clu_mem_empty());
}



void test_num_sub_uint_offset(bool show)
{
    printf("\n\t%s\t\t", __func__);

    #define TEST_NUM_SUB_UINT_OFFSET(TAG, OFFSET, VALUE, RES, ...)  \
    {                                                               \
        num_t num[2];                                               \
        if(show) printf("\n\t\t%s %2d\t\t", __func__, TAG);         \
        num_create_immed_vec(num, 2, __VA_ARGS__);                  \
        chunk_p chunk = num_get_chunk(num[0], OFFSET);              \
        bool res = num_sub_uint_offset(&num[0], chunk, VALUE);      \
        assert(num_str(num[0], num[1]));                            \
        assert(res == RES);                                         \
    }

    TEST_NUM_SUB_UINT_OFFSET( 1, 0, 1, false,
        2, 2, 3,
        2, 2, 2
    );
    TEST_NUM_SUB_UINT_OFFSET( 2, 1, 1, false,
        2, 2, 3,
        2, 1, 3
    );
    TEST_NUM_SUB_UINT_OFFSET( 3, 1, 1, true,
        2, 1, 3,
        1, 3
    )
    TEST_NUM_SUB_UINT_OFFSET( 4, 1, 1, true,
        2, 1, 0,
        1, 0
    );
    TEST_NUM_SUB_UINT_OFFSET( 5, 2, 1, true,
        3, 1, 0, 1,
        2, 0, 1
    );
    TEST_NUM_SUB_UINT_OFFSET( 6, 1, 1, false,
        3, 1, 0, 1,
        2, UINT64_MAX, 1
    );
    TEST_NUM_SUB_UINT_OFFSET( 7, 1, 1, true,
        2, 1, 0,
        1, 0
    );
    TEST_NUM_SUB_UINT_OFFSET( 8, 0, 0, false,
        0,
        0
    );

    #undef TEST_NUM_SUB_UINT_OFFSET

    #define TEST_NUM_SUB_UINT_OFFSET(TAG, OFFSET, VALUE, ...)   \
    {                                                           \
        if(show) printf("\n\t\t%s %2d\t\t", __func__, TAG);     \
        num_t num = num_create_immed(__VA_ARGS__);              \
        chunk_p chunk = num_get_chunk(num, OFFSET);             \
        TEST_REVERT_OPEN                                        \
        num_sub_uint_offset(&num, chunk, VALUE);                \
        TEST_REVERT_CLOSE                                       \
        num_free(num);                                          \
    }

    TEST_NUM_SUB_UINT_OFFSET( 9, 0, 1, 0);
    TEST_NUM_SUB_UINT_OFFSET(10, 0, 3, 1, 2);
    TEST_NUM_SUB_UINT_OFFSET(11, 1, 1, 1, 2);

    #undef TEST_NUM_SUB_UINT_OFFSET

    chunk_pool_clean();
    assert(clu_mem_empty());
}



void test_num_shl_uint(bool show)
{
    printf("\n\t%s", __func__);

    #define TEST_NUM_SHL_UINT(TAG, VALUE, ...)          \
    {                                                   \
        num_t num[2];                                   \
        if(show) printf("\n\t\t%s %d", __func__, TAG);  \
        num_create_immed_vec(num, 2, __VA_ARGS__);      \
        num[0] = num_shl_uint(num[0], VALUE);           \
        assert(num_str(num[0], num[1]));                \
    }

    TEST_NUM_SHL_UINT(1, 0,
        0,
        0
    );
    TEST_NUM_SHL_UINT(2, 63,
        0,
        0
    );
    TEST_NUM_SHL_UINT(3, 1,
        1, 1,
        1, 2
    );
    TEST_NUM_SHL_UINT(4, 1,
        1, 1,
        1, 2
    );
    TEST_NUM_SHL_UINT(5, 63,
        1, 1,
        1, 0x8000000000000000
    );
    TEST_NUM_SHL_UINT(6, 63,
        1, 2,
        2, 1, 0
    );
    TEST_NUM_SHL_UINT(7, 63,
        2, 1, 2,
        2, 0x8000000000000001, 0
    );

    #undef TEST_NUM_SHL_UINT

    chunk_pool_clean();
    assert(clu_mem_empty());
}

void test_num_shr_uint(bool show)
{
    printf("\n\t%s", __func__);

    #define TEST_NUM_SHR_UINT(TAG, BITS, ...)           \
    {                                                   \
        num_t num[2];                                   \
        if(show) printf("\n\t\t%s %d", __func__, TAG);  \
        num_create_immed_vec(num, 2, __VA_ARGS__);      \
        num[0] = num_shr(num[0], BITS);                 \
        assert(num_str(num[0], num[1]));                \
    }

    TEST_NUM_SHR_UINT(1, 1,
        0,
        0
    );
    TEST_NUM_SHR_UINT(2, 0,
        1, 1,
        1, 1
    );
    TEST_NUM_SHR_UINT(3, 1,
        1, 1,
        0
    );
    TEST_NUM_SHR_UINT(4, 1,
        1, 2,
        1, 1
    );
    TEST_NUM_SHR_UINT(5, 63,
        1, UINT64_MAX,
        1, 1
    );
    TEST_NUM_SHR_UINT(6, 63,
        2, 1, 0x8000000000000000,
        1, 3
    );

    #undef TEST_NUM_SHR_UINT

    chunk_pool_clean();
    assert(clu_mem_empty());
}

void test_num_add_uint(bool show)
{
    printf("\n\t%s", __func__);

    #define TEST_NUM_ADD_UINT(TAG, VALUE, ...)          \
    {                                                   \
        num_t num[2];                                   \
        if(show) printf("\n\t\t%s %d", __func__, TAG);  \
        num_create_immed_vec(num, 2, __VA_ARGS__);      \
        num[0] = num_add_uint(num[0], VALUE);           \
        assert(num_str(num[0], num[1]));                \
    }

    TEST_NUM_ADD_UINT(1, 0,
        0,
        0
    );
    TEST_NUM_ADD_UINT(2, 1,
        0,
        1, 1
    );
    TEST_NUM_ADD_UINT(3, 2,
        1, 1,
        1, 3
    );
    TEST_NUM_ADD_UINT(4, 3,
        1, UINT64_MAX,
        2, 1, 2
    );

    #undef TEST_NUM_ADD_UINT

    chunk_pool_clean();
    assert(clu_mem_empty());
}

void test_num_sub_uint(bool show)
{
    printf("\n\t%s\t\t", __func__);

    #define TEST_NUM_SUB_UINT(TAG, VALUE, ...)              \
    {                                                       \
        num_t num[2];                                       \
        if(show) printf("\n\t\t%s %d\t\t", __func__, TAG);  \
        num_create_immed_vec(num, 2, __VA_ARGS__);          \
        num[0] = num_sub_uint(num[0], VALUE);               \
        assert(num_str(num[0], num[1]));                    \
    }

    TEST_NUM_SUB_UINT(1, 0,
        0,
        0
    );
    TEST_NUM_SUB_UINT(2, 0,
        1, 1,
        1, 1
    );
    TEST_NUM_SUB_UINT(3, 1,
        1, 1,
        0
    );
    TEST_NUM_SUB_UINT(4, 1,
        1, 2,
        1, 1
    );
    TEST_NUM_SUB_UINT(5, 1,
        2, 1, 0,
        1, UINT64_MAX
    );

    #undef TEST_NUM_SUB_UINT

    chunk_pool_clean();
    assert(clu_mem_empty());
}

void test_num_mul_uint(bool show)
{
    printf("\n\t%s\t\t", __func__);

    #define TEST_NUM_MUL_UINT(TAG, VALUE, ...)              \
    {                                                       \
        num_t num[2];                                       \
        if(show) printf("\n\t\t%s %d\t\t", __func__, TAG);  \
        num_create_immed_vec(num, 2, __VA_ARGS__);          \
        num_t num_res = num_mul_uint(num[0], VALUE);        \
        assert(num_str(num_res, num[1]));                   \
        num_free(num[0]);                                   \
    }

    TEST_NUM_MUL_UINT(1, 0,
        0,
        0
    );
    TEST_NUM_MUL_UINT(2, 1,
        0,
        0
    );
    TEST_NUM_MUL_UINT(3, 0,
        1, 2,
        0
    );
    TEST_NUM_MUL_UINT(4, 3,
        1, 2,
        1, 6
    );
    TEST_NUM_MUL_UINT(5, UINT64_MAX,
        1, 2,
        2, 1, UINT64_MAX - 1
    );
    TEST_NUM_MUL_UINT(6, 4,
        2, 2, 3,
        2, 8, 12
    );
    TEST_NUM_MUL_UINT(7, UINT64_MAX,
        2, UINT64_MAX, UINT64_MAX,
        3, UINT64_MAX - 1, UINT64_MAX, 1
    );
    TEST_NUM_MUL_UINT(8, 3,
        2, 2, 0,
        2, 6, 0
    );
    TEST_NUM_MUL_UINT(9, 123427205323,
        2, 6, 0xea7db545decb57a4,
        2, 0xc6bfba060d, 0xdf4c110fd2f33f0c
    );

    #undef TEST_NUM_MUL_UINT

    chunk_pool_clean();
    assert(clu_mem_empty());
}

void test_num_add_mul_uint(bool show)
{
    printf("\n\t%s\t\t", __func__);

    #define TEST_NUM_ADD_MUL_UINT(TAG, VALUE, ...)          \
    {                                                       \
        num_t num[2];                                       \
        if(show) printf("\n\t\t%s %d\t\t", __func__, TAG);  \
        num_create_immed_vec(num, 2, __VA_ARGS__);          \
        num_t num_res = num_create_immed(1, 1);             \
        num_res = num_add_mul_uint(num_res, num[0], VALUE); \
        assert(num_str(num_res, num[1]));                   \
        num_free(num[0]);                                   \
    }

    TEST_NUM_ADD_MUL_UINT(1, 0,
        0,
        1, 1
    );
    TEST_NUM_ADD_MUL_UINT(2, 1,
        0,
        1, 1
    );
    TEST_NUM_ADD_MUL_UINT(3, 0,
        1, 2,
        1, 1
    );
    TEST_NUM_ADD_MUL_UINT(4, 3,
        1, 2,
        1, 7
    );
    TEST_NUM_ADD_MUL_UINT(5, UINT64_MAX,
        1, 2,
        2, 1, UINT64_MAX
    );
    TEST_NUM_ADD_MUL_UINT(6, 4,
        2, 2, 3,
        2, 8, 13
    );
    TEST_NUM_ADD_MUL_UINT(7, UINT64_MAX,
        2, UINT64_MAX, UINT64_MAX,
        3, UINT64_MAX - 1, UINT64_MAX, 2
    );
    TEST_NUM_ADD_MUL_UINT(8, 3,
        2, 2, 0,
        2, 6, 1
    );
    TEST_NUM_ADD_MUL_UINT(9, 123427205323,
        2, 6, 0xea7db545decb57a4,
        2, 0xc6bfba060d, 0xdf4c110fd2f33f0d
    );

    #undef TEST_NUM_ADD_MUL_UINT

    chunk_pool_clean();
    assert(clu_mem_empty());
}



void test_num_sub_offset(bool show)
{
    printf("\n\t%s\t\t", __func__);

    #define TEST_NUM_SUB_OFFSET(TAG, OFFSET, ELIMINATE, ...)            \
    {                                                                   \
        num_t num[3];                                                   \
        if(show) printf("\n\t\t%s %d\t\t", __func__, TAG);              \
        num_create_immed_vec(num, 3, __VA_ARGS__);                      \
        chunk_p chunk_bef = num_get_chunk(num[0], OFFSET);              \
        chunk_p chunk_aft = num_sub_offset(&num[0], chunk_bef, num[1]); \
        CLU_CHECK_PTR(chunk_aft);                                       \
        assert(num_str(num[0], num[2]));                                \
        if(ELIMINATE) {assert(chunk_aft == NULL);}                      \
        else          {assert(chunk_aft == chunk_bef);}                 \
    }

    TEST_NUM_SUB_OFFSET(1, 0, true,
        0,
        0,
        0
    );
    TEST_NUM_SUB_OFFSET(2, 0, false,
        0,
        0,
        0
    );
    TEST_NUM_SUB_OFFSET(3, 1, true,
        2, 1, 2,
        1, 1,
        1, 2
    );
    TEST_NUM_SUB_OFFSET(4, 1, true,
        2, 1, 0,
        1, 1,
        1, 0
    );
    TEST_NUM_SUB_OFFSET(5, 1, true,
        3, 1, 2, 3,
        2, 1, 2,
        1, 3
    );
    TEST_NUM_SUB_OFFSET(6, 1, false,
        3, 1, 2, 3,
        2, 1, 1,
        2, 1, 3
    );

    #undef TEST_NUM_SUB_OFFSET

    #define TEST_NUM_SUB_OFFSET(TAG, OFFSET, ...)           \
    {                                                       \
        num_t num[3];                                       \
        if(show) printf("\n\t\t%s %d\t\t", __func__, TAG);  \
        num_create_immed_vec(num, 2, __VA_ARGS__);          \
        chunk_p chunk = num_get_chunk(num[0], OFFSET);      \
        TEST_REVERT_OPEN                                    \
        num_sub_offset(&num[0], chunk, num[1]);             \
        TEST_REVERT_CLOSE                                   \
        num_free(num[0]);                                   \
        num_free(num[1]);                                   \
    }

    TEST_NUM_SUB_OFFSET(7, 0,
        0,
        1, 1
    );
    TEST_NUM_SUB_OFFSET(8, 1,
        2, 2, 1,
        1, 3
    );
    TEST_NUM_SUB_OFFSET(9, 1,
        2, 2, 1,
        2, 1, 2
    );

    #undef TEST_NUM_SUB_OFFSET

    chunk_pool_clean();
    assert(clu_mem_empty());
}



void test_num_is_zero(bool show)
{
    printf("\n\t%s", __func__);

    #define TEST_NUM_IS_ZERO(TAG, IS_ZERO, ...)             \
    {                                                       \
        if(show) printf("\n\t\t%s %d\t\t", __func__, TAG);  \
        num_t num = num_create_immed(__VA_ARGS__);          \
        bool res = num_is_zero(num);                        \
        assert(res == IS_ZERO);                             \
        num_free(num);                                      \
    }

    TEST_NUM_IS_ZERO(1, true, 0);
    TEST_NUM_IS_ZERO(1, false, 1, 1);
    TEST_NUM_IS_ZERO(1, false, 2, 1, 2);

    #undef TEST_NUM_IS_ZERO

    chunk_pool_clean();
    assert(clu_mem_empty());
}

void test_num_cmp(bool show)
{
    printf("\n\t%s\t\t", __func__);

    #define TEST_NUM_CMP(TAG, RELATION, ...)                \
    {                                                       \
        num_t num[2];                                       \
        if(show) printf("\n\t\t%s %2d\t\t", __func__, TAG); \
        num_create_immed_vec(num, 2, __VA_ARGS__);          \
        int64_t cmp = num_cmp(num[0], num[1]);              \
        assert(cmp RELATION 0);                             \
        num_free(num[0]);                                   \
        num_free(num[1]);                                   \
    }

    TEST_NUM_CMP( 1, ==,
        0,
        0
    );
    TEST_NUM_CMP( 2, >,
        1, 1,
        0
    );
    TEST_NUM_CMP( 3, <,
        0,
        1, 1
    );
    TEST_NUM_CMP( 4, <,
        1, 1,
        1, 2
    );
    TEST_NUM_CMP( 5, ==,
        1, 2,
        1, 2
    );
    TEST_NUM_CMP( 6, >,
        1, 3,
        1, 2
    );
    TEST_NUM_CMP( 7, >,
        2, 2, 0,
        2, 1, 0
    );
    TEST_NUM_CMP( 8, ==,
        2, 2, 0,
        2, 2, 0
    );
    TEST_NUM_CMP( 9, <,
        2, 2, 0,
        2, 3, 0
    );
    TEST_NUM_CMP(10, <,
        2, 0x8000000000000000, 0,
        2, 0x8000000000000000, 0x7fffffffffffffff
    );
    TEST_NUM_CMP(11, <,
        3, 0x8000000000000000, 0, 0,
        3, 0x8000000000000000, 0x7fffffffffffffff, 0x8000000000000000
    );

    #undef TEST_NUM_CMP

    chunk_pool_clean();
    assert(clu_mem_empty());
}



void test_num_shl(bool show)
{
    printf("\n\t%s", __func__);

    #define TEST_NUM_SHL(TAG, BITS, ...)                \
    {                                                   \
        num_t num[2];                                   \
        if(show) printf("\n\t\t%s %d", __func__, TAG);  \
        num_create_immed_vec(num, 2, __VA_ARGS__);      \
        num[0] = num_shl(num[0], BITS);                 \
        assert(num_str(num[0], num[1]));                \
    }

    TEST_NUM_SHL(1, 0,
        0,
        0
    );
    TEST_NUM_SHL(2, 1,
        0,
        0
    );
    TEST_NUM_SHL(3, 1,
        1, 1,
        1, 2
    );
    TEST_NUM_SHL(4, 64,
        1, 1,
        2, 1, 0
    );
    TEST_NUM_SHL(5, 65,
        1, 1,
        2, 2, 0
    );

    #undef TEST_NUM_SHL

    chunk_pool_clean();
    assert(clu_mem_empty());
}

void test_num_shr(bool show)
{
    printf("\n\t%s\t\t", __func__);

    #define TEST_NUM_SHR(TAG, BITS, ...)                \
    {                                                   \
        num_t num[2];                                   \
        if(show) printf("\n\t\t%s %d", __func__, TAG);  \
        num_create_immed_vec(num, 2, __VA_ARGS__);      \
        num[0] = num_shr(num[0], BITS);                 \
        assert(num_str(num[0], num[1]));                \
    }

    TEST_NUM_SHR(1, 0,
        0,
        0
    );
    TEST_NUM_SHR(2, 1,
        0,
        0
    );
    TEST_NUM_SHR(3, 1,
        1, 1,
        0
    );
    TEST_NUM_SHR(4, 1,
        1, 2,
        1, 1
    );
    TEST_NUM_SHR(5, 64,
        2, 1, 0,
        1, 1
    );

    #undef TEST_NUM_SHR

    chunk_pool_clean();
    assert(clu_mem_empty());
}




void test_num_add(bool show)
{
    printf("\n\t%s", __func__);

    #define TEST_NUM_ADD(TAG, ...)                      \
    {                                                   \
        num_t num[3];                                   \
        if(show) printf("\n\t\t%s %d", __func__, TAG);  \
        num_create_immed_vec(num, 3, __VA_ARGS__);      \
        num[0] = num_add(num[0], num[1]);               \
        assert(num_str(num[0], num[2]));                \
    }

    TEST_NUM_ADD(1,
        0,
        0,
        0
    )
    TEST_NUM_ADD(2,
        1, 1,
        0,
        1, 1
    );
    TEST_NUM_ADD(3,
        0,
        1, 1,
        1, 1
    );
    TEST_NUM_ADD(4,
        1, 1,
        1, 2,
        1, 3
    );
    TEST_NUM_ADD(5,
        2, 2, 1,
        1, 2,
        2, 2, 3
    );
    TEST_NUM_ADD(6,
        1, 1,
        2, 2, 3,
        2, 2, 4
    );
    TEST_NUM_ADD(7,
        2, UINT64_MAX, UINT64_MAX,
        1, 1,
        3, 1, 0, 0
    );
    TEST_NUM_ADD(8,
        1, 1,
        2, UINT64_MAX, UINT64_MAX,
        3, 1, 0, 0
    );

    #undef TEST_NUM_ADD

    chunk_pool_clean();
    assert(clu_mem_empty());
}

void test_num_sub(bool show)
{
    printf("\n\t%s", __func__);

    #define TEST_NUM_SUB(TAG, ...)                          \
    {                                                       \
        num_t num[3];                                       \
        if(show) printf("\n\t\t%s %d\t\t", __func__, TAG);  \
        num_create_immed_vec(num, 3, __VA_ARGS__);          \
        num[0] = num_sub(num[0], num[1]);                   \
        assert(num_str(num[0], num[2]));                    \
    }

    TEST_NUM_SUB(1,
        0,
        0,
        0
    );
    TEST_NUM_SUB(2,
        1, 1,
        0,
        1, 1
    );
    TEST_NUM_SUB(3,
        1, 2,
        1, 1,
        1, 1
    );
    TEST_NUM_SUB(4,
        1, 2,
        1, 2,
        0
    );
    TEST_NUM_SUB(5,
        2, 1, 0,
        1, 1,
        1, UINT64_MAX
    );
    TEST_NUM_SUB(6,
        2, 4, 3,
        2, 1, 2,
        2, 3, 1
    );
    TEST_NUM_SUB(7,
        2, 1, 0,
        2, 1, 0,
        0
    );

    #undef TEST_NUM_SUB

    chunk_pool_clean();
    assert(clu_mem_empty());
}

void test_num_mul(bool show)
{
    printf("\n\t%s", __func__);

    #define TEST_NUM_MUL(TAG, ...)                      \
    {                                                   \
        num_t num[3];                                   \
        if(show) printf("\n\t\t%s %2d", __func__, TAG); \
        num_create_immed_vec(num, 3, __VA_ARGS__);      \
        num[0] = num_mul(num[0], num[1]);               \
        assert(num_str(num[0], num[2]));                \
    }

    TEST_NUM_MUL( 1,
        0,
        0,
        0
    );
    TEST_NUM_MUL( 2,
        1, 1,
        0,
        0
    );
    TEST_NUM_MUL( 3,
        0,
        1, 1,
        0
    );
    TEST_NUM_MUL( 4,
        1, 2,
        1, 3,
        1, 6
    );
    TEST_NUM_MUL( 5,
        1, UINT64_MAX,
        1, UINT64_MAX,
        2, UINT64_MAX - 1, 1
    );
    TEST_NUM_MUL( 6,
        2, 2, 3,
        1, 4,
        2, 8, 12
    );
    TEST_NUM_MUL( 7,
        1, 2,
        2, 3, 4,
        2, 6, 8
    );
    TEST_NUM_MUL( 8,
        2, 2, 3,
        2, 4, 5,
        3, 8, 22, 15
    );
    TEST_NUM_MUL( 9,
        2, 1, 0,
        1, 1,
        2, 1, 0
    );
    TEST_NUM_MUL(10,
        1, 1,
        2, 1, 0,
        2, 1, 0
    );
    TEST_NUM_MUL(11,
        2, 1, 0,
        2, 1, 0,
        3, 1, 0, 0
    );
    TEST_NUM_MUL(12,
        1, 2,
        2, 2, UINT64_MAX,
        2, 5, UINT64_MAX - 1
    );
    TEST_NUM_MUL(13,
        2, UINT64_MAX, UINT64_MAX,
        2, UINT64_MAX, UINT64_MAX,
        4, UINT64_MAX, UINT64_MAX - 1, 0, 1
    );
    TEST_NUM_MUL(14,
        3, 2, 3, 4,
        3, 5, 6, 7,
        5, 10, 27, 52, 45, 28
    );
    TEST_NUM_MUL(14,
        3, 2, 3, 4,
        3, 5, 6, 7,
        5, 10, 27, 52, 45, 28
    );

    #undef TEST_NUM_MUL

    chunk_pool_clean();
    assert(clu_mem_empty());
}

void test_num_sqr(bool show)
{
    printf("\n\t%s\t\t", __func__);

    #define TEST_NUM_SQR(TAG, ...)                          \
    {                                                       \
        num_t num[2];                                       \
        if(show) printf("\n\t\t%s %d\t\t", __func__, TAG);  \
        num_create_immed_vec(num, 2, __VA_ARGS__);          \
        num[0] = num_sqr(num[0]);                           \
        assert(num_str(num[0], num[1]));                    \
    }

    TEST_NUM_SQR(1,
        0,
        0
    );
    TEST_NUM_SQR(2,
        1, 1,
        1, 1
    );
    TEST_NUM_SQR(3,
        1, 2,
        1, 4
    );
    TEST_NUM_SQR(4,
        1, UINT32_MAX,
        1, 0xfffffffe00000001
    );
    TEST_NUM_SQR(5,
        1, (uint64_t)1 << 32,
        2, 1, 0
    );
    TEST_NUM_SQR(6,
        2, 1, 0,
        3, 1, 0, 0
    );
    TEST_NUM_SQR(7,
        2, 2, 3,
        3, 4, 12, 9
    );
    TEST_NUM_SQR(8,
        2, UINT64_MAX, UINT64_MAX,
        4, UINT64_MAX, UINT64_MAX - 1, 0, 1
    );

    #undef TEST_NUM_SQR

    chunk_pool_clean();
    assert(clu_mem_empty());
}

void test_num_div_mod(bool show)
{
    printf("\n\t%s", __func__);

    #define TEST_NUM_DIV_MOD(TAG, ...)                      \
    {                                                       \
        num_t num_q, num_r, num[4];                         \
        if(show) printf("\n\t\t%s %2d\t\t", __func__, TAG); \
        num_create_immed_vec(num, 4, __VA_ARGS__);          \
        num_div_mod(&num_q, &num_r, num[0], num[1]);        \
        assert(num_str(num_q, num[2]));                     \
        assert(num_str(num_r, num[3]));                     \
        chunk_pool_clean();                                 \
        assert(clu_mem_empty());                            \
    }

    TEST_NUM_DIV_MOD( 1,
        0,
        1, 1,
        0,
        0
    );
    TEST_NUM_DIV_MOD( 2,
        1, 4,
        1, 2,
        1, 2,
        0
    );
    TEST_NUM_DIV_MOD( 3,
        1, 5,
        1, 2,
        1, 2,
        1, 1
    );
    TEST_NUM_DIV_MOD( 4,
        1, 5,
        1, 5,
        1, 1,
        0
    );
    TEST_NUM_DIV_MOD( 5,
        1, 9,
        1, 3,
        1, 3,
        0
    );
    TEST_NUM_DIV_MOD( 6,
        3, 1, 0, 0,
        2, 1, 0,
        2, 1, 0,
        0
    );
    TEST_NUM_DIV_MOD( 7,
        1, 1,
        2, 1, 0,
        0,
        1, 1
    );
    TEST_NUM_DIV_MOD( 8,
        2, 4, UINT64_MAX,
        2, 2, 0,
        1, 2,
        1, UINT64_MAX
    );
    TEST_NUM_DIV_MOD( 9,
        2, 4, 0,
        2, 2, UINT64_MAX,
        1, 1,
        2, 1, 1
    );
    TEST_NUM_DIV_MOD(10,
        2, 1, 0,
        1, UINT64_MAX,
        1, 1,
        1, 1
    );
    TEST_NUM_DIV_MOD(11,
        2, UINT64_MAX, 0,
        2, 1, UINT64_MAX,
        1, 0x7fffffffffffffff,
        2, 1, 0x7fffffffffffffff
    );
    TEST_NUM_DIV_MOD(12,
        2, 0xc929d7d593, 0xb7090a859117cfa4,
        2, 6, 0xea7db545decb57a4,
        1, 0x0000001d1635b735,
        1, 0x88c80995d8646eb0
    );
    TEST_NUM_DIV_MOD(13,
        3, UINT64_MAX, 0, UINT64_MAX,
        1, UINT64_MAX,
        3, 1, 0, 1,
        0
    );
    TEST_NUM_DIV_MOD(14,
        5, UINT64_MAX, 0, 0, 0, UINT64_MAX,
        1, UINT64_MAX,
        5, 1, 0, 0, 0, 1,
        0
    );
    TEST_NUM_DIV_MOD(15,
        6, 4, 0, 8, 4, 0, 0,
        4, 4, 0, 0, 4,
        3, 1, 0, 1,
        4, 3, UINT64_MAX, UINT64_MAX, UINT64_MAX - 3
    );
    TEST_NUM_DIV_MOD(16,
        3, 1, 1, 0,
        2, 1, 2,
        1, UINT64_MAX,
        1, 2
    );
    TEST_NUM_DIV_MOD(17,
        3, UINT64_MAX, UINT64_MAX, UINT64_MAX,
        2, UINT64_MAX, UINT64_MAX,
        2, 1, 0,
        1, UINT64_MAX
    );
    TEST_NUM_DIV_MOD(18,
        3, 1, 0, 0,
        2, 1, UINT64_MAX,
        1, 0x8000000000000000,
        1, 0x8000000000000000
    );
    TEST_NUM_DIV_MOD(19,
        4, 1, 0, 0, 0,
        3, 1, 0, UINT64_MAX,
        1, UINT64_MAX,
        2, 1, UINT64_MAX
    );
    TEST_NUM_DIV_MOD(20,
        3, 1, UINT64_MAX - 1, 0,
        2, 1, UINT64_MAX,
        1, UINT64_MAX,
        1, UINT64_MAX
    );
    TEST_NUM_DIV_MOD(21,
        4, 1, UINT64_MAX, UINT64_MAX - 1, 0,
        3, 1, UINT64_MAX, UINT64_MAX,
        1, UINT64_MAX,
        3, 1, UINT64_MAX - 1, UINT64_MAX
    );

    #undef TEST_NUM_DIV_MOD

    if(show) printf("\n\t\t%s 22\t\t", __func__);
    num_t num_1 = num_create_immed(1, 1);
    num_t num_2 = num_create_immed(0);
    num_t num_q, num_r;
    TEST_REVERT_OPEN
    num_div_mod(&num_q, &num_r, num_1, num_2);
    TEST_REVERT_CLOSE
    num_free(num_1);
    num_free(num_2);

    chunk_pool_clean();
    assert(clu_mem_empty());
}



void test_num()
{
    printf("\n%s", __func__);

    bool show = true;

    test_uint_from_char(show);
    test_uint128(show);

    test_chunk_create(show);

    test_num_create_immed(show);
    test_num_create_immed_vec(show);

    test_num_insert(show);
    test_num_insert_head(show);
    test_num_remove_head(show);
    test_num_denormalize(show);
    test_num_normalize(show);
    test_num_break(show);

    test_num_wrap(show);
    test_num_wrap_dec(show);
    test_num_wrap_hex(show);
    test_num_wrap_str(show);
    test_num_read_dec(show);
    test_num_unwrap(show);
    test_num_copy(show);

    test_num_sub_uint_offset(show);

    test_num_shl_uint(show);
    test_num_shr_uint(show);
    test_num_add_uint(show);
    test_num_sub_uint(show);
    test_num_mul_uint(show);
    test_num_add_mul_uint(show);

    test_num_sub_offset(show);

    test_num_is_zero(show);
    test_num_cmp(show);

    test_num_shl(show);
    test_num_shr(show);

    test_num_add(show);
    test_num_sub(show);
    test_num_mul(show);
    test_num_sqr(show);
    test_num_div_mod(show);

    test_num_base_to(show);
    test_num_base_from(show);

    chunk_pool_clean();
    assert(clu_mem_empty());
}



int main()
{
    setbuf(stdout, NULL);
    test_num();
    printf("\n\n\tTest successful\n\n");
    return 0;
}
