#include "../debug.h"
#include "../../../testrc.h"
#include "../../../mods/macros/test.h"

#include "../../num/debug.h"
#include "../../sig/debug.h"



#define FLOAT_NUM_ZERO(SIZE) (0, SIZE, ZERO, 0)

void test_int64_get_sign(bool show)
{
    TEST_FN_OPEN

    #define TEST_INT64_GET_SIGN(TAG, IN, RES)   \
    {                                           \
        TEST_CASE_OPEN(TAG)                     \
        {                                       \
            uint64_t res = int64_get_sign(IN);  \
            assert(uint64(res, RES));           \
        }                                       \
        TEST_CASE_CLOSE                         \
    }

    TEST_INT64_GET_SIGN(1, INT64_MIN, NEGATIVE);
    TEST_INT64_GET_SIGN(2, -1, NEGATIVE);
    TEST_INT64_GET_SIGN(3, 0, ZERO);
    TEST_INT64_GET_SIGN(4, 1, POSITIVE);
    TEST_INT64_GET_SIGN(5, INT64_MAX, POSITIVE);

    #undef TEST_INT64_GET_SIGN

    TEST_FN_CLOSE
}

void test_int64_add(bool show)
{
    TEST_FN_OPEN

    #define TEST_INT64_ADD(TAG, A, B, RES)  \
    {                                       \
        TEST_CASE_OPEN(TAG)                 \
        {                                   \
            int64_t res = int64_add(A, B);  \
            assert(int64(res, RES));        \
        }                                   \
        TEST_CASE_CLOSE                     \
    }

    for(int64_t a=-2; a<3; a++)
    for(int64_t b=-2; b<3; b++)
    {
        TEST_INT64_ADD(13 + 5*a + b, a, b, a + b);
    }

    TEST_INT64_ADD(26, INT64_MIN, 0, INT64_MIN);
    TEST_INT64_ADD(27, INT64_MIN + 1, -1, INT64_MIN);
    TEST_INT64_ADD(28, INT64_MAX, 0, INT64_MAX);
    TEST_INT64_ADD(29, INT64_MAX - 1, 1, INT64_MAX);
    TEST_INT64_ADD(31, INT64_MIN, INT64_MAX, -1);
    TEST_INT64_ADD(32, INT64_MAX, INT64_MIN, -1);

    #undef TEST_INT64_ADD

    #define TEST_INT64_ADD(TAG, A, B)   \
    {                                   \
        TEST_CASE_OPEN(TAG)             \
        {                               \
            TEST_REVERT_OPEN            \
            {                           \
                int64_add(A, B);        \
            }                           \
            TEST_REVERT_CLOSE           \
        }                               \
        TEST_CASE_CLOSE                 \
    }

    TEST_INT64_ADD(33, INT64_MIN, -1);
    TEST_INT64_ADD(34, INT64_MAX, 1);
    TEST_INT64_ADD(35, INT64_MIN, INT64_MIN);
    TEST_INT64_ADD(36, INT64_MAX, INT64_MAX);

    #undef TEST_INT64_ADD

    TEST_FN_CLOSE
}



void test_float_num_normalize(bool show)
{
    TEST_FN_OPEN

    #define TEST_FLOAT_NUM_NORMALIZE(TAG, FLT, EXPONENT, SIZE, SIG) \
    {                                                               \
        TEST_CASE_OPEN(TAG)                                         \
        {                                                           \
            float_num_t flt = float_num_create_immed(ARG_OPEN FLT); \
            flt = float_num_normalize(flt);                         \
            assert(int64(flt.exponent, EXPONENT));                  \
            assert(uint64(flt.size, SIZE));                         \
            assert(sig_num_immed(flt.sig, ARG_OPEN SIG));           \
        }                                                           \
        TEST_CASE_CLOSE                                             \
    }

    TEST_FLOAT_NUM_NORMALIZE(1, FLOAT_NUM_ZERO(2), 0, 2, (ZERO, 0));
    TEST_FLOAT_NUM_NORMALIZE(2, (0, 2, POSITIVE, 2, 1, 0), 0, 2, (POSITIVE, 2, 1, 0));
    TEST_FLOAT_NUM_NORMALIZE(3, (0, 2, POSITIVE, 1, 1), -1, 2, (POSITIVE, 2, 1, 0));
    TEST_FLOAT_NUM_NORMALIZE(4, (0, 2, NEGATIVE, 1, 1), -1, 2, (NEGATIVE, 2, 1, 0));
    TEST_FLOAT_NUM_NORMALIZE(5, (0, 2, POSITIVE, 3, 1, 2, 3), 1, 2, (POSITIVE, 2, 1, 2));

    #undef TEST_FLOAT_NUM_NORMALIZE

    #define TEST_FLOAT_NUM_NORMALIZE(TAG, FLT)                      \
    {                                                               \
        TEST_CASE_OPEN(TAG)                                         \
        {                                                           \
            float_num_t flt = float_num_create_immed(ARG_OPEN FLT); \
            TEST_REVERT_OPEN                                        \
            {                                                       \
                float_num_normalize(flt);                           \
            }                                                       \
            TEST_REVERT_CLOSE                                       \
        }                                                           \
        TEST_CASE_CLOSE                                             \
    }

    TEST_FLOAT_NUM_NORMALIZE(6, (INT64_MIN, 2, POSITIVE, 1, 1));
    TEST_FLOAT_NUM_NORMALIZE(7, (INT64_MAX, 2, POSITIVE, 3, 1, 0, 1));

    #undef TEST_FLOAT_NUM_NORMALIZE

    TEST_FN_CLOSE
}



void test_float_num_set_exponent(bool show)
{
    TEST_FN_OPEN

    #define TEST_FLOAT_NUM_SET_EXPONENT(TAG, FLT, EXPONENT, RES)    \
    {                                                               \
        TEST_CASE_OPEN(TAG)                                         \
        {                                                           \
            float_num_t flt = float_num_create_immed(ARG_OPEN FLT); \
            flt = float_num_set_exponent(flt, EXPONENT);            \
            assert(float_num_immed(flt, ARG_OPEN RES));             \
        }                                                           \
        TEST_CASE_CLOSE                                             \
    }

    TEST_FLOAT_NUM_SET_EXPONENT(1, FLOAT_NUM_ZERO(2), 0, FLOAT_NUM_ZERO(2));
    TEST_FLOAT_NUM_SET_EXPONENT(2, FLOAT_NUM_ZERO(2), 1, (1, 2, ZERO, 0));
    TEST_FLOAT_NUM_SET_EXPONENT(3, FLOAT_NUM_ZERO(2), -1, (-1, 2, ZERO, 0));
    TEST_FLOAT_NUM_SET_EXPONENT(4, (0, 2, POSITIVE, 2, 1, 2), 0, (0, 2, POSITIVE, 2, 1, 2));
    TEST_FLOAT_NUM_SET_EXPONENT(5, (0, 2, POSITIVE, 2, 1, 2), -1, (-1, 2, POSITIVE, 3, 1, 2, 0));
    TEST_FLOAT_NUM_SET_EXPONENT(6, (0, 2, POSITIVE, 2, 1, 2), 2, (2, 2, ZERO, 0));
    TEST_FLOAT_NUM_SET_EXPONENT(7, (0, 3, POSITIVE, 3, 1, 2, 3), 1, (1, 3, POSITIVE, 2, 1, 2));

    #undef TEST_FLOAT_NUM_SET_EXPONENT

    TEST_FN_CLOSE
}



void test_float_num_is_zero(bool show)
{
    TEST_FN_OPEN

    #define TEST_FLOAT_NUM_IS_ZERO(TAG, FLT, RES)                   \
    {                                                               \
        TEST_CASE_OPEN(TAG)                                         \
        {                                                           \
            float_num_t flt = float_num_create_immed(ARG_OPEN FLT); \
            bool res = float_num_is_zero(flt);                      \
            assert(res == RES);                                     \
            float_num_free(flt);                                    \
        }                                                           \
        TEST_CASE_CLOSE                                             \
    }

    TEST_FLOAT_NUM_IS_ZERO(1, FLOAT_NUM_ZERO(2), true);
    TEST_FLOAT_NUM_IS_ZERO(2, (0, 2, POSITIVE, 2, 1, 0), false);

    #undef TEST_FLOAT_NUM_IS_ZERO

    TEST_FN_CLOSE
}



void test_float_num_add(bool show)
{
    TEST_FN_OPEN

    #define TEST_FLOAT_NUM_ADD(TAG, FLT_1, FLT_2, RES)                  \
    {                                                                   \
        TEST_CASE_OPEN(TAG)                                             \
        {                                                               \
            float_num_t flt_1 = float_num_create_immed(ARG_OPEN FLT_1); \
            float_num_t flt_2 = float_num_create_immed(ARG_OPEN FLT_2); \
            flt_1 = float_num_add(flt_1, flt_2);                        \
            assert(float_num_immed(flt_1, ARG_OPEN RES))                \
        }                                                               \
        TEST_CASE_CLOSE                                                 \
    }

    TEST_FLOAT_NUM_ADD(1,
        FLOAT_NUM_ZERO(2),
        FLOAT_NUM_ZERO(2),
        FLOAT_NUM_ZERO(2)
    );
    TEST_FLOAT_NUM_ADD(2,
        FLOAT_NUM_ZERO(2),
        (0, 2, POSITIVE, 1, 1, 0),
        (0, 2, POSITIVE, 1, 1, 0)
    );
    TEST_FLOAT_NUM_ADD(3,
        (0, 2, POSITIVE, 2, 1, 0),
        FLOAT_NUM_ZERO(2),
        (0, 2, POSITIVE, 2, 1, 0)
    );
    TEST_FLOAT_NUM_ADD(4,
        (0, 2, POSITIVE, 2, 1, 0),
        (0, 2, POSITIVE, 2, 2, 0),
        (0, 2, POSITIVE, 2, 3, 0)
    );
    TEST_FLOAT_NUM_ADD(5,
        ( 0, 2, POSITIVE, 2, 1, 0),
        ( 0, 2, NEGATIVE, 2, 1, 0),
        (-1, 2, ZERO, 0)
    );
    TEST_FLOAT_NUM_ADD(6,
        (2, 2, POSITIVE, 2, 1, 0),
        (0, 2, POSITIVE, 2, 1, 0),
        (2, 2, POSITIVE, 2, 1, 0)
    );
    TEST_FLOAT_NUM_ADD(7,
        (1, 2, POSITIVE, 2, 1, 0),
        (0, 2, POSITIVE, 2, 1, 0),
        (1, 2, POSITIVE, 2, 1, 1)
    );
    TEST_FLOAT_NUM_ADD(8,
        (1, 2, POSITIVE, 2, 1, 1),
        (1, 2, NEGATIVE, 2, 1, 0),
        (0, 2, POSITIVE, 2, 1, 0)
    );
    TEST_FLOAT_NUM_ADD(9,
        (0, 2, POSITIVE, 2, 1, 0),
        (2, 2, POSITIVE, 2, 1, 0),
        (2, 2, POSITIVE, 2, 1, 0)
    );
    TEST_FLOAT_NUM_ADD(10,
        (0, 2, POSITIVE, 2, 1, 0),
        (1, 2, POSITIVE, 2, 1, 0),
        (1, 2, POSITIVE, 2, 1, 1)
    );
    TEST_FLOAT_NUM_ADD(11,
        (0, 2, POSITIVE, 2, 1, 0),
        (1, 2, POSITIVE, 2, 1, 0),
        (1, 2, POSITIVE, 2, 1, 1)
    );

    #undef TEST_FLOAT_NUM_ADD

    TEST_FN_CLOSE
}

void test_float_num_sub(bool show)
{
    TEST_FN_OPEN

    #define TEST_FLOAT_NUM_SUB(TAG, FLT_1, FLT_2, RES)                  \
    {                                                                   \
        TEST_CASE_OPEN(TAG)                                             \
        {                                                               \
            float_num_t flt_1 = float_num_create_immed(ARG_OPEN FLT_1); \
            float_num_t flt_2 = float_num_create_immed(ARG_OPEN FLT_2); \
            flt_1 = float_num_sub(flt_1, flt_2);                        \
            assert(float_num_immed(flt_1, ARG_OPEN RES))                \
        }                                                               \
        TEST_CASE_CLOSE                                                 \
    }

    TEST_FLOAT_NUM_SUB(1,
        FLOAT_NUM_ZERO(2),
        FLOAT_NUM_ZERO(2),
        FLOAT_NUM_ZERO(2)
    );
    TEST_FLOAT_NUM_SUB(2,
        FLOAT_NUM_ZERO(2),
        (0, 2, POSITIVE, 2, 1, 0),
        (0, 2, NEGATIVE, 2, 1, 0)
    );

    #undef TEST_FLOAT_NUM_SUB

    TEST_FN_CLOSE
}

void test_float_num_mul(bool show)
{
    TEST_FN_OPEN

    #define TEST_FLOAT_NUM_MUL(TAG, FLT_1, FLT_2, RES)                  \
    {                                                                   \
        TEST_CASE_OPEN(TAG)                                             \
        {                                                               \
            float_num_t flt_1 = float_num_create_immed(ARG_OPEN FLT_1); \
            float_num_t flt_2 = float_num_create_immed(ARG_OPEN FLT_2); \
            flt_1 = float_num_mul(flt_1, flt_2);                        \
            assert(float_num_immed(flt_1, ARG_OPEN RES))                \
        }                                                               \
        TEST_CASE_CLOSE                                                 \
    }

    TEST_FLOAT_NUM_MUL(1,
        FLOAT_NUM_ZERO(2),
        FLOAT_NUM_ZERO(2),
        (0, 2, ZERO, 0)
    );
    TEST_FLOAT_NUM_MUL(2,
        FLOAT_NUM_ZERO(2),
        (-1, 2, POSITIVE, 2, 1, 0),
        (-1, 2, ZERO, 0)
    );
    TEST_FLOAT_NUM_MUL(3,
        (-1, 2, POSITIVE, 2, 1, 0),
        FLOAT_NUM_ZERO(2),
        (-1, 2, ZERO, 0)
    );
    TEST_FLOAT_NUM_MUL(4,
        (-1, 2, POSITIVE, 2, 1, 0),
        (-1, 2, POSITIVE, 2, 1, 0),
        (-1, 2, POSITIVE, 2, 1, 0)
    );

    #undef TEST_FLOAT_NUM_MUL

    TEST_FN_CLOSE
}

void test_float_num_sqr(bool show)
{
    TEST_FN_OPEN

    #define TEST_FLOAT_NUM_SQR(TAG, FLT, RES)                       \
    {                                                               \
        TEST_CASE_OPEN(TAG)                                         \
        {                                                           \
            float_num_t flt = float_num_create_immed(ARG_OPEN FLT); \
            flt = float_num_sqr(flt);                               \
            assert(float_num_immed(flt, ARG_OPEN RES))              \
        }                                                           \
        TEST_CASE_CLOSE                                             \
    }

    TEST_FLOAT_NUM_SQR(1,
        (-1, 2, POSITIVE, 2, 3, 0),
        (-1, 2, POSITIVE, 2, 9, 0)
    );
    TEST_FLOAT_NUM_SQR(2,
        (-2, 2, NEGATIVE, 2, 3, 0),
        (-3, 2, POSITIVE, 2, 9, 0)
    );

    #undef TEST_FLOAT_NUM_SQR
    
    #define TEST_FLOAT_NUM_SQR(TAG, FLT)                            \
    {                                                               \
        TEST_CASE_OPEN(TAG)                                         \
        {                                                           \
            float_num_t flt = float_num_create_immed(ARG_OPEN FLT); \
            TEST_REVERT_OPEN                                        \
            {                                                       \
                float_num_sqr(flt);                                 \
            }                                                       \
            TEST_REVERT_CLOSE                                       \
        }                                                           \
        TEST_CASE_CLOSE                                             \
    }

    TEST_FLOAT_NUM_SQR(3, (INT64_MAX, 2, POSITIVE, 2, 1, 0));

    #undef TEST_FLOAT_NUM_SQR

    TEST_FN_CLOSE
}

void test_float_num_div(bool show)
{
    TEST_FN_OPEN

    #define TEST_FLOAT_NUM_DIV(TAG, FLT_1, FLT_2, RES)                  \
    {                                                                   \
        TEST_CASE_OPEN(TAG)                                             \
        {                                                               \
            float_num_t flt_1 = float_num_create_immed(ARG_OPEN FLT_1); \
            float_num_t flt_2 = float_num_create_immed(ARG_OPEN FLT_2); \
            flt_1 = float_num_div(flt_1, flt_2);                        \
            assert(float_num_immed(flt_1, ARG_OPEN RES))                \
        }                                                               \
        TEST_CASE_CLOSE                                                 \
    }

    TEST_FLOAT_NUM_DIV(1,
        (-1, 2, POSITIVE, 2, 6, 0),
        (-1, 2, POSITIVE, 2, 3, 0),
        (-1, 2, POSITIVE, 2, 2, 0)
    );

    #undef TEST_FLOAT_NUM_DIV

    TEST_FN_CLOSE
}



void test_float_num_div_sig(bool show)
{
    TEST_FN_OPEN

    #define TEST_FLOAT_NUM_DIV_SIG(TAG, FLT, SIG, RES)                  \
    {                                                               \
        TEST_CASE_OPEN(TAG)                                         \
        {                                                           \
            float_num_t flt = float_num_create_immed(ARG_OPEN FLT); \
            sig_num_t flt_2 = sig_num_create_immed(ARG_OPEN SIG);   \
            flt = float_num_div_sig(flt, flt_2);                    \
            assert(float_num_immed(flt, ARG_OPEN RES))              \
        }                                                           \
        TEST_CASE_CLOSE                                             \
    }

    TEST_FLOAT_NUM_DIV_SIG(1,
        (0, 2, POSITIVE, 2, 6, 0),
        (POSITIVE, 1, 3),
        (0, 2, POSITIVE, 2, 2, 0)
    );
    TEST_FLOAT_NUM_DIV_SIG(2,
        (0, 2, POSITIVE, 2, 6, 0),
        (POSITIVE, 2, 3, 0),
        (-1, 2, POSITIVE, 2, 2, 0)
    );
    TEST_FLOAT_NUM_DIV_SIG(3,
        (0, 2, POSITIVE, 2, 5, 0),
        (POSITIVE, 1, 3),
        (0, 2, POSITIVE, 2, 1, 0xAAAAAAAAAAAAAAAA)
    );
    TEST_FLOAT_NUM_DIV_SIG(4,
        (0, 2, POSITIVE, 2, 2, 0),
        (POSITIVE, 1, 3),
        (-1, 2, POSITIVE, 2, 0xAAAAAAAAAAAAAAAA, 0xAAAAAAAAAAAAAAAA)
    );

    #undef TEST_FLOAT_NUM_DIV_SIG

    TEST_FN_CLOSE
}



void test_float()
{
    TEST_LIB

    bool show = true;

    test_int64_get_sign(show);
    test_int64_add(show);

    test_float_num_normalize(show);

    test_float_num_set_exponent(show);

    test_float_num_is_zero(show);

    test_float_num_add(show);
    test_float_num_sub(show);
    test_float_num_mul(show);
    test_float_num_sqr(show);
    test_float_num_div(show);

    test_float_num_div_sig(show);

    TEST_ASSERT_MEM_EMPTY
}



int main()
{
    setbuf(stdout, NULL);
    test_float();
    printf("\n\n\tTest successful\n\n");
    return 0;
}
