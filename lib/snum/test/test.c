#include "../debug.h"
#include "../../num/debug.h"
#include "../../../utils/assert.h"
#include "../../../utils/clu/header.h"



void test_snum_create(bool show)
{
    printf("\n\t%s\t\t", __func__);

    if(show) printf("\n\t\t%s 1\t\t", __func__);
    num_t num = num_create_immed(1, 1);
    snum_t snum = snum_create(POSITIVE, num);
    assert(snum.signal == POSITIVE);
    assert(snum.num.count == num.count);
    assert(snum.num.head == num.head);
    assert(snum.num.tail == num.tail);
    snum_free(snum);

    if(show) printf("\n\t\t%s 2\t\t", __func__);
    num = num_create_immed(1, 1);
    snum = snum_create(NEGATIVE, num);
    assert(snum.signal == NEGATIVE);
    assert(snum.num.count == num.count);
    assert(snum.num.head == num.head);
    assert(snum.num.tail == num.tail);
    snum_free(snum);

    if(show) printf("\n\t\t%s 3\t\t", __func__);
    num = num_create_immed(0);
    snum = snum_create(POSITIVE, num);
    assert(snum.signal == ZERO);
    assert(snum.num.count == num.count);
    assert(snum.num.head == num.head);
    assert(snum.num.tail == num.tail);
    snum_free(snum);

    if(show) printf("\n\t\t%s 4\t\t", __func__);
    num = num_create_immed(0);
    snum = snum_create(ZERO, num);
    assert(snum.signal == ZERO);
    assert(snum.num.count == num.count);
    assert(snum.num.head == num.head);
    assert(snum.num.tail == num.tail);
    snum_free(snum);

    if(show) printf("\n\t\t%s 5\t\t", __func__);
    num = num_create_immed(0);
    snum = snum_create(NEGATIVE, num);
    assert(snum.signal == ZERO);
    assert(snum.num.count == num.count);
    assert(snum.num.head == num.head);
    assert(snum.num.tail == num.tail);
    snum_free(snum);

    chunk_pool_clean();
    assert(clu_mem_empty());
}

void test_snum_create_immed(bool show)
{
    printf("\n\t%s\t\t", __func__);

    if(show) printf("\n\t\t%s 1\t\t", __func__);
    snum_t snum = snum_create_immed(POSITIVE, 1, 1);
    assert(snum.signal == POSITIVE);
    assert(num_immed(snum.num, 1, 1));

    if(show) printf("\n\t\t%s 2\t\t", __func__);
    snum = snum_create_immed(NEGATIVE, 1, 1);
    assert(snum.signal == NEGATIVE);
    assert(num_immed(snum.num, 1, 1));

    if(show) printf("\n\t\t%s 3\t\t", __func__);
    snum = snum_create_immed(POSITIVE, 0);
    assert(snum.signal == ZERO);
    assert(num_immed(snum.num, 0));

    if(show) printf("\n\t\t%s 4\t\t", __func__);
    snum = snum_create_immed(ZERO, 0);
    assert(snum.signal == ZERO);
    assert(num_immed(snum.num, 0));

    if(show) printf("\n\t\t%s 5\t\t", __func__);
    snum = snum_create_immed(NEGATIVE, 0);
    assert(snum.signal == ZERO);
    assert(num_immed(snum.num, 0));

    chunk_pool_clean();
    assert(clu_mem_empty());
}



void test_snum_wrap(bool show)
{
    printf("\n\t%s\t\t", __func__);

    if(show) printf("\n\t\t%s 1\t\t", __func__);
    snum_t snum = snum_wrap(0);
    assert(snum_immed(snum, ZERO, 0));

    if(show) printf("\n\t\t%s 2\t\t", __func__);
    snum = snum_wrap(1);
    assert(snum_immed(snum, POSITIVE, 1, 1));

    if(show) printf("\n\t\t%s 3\t\t", __func__);
    snum = snum_wrap(-1);
    assert(snum_immed(snum, NEGATIVE, 1, 1));

    if(show) printf("\n\t\t%s 4\t\t", __func__);
    snum = snum_wrap(INT64_MAX);
    assert(snum_immed(snum, POSITIVE, 1, INT64_MAX));

    if(show) printf("\n\t\t%s 5\t\t", __func__);
    snum = snum_wrap(INT64_MIN);
    assert(snum_immed(snum, NEGATIVE, 1, INT64_MIN));

    chunk_pool_clean();
    assert(clu_mem_empty());
}

void test_snum_wrap_str(bool show)
{
    printf("\n\t%s\t\t", __func__);

    #define TEST_SIG_WRAP_STR(TAG, STR, ...)                    \
    {                                                           \
        if(show) printf("\n\t\t%s %2d \t\t", __func__, TAG);    \
        snum_t snum = snum_wrap_str(STR);                       \
        assert(snum_immed(snum, __VA_ARGS__));                  \
    }

    TEST_SIG_WRAP_STR( 1, "", ZERO, 0);
    TEST_SIG_WRAP_STR( 2, "0", ZERO, 0);
    TEST_SIG_WRAP_STR( 3, "1", POSITIVE, 1, 1);
    TEST_SIG_WRAP_STR( 4, "10", POSITIVE, 1, 10);
    TEST_SIG_WRAP_STR( 5, "+", ZERO, 0);
    TEST_SIG_WRAP_STR( 6, "+0", ZERO, 0);
    TEST_SIG_WRAP_STR( 7, "+1", POSITIVE, 1, 1);
    TEST_SIG_WRAP_STR( 8, "+10", POSITIVE, 1, 10);
    TEST_SIG_WRAP_STR( 9, "-", ZERO, 0);
    TEST_SIG_WRAP_STR(10, "-0", ZERO, 0);
    TEST_SIG_WRAP_STR(11, "-1", NEGATIVE, 1, 1);
    TEST_SIG_WRAP_STR(12, "-10", NEGATIVE, 1, 10);
    TEST_SIG_WRAP_STR(13, "0x", ZERO, 0);
    TEST_SIG_WRAP_STR(14, "0x0", ZERO, 0);
    TEST_SIG_WRAP_STR(15, "0x1", POSITIVE, 1, 1);
    TEST_SIG_WRAP_STR(16, "0x10", POSITIVE, 1, 16);
    TEST_SIG_WRAP_STR(17, "+0x", ZERO, 0);
    TEST_SIG_WRAP_STR(18, "+0x0", ZERO, 0);
    TEST_SIG_WRAP_STR(19, "+0x1", POSITIVE, 1, 1);
    TEST_SIG_WRAP_STR(20, "+0x10", POSITIVE, 1, 16);
    TEST_SIG_WRAP_STR(21, "-0x", ZERO, 0);
    TEST_SIG_WRAP_STR(22, "-0x0", ZERO, 0);
    TEST_SIG_WRAP_STR(23, "-0x1", NEGATIVE, 1, 1);
    TEST_SIG_WRAP_STR(24, "-0x10", NEGATIVE, 1, 16);

    #undef TEST_SIG_WRAP_STR

    chunk_pool_clean();
    assert(clu_mem_empty());
}

void test_snum_copy(bool show)
{
    printf("\n\t%s\t\t", __func__);

    #define TEST_SIG_COPY(TAG, ...)                                 \
        {                                                           \
            if(show) printf("\n\t\t%s  " #TAG "\t\t", __func__);    \
            snum_t snum = snum_create_immed(__VA_ARGS__);              \
            snum_t snum_res = snum_copy(snum);                          \
            assert(snum_immed(snum_res, __VA_ARGS__))                 \
            snum_free(snum);                                          \
        }

    TEST_SIG_COPY(1, POSITIVE, 0);

    TEST_SIG_COPY(2, POSITIVE, 1, 1);
    TEST_SIG_COPY(3, POSITIVE, 2, 1, 2);

    TEST_SIG_COPY(4, NEGATIVE, 1, 1);
    TEST_SIG_COPY(5, NEGATIVE, 2, 1, 2);

    #undef TEST_SIG_COPY

    chunk_pool_clean();
    assert(clu_mem_empty());
}



void test_snum_is_zero(bool show)
{
    printf("\n\t%s\t\t", __func__);

    #define TEST_SIG_IS_ZERO(TAG, NUM, RES)                     \
        {                                                       \
            if(show) printf("\n\t\t%s %d\t\t", __func__, TAG); \
            snum_t snum = snum_wrap(NUM);                          \
            int64_t res = snum_is_zero(snum);                     \
            assert(uint64(res, RES));                           \
            snum_free(snum);                                      \
        }

    TEST_SIG_IS_ZERO(1,  1, false);
    TEST_SIG_IS_ZERO(2,  0, true);
    TEST_SIG_IS_ZERO(3, -1, false);

    #undef TEST_SIG_IS_ZERO

    chunk_pool_clean();
    assert(clu_mem_empty());
}

void test_snum_cmp(bool show)
{
    printf("\n\t%s\t\t", __func__);

    #define TEST_SIG_CMP(TAG, NUM_1, NUM_2, RES)                    \
        {                                                           \
            if(show) printf("\n\t\t%s %2d\t\t", __func__, TAG);     \
            snum_t snum_1 = snum_wrap(NUM_1);                          \
            snum_t snum_2 = snum_wrap(NUM_2);                          \
            int64_t res = snum_cmp(snum_1, snum_2);                    \
            assert(int64(res, RES));                                \
            snum_free(snum_1);                                        \
            snum_free(snum_2);                                        \
        }

    TEST_SIG_CMP( 1,  2,  3, -1);
    TEST_SIG_CMP( 2,  2,  2,  0);
    TEST_SIG_CMP( 3,  2,  1,  1);
    TEST_SIG_CMP( 4,  2,  0,  1);
    TEST_SIG_CMP( 5,  2, -1,  1);
    TEST_SIG_CMP( 6,  2, -2,  1);
    TEST_SIG_CMP( 7,  2, -3,  1);

    TEST_SIG_CMP( 8,  0,  1, -1);
    TEST_SIG_CMP( 9,  0,  0,  0);
    TEST_SIG_CMP(10,  0, -1,  1);

    TEST_SIG_CMP(11, -2,  2, -1);
    TEST_SIG_CMP(12, -2,  3, -1);
    TEST_SIG_CMP(13, -2,  1, -1);
    TEST_SIG_CMP(14, -2,  0, -1);
    TEST_SIG_CMP(15, -2, -1, -1);
    TEST_SIG_CMP(16, -2, -2,  0);
    TEST_SIG_CMP(17, -2, -3,  1);

    #undef TEST_SIG_CMP

    chunk_pool_clean();
    assert(clu_mem_empty());
}



void test_snum_shl(bool show)
{
    printf("\n\t%s\t\t", __func__);

    if(show) printf("\n\t\t%s 1\t\t", __func__);
    snum_t snum = snum_create_immed(ZERO, 0);
    snum = snum_shl(snum, 1);
    assert(snum_immed(snum, ZERO, 0));

    if(show) printf("\n\t\t%s 2\t\t", __func__);
    snum = snum_create_immed(POSITIVE, 1, 1);
    snum = snum_shl(snum, 1);
    assert(snum_immed(snum, POSITIVE, 1, 2));

    if(show) printf("\n\t\t%s 3\t\t", __func__);
    snum = snum_create_immed(POSITIVE, 1, 1);
    snum = snum_shl(snum, 64);
    assert(snum_immed(snum, POSITIVE, 2, 1, 0));

    if(show) printf("\n\t\t%s 4\t\t", __func__);
    snum = snum_create_immed(POSITIVE, 1, 1);
    snum = snum_shl(snum, 65);
    assert(snum_immed(snum, POSITIVE, 2, 2, 0));

    if(show) printf("\n\t\t%s 5\t\t", __func__);
    snum = snum_create_immed(NEGATIVE, 1, 1);
    snum = snum_shl(snum, 1);
    assert(snum_immed(snum, NEGATIVE, 1, 2));

    if(show) printf("\n\t\t%s 6\t\t", __func__);
    snum = snum_create_immed(NEGATIVE, 1, 1);
    snum = snum_shl(snum, 64);
    assert(snum_immed(snum, NEGATIVE, 2, 1, 0));

    if(show) printf("\n\t\t%s 7\t\t", __func__);
    snum = snum_create_immed(NEGATIVE, 1, 1);
    snum = snum_shl(snum, 65);
    assert(snum_immed(snum, NEGATIVE, 2, 2, 0));

    chunk_pool_clean();
    assert(clu_mem_empty());
}

void test_snum_shr(bool show)
{
    printf("\n\t%s\t\t", __func__);

    if(show) printf("\n\t\t%s 1\t\t", __func__);
    snum_t snum = snum_create_immed(ZERO, 0);
    snum = snum_shr(snum, 1);
    assert(snum_immed(snum, ZERO, 0));

    if(show) printf("\n\t\t%s 2\t\t", __func__);
    snum = snum_create_immed(POSITIVE, 1, 1);
    snum = snum_shr(snum, 1);
    assert(snum_immed(snum, ZERO, 0));

    if(show) printf("\n\t\t%s 3\t\t", __func__);
    snum = snum_create_immed(POSITIVE, 2, 2, 0);
    snum = snum_shr(snum, 1);
    assert(snum_immed(snum, POSITIVE, 2, 1, 0));

    if(show) printf("\n\t\t%s 4\t\t", __func__);
    snum = snum_create_immed(POSITIVE, 2, 2, 0);
    snum = snum_shr(snum, 64);
    assert(snum_immed(snum, POSITIVE, 1, 2));

    if(show) printf("\n\t\t%s 5\t\t", __func__);
    snum = snum_create_immed(POSITIVE, 2, 2, 0);
    snum = snum_shr(snum, 65);
    assert(snum_immed(snum, POSITIVE, 1, 1));

    if(show) printf("\n\t\t%s 6\t\t", __func__);
    snum = snum_create_immed(NEGATIVE, 1, 1);
    snum = snum_shr(snum, 1);
    assert(snum_immed(snum, ZERO, 0));

    if(show) printf("\n\t\t%s 7\t\t", __func__);
    snum = snum_create_immed(NEGATIVE, 2, 2, 0);
    snum = snum_shr(snum, 1);
    assert(snum_immed(snum, NEGATIVE, 2, 1, 0));

    if(show) printf("\n\t\t%s 8\t\t", __func__);
    snum = snum_create_immed(NEGATIVE, 2, 2, 0);
    snum = snum_shr(snum, 64);
    assert(snum_immed(snum, NEGATIVE, 1, 2));

    if(show) printf("\n\t\t%s 9\t\t", __func__);
    snum = snum_create_immed(NEGATIVE, 2, 2, 0);
    snum = snum_shr(snum, 65);
    assert(snum_immed(snum, NEGATIVE, 1, 1));

    chunk_pool_clean();
    assert(clu_mem_empty());
}



void test_snum_opposite(bool show)
{
    printf("\n\t%s\t\t", __func__);

    #define TEST_SIG_OPPOSITE(TAG, SIGNAL_IN, SIGNAL_OUT, ...)      \
        {                                                           \
            if(show) printf("\n\t\t%s %d\t\t", __func__, TAG);      \
            snum_t snum = snum_create_immed(SIGNAL_IN, __VA_ARGS__);   \
            snum = snum_opposite(snum);                                \
            assert(snum_immed(snum, SIGNAL_OUT, __VA_ARGS__));        \
        }

    TEST_SIG_OPPOSITE(1, ZERO, ZERO, 0);
    TEST_SIG_OPPOSITE(2, POSITIVE, NEGATIVE, 1, 1);
    TEST_SIG_OPPOSITE(3, NEGATIVE, POSITIVE, 1, 1);

    chunk_pool_clean();
    assert(clu_mem_empty());
}

void test_snum_add(bool show)
{
    printf("\n\t%s\t\t", __func__);

    if(show) printf("\n\t\t%s  1\t\t", __func__);
    snum_t snum_1 = snum_create_immed(POSITIVE, 1, 2);
    snum_t snum_2 = snum_create_immed(POSITIVE, 1, 1);
    snum_t snum_res = snum_add(snum_1, snum_2);
    assert(snum_immed(snum_res, POSITIVE, 1, 3));

    if(show) printf("\n\t\t%s  2\t\t", __func__);
    snum_1 = snum_create_immed(POSITIVE, 1, 2);
    snum_2 = snum_create_immed(ZERO, 0);
    snum_res = snum_add(snum_1, snum_2);
    assert(snum_immed(snum_res, POSITIVE, 1, 2));

    if(show) printf("\n\t\t%s  3\t\t", __func__);
    snum_1 = snum_create_immed(POSITIVE, 1, 2);
    snum_2 = snum_create_immed(NEGATIVE, 1, 1);
    snum_res = snum_add(snum_1, snum_2);
    assert(snum_immed(snum_res, POSITIVE, 1, 1));

    if(show) printf("\n\t\t%s  4\t\t", __func__);
    snum_1 = snum_create_immed(POSITIVE, 1, 2);
    snum_2 = snum_create_immed(NEGATIVE, 1, 2);
    snum_res = snum_add(snum_1, snum_2);
    assert(snum_immed(snum_res, ZERO, 0));

    if(show) printf("\n\t\t%s  5\t\t", __func__);
    snum_1 = snum_create_immed(POSITIVE, 1, 2);
    snum_2 = snum_create_immed(NEGATIVE, 1, 3);
    snum_res = snum_add(snum_1, snum_2);
    assert(snum_immed(snum_res, NEGATIVE, 1, 1));

    if(show) printf("\n\t\t%s  6\t\t", __func__);
    snum_1 = snum_create_immed(POSITIVE, 1, 1);
    snum_2 = snum_create_immed(POSITIVE, 1, 2);
    snum_res = snum_add(snum_1, snum_2);
    assert(snum_immed(snum_res, POSITIVE, 1, 3));

    if(show) printf("\n\t\t%s  7\t\t", __func__);
    snum_1 = snum_create_immed(ZERO, 0);
    snum_2 = snum_create_immed(POSITIVE, 1, 2);
    snum_res = snum_add(snum_1, snum_2);
    assert(snum_immed(snum_res, POSITIVE, 1, 2));

    if(show) printf("\n\t\t%s  8\t\t", __func__);
    snum_1 = snum_create_immed(NEGATIVE, 1, 1);
    snum_2 = snum_create_immed(POSITIVE, 1, 2);
    snum_res = snum_add(snum_1, snum_2);
    assert(snum_immed(snum_res, POSITIVE, 1, 1));

    if(show) printf("\n\t\t%s  9\t\t", __func__);
    snum_1 = snum_create_immed(NEGATIVE, 1, 2);
    snum_2 = snum_create_immed(POSITIVE, 1, 2);
    snum_res = snum_add(snum_1, snum_2);
    assert(snum_immed(snum_res, ZERO, 0));

    if(show) printf("\n\t\t%s 10\t\t", __func__);
    snum_1 = snum_create_immed(NEGATIVE, 1, 3);
    snum_2 = snum_create_immed(POSITIVE, 1, 2);
    snum_res = snum_add(snum_1, snum_2);
    assert(snum_immed(snum_res, NEGATIVE, 1, 1));

    chunk_pool_clean();
    assert(clu_mem_empty());
}

void test_snum_sub(bool show)
{
    printf("\n\t%s\t\t", __func__);

    if(show) printf("\n\t\t%s 1\t\t", __func__);
    snum_t snum_1 = snum_create_immed(POSITIVE, 1, 2);
    snum_t snum_2 = snum_create_immed(POSITIVE, 1, 1);
    snum_t snum_res = snum_sub(snum_1, snum_2);
    assert(snum_immed(snum_res, POSITIVE, 1, 1));

    if(show) printf("\n\t\t%s 2\t\t", __func__);
    snum_1 = snum_create_immed(POSITIVE, 1, 2);
    snum_2 = snum_create_immed(POSITIVE, 1, 2);
    snum_res = snum_sub(snum_1, snum_2);
    assert(snum_immed(snum_res, ZERO, 0));

    if(show) printf("\n\t\t%s 3\t\t", __func__);
    snum_1 = snum_create_immed(POSITIVE, 1, 2);
    snum_2 = snum_create_immed(POSITIVE, 1, 3);
    snum_res = snum_sub(snum_1, snum_2);
    assert(snum_immed(snum_res, NEGATIVE, 1, 1));

    if(show) printf("\n\t\t%s 4\t\t", __func__);
    snum_1 = snum_create_immed(POSITIVE, 1, 2);
    snum_2 = snum_create_immed(NEGATIVE, 1, 1);
    snum_res = snum_sub(snum_1, snum_2);
    assert(snum_immed(snum_res, POSITIVE, 1, 3));

    chunk_pool_clean();
    assert(clu_mem_empty());
}

void test_snum_mul(bool show)
{
    printf("\n\t%s\t\t", __func__);

    if(show) printf("\n\t\t%s 1\t\t", __func__);
    snum_t snum_1 = snum_create_immed(POSITIVE, 1, 2);
    snum_t snum_2 = snum_create_immed(POSITIVE, 1, 3);
    snum_t snum_res = snum_mul(snum_1, snum_2);
    assert(snum_immed(snum_res, POSITIVE, 1, 6));

    if(show) printf("\n\t\t%s 2\t\t", __func__);
    snum_1 = snum_create_immed(POSITIVE, 1, 2);
    snum_2 = snum_create_immed(ZERO, 0);
    snum_res = snum_mul(snum_1, snum_2);
    assert(snum_immed(snum_res, ZERO, 0));

    if(show) printf("\n\t\t%s 3\t\t", __func__);
    snum_1 = snum_create_immed(POSITIVE, 1, 2);
    snum_2 = snum_create_immed(NEGATIVE, 1, 3);
    snum_res = snum_mul(snum_1, snum_2);
    assert(snum_immed(snum_res, NEGATIVE, 1, 6));

    if(show) printf("\n\t\t%s 4\t\t", __func__);
    snum_1 = snum_create_immed(POSITIVE, 1, 3);
    snum_2 = snum_create_immed(POSITIVE, 1, 2);
    snum_res = snum_mul(snum_1, snum_2);
    assert(snum_immed(snum_res, POSITIVE, 1, 6));

    if(show) printf("\n\t\t%s 5\t\t", __func__);
    snum_1 = snum_create_immed(ZERO, 0);
    snum_2 = snum_create_immed(POSITIVE, 1, 2);
    snum_res = snum_mul(snum_1, snum_2);
    assert(snum_immed(snum_res, ZERO, 0));

    if(show) printf("\n\t\t%s 6\t\t", __func__);
    snum_1 = snum_create_immed(NEGATIVE, 1, 3);
    snum_2 = snum_create_immed(POSITIVE, 1, 2);
    snum_res = snum_mul(snum_1, snum_2);
    assert(snum_immed(snum_res, NEGATIVE, 1, 6));

    chunk_pool_clean();
    assert(clu_mem_empty());
}

void test_snum_div(bool show)
{
    printf("\n\t%s\t\t", __func__);

    if(show) printf("\n\t\t%s 1\t\t", __func__);
    snum_t snum_1 = snum_create_immed(POSITIVE, 1, 2);
    snum_t snum_2 = snum_create_immed(POSITIVE, 1, 3);
    snum_t snum_res = snum_div(snum_1, snum_2);
    assert(snum_immed(snum_res, ZERO, 0));

    if(show) printf("\n\t\t%s 2\t\t", __func__);
    snum_1 = snum_create_immed(POSITIVE, 1, 2);
    snum_2 = snum_create_immed(NEGATIVE, 1, 3);
    snum_res = snum_div(snum_1, snum_2);
    assert(snum_immed(snum_res, ZERO, 0));

    if(show) printf("\n\t\t%s 3\t\t", __func__);
    snum_1 = snum_create_immed(POSITIVE, 1, 3);
    snum_2 = snum_create_immed(POSITIVE, 1, 2);
    snum_res = snum_div(snum_1, snum_2);
    assert(snum_immed(snum_res, POSITIVE, 1, 1));

    if(show) printf("\n\t\t%s 4\t\t", __func__);
    snum_1 = snum_create_immed(ZERO, 0);
    snum_2 = snum_create_immed(POSITIVE, 1, 2);
    snum_res = snum_div(snum_1, snum_2);
    assert(snum_immed(snum_res, ZERO, 0));

    if(show) printf("\n\t\t%s 5\t\t", __func__);
    snum_1 = snum_create_immed(NEGATIVE, 1, 3);
    snum_2 = snum_create_immed(POSITIVE, 1, 2);
    snum_res = snum_div(snum_1, snum_2);
    assert(snum_immed(snum_res, NEGATIVE, 1, 1));

    chunk_pool_clean();
    assert(clu_mem_empty());
}



void test_snum()
{
    printf("\n%s\t\t", __func__);

    bool show = false;

    test_snum_create(show);
    test_snum_create_immed(show);

    test_snum_wrap(show);
    test_snum_wrap_str(show);
    test_snum_copy(show);

    test_snum_is_zero(show);
    test_snum_cmp(show);

    test_snum_shl(show);
    test_snum_shr(show);

    test_snum_opposite(show);
    test_snum_add(show);
    test_snum_sub(show);
    test_snum_mul(show);
    test_snum_div(show);

    chunk_pool_clean();
    assert(clu_mem_empty());
}



int main()
{
    setbuf(stdout, NULL);
    test_snum();
    printf("\n\n\tTest successful\n\n");
    return 0;
}
