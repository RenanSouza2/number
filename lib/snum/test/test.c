#include "../debug.h"
#include "../../num/debug.h"

#include "../../macros/test.h"
#include "../../macros/assert.h"
#include "../../../mods/clu/header.h"



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
    assert(clu_mem_is_empty());
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
    assert(clu_mem_is_empty());
}



void test_snum_wrap(bool show)
{
    printf("\n\t%s\t\t", __func__);

    #define TEST_SNUM_WRAP(TAG, VALUE, ...)                 \
    {                                                       \
        if(show) printf("\n\t\t%s %d\t\t", __func__, TAG);  \
        snum_t snum = snum_wrap(VALUE);                     \
        assert(snum_immed(snum, __VA_ARGS__));              \
    }

    TEST_SNUM_WRAP(1,  0, ZERO, 0);
    TEST_SNUM_WRAP(2,  1, POSITIVE, 1, 1);
    TEST_SNUM_WRAP(3, -1, NEGATIVE, 1, 1);
    TEST_SNUM_WRAP(4, INT64_MAX, POSITIVE, 1, INT64_MAX);
    TEST_SNUM_WRAP(5, INT64_MIN, NEGATIVE, 1, INT64_MIN);

    #undef TEST_SNUM_WRAP

    chunk_pool_clean();
    assert(clu_mem_is_empty());
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
    assert(clu_mem_is_empty());
}

void test_snum_copy(bool show)
{
    printf("\n\t%s\t\t", __func__);

    #define TEST_SIG_COPY(TAG, ...)                             \
    {                                                           \
        if(show) printf("\n\t\t%s  " #TAG "\t\t", __func__);    \
        snum_t snum = snum_create_immed(__VA_ARGS__);           \
        snum_t snum_res = snum_copy(snum);                      \
        assert(snum_immed(snum_res, __VA_ARGS__))               \
        snum_free(snum);                                        \
    }

    TEST_SIG_COPY(1, POSITIVE, 0);

    TEST_SIG_COPY(2, POSITIVE, 1, 1);
    TEST_SIG_COPY(3, POSITIVE, 2, 1, 2);

    TEST_SIG_COPY(4, NEGATIVE, 1, 1);
    TEST_SIG_COPY(5, NEGATIVE, 2, 1, 2);

    #undef TEST_SIG_COPY

    chunk_pool_clean();
    assert(clu_mem_is_empty());
}



void test_snum_is_zero(bool show)
{
    printf("\n\t%s\t\t", __func__);

    #define TEST_SIG_IS_ZERO(TAG, NUM, RES)                 \
    {                                                       \
        if(show) printf("\n\t\t%s %d\t\t", __func__, TAG);  \
        snum_t snum = snum_wrap(NUM);                       \
        int64_t res = snum_is_zero(snum);                   \
        assert(uint64(res, RES));                           \
        snum_free(snum);                                    \
    }

    TEST_SIG_IS_ZERO(1,  1, false);
    TEST_SIG_IS_ZERO(2,  0, true);
    TEST_SIG_IS_ZERO(3, -1, false);

    #undef TEST_SIG_IS_ZERO

    chunk_pool_clean();
    assert(clu_mem_is_empty());
}

void test_snum_cmp(bool show)
{
    printf("\n\t%s\t\t", __func__);

    #define TEST_SIG_CMP(TAG, NUM_1, NUM_2, RES)            \
    {                                                       \
        if(show) printf("\n\t\t%s %2d\t\t", __func__, TAG); \
        snum_t snum_1 = snum_wrap(NUM_1);                   \
        snum_t snum_2 = snum_wrap(NUM_2);                   \
        int64_t res = snum_cmp(snum_1, snum_2);             \
        assert(int64(res, RES));                            \
        snum_free(snum_1);                                  \
        snum_free(snum_2);                                  \
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
    assert(clu_mem_is_empty());
}



void test_snum_shl(bool show)
{
    printf("\n\t%s\t\t", __func__);

    #define TEST_SNUM_SHL(TAG, BITS, ...)                   \
    {                                                       \
        snum_t snum[2];                                     \
        if(show) printf("\n\t\t%s %2d\t\t", __func__, TAG); \
        snum_create_vec_immed(snum, 2, __VA_ARGS__);        \
        snum[0] = snum_shl(snum[0], BITS);                  \
        assert(snum_str(snum[0], snum[1]));                 \
    }

    TEST_SNUM_SHL(1, 0,
        ZERO, 0,
        ZERO, 0
    );
    TEST_SNUM_SHL(2, 1,
        ZERO, 0,
        ZERO, 0
    );
    TEST_SNUM_SHL(3, 64,
        ZERO, 0,
        ZERO, 0
    );
    TEST_SNUM_SHL(4, 65,
        ZERO, 0,
        ZERO, 0
    );
    TEST_SNUM_SHL(5, 0,
        POSITIVE, 1, 1,
        POSITIVE, 1, 1
    );
    TEST_SNUM_SHL(6, 1,
        POSITIVE, 1, 1,
        POSITIVE, 1, 2
    );
    TEST_SNUM_SHL(7, 63,
        POSITIVE, 1, 1,
        POSITIVE, 1, 0x8000000000000000
    );
    TEST_SNUM_SHL(8, 64,
        POSITIVE, 1, 1,
        POSITIVE, 2, 1, 0
    );
    TEST_SNUM_SHL(9, 65,
        POSITIVE, 1, 1,
        POSITIVE, 2, 2, 0
    );

    #undef TEST_SNUM_SHL

    chunk_pool_clean();
    assert(clu_mem_is_empty());
}

void test_snum_shr(bool show)
{
    printf("\n\t%s\t\t", __func__);

    #define TEST_SNUM_SHR(TAG, BITS, ...)                   \
    {                                                       \
        snum_t snum[2];                                     \
        if(show) printf("\n\t\t%s %2d\t\t", __func__, TAG); \
        snum_create_vec_immed(snum, 2, __VA_ARGS__);        \
        snum[0] = snum_shr(snum[0], BITS);                  \
        assert(snum_str(snum[0], snum[1]));                 \
    }

    TEST_SNUM_SHR(1, 0,
        ZERO, 0,
        ZERO, 0
    );
    TEST_SNUM_SHR(2, 1,
        ZERO, 0,
        ZERO, 0
    );
    TEST_SNUM_SHR(3, 63,
        ZERO, 0,
        ZERO, 0
    );
    TEST_SNUM_SHR(4, 64,
        ZERO, 0,
        ZERO, 0
    );
    TEST_SNUM_SHR(5, 65,
        ZERO, 0,
        ZERO, 0
    );
    TEST_SNUM_SHR(6, 0,
        POSITIVE, 1, 1,
        POSITIVE, 1, 1
    );
    TEST_SNUM_SHR(7, 1,
        POSITIVE, 1, 1,
        ZERO, 0
    );
    TEST_SNUM_SHR(8, 0,
        POSITIVE, 2, 2, 0,
        POSITIVE, 2, 2, 0
    );
    TEST_SNUM_SHR(9, 1,
        POSITIVE, 2, 2, 0,
        POSITIVE, 2, 1, 0
    );
    TEST_SNUM_SHR(10, 2,
        POSITIVE, 2, 2, 0,
        POSITIVE, 1, 0x8000000000000000
    );
    TEST_SNUM_SHR(11, 64,
        POSITIVE, 2, 2, 0,
        POSITIVE, 1, 2
    );
    TEST_SNUM_SHR(11, 65,
        POSITIVE, 2, 2, 0,
        POSITIVE, 1, 1
    );

    #undef TEST_SNUM_SHR

    chunk_pool_clean();
    assert(clu_mem_is_empty());
}



void test_snum_opposite(bool show)
{
    printf("\n\t%s\t\t", __func__);

    #define TEST_SIG_OPPOSITE(TAG, SIGNAL_IN, SIGNAL_OUT, ...)      \
    {                                                               \
        if(show) printf("\n\t\t%s %d\t\t", __func__, TAG);          \
        snum_t snum = snum_create_immed(SIGNAL_IN, __VA_ARGS__);    \
        snum = snum_opposite(snum);                                 \
        assert(snum_immed(snum, SIGNAL_OUT, __VA_ARGS__));          \
    }

    TEST_SIG_OPPOSITE(1, ZERO, ZERO, 0);
    TEST_SIG_OPPOSITE(2, POSITIVE, NEGATIVE, 1, 1);
    TEST_SIG_OPPOSITE(3, NEGATIVE, POSITIVE, 1, 1);

    chunk_pool_clean();
    assert(clu_mem_is_empty());
}

void test_snum_add(bool show)
{
    printf("\n\t%s\t\t", __func__);

    #define TEST_SNUM_ADD(TAG, ...) \
    {   \
        snum_t snum[3]; \
        if(show) printf("\n\t\t%s %2d\t\t", __func__, TAG); \
        snum_create_vec_immed(snum, 3, __VA_ARGS__);    \
        snum[0] = snum_add(snum[0], snum[1]);   \
        assert(snum_str(snum[0], snum[2])); \
    }

    TEST_SNUM_ADD(1,
        ZERO, 0,
        ZERO, 0,
        ZERO, 0
    );
    TEST_SNUM_ADD(2,
        ZERO, 0,
        POSITIVE, 1, 1,
        POSITIVE, 1, 1
    );
    TEST_SNUM_ADD(3,
        ZERO, 0,
        NEGATIVE, 1, 1,
        NEGATIVE, 1, 1
    );
    TEST_SNUM_ADD(4,
        POSITIVE, 1, 2,
        POSITIVE, 1, 1,
        POSITIVE, 1, 3
    );
    TEST_SNUM_ADD(5,
        POSITIVE, 1, 2,
        ZERO, 0,
        POSITIVE, 1, 2
    );
    TEST_SNUM_ADD(6,
        POSITIVE, 1, 2,
        NEGATIVE, 1, 1,
        POSITIVE, 1, 1
    );
    TEST_SNUM_ADD(7,
        POSITIVE, 1, 2,
        NEGATIVE, 1, 2,
        ZERO, 0
    );
    TEST_SNUM_ADD(8,
        POSITIVE, 1, 2,
        NEGATIVE, 1, 3,
        NEGATIVE, 1, 1
    );
    TEST_SNUM_ADD(9,
        NEGATIVE, 1, 2,
        NEGATIVE, 1, 1,
        NEGATIVE, 1, 3
    );
    TEST_SNUM_ADD(10,
        NEGATIVE, 1, 2,
        ZERO, 0,
        NEGATIVE, 1, 2
    );
    TEST_SNUM_ADD(11,
        NEGATIVE, 1, 2,
        POSITIVE, 1, 1,
        NEGATIVE, 1, 1
    );
    TEST_SNUM_ADD(12,
        NEGATIVE, 1, 2,
        POSITIVE, 1, 2,
        ZERO, 0
    );
    TEST_SNUM_ADD(13,
        NEGATIVE, 1, 2,
        POSITIVE, 1, 3,
        POSITIVE, 1, 1
    );

    #undef TEST_SNUM_ADD

    chunk_pool_clean();
    assert(clu_mem_is_empty());
}

void test_snum_sub(bool show)
{
    printf("\n\t%s\t\t", __func__);

    #define TESTS_SNUM_SUB(TAG, ...)                        \
    {                                                       \
        snum_t snum[3];                                     \
        if(show) printf("\n\t\t%s %d\t\t", __func__, TAG);  \
        snum_create_vec_immed(snum, 3, __VA_ARGS__);        \
        snum[0] = snum_sub(snum[0], snum[1]);               \
        assert(snum_str(snum[0], snum[2]));                 \
    }

    TESTS_SNUM_SUB(1,
        POSITIVE, 1, 3,
        POSITIVE, 1, 1,
        POSITIVE, 1, 2
    );
    TESTS_SNUM_SUB(2,
        POSITIVE, 1, 3,
        ZERO, 0,
        POSITIVE, 1, 3
    );
    TESTS_SNUM_SUB(1,
        POSITIVE, 1, 3,
        NEGATIVE, 1, 1,
        POSITIVE, 1, 4
    );

    #undef TESTS_SNUM_SUB

    chunk_pool_clean();
    assert(clu_mem_is_empty());
}

void test_snum_mul(bool show)
{
    printf("\n\t%s\t\t", __func__);

    #define TEST_SNUM_MUL(TAG, ...)                         \
    {                                                       \
        snum_t snum[3];                                     \
        if(show) printf("\n\t\t%s %d\t\t", __func__, TAG);  \
        snum_create_vec_immed(snum, 3, __VA_ARGS__);        \
        snum[0] = snum_mul(snum[0], snum[1]);               \
        assert(snum_str(snum[0], snum[2]));                 \
    }

    TEST_SNUM_MUL(1,
        POSITIVE, 1, 2,
        POSITIVE, 1, 3,
        POSITIVE, 1, 6
    );
    TEST_SNUM_MUL(2,
        POSITIVE, 1, 2,
        ZERO, 0,
        ZERO, 0
    );
    TEST_SNUM_MUL(3,
        NEGATIVE, 1, 2,
        ZERO, 0,
        ZERO, 0
    );
    TEST_SNUM_MUL(4,
        ZERO, 0,
        POSITIVE, 1, 2,
        ZERO, 0
    );
    TEST_SNUM_MUL(5,
        ZERO, 0,
        NEGATIVE, 1, 2,
        ZERO, 0
    );
    TEST_SNUM_MUL(6,
        POSITIVE, 1, 2,
        NEGATIVE, 1, 3,
        NEGATIVE, 1, 6
    );
    TEST_SNUM_MUL(7,
        NEGATIVE, 1, 2,
        POSITIVE, 1, 3,
        NEGATIVE, 1, 6
    );
    TEST_SNUM_MUL(8,
        NEGATIVE, 1, 2,
        NEGATIVE, 1, 3,
        POSITIVE, 1, 6
    );

    #undef TEST_SNUM_MUL

    chunk_pool_clean();
    assert(clu_mem_is_empty());
}

void test_snum_div(bool show)
{
    printf("\n\t%s\t\t", __func__);

    #define TEST_SNUM_DIV(TAG, ...)                         \
    {                                                       \
        snum_t snum[3];                                     \
        if(show) printf("\n\t\t%s %d\t\t", __func__, TAG);  \
        snum_create_vec_immed(snum, 3, __VA_ARGS__);        \
        snum[0] = snum_div(snum[0], snum[1]);               \
        assert(snum_str(snum[0], snum[2]));                 \
    }

    TEST_SNUM_DIV(1,
        POSITIVE, 1, 2,
        POSITIVE, 1, 3,
        ZERO, 0
    );
    TEST_SNUM_DIV(2,
        POSITIVE, 1, 2,
        NEGATIVE, 1, 3,
        ZERO, 0
    );
    TEST_SNUM_DIV(3,
        POSITIVE, 1, 3,
        POSITIVE, 1, 2,
        POSITIVE, 1, 1
    );
    TEST_SNUM_DIV(4,
        ZERO, 0,
        POSITIVE, 1, 2,
        ZERO, 0
    );
    TEST_SNUM_DIV(5,
        NEGATIVE, 1, 3,
        POSITIVE, 1, 2,
        NEGATIVE, 1, 1
    );

    #undef TEST_SNUM_DIV

    chunk_pool_clean();
    assert(clu_mem_is_empty());
}



void test_snum()
{
    printf("\n%s\t\t", __func__);

    bool show = true;

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
    assert(clu_mem_is_empty());
}



int main()
{
    setbuf(stdout, NULL);
    TEST_TIMEOUT_OPEN(5)
    test_snum();
    TEST_TIMEOUT_CLOSE
    printf("\n\n\tTest successful\n\n");
    return 0;
}
