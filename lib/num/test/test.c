#include "../debug.h"
#include "../../../utils/U64.h"
#include "../../../utils/assert.h"
#include "../../../utils/clu/header.h"



void test_uint_from_char(bool show)
{
    printf("\n\t%s", __func__);

    #define TEST_UINT_FROM_CHAR(TAG, CHAR, UINT)            \
    {                                                       \
        if(show) printf("\n\t\t%s " #TAG "\t\t", __func__); \
        uint64_t res = uint_from_char(CHAR);                \
        assert(uint64(res, UINT));                          \
    }

    TEST_UINT_FROM_CHAR(1, '0',  0);
    TEST_UINT_FROM_CHAR(2, '1',  1);
    TEST_UINT_FROM_CHAR(3, '9',  9);
    TEST_UINT_FROM_CHAR(4, 'a', 10);
    TEST_UINT_FROM_CHAR(5, 'b', 11);
    TEST_UINT_FROM_CHAR(6, 'f', 15);
    TEST_UINT_FROM_CHAR(7, 'A', 10);
    TEST_UINT_FROM_CHAR(8, 'B', 11);
    TEST_UINT_FROM_CHAR(9, 'F', 15);

    #undef TEST_UINT_FROM_CHAR

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

void test_chunk_consume(bool show)
{
    printf("\n\t%s", __func__);

    if(show) printf("\n\t\t%s 1\t\t", __func__);
    chunk_p chunk = chunk_consume(NULL);
    assert(chunk == NULL);

    if(show) printf("\n\t\t%s 2\t\t", __func__);
    chunk = chunk_create(1, NULL, NULL);
    chunk = chunk_consume(chunk);
    assert(chunk == NULL);

    if(show) printf("\n\t\t%s 2\t\t", __func__);
    chunk = chunk_create(1, NULL, NULL);
    chunk = chunk_create(2, chunk, NULL);
    chunk = chunk_consume(chunk);
    assert(chunk != NULL);
    assert(chunk->value == 1);
    assert(chunk->prev == NULL);
    chunk_free(chunk);

    chunk_pool_clean();
    assert(clu_mem_empty());
}



void test_num_create_immed(bool show)
{
    printf("\n\t%s\t\t", __func__);

    if(show) printf("\n\t\t%s 1\t\t", __func__);
    num_p num = num_create_immed(0);
    assert(num != NULL);
    assert(num->count == 0);
    assert(num->head == NULL);
    assert(num->tail == NULL);
    num_free(num);

    if(show) printf("\n\t\t%s 2\t\t", __func__);
    num = num_create_immed(1, 2);
    assert(num != NULL);
    assert(num->count == 1);
    assert(num->head != NULL);
    assert(num->head->value == 2);
    assert(num->head->next == NULL);
    assert(num->head->prev == NULL);
    assert(num->tail == num->head);
    num_free(num);

    if(show) printf("\n\t\t%s 3\t\t", __func__);
    num = num_create_immed(2, 2, 1);
    assert(num != NULL);
    assert(num->count == 2);
    assert(num->head != NULL)
    assert(num->head->value == 1);
    assert(num->head->next != NULL);
    assert(num->head->prev == NULL);
    assert(num->head->next->value == 2);
    assert(num->head->next->next == NULL);
    assert(num->head->next->prev == num->head);
    assert(num->tail == num->head->next);
    num_free(num);

    if(show) printf("\n\t\t%s 4\t\t", __func__);
    num = num_create_immed(2, 2, 0);
    assert(num->count == 2);
    assert(num->head != NULL)
    assert(num->head->value == 0);
    assert(num->head->next != NULL);
    assert(num->head->prev == NULL);
    assert(num->head->next->value == 2);
    assert(num->head->next->next == NULL);
    assert(num->head->next->prev == num->head);
    assert(num->tail == num->head->next);
    num_free(num);

    chunk_pool_clean();
    assert(clu_mem_empty());
}

void test_num_create_immed_vec(bool show)
{
    printf("\n\t%s", __func__);

    if(show) printf("\n\t\t%s 1", __func__);
    num_p num[2];
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

    if(show) printf("\n\t\t%s 1", __func__);
    num_p num = num_create_immed(0);
    chunk_p chunk = num_insert(num, 0);
    assert(uint64(chunk->value, 0));
    assert(num->head == chunk);
    assert(num->tail == chunk);
    assert(num_immed(num, 1, 0));

    if(show) printf("\n\t\t%s 2", __func__);
    num = num_create_immed(0);
    chunk = num_insert(num, 1);
    assert(uint64(chunk->value, 1));
    assert(num->head == chunk);
    assert(num->tail == chunk);
    assert(num_immed(num, 1, 1));

    if(show) printf("\n\t\t%s 3", __func__);
    num = num_create_immed(1, 2);
    chunk = num_insert(num, 0);
    assert(uint64(chunk->value, 0));
    assert(num->head != chunk);
    assert(num->tail == chunk);
    assert(num_immed(num, 2, 0, 2));

    if(show) printf("\n\t\t%s 4", __func__);
    num = num_create_immed(1, 2);
    chunk = num_insert(num, 1);
    assert(uint64(chunk->value, 1));
    assert(num->head != chunk);
    assert(num->tail == chunk);
    assert(num_immed(num, 2, 1, 2));

    chunk_pool_clean();
    assert(clu_mem_empty());
}

void test_num_insert_head(bool show)
{
    printf("\n\t%s", __func__);

    if(show) printf("\n\t\t%s 1", __func__);
    num_p num = num_create_immed(0);
    chunk_p chunk = num_insert_head(num, 0);
    assert(uint64(chunk->value, 0));
    assert(num->head == chunk);
    assert(num->tail == chunk);
    assert(num_immed(num, 1, 0));

    if(show) printf("\n\t\t%s 2", __func__);
    num = num_create_immed(0);
    chunk = num_insert_head(num, 1);
    assert(uint64(chunk->value, 1));
    assert(num->head == chunk);
    assert(num->tail == chunk);
    assert(num_immed(num, 1, 1));

    if(show) printf("\n\t\t%s 3", __func__);
    num = num_create_immed(1, 2);
    chunk = num_insert_head(num, 0);
    assert(uint64(chunk->value, 0));
    assert(num->head == chunk);
    assert(num->tail != chunk);
    assert(num_immed(num, 2, 2, 0));

    if(show) printf("\n\t\t%s 4", __func__);
    num = num_create_immed(1, 2);
    chunk = num_insert_head(num, 1);
    assert(uint64(chunk->value, 1));
    assert(num->head == chunk);
    assert(num->tail != chunk);
    assert(num_immed(num, 2, 2, 1));

    chunk_pool_clean();
    assert(clu_mem_empty());
}

void test_num_remove_head(bool show)
{
    printf("\n\t%s", __func__);

    if(show) printf("\n\t\t%s 1\t\t", __func__);
    num_p num = num_create_immed(0);
    num_remove_head(num);
    assert(num_immed(num, 0));

    if(show) printf("\n\t\t%s 2\t\t", __func__);
    num = num_create_immed(1, 1);
    num_remove_head(num);
    assert(num_immed(num, 0));

    if(show) printf("\n\t\t%s 3\t\t", __func__);
    num = num_create_immed(2, 1, 2);\
    num_remove_head(num);
    assert(num_immed(num, 1, 1));

    chunk_pool_clean();
    assert(clu_mem_empty());
}

void test_num_insert_list(bool show)
{
    printf("\n\t%s", __func__);

    #define TEST_NUM_INSERT_LIST(TAG, ...)                                  \
    {                                                                       \
        num_p num[3];                                                       \
        if(show) printf("\n\t\t%s %d\t\t", __func__, TAG);                  \
        num_create_immed_vec(num, 3, __VA_ARGS__);                          \
        num_insert_list(num[0], num[1]->head, num[1]->tail, num[1]->count); \
        assert(num_str(num[0], num[2]));                                    \
        free(num[1]);                                                       \
    }

    TEST_NUM_INSERT_LIST(1, 
        0, 
        0, 
        0
    );
    TEST_NUM_INSERT_LIST(2, 
        0, 
        1, 2, 
        1, 2
    );
    TEST_NUM_INSERT_LIST(3, 
        1, 1, 
        0, 
        1, 1
    );
    TEST_NUM_INSERT_LIST(4, 
        1, 1, 
        1, 2, 
        2, 2, 1
    );
    TEST_NUM_INSERT_LIST(5, 
        2, 1, 2, 
        2, 3, 4, 
        4, 3, 4, 1, 2
    );

    #undef TEST_NUM_INSERT_LIST

    chunk_pool_clean();
    assert(clu_mem_empty());
}

void test_num_denormalize(bool show)
{
    printf("\n\t%s", __func__);

    #define TEST_NUM_DENORMALIZE(TAG, ...)                  \
    {                                                       \
        num_p num[2];                                       \
        if(show) printf("\n\t\t%s " #TAG "\t\t", __func__); \
        num_create_immed_vec(num, 2, __VA_ARGS__);          \
        chunk_p chunk = num_denormalize(num[0], NULL);      \
        assert(num[0]->tail == chunk);                      \
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
    num_p num =  num_create_immed(1, 1);
    chunk_p chunk = num_denormalize(num, num->head);
    assert(num->head == chunk);
    assert(num_immed(num, 1, 1));

    chunk_pool_clean();
    assert(clu_mem_empty());
}

void test_num_normalize(bool show)
{
    printf("\n\t%s", __func__);

    #define TEST_NUM_NORMALIZE(TAG, RES, ...)               \
    {                                                       \
        num_p num[2];                                       \
        if(show) printf("\n\t\t%s %d\t\t", __func__, TAG);  \
        num_create_immed_vec(num, 2, __VA_ARGS__);          \
        bool res = num_normalize(num[0]);                   \
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
        num_p num[3], num_h, num_l;                         \
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
        num_p num = num_wrap(NUM);                      \
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
        num_p num = num_wrap_dec(STR);                  \
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
        num_p num = num_wrap_hex(STR);                  \
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

    chunk_pool_clean();
    assert(clu_mem_empty());
}

void test_num_wrap_str(bool show)
{
    printf("\n\t%s", __func__);

    #define TEST_NUM_WRAP_STR(TAG, STR, ...)            \
    {                                                   \
        if(show) printf("\n\t\t%s %2d", __func__, TAG); \
        num_p num = num_wrap_str(STR);                  \
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
        if(show) printf("\n\t\t%s " #TAG "", __func__);         \
        num_p num = num_read_dec("numbers/num_" #TAG ".txt");   \
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

    chunk_pool_clean();
    assert(clu_mem_empty());
}

void test_num_copy(bool show)
{
    printf("\n\t%s", __func__);
    
    #define TEST_NUM_COPY(TAG, ...)                     \
    {                                                   \
        if(show) printf("\n\t\t%s %d", __func__, TAG);  \
        num_p num = num_create_immed(__VA_ARGS__);      \
        num_p num_res = num_copy(num);                  \
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
        num_p num[2];                                       \
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
        num_p num[2];                                       \
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
        num_p num[2];                                               \
        if(show) printf("\n\t\t%s %d\t\t", __func__, TAG);          \
        num_create_immed_vec(num, 2, __VA_ARGS__);                  \
        chunk_p chunk = num_get_chunk(num[0], OFFSET);              \
        bool res = num_sub_uint_offset(num[0], chunk, VALUE);       \
        assert(num_str(num[0], num[1]));                            \
        assert(res == RES);                                         \
    }

    TEST_NUM_SUB_UINT_OFFSET(1, 0, 1, false,
        2, 2, 3,
        2, 2, 2
    );
    TEST_NUM_SUB_UINT_OFFSET(2, 1, 1, false,
        2, 2, 3,
        2, 1, 3
    );
    TEST_NUM_SUB_UINT_OFFSET(3, 1, 1, true,
        2, 1, 3,
        2, 1, 3
    )
    TEST_NUM_SUB_UINT_OFFSET(4, 1, 1, true,
        2, 1, 0,
        1, 0
    );
    TEST_NUM_SUB_UINT_OFFSET(5, 2, 1, true,
        3, 1, 0, 1,
        2, 0, 1
    );
    TEST_NUM_SUB_UINT_OFFSET(6, 1, 1, false,
        3, 1, 0, 1,
        2, UINT64_MAX, 1
    );
    TEST_NUM_SUB_UINT_OFFSET(6, 1, 1, true,
        2, 1, 0,
        1, 0
    );

    #undef TEST_NUM_SUB_UINT_OFFSET

    chunk_pool_clean();
    assert(clu_mem_empty());
}



void test_num_shl_uint(bool show) // TODO refactor
{
    printf("\n\t%s", __func__);

    #define TEST_NUM_SHL_UINT(TAG, VALUE, ...)          \
    {                                                   \
        num_p num[2];                                   \
        if(show) printf("\n\t\t%s %d", __func__, TAG);  \
        num_create_immed_vec(num, 2, __VA_ARGS__);      \
        num_shl_uint(num[0], VALUE);                    \
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

void test_num_shr_uint(bool show) // TODO refactor
{
    printf("\n\t%s", __func__);

    if(show) printf("\n\t\t%s 1", __func__);
    num_p num = num_create_immed(0);
    num_shr(num, 1);
    assert(num_immed(num, 0));

    if(show) printf("\n\t\t%s 2", __func__);
    num = num_create_immed(1, 1);
    num_shr(num, 0);
    assert(num_immed(num, 1, 1));

    if(show) printf("\n\t\t%s 3", __func__);
    num = num_create_immed(1, 1);
    num_shr(num, 1);
    assert(num_immed(num, 0));

    if(show) printf("\n\t\t%s 4", __func__);
    num = num_create_immed(1, 2);
    num_shr(num, 1);
    assert(num_immed(num, 1, 1));

    if(show) printf("\n\t\t%s 5", __func__);
    num = num_create_immed(1, UINT64_MAX);
    num_shr(num, 63);
    assert(num_immed(num, 1, 1));

    if(show) printf("\n\t\t%s 6", __func__);
    num = num_create_immed(2, 1, 0x8000000000000000);
    num_shr(num, 63);
    assert(num_immed(num, 1, 3));


    chunk_pool_clean();
    assert(clu_mem_empty());
}

void test_num_add_uint(bool show) // TODO refactor
{
    printf("\n\t%s", __func__);

    if(show) printf("\n\t\t%s 1", __func__);
    num_p num = num_create_immed(0);
    num_add_uint(num, 0);
    assert(num_immed(num, 0));

    if(show) printf("\n\t\t%s 2", __func__);
    num = num_create_immed(0);
    num_add_uint(num, 1);
    assert(num_immed(num, 1, 1));

    if(show) printf("\n\t\t%s 3", __func__);
    num = num_create_immed(1, 1);
    num_add_uint(num, 2);
    assert(num_immed(num, 1, 3));

    if(show) printf("\n\t\t%s 4", __func__);
    num = num_create_immed(1, UINT64_MAX);
    num_add_uint(num, 3);
    assert(num_immed(num, 2, 1, 2));

    chunk_pool_clean();
    assert(clu_mem_empty());
}

void test_num_sub_uint(bool show) // TODO refactor
{
    printf("\n\t%s\t\t", __func__);

    if(show) printf("\n\t\t%s 1\t\t", __func__);
    num_p num = num_create_immed(0);
    num_sub_uint(num, 0);
    assert(num_immed(num, 0));

    if(show) printf("\n\t\t%s 2\t\t", __func__);
    num = num_create_immed(1, 1);
    num_sub_uint(num, 0);
    assert(num_immed(num, 1, 1));

    if(show) printf("\n\t\t%s 3\t\t", __func__);
    num = num_create_immed(1, 1);
    num_sub_uint(num, 1);
    assert(num_immed(num, 0));

    if(show) printf("\n\t\t%s 4\t\t", __func__);
    num = num_create_immed(1, 2);
    num_sub_uint(num, 1);
    assert(num_immed(num, 1, 1));

    if(show) printf("\n\t\t%s 5\t\t", __func__);
    num = num_create_immed(2, 1, 0);
    num_sub_uint(num, 1);
    assert(num_immed(num, 1, UINT64_MAX));

    chunk_pool_clean();
    assert(clu_mem_empty());
}

void test_num_mul_uint(bool show) // TODO refactor
{
    printf("\n\t%s\t\t", __func__);

    if(show) printf("\n\t\t%s  1\t\t", __func__);
    num_p num = num_create_immed(0);
    num_p num_res = num_mul_uint(NULL, num, 0);
    assert(num_immed(num_res, 0));
    num_free(num);

    if(show) printf("\n\t\t%s  2\t\t", __func__);
    num = num_create_immed(0);
    num_res = num_mul_uint(NULL, num, 1);
    assert(num_immed(num_res, 0));
    num_free(num);

    if(show) printf("\n\t\t%s  3\t\t", __func__);
    num = num_create_immed(1, 2);
    num_res = num_mul_uint(NULL, num, 0);
    assert(num_immed(num_res, 0));
    num_free(num);

    if(show) printf("\n\t\t%s  4\t\t", __func__);
    num = num_create_immed(1, 2);
    num_res = num_mul_uint(NULL, num, 3);
    assert(num_immed(num_res, 1, 6));
    num_free(num);

    if(show) printf("\n\t\t%s  5\t\t", __func__);
    num = num_create_immed(1, 2);
    num_res = num_mul_uint(NULL, num, UINT64_MAX);
    assert(num_immed(num_res, 2, 1, UINT64_MAX - 1));
    num_free(num);

    if(show) printf("\n\t\t%s  6\t\t", __func__);
    num = num_create_immed(2, 2, 3);
    num_res = num_mul_uint(NULL, num, 4);
    assert(num_immed(num_res, 2, 8, 12));
    num_free(num);

    if(show) printf("\n\t\t%s  7\t\t", __func__);
    num = num_create_immed(2, UINT64_MAX, UINT64_MAX);
    num_res = num_mul_uint(NULL, num, UINT64_MAX);
    assert(num_immed(num_res, 3, UINT64_MAX - 1, UINT64_MAX, 1));
    num_free(num);

    if(show) printf("\n\t\t%s  8\t\t", __func__);
    num = num_create_immed(2, 2, 0);
    num_res = num_mul_uint(NULL, num, 3);
    assert(num_immed(num_res, 2, 6, 0));
    num_free(num);

    if(show) printf("\n\t\t%s  9\t\t", __func__);
    num = num_create_immed(0);
    num_res = num_create_immed(1, 1);
    num_res = num_mul_uint(num_res, num, 0);
    assert(num_immed(num_res, 1, 1));
    num_free(num);

    if(show) printf("\n\t\t%s 10\t\t", __func__);
    num = num_create_immed(0);
    num_res = num_create_immed(1, 1);
    num_res = num_mul_uint(num_res, num, 1);
    assert(num_immed(num_res, 1, 1));
    num_free(num);

    if(show) printf("\n\t\t%s 11\t\t", __func__);
    num_res = num_create_immed(1, 1);
    num = num_create_immed(1, 2);
    num_res = num_mul_uint(num_res, num, 0);
    assert(num_immed(num_res, 1, 1));
    num_free(num);

    if(show) printf("\n\t\t%s 12\t\t", __func__);
    num_res = num_create_immed(1, 1);
    num = num_create_immed(1, 2);
    num_res = num_mul_uint(num_res, num, 3);
    assert(num_immed(num_res, 1, 7));
    num_free(num);

    if(show) printf("\n\t\t%s 13\t\t", __func__);
    num_res = num_create_immed(1, 1);
    num = num_create_immed(1, 2);
    num_res = num_mul_uint(num_res, num, UINT64_MAX);
    assert(num_immed(num_res, 2, 1, UINT64_MAX));
    num_free(num);

    if(show) printf("\n\t\t%s 14\t\t", __func__);
    num_res = num_create_immed(1, 1);
    num = num_create_immed(2, 2, 3);
    num_res = num_mul_uint(num_res, num, 4);
    assert(num_immed(num_res, 2, 8, 13));
    num_free(num);

    if(show) printf("\n\t\t%s 15\t\t", __func__);
    num_res = num_create_immed(1, 1);
    num = num_create_immed(2, UINT64_MAX, UINT64_MAX);
    num_res = num_mul_uint(num_res, num, UINT64_MAX);
    assert(num_immed(num_res, 3, UINT64_MAX - 1, UINT64_MAX, 2));
    num_free(num);

    if(show) printf("\n\t\t%s 16\t\t", __func__);
    num_res = num_create_immed(1, 1);
    num = num_create_immed(2, 2, 0);
    num_res = num_mul_uint(num_res, num, 3);
    assert(num_immed(num_res, 2, 6, 1));
    num_free(num);

    if(show) printf("\n\t\t%s 17\t\t", __func__);
    num = num_create_immed(2, 6, 0xea7db545decb57a4);
    num_res = num_mul_uint(NULL, num, 123427205323);
    assert(num_immed(num_res, 2, 0xc6bfba060d, 0xdf4c110fd2f33f0c));
    num_free(num);

    chunk_pool_clean();
    assert(clu_mem_empty());
}



void test_num_is_zero(bool show) // TODO refactor
{
    printf("\n\t%s", __func__);

    if(show) printf("\n\t\t%s 1\t\t", __func__);
    num_p num = num_create_immed(0);
    bool res = num_is_zero(num);
    assert(res == true);
    num_free(num);

    if(show) printf("\n\t\t%s 2\t\t", __func__);
    num = num_create_immed(1, 1);
    res = num_is_zero(num);
    assert(res == false);
    num_free(num);

    chunk_pool_clean();
    assert(clu_mem_empty());
}

void test_num_cmp(bool show) // TODO refactor
{
    printf("\n\t%s", __func__);

    if(show) printf("\n\t\t%s  1\t\t", __func__);
    num_p num_1 = num_create_immed(0);
    num_p num_2 = num_create_immed(0);
    int64_t cmp = num_cmp(num_1, num_2);
    assert(cmp == 0);
    num_free(num_1);
    num_free(num_2);

    if(show) printf("\n\t\t%s  2\t\t", __func__);
    num_1 = num_create_immed(1, 1);
    num_2 = num_create_immed(0);
    cmp = num_cmp(num_1, num_2);
    assert(cmp > 0);
    num_free(num_1);
    num_free(num_2);

    if(show) printf("\n\t\t%s  3\t\t", __func__);
    num_1 = num_create_immed(0);
    num_2 = num_create_immed(1, 1);
    cmp = num_cmp(num_1, num_2);
    assert(cmp < 0);
    num_free(num_1);
    num_free(num_2);

    if(show) printf("\n\t\t%s  4\t\t", __func__);
    num_1 = num_create_immed(1, 1);
    num_2 = num_create_immed(1, 2);
    cmp = num_cmp(num_1, num_2);
    assert(cmp < 0);
    num_free(num_1);
    num_free(num_2);

    if(show) printf("\n\t\t%s  5\t\t", __func__);
    num_1 = num_create_immed(1, 2);
    num_2 = num_create_immed(1, 2);
    cmp = num_cmp(num_1, num_2);
    assert(cmp == 0);
    num_free(num_1);
    num_free(num_2);

    if(show) printf("\n\t\t%s  6\t\t", __func__);
    num_1 = num_create_immed(1, 3);
    num_2 = num_create_immed(1, 2);
    cmp = num_cmp(num_1, num_2);
    assert(cmp > 0);
    num_free(num_1);
    num_free(num_2);

    if(show) printf("\n\t\t%s  7\t\t", __func__);
    num_1 = num_create_immed(2, 2, 0);
    num_2 = num_create_immed(2, 1, 0);
    cmp = num_cmp(num_1, num_2);
    assert(cmp > 0);
    num_free(num_1);
    num_free(num_2);

    if(show) printf("\n\t\t%s  8\t\t", __func__);
    num_1 = num_create_immed(2, 2, 0);
    num_2 = num_create_immed(2, 2, 0);
    cmp = num_cmp(num_1, num_2);
    assert(cmp == 0);
    num_free(num_1);
    num_free(num_2);

    if(show) printf("\n\t\t%s  9\t\t", __func__);
    num_1 = num_create_immed(2, 2, 0);
    num_2 = num_create_immed(2, 3, 0);
    cmp = num_cmp(num_1, num_2);
    assert(cmp < 0);
    num_free(num_1);
    num_free(num_2);

    if(show) printf("\n\t\t%s 10\t\t", __func__);
    num_1 = num_create_immed(2, 0x8000000000000000, 0);
    num_2 = num_create_immed(2, 0x8000000000000000, 0x7fffffffffffffff);
    cmp = num_cmp(num_1, num_2);
    assert(cmp < 0);
    num_free(num_1);
    num_free(num_2);

    if(show) printf("\n\t\t%s 11\t\t", __func__);
    num_1 = num_create_immed(3, 0x8000000000000000, 0, 0);
    num_2 = num_create_immed(3, 0x8000000000000000, 0x7fffffffffffffff, 0x8000000000000000);
    cmp = num_cmp(num_1, num_2);
    assert(cmp < 0);
    num_free(num_1);
    num_free(num_2);

    chunk_pool_clean();
    assert(clu_mem_empty());
}



void test_num_shl(bool show) // TODO refactor
{
    printf("\n\t%s", __func__);

    if(show) printf("\n\t\t%s 1", __func__);
    num_p num = num_create_immed(0);
    num_shl(num, 1);
    assert(num_immed(num, 0));

    if(show) printf("\n\t\t%s 2", __func__);
    num = num_create_immed(1, 1);
    num_shl(num, 1);
    assert(num_immed(num, 1, 2));

    if(show) printf("\n\t\t%s 3", __func__);
    num = num_create_immed(1, 1);
    num_shl(num, 64);
    assert(num_immed(num, 2, 1, 0));

    if(show) printf("\n\t\t%s 4", __func__);
    num = num_create_immed(1, 1);
    num_shl(num, 65);
    assert(num_immed(num, 2, 2, 0));

    chunk_pool_clean();
    assert(clu_mem_empty());
}

void test_num_shr(bool show) // TODO refactor
{
    printf("\n\t%s", __func__);

    if(show) printf("\n\t\t%s 1", __func__);
    num_p num = num_create_immed(0);
    num_shr(num, 1);
    assert(num_immed(num, 0));

    if(show) printf("\n\t\t%s 2", __func__);
    num = num_create_immed(1, 1);
    num_shr(num, 1);
    assert(num_immed(num, 0));

    if(show) printf("\n\t\t%s 3", __func__);
    num = num_create_immed(1, 2);
    num_shr(num, 1);
    assert(num_immed(num, 1, 1));

    if(show) printf("\n\t\t%s 4", __func__);
    num = num_create_immed(2, 1, 0);
    num_shr(num, 64);
    assert(num_immed(num, 1, 1));

    chunk_pool_clean();
    assert(clu_mem_empty());
}




void test_num_add(bool show) // TODO refactor
{
    printf("\n\t%s", __func__);

    if(show) printf("\n\t\t%s 1", __func__);
    num_p num_1 = num_create_immed(0);
    num_p num_2 = num_create_immed(0);
    num_p num_res = num_add(num_1, num_2);
    assert(num_immed(num_res, 0));

    if(show) printf("\n\t\t%s 2", __func__);
    num_1 = num_create_immed(1, 1);
    num_2 = num_create_immed(0);
    num_res = num_add(num_1, num_2);
    assert(num_immed(num_res, 1, 1));

    if(show) printf("\n\t\t%s 3", __func__);
    num_1 = num_create_immed(0);
    num_2 = num_create_immed(1, 1);
    num_res = num_add(num_1, num_2);
    assert(num_immed(num_res, 1, 1));

    if(show) printf("\n\t\t%s 4", __func__);
    num_1 = num_create_immed(1, 1);
    num_2 = num_create_immed(1, 2);
    num_res = num_add(num_1, num_2);
    assert(num_immed(num_res, 1, 3));

    if(show) printf("\n\t\t%s 5", __func__);
    num_1 = num_create_immed(2, 2, 1);
    num_2 = num_create_immed(1, 2);
    num_res = num_add(num_1, num_2);
    assert(num_immed(num_res, 2, 2, 3));

    if(show) printf("\n\t\t%s 6", __func__);
    num_1 = num_create_immed(1, 1);
    num_2 = num_create_immed(2, 2, 3);
    num_res = num_add(num_1, num_2);
    assert(num_immed(num_res, 2, 2, 4));

    if(show) printf("\n\t\t%s 7", __func__);
    num_1 = num_create_immed(2, UINT64_MAX, UINT64_MAX);
    num_2 = num_create_immed(1, 1);
    num_res = num_add(num_1, num_2);
    assert(num_immed(num_res, 3, 1, 0, 0));

    if(show) printf("\n\t\t%s 8", __func__);
    num_1 = num_create_immed(1, 1);
    num_2 = num_create_immed(2, UINT64_MAX, UINT64_MAX);
    num_res = num_add(num_1, num_2);
    assert(num_immed(num_res, 3, 1, 0, 0));

    chunk_pool_clean();
    assert(clu_mem_empty());
}

void test_num_sub(bool show) // TODO refactor
{
    printf("\n\t%s", __func__);

    if(show) printf("\n\t\t%s 1\t\t", __func__);
    num_p num_1 = num_create_immed(0);
    num_p num_2 = num_create_immed(0);
    num_p num_res = num_sub(num_1, num_2);
    assert(num_immed(num_res, 0));

    if(show) printf("\n\t\t%s 2\t\t", __func__);
    num_1 = num_create_immed(1, 1);
    num_2 = num_create_immed(0);
    num_res = num_sub(num_1, num_2);
    assert(num_immed(num_res, 1, 1));

    if(show) printf("\n\t\t%s 3\t\t", __func__);
    num_1 = num_create_immed(1, 2);
    num_2 = num_create_immed(1, 1);
    num_res = num_sub(num_1, num_2);
    assert(num_immed(num_res, 1, 1));

    if(show) printf("\n\t\t%s 3\t\t", __func__);
    num_1 = num_create_immed(1, 2);
    num_2 = num_create_immed(1, 2);
    num_res = num_sub(num_1, num_2);
    assert(num_immed(num_res, 0));

    if(show) printf("\n\t\t%s 4\t\t", __func__);
    num_1 = num_create_immed(2, 1, 0);
    num_2 = num_create_immed(1, 1);
    num_res = num_sub(num_1, num_2);
    assert(num_immed(num_res, 1, UINT64_MAX));

    if(show) printf("\n\t\t%s 5\t\t", __func__);
    num_1 = num_create_immed(2, 4, 3);
    num_2 = num_create_immed(2, 1, 2);
    num_res = num_sub(num_1, num_2);
    assert(num_immed(num_res, 2, 3, 1));

    if(show) printf("\n\t\t%s 6\t\t", __func__);
    num_1 = num_create_immed(2, 1, 0);
    num_2 = num_create_immed(2, 1, 0);
    num_res = num_sub(num_1, num_2);
    assert(num_immed(num_res, 0));

    chunk_pool_clean();
    assert(clu_mem_empty());
}

void test_num_mul(bool show) // TODO refactor
{
    printf("\n\t%s", __func__);

    if(show) printf("\n\t\t%s  1", __func__);
    num_p num_1 = num_create_immed(0);
    num_p num_2 = num_create_immed(0);
    num_p num_res = num_mul(num_1, num_2);
    assert(num_immed(num_res, 0));

    if(show) printf("\n\t\t%s  2", __func__);
    num_1 = num_create_immed(1, 1);
    num_2 = num_create_immed(0);
    num_res = num_mul(num_1, num_2);
    assert(num_immed(num_res, 0));

    if(show) printf("\n\t\t%s  3", __func__);
    num_1 = num_create_immed(0);
    num_2 = num_create_immed(1, 1);
    num_res = num_mul(num_1, num_2);
    assert(num_immed(num_res, 0));

    if(show) printf("\n\t\t%s  4", __func__);
    num_1 = num_create_immed(1, 2);
    num_2 = num_create_immed(1, 3);
    num_res = num_mul(num_1, num_2);
    assert(num_immed(num_res, 1, 6));

    if(show) printf("\n\t\t%s  5", __func__);
    num_1 = num_create_immed(1, UINT64_MAX);
    num_2 = num_create_immed(1, UINT64_MAX);
    num_res = num_mul(num_1, num_2);
    assert(num_immed(num_res, 2, UINT64_MAX - 1, 1));

    if(show) printf("\n\t\t%s  6", __func__);
    num_1 = num_create_immed(2, 2, 3);
    num_2 = num_create_immed(1, 4);
    num_res = num_mul(num_1, num_2);
    assert(num_immed(num_res, 2, 8, 12));

    if(show) printf("\n\t\t%s  7", __func__);
    num_1 = num_create_immed(1, 2);
    num_2 = num_create_immed(2, 3, 4);
    num_res = num_mul(num_1, num_2);
    assert(num_immed(num_res, 2, 6, 8));

    if(show) printf("\n\t\t%s  8", __func__);
    num_1 = num_create_immed(2, 2, 3);
    num_2 = num_create_immed(2, 4, 5);
    num_res = num_mul(num_1, num_2);
    assert(num_immed(num_res, 3, 8, 22, 15));

    if(show) printf("\n\t\t%s  9", __func__);
    num_1 = num_create_immed(2, 1, 0);
    num_2 = num_create_immed(1, 1);
    num_res = num_mul(num_1, num_2);
    assert(num_immed(num_res, 2, 1, 0));

    if(show) printf("\n\t\t%s 10", __func__);
    num_1 = num_create_immed(1, 1);
    num_2 = num_create_immed(2, 1, 0);
    num_res = num_mul(num_1, num_2);
    assert(num_immed(num_res, 2, 1, 0));

    if(show) printf("\n\t\t%s 11", __func__);
    num_1 = num_create_immed(2, 1, 0);
    num_2 = num_create_immed(2, 1, 0);
    num_res = num_mul(num_1, num_2);
    assert(num_immed(num_res, 3, 1, 0, 0));

    if(show) printf("\n\t\t%s 12", __func__);
    num_1 = num_create_immed(1, 2);
    num_2 = num_create_immed(2, 2, UINT64_MAX);
    num_res = num_mul(num_1, num_2);
    assert(num_immed(num_res, 2, 5, UINT64_MAX - 1));

    if(show) printf("\n\t\t%s 13", __func__);
    num_1 = num_create_immed(2, UINT64_MAX, UINT64_MAX);
    num_2 = num_create_immed(2, UINT64_MAX, UINT64_MAX);
    num_res = num_mul(num_1, num_2);
    assert(num_immed(num_res, 4, UINT64_MAX, UINT64_MAX - 1, 0, 1));

    if(show) printf("\n\t\t%s 14", __func__);
    num_1 = num_create_immed(3, 2, 3, 4);
    num_2 = num_create_immed(3, 5, 6, 7);
    num_res = num_mul(num_1, num_2);
    assert(num_immed(num_res, 5, 10, 27, 52, 45, 28));

    if(show) printf("\n\t\t%s 14", __func__);
    num_1 = num_create_immed(3, 2, 3, 4);
    num_2 = num_create_immed(3, 5, 6, 7);
    num_res = num_mul(num_1, num_2);
    assert(num_immed(num_res, 5, 10, 27, 52, 45, 28));

    chunk_pool_clean();
    assert(clu_mem_empty());
}

void test_num_div_mod(bool show) // TODO refactor
{
    printf("\n\t%s", __func__);

    num_p num_q, num_r;

    if(show) printf("\n\t\t%s  1\t\t", __func__);
    num_p num_1 = num_create_immed(0);
    num_p num_2 = num_create_immed(1, 1);
    num_div_mod(&num_q, &num_r, num_1, num_2);
    assert(num_immed(num_q, 0));
    assert(num_immed(num_r, 0));

    if(show) printf("\n\t\t%s  2\t\t", __func__);
    num_1 = num_create_immed(1, 4);
    num_2 = num_create_immed(1, 2);
    num_div_mod(&num_q, &num_r, num_1, num_2);
    assert(num_immed(num_q, 1, 2));
    assert(num_immed(num_r, 0));

    if(show) printf("\n\t\t%s  3\t\t", __func__);
    num_1 = num_create_immed(1, 5);
    num_2 = num_create_immed(1, 2);
    num_div_mod(&num_q, &num_r, num_1, num_2);
    assert(num_immed(num_q, 1, 2));
    assert(num_immed(num_r, 1, 1));

    if(show) printf("\n\t\t%s  4\t\t", __func__);
    num_1 = num_create_immed(1, 5);
    num_2 = num_create_immed(1, 5);
    num_div_mod(&num_q, &num_r, num_1, num_2);
    assert(num_immed(num_q, 1, 1));
    assert(num_immed(num_r, 0));

    if(show) printf("\n\t\t%s  5\t\t", __func__);
    num_1 = num_create_immed(1, 9);
    num_2 = num_create_immed(1, 3);
    num_div_mod(&num_q, &num_r, num_1, num_2);
    assert(num_immed(num_q, 1, 3));
    assert(num_immed(num_r, 0));

    if(show) printf("\n\t\t%s  6\t\t", __func__);
    num_1 = num_create_immed(3, 1, 0, 0);
    num_2 = num_create_immed(2, 1, 0);
    num_div_mod(&num_q, &num_r, num_1, num_2);
    assert(num_immed(num_q, 2, 1, 0));
    assert(num_immed(num_r, 0));

    if(show) printf("\n\t\t%s  7\t\t", __func__);
    num_1 = num_create_immed(1, 1);
    num_2 = num_create_immed(2, 1, 0);
    num_div_mod(&num_q, &num_r, num_1, num_2);
    assert(num_immed(num_q, 0));
    assert(num_immed(num_r, 1, 1));

    if(show) printf("\n\t\t%s  8\t\t", __func__);
    num_1 = num_create_immed(2, 4, UINT64_MAX);
    num_2 = num_create_immed(2, 2, 0);
    num_div_mod(&num_q, &num_r, num_1, num_2);
    assert(num_immed(num_q, 1, 2));
    assert(num_immed(num_r, 1, UINT64_MAX));

    if(show) printf("\n\t\t%s  9\t\t", __func__);
    num_1 = num_create_immed(2, 4, 0);
    num_2 = num_create_immed(2, 2, UINT64_MAX);
    num_div_mod(&num_q, &num_r, num_1, num_2);
    assert(num_immed(num_q, 1, 1));
    assert(num_immed(num_r, 2, 1, 1));

    if(show) printf("\n\t\t%s 10\t\t", __func__);
    num_1 = num_create_immed(2, 1, 0);
    num_2 = num_create_immed(1, UINT64_MAX);
    num_div_mod(&num_q, &num_r, num_1, num_2);
    assert(num_immed(num_q, 1, 1));
    assert(num_immed(num_r, 1, 1));

    if(show) printf("\n\t\t%s 11\t\t", __func__);
    num_1 = num_create_immed(2, UINT64_MAX, 0);
    num_2 = num_create_immed(2, 1, UINT64_MAX);
    num_div_mod(&num_q, &num_r, num_1, num_2);
    assert(num_immed(num_q, 1, 0x7fffffffffffffff));
    assert(num_immed(num_r, 2, 1, 0x7fffffffffffffff));

    if(show) printf("\n\t\t%s 12\t\t", __func__);
    num_1 = num_create_immed(2, 0xc929d7d593, 0xb7090a859117cfa4);
    num_2 = num_create_immed(2, 6, 0xea7db545decb57a4);
    num_div_mod(&num_q, &num_r, num_1, num_2);
    assert(num_immed(num_q, 1, 0x0000001d1635b735));
    assert(num_immed(num_r, 1, 0x88c80995d8646eb0));

    if(show) printf("\n\t\t%s 13\t\t", __func__);
    num_1 = num_create_immed(3, UINT64_MAX, 0, UINT64_MAX);
    num_2 = num_create_immed(1, UINT64_MAX);
    num_div_mod(&num_q, &num_r, num_1, num_2);
    assert(num_immed(num_q, 3, 1, 0, 1));
    assert(num_immed(num_r, 0));

    if(show) printf("\n\t\t%s 14\t\t", __func__);
    num_1 = num_create_immed(5, UINT64_MAX, 0, 0, 0, UINT64_MAX);
    num_2 = num_create_immed(1, UINT64_MAX);
    num_div_mod(&num_q, &num_r, num_1, num_2);
    assert(num_immed(num_q, 5, 1, 0, 0, 0, 1));
    assert(num_immed(num_r, 0));

    if(show) printf("\n\t\t%s 15\t\t", __func__);
    num_1 = num_create_immed(6, 4, 0, 8, 4, 0, 0);
    num_2 = num_create_immed(4, 4, 0, 0, 4);
    num_div_mod(&num_q, &num_r, num_1, num_2);
    assert(num_immed(num_q, 3, 1, 0, 1));
    assert(num_immed(num_r, 4, 3, UINT64_MAX, UINT64_MAX, UINT64_MAX - 3));

    if(show) printf("\n\t\t%s 16\t\t", __func__);
    num_1 = num_create_immed(3, 1, 1, 0);
    num_2 = num_create_immed(2, 1, 2);
    num_div_mod(&num_q, &num_r, num_1, num_2);
    assert(num_immed(num_q, 1, UINT64_MAX));
    assert(num_immed(num_r, 1, 2));

    if(show) printf("\n\t\t%s 17\t\t", __func__);
    num_1 = num_create_immed(3, UINT64_MAX, UINT64_MAX, UINT64_MAX);
    num_2 = num_create_immed(2, UINT64_MAX, UINT64_MAX);
    num_div_mod(&num_q, &num_r, num_1, num_2);
    assert(num_immed(num_q, 2, 1, 0));
    assert(num_immed(num_r, 1, UINT64_MAX));

    if(show) printf("\n\t\t%s 18\t\t", __func__);
    num_1 = num_create_immed(3, 1, 0, 0);
    num_2 = num_create_immed(2, 1, UINT64_MAX);
    num_div_mod(&num_q, &num_r, num_1, num_2);
    assert(num_immed(num_q, 1, 0x8000000000000000));
    assert(num_immed(num_r, 1, 0x8000000000000000));

    if(show) printf("\n\t\t%s 19\t\t", __func__);
    num_1 = num_create_immed(4, 1, 0, 0, 0);
    num_2 = num_create_immed(3, 1, 0, UINT64_MAX);
    num_div_mod(&num_q, &num_r, num_1, num_2);
    assert(num_immed(num_q, 1, UINT64_MAX));
    assert(num_immed(num_r, 2, 1, UINT64_MAX));

    if(show) printf("\n\t\t%s 20\t\t", __func__);
    num_1 = num_create_immed(3, 1, UINT64_MAX - 1, 0);
    num_2 = num_create_immed(2, 1, UINT64_MAX);
    num_div_mod(&num_q, &num_r, num_1, num_2);
    assert(num_immed(num_q, 1, UINT64_MAX));
    assert(num_immed(num_r, 1, UINT64_MAX));

    if(show) printf("\n\t\t%s 21\t\t", __func__);
    num_1 = num_create_immed(4, 1, UINT64_MAX, UINT64_MAX - 1, 0);
    num_2 = num_create_immed(3, 1, UINT64_MAX, UINT64_MAX);
    num_div_mod(&num_q, &num_r, num_1, num_2);
    assert(num_immed(num_q, 1, UINT64_MAX));
    assert(num_immed(num_r, 3, 1, UINT64_MAX - 1, UINT64_MAX));

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
    test_chunk_consume(show);

    test_num_create_immed(show);
    test_num_create_immed_vec(show);

    test_num_insert(show);
    test_num_insert_head(show);
    test_num_remove_head(show);
    test_num_insert_list(show);
    test_num_denormalize(show);
    test_num_normalize(show);
    test_num_break(show);

    test_num_wrap(show);
    test_num_wrap_dec(show);
    test_num_wrap_hex(show);
    test_num_wrap_str(show);
    test_num_read_dec(show);
    test_num_copy(show);

    test_num_base_to(show);
    test_num_base_from(show);

    test_num_sub_uint_offset(show);

    test_num_shl_uint(show);
    test_num_shr_uint(show);
    test_num_add_uint(show);
    test_num_sub_uint(show);
    test_num_mul_uint(show);

    test_num_is_zero(show);
    test_num_cmp(show);

    test_num_shl(show);
    test_num_shr(show);

    test_num_add(show);
    test_num_sub(show);
    test_num_mul(show);
    test_num_div_mod(show);

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
