#include "../debug.h"
#include "../../../testrc.h"
#include "../../../mods/macros/test.h"

#include "../../num/debug.h"



void test_snum_create(bool show)
{
    TEST_FN_OPEN

    TEST_CASE_OPEN(1)
    {
        num_t num = num_create_immed(1, 1);
        snum_t snum = snum_create(POSITIVE, num);
        assert(snum.signal == POSITIVE);
        assert(snum.num.count == num.count);
        assert(snum.num.head == num.head);
        assert(snum.num.tail == num.tail);
        snum_free(snum);
    }
    TEST_CASE_CLOSE

    TEST_CASE_OPEN(2)
    {
        num_t num = num_create_immed(1, 1);
        snum_t snum = snum_create(NEGATIVE, num);
        assert(snum.signal == NEGATIVE);
        assert(snum.num.count == num.count);
        assert(snum.num.head == num.head);
        assert(snum.num.tail == num.tail);
        snum_free(snum);
    }
    TEST_CASE_CLOSE

    TEST_CASE_OPEN(3)
    {
        num_t num = num_create_immed(0);
        snum_t snum = snum_create(POSITIVE, num);
        assert(snum.signal == ZERO);
        assert(snum.num.count == num.count);
        assert(snum.num.head == num.head);
        assert(snum.num.tail == num.tail);
        snum_free(snum);
    }
    TEST_CASE_CLOSE
        
    TEST_CASE_OPEN(4)
    {
        num_t num = num_create_immed(0);
        snum_t snum = snum_create(ZERO, num);
        assert(snum.signal == ZERO);
        assert(snum.num.count == num.count);
        assert(snum.num.head == num.head);
        assert(snum.num.tail == num.tail);
        snum_free(snum);
    }
    TEST_CASE_CLOSE

    TEST_CASE_OPEN(5)
    {
        num_t num = num_create_immed(0);
        snum_t snum = snum_create(NEGATIVE, num);
        assert(snum.signal == ZERO);
        assert(snum.num.count == num.count);
        assert(snum.num.head == num.head);
        assert(snum.num.tail == num.tail);
        snum_free(snum);
    }
    TEST_CASE_CLOSE

    TEST_CASE_OPEN(6)
    {
        num_t num = num_create_immed(1, 5);
        TEST_REVERT_OPEN
        {
            snum_create(ZERO, num);
        }
        TEST_REVERT_CLOSE
    }
    TEST_CASE_CLOSE

    TEST_FN_CLOSE
}

void test_snum_create_immed(bool show)
{
    TEST_FN_OPEN

    TEST_CASE_OPEN(1)
    {
        snum_t snum = snum_create_immed(POSITIVE, 1, 1);
        assert(snum.signal == POSITIVE);
        assert(num_immed(snum.num, 1, 1));
    }
    TEST_CASE_CLOSE

    TEST_CASE_OPEN(2)
    {
        snum_t snum = snum_create_immed(NEGATIVE, 1, 1);
        assert(snum.signal == NEGATIVE);
        assert(num_immed(snum.num, 1, 1));
    }
    TEST_CASE_CLOSE

    TEST_CASE_OPEN(3)
    {
        snum_t snum = snum_create_immed(POSITIVE, 0);
        assert(snum.signal == ZERO);
        assert(num_immed(snum.num, 0));
    }
    TEST_CASE_CLOSE

    TEST_CASE_OPEN(4)
    {
        snum_t snum = snum_create_immed(ZERO, 0);
        assert(snum.signal == ZERO);
        assert(num_immed(snum.num, 0));
    }
    TEST_CASE_CLOSE

    TEST_CASE_OPEN(5)
    {
        snum_t snum = snum_create_immed(NEGATIVE, 0);
        assert(snum.signal == ZERO);
        assert(num_immed(snum.num, 0));
    }
    TEST_CASE_CLOSE

    TEST_FN_CLOSE
}



void test_snum_wrap(bool show)
{
    TEST_FN_OPEN

    #define TEST_SNUM_WRAP(TAG, VALUE, ...)         \
    {                                               \
        TEST_CASE_OPEN(TAG)                         \
        {                                           \
            snum_t snum = snum_wrap(VALUE);         \
            assert(snum_immed(snum, __VA_ARGS__));  \
        }                                           \
        TEST_CASE_CLOSE                             \
    }

    TEST_SNUM_WRAP(1,  0, ZERO, 0);
    TEST_SNUM_WRAP(2,  1, POSITIVE, 1, 1);
    TEST_SNUM_WRAP(3, -1, NEGATIVE, 1, 1);
    TEST_SNUM_WRAP(4, INT64_MAX, POSITIVE, 1, INT64_MAX);
    TEST_SNUM_WRAP(5, INT64_MIN, NEGATIVE, 1, INT64_MIN);

    #undef TEST_SNUM_WRAP

    TEST_FN_CLOSE
}

void test_snum_wrap_str(bool show)
{
    TEST_FN_OPEN

    #define TEST_SIG_WRAP_STR(TAG, STR, ...)        \
    {                                               \
        TEST_CASE_OPEN(TAG)                         \
        {                                           \
            snum_t snum = snum_wrap_str(STR);       \
            assert(snum_immed(snum, __VA_ARGS__));  \
        }                                           \
        TEST_CASE_CLOSE                             \
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

    TEST_FN_CLOSE
}

void test_snum_copy(bool show)
{
    TEST_FN_OPEN

    #define TEST_SIG_COPY(TAG, ...)                         \
    {                                                       \
        TEST_CASE_OPEN(TAG)                                 \
        {                                                   \
            snum_t snum = snum_create_immed(__VA_ARGS__);   \
            snum_t snum_res = snum_copy(snum);              \
            assert(snum_immed(snum_res, __VA_ARGS__))       \
            snum_free(snum);                                \
        }                                                   \
        TEST_CASE_CLOSE                                     \
    }

    TEST_SIG_COPY(1, POSITIVE, 0);

    TEST_SIG_COPY(2, POSITIVE, 1, 1);
    TEST_SIG_COPY(3, POSITIVE, 2, 1, 2);

    TEST_SIG_COPY(4, NEGATIVE, 1, 1);
    TEST_SIG_COPY(5, NEGATIVE, 2, 1, 2);

    #undef TEST_SIG_COPY

    TEST_FN_CLOSE
}



void test_snum_is_zero(bool show)
{
    TEST_FN_OPEN

    #define TEST_SIG_IS_ZERO(TAG, NUM, RES)                 \
    {                                                       \
        TEST_CASE_OPEN(TAG)                                 \
        {                                                   \
            snum_t snum = snum_wrap(NUM);                   \
            int64_t res = snum_is_zero(snum);               \
            assert(uint64(res, RES));                       \
            snum_free(snum);                                \
        }                                                   \
        TEST_CASE_CLOSE                                     \
    }

    TEST_SIG_IS_ZERO(1,  1, false);
    TEST_SIG_IS_ZERO(2,  0, true);
    TEST_SIG_IS_ZERO(3, -1, false);

    #undef TEST_SIG_IS_ZERO

    TEST_FN_CLOSE
}

void test_snum_cmp(bool show)
{
    TEST_FN_OPEN

    #define TEST_SIG_CMP(TAG, NUM_1, NUM_2, RES)    \
    {                                               \
        TEST_CASE_OPEN(TAG)                         \
        {                                           \
            snum_t snum_1 = snum_wrap(NUM_1);       \
            snum_t snum_2 = snum_wrap(NUM_2);       \
            int64_t res = snum_cmp(snum_1, snum_2); \
            assert(int64(res, RES));                \
            snum_free(snum_1);                      \
            snum_free(snum_2);                      \
        }                                           \
        TEST_CASE_CLOSE                             \
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

    TEST_FN_CLOSE
}



void test_snum_shl(bool show)
{
    TEST_FN_OPEN

    #define TEST_SNUM_SHL(TAG, SNUM_BEF, BITS, SNUM_AFT)        \
    {                                                           \
        TEST_CASE_OPEN(TAG)                                     \
        {                                                       \
            snum_t snum = snum_create_immed(ARG_OPEN SNUM_BEF); \
            snum = snum_shl(snum, BITS);                        \
            assert(snum_immed(snum, ARG_OPEN SNUM_AFT));        \
        }                                                       \
        TEST_CASE_CLOSE                                         \
    }

    TEST_SNUM_SHL(1, (ZERO, 0), 0, (ZERO, 0));
    TEST_SNUM_SHL(2, (ZERO, 0), 1, (ZERO, 0));
    TEST_SNUM_SHL(3, (ZERO, 0), 64, (ZERO, 0));
    TEST_SNUM_SHL(4, (ZERO, 0), 65, (ZERO, 0));
    TEST_SNUM_SHL(5, (POSITIVE, 1, 1), 0, (POSITIVE, 1, 1));
    TEST_SNUM_SHL(6, (POSITIVE, 1, 1), 1, (POSITIVE, 1, 2));
    TEST_SNUM_SHL(7, (POSITIVE, 1, 1), 63, (POSITIVE, 1, 0x8000000000000000));
    TEST_SNUM_SHL(8, (POSITIVE, 1, 1), 64, (POSITIVE, 2, 1, 0));
    TEST_SNUM_SHL(9, (POSITIVE, 1, 1), 65, (POSITIVE, 2, 2, 0));

    #undef TEST_SNUM_SHL

    TEST_FN_CLOSE
}

void test_snum_shr(bool show)
{
    TEST_FN_OPEN

    #define TEST_SNUM_SHR(TAG, SNUM_BEF, BITS, SNUM_AFT)        \
    {                                                           \
        TEST_CASE_OPEN(TAG)                                     \
        {                                                       \
            snum_t snum = snum_create_immed(ARG_OPEN SNUM_BEF); \
            snum = snum_shr(snum, BITS);                        \
            assert(snum_immed(snum, ARG_OPEN SNUM_AFT));        \
        }                                                       \
        TEST_CASE_CLOSE                                         \
    }

    TEST_SNUM_SHR(1, (ZERO, 0), 0, (ZERO, 0));
    TEST_SNUM_SHR(2, (ZERO, 0), 1, (ZERO, 0));
    TEST_SNUM_SHR(3, (ZERO, 0), 63, (ZERO, 0));
    TEST_SNUM_SHR(4, (ZERO, 0), 64, (ZERO, 0));
    TEST_SNUM_SHR(5, (ZERO, 0), 65, (ZERO, 0));
    TEST_SNUM_SHR(6, (POSITIVE, 1, 1), 0, (POSITIVE, 1, 1));
    TEST_SNUM_SHR(7, (POSITIVE, 1, 1), 1, (ZERO, 0));
    TEST_SNUM_SHR(8, (POSITIVE, 2, 2, 0), 0, (POSITIVE, 2, 2, 0));
    TEST_SNUM_SHR(9, (POSITIVE, 2, 2, 0), 1, (POSITIVE, 2, 1, 0));
    TEST_SNUM_SHR(10, (POSITIVE, 2, 2, 0), 2, (POSITIVE, 1, 0x8000000000000000));
    TEST_SNUM_SHR(11, (POSITIVE, 2, 2, 0), 64, (POSITIVE, 1, 2));
    TEST_SNUM_SHR(11, (POSITIVE, 2, 2, 0), 65, (POSITIVE, 1, 1));

    #undef TEST_SNUM_SHR

    TEST_FN_CLOSE
}



void test_snum_opposite(bool show)
{
    TEST_FN_OPEN

    #define TEST_SIG_OPPOSITE(TAG, SIGNAL_IN, SIGNAL_OUT, ...)          \
    {                                                                   \
        TEST_CASE_OPEN(TAG)                                             \
        {                                                               \
            snum_t snum = snum_create_immed(SIGNAL_IN, __VA_ARGS__);    \
            snum = snum_opposite(snum);                                 \
            assert(snum_immed(snum, SIGNAL_OUT, __VA_ARGS__));          \
        }                                                               \
        TEST_CASE_CLOSE                                                 \
    }

    TEST_SIG_OPPOSITE(1, ZERO, ZERO, 0);
    TEST_SIG_OPPOSITE(2, POSITIVE, NEGATIVE, 1, 1);
    TEST_SIG_OPPOSITE(3, NEGATIVE, POSITIVE, 1, 1);

    TEST_FN_CLOSE
}

void test_snum_add(bool show)
{
    TEST_FN_OPEN

    #define TEST_SNUM_ADD(TAG, SNUM_1, SNUM_2, SNUM_OUT)        \
    {                                                           \
        TEST_CASE_OPEN(TAG)                                     \
        {                                                       \
            snum_t snum_1 = snum_create_immed(ARG_OPEN SNUM_1); \
            snum_t snum_2 = snum_create_immed(ARG_OPEN SNUM_2); \
            snum_1 = snum_add(snum_1, snum_2);                  \
            assert(snum_immed(snum_1, ARG_OPEN SNUM_OUT));      \
        }                                                       \
        TEST_CASE_CLOSE                                         \
    }

    TEST_SNUM_ADD(1,
        (ZERO, 0),
        (ZERO, 0),
        (ZERO, 0)
    );
    TEST_SNUM_ADD(2,
        (ZERO, 0),
        (POSITIVE, 1, 1),
        (POSITIVE, 1, 1)
    );
    TEST_SNUM_ADD(3,
        (ZERO, 0),
        (NEGATIVE, 1, 1),
        (NEGATIVE, 1, 1)
    );
    TEST_SNUM_ADD(4,
        (POSITIVE, 1, 2),
        (POSITIVE, 1, 1),
        (POSITIVE, 1, 3)
    );
    TEST_SNUM_ADD(5,
        (POSITIVE, 1, 2),
        (ZERO, 0),
        (POSITIVE, 1, 2)
    );
    TEST_SNUM_ADD(6,
        (POSITIVE, 1, 2),
        (NEGATIVE, 1, 1),
        (POSITIVE, 1, 1)
    );
    TEST_SNUM_ADD(7,
        (POSITIVE, 1, 2),
        (NEGATIVE, 1, 2),
        (ZERO, 0)
    );
    TEST_SNUM_ADD(8,
        (POSITIVE, 1, 2),
        (NEGATIVE, 1, 3),
        (NEGATIVE, 1, 1)
    );
    TEST_SNUM_ADD(9,
        (NEGATIVE, 1, 2),
        (NEGATIVE, 1, 1),
        (NEGATIVE, 1, 3)
    );
    TEST_SNUM_ADD(10,
        (NEGATIVE, 1, 2),
        (ZERO, 0),
        (NEGATIVE, 1, 2)
    );
    TEST_SNUM_ADD(11,
        (NEGATIVE, 1, 2),
        (POSITIVE, 1, 1),
        (NEGATIVE, 1, 1)
    );
    TEST_SNUM_ADD(12,
        (NEGATIVE, 1, 2),
        (POSITIVE, 1, 2),
        (ZERO, 0)
    );
    TEST_SNUM_ADD(13,
        (NEGATIVE, 1, 2),
        (POSITIVE, 1, 3),
        (POSITIVE, 1, 1)
    );

    #undef TEST_SNUM_ADD

    TEST_FN_CLOSE
}

void test_snum_sub(bool show)
{
    TEST_FN_OPEN

    #define TESTS_SNUM_SUB(TAG, SNUM_1, SNUM_2, SNUM_OUT)       \
    {                                                           \
        TEST_CASE_OPEN(TAG)                                     \
        {                                                       \
            snum_t snum_1 = snum_create_immed(ARG_OPEN SNUM_1); \
            snum_t snum_2 = snum_create_immed(ARG_OPEN SNUM_2); \
            snum_1 = snum_sub(snum_1, snum_2);                  \
            assert(snum_immed(snum_1, ARG_OPEN SNUM_OUT));      \
        }                                                       \
        TEST_CASE_CLOSE                                         \
    }

    TESTS_SNUM_SUB(1,
        (POSITIVE, 1, 3),
        (POSITIVE, 1, 1),
        (POSITIVE, 1, 2)
    );
    TESTS_SNUM_SUB(2,
        (POSITIVE, 1, 3),
        (ZERO, 0),
        (POSITIVE, 1, 3)
    );
    TESTS_SNUM_SUB(1,
        (POSITIVE, 1, 3),
        (NEGATIVE, 1, 1),
        (POSITIVE, 1, 4)
    );

    #undef TESTS_SNUM_SUB

    TEST_FN_CLOSE
}

void test_snum_mul(bool show)
{
    TEST_FN_OPEN

    #define TEST_SNUM_MUL(TAG, SNUM_1, SNUM_2, SNUM_OUT)        \
    {                                                           \
        TEST_CASE_OPEN(TAG)                                     \
        {                                                       \
            snum_t snum_1 = snum_create_immed(ARG_OPEN SNUM_1); \
            snum_t snum_2 = snum_create_immed(ARG_OPEN SNUM_2); \
            snum_1 = snum_mul(snum_1, snum_2);                  \
            assert(snum_immed(snum_1, ARG_OPEN SNUM_OUT));      \
        }                                                       \
        TEST_CASE_CLOSE                                         \
    }

    TEST_SNUM_MUL(1,
        (POSITIVE, 1, 2),
        (POSITIVE, 1, 3),
        (POSITIVE, 1, 6)
    );
    TEST_SNUM_MUL(2,
        (POSITIVE, 1, 2),
        (ZERO, 0),
        (ZERO, 0)
    );
    TEST_SNUM_MUL(3,
        (NEGATIVE, 1, 2),
        (ZERO, 0),
        (ZERO, 0)
    );
    TEST_SNUM_MUL(4,
        (ZERO, 0),
        (POSITIVE, 1, 2),
        (ZERO, 0)
    );
    TEST_SNUM_MUL(5,
        (ZERO, 0),
        (NEGATIVE, 1, 2),
        (ZERO, 0)
    );
    TEST_SNUM_MUL(6,
        (POSITIVE, 1, 2),
        (NEGATIVE, 1, 3),
        (NEGATIVE, 1, 6)
    );
    TEST_SNUM_MUL(7,
        (NEGATIVE, 1, 2),
        (POSITIVE, 1, 3),
        (NEGATIVE, 1, 6)
    );
    TEST_SNUM_MUL(8,
        (NEGATIVE, 1, 2),
        (NEGATIVE, 1, 3),
        (POSITIVE, 1, 6)
    );

    #undef TEST_SNUM_MUL

    TEST_FN_CLOSE
}

void test_snum_div(bool show)
{
    TEST_FN_OPEN

    #define TEST_SNUM_DIV(TAG, SNUM_1, SNUM_2, SNUM_OUT)        \
    {                                                           \
        TEST_CASE_OPEN(TAG)                                     \
        {                                                       \
            snum_t snum_1 = snum_create_immed(ARG_OPEN SNUM_1); \
            snum_t snum_2 = snum_create_immed(ARG_OPEN SNUM_2); \
            snum_1 = snum_div(snum_1, snum_2);                  \
            assert(snum_immed(snum_1, ARG_OPEN SNUM_OUT));      \
        }                                                       \
        TEST_CASE_CLOSE                                         \
    }

    TEST_SNUM_DIV(1,
        (POSITIVE, 1, 2),
        (POSITIVE, 1, 3),
        (ZERO, 0)
    );
    TEST_SNUM_DIV(2,
        (POSITIVE, 1, 2),
        (NEGATIVE, 1, 3),
        (ZERO, 0)
    );
    TEST_SNUM_DIV(3,
        (POSITIVE, 1, 3),
        (POSITIVE, 1, 2),
        (POSITIVE, 1, 1)
    );
    TEST_SNUM_DIV(4,
        (ZERO, 0),
        (POSITIVE, 1, 2),
        (ZERO, 0)
    );
    TEST_SNUM_DIV(5,
        (NEGATIVE, 1, 3),
        (POSITIVE, 1, 2),
        (NEGATIVE, 1, 1)
    );

    #undef TEST_SNUM_DIV

    TEST_FN_CLOSE
}



void test_snum()
{
    TEST_LIB

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

    TEST_ASSERT_MEM_EMPTY
}



int main()
{
    setbuf(stdout, NULL);
    test_snum();
    printf("\n\n\tTest successful\n\n");
    return 0;
}
