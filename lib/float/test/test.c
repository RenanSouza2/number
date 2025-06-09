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
    {                                               \
        TEST_CASE_OPEN(TAG)                         \
        {                                           \
            printf("\na: %lld\tb: %lld", (int64_t)(A), (int64_t)(B)); \
            int64_t res = int64_add((int64_t)(A), (int64_t)(B));          \
            printf("\nres: %lld", res); \
            assert(int64(res, RES));                \
        }                                           \
        TEST_CASE_CLOSE                             \
    }

    for(int64_t a=-2; a<3; a++)
    for(int64_t b=-2; b<3; b++)
    {
        TEST_INT64_ADD(13 + 5*a + b, a, b, a + b);
    }

    TEST_INT64_ADD(26, INT64_MIN, 0, INT64_MIN);
    TEST_INT64_ADD(27, INT64_MAX, 0, INT64_MAX);

    #undef TEST_INT64_ADD

    #define TEST_INT64_ADD(TAG, FLT)                    \
    {                                                   \
        TEST_CASE_OPEN(TAG)                             \
        {                                               \
            TEST_REVERT_OPEN                            \
            {                                           \
                float_num_create_immed(ARG_OPEN FLT);   \
            }                                           \
            TEST_REVERT_CLOSE                           \
        }                                               \
        TEST_CASE_CLOSE                                 \
    }

    TEST_INT64_ADD(6, (INT64_MAX, 1, POSITIVE, 2, 1, 0));
    TEST_INT64_ADD(5, (INT64_MIN, 2, POSITIVE, 1, 1));

    #undef TEST_INT64_ADD

    TEST_FN_CLOSE
}



void test_float_num_create(bool show)
{
    TEST_FN_OPEN

    #define TEST_FLOAT_NUM_CREATE(TAG, FLT, EXPONENT, SIZE, SIG)    \
    {                                                               \
        TEST_CASE_OPEN(TAG)                                         \
        {                                                           \
            float_num_t flt = float_num_create_immed(ARG_OPEN FLT); \
            assert(int64(flt.exponent, EXPONENT));                  \
            assert(uint64(flt.size, SIZE));                         \
            assert(sig_num_immed(flt.sig, ARG_OPEN SIG));           \
        }                                                           \
        TEST_CASE_CLOSE                                             \
    }

    TEST_FLOAT_NUM_CREATE(1, FLOAT_NUM_ZERO(1), 0, 1, (ZERO, 0));
    TEST_FLOAT_NUM_CREATE(2, (0, 1, POSITIVE, 1, 1), 0, 1, (POSITIVE, 1, 1));
    TEST_FLOAT_NUM_CREATE(2, (0, 1, NEGATIVE, 1, 1), 0, 1, (NEGATIVE, 1, 1));
    TEST_FLOAT_NUM_CREATE(3, (0, 2, POSITIVE, 1, 1), -1, 2, (POSITIVE, 2, 1, 0));
    TEST_FLOAT_NUM_CREATE(4, (0, 1, POSITIVE, 2, 1, 2), 1, 1, (POSITIVE, 1, 1));

    #undef TEST_FLOAT_NUM_CREATE

    #define TEST_FLOAT_NUM_CREATE(TAG, FLT)             \
    {                                                   \
        TEST_CASE_OPEN(TAG)                             \
        {                                               \
            TEST_REVERT_OPEN                            \
            {                                           \
                float_num_create_immed(ARG_OPEN FLT);   \
            }                                           \
            TEST_REVERT_CLOSE                           \
        }                                               \
        TEST_CASE_CLOSE                                 \
    }

    TEST_FLOAT_NUM_CREATE(5, (INT64_MIN, 2, POSITIVE, 1, 1));
    TEST_FLOAT_NUM_CREATE(6, (INT64_MAX, 1, POSITIVE, 2, 1, 0));

    #undef TEST_FLOAT_NUM_CREATE

    TEST_FN_CLOSE
}

void test_float_num_is_zero(bool show)
{
    TEST_FN_OPEN

    #define TEST_FLOAT_NUM_IS_ZERO(TAG, FLOAT, RES)                     \
    {                                                                   \
        TEST_CASE_OPEN(TAG)                                             \
        {                                                               \
            float_num_t flt = float_num_create_immed(ARG_OPEN FLOAT);   \
            bool res = float_num_is_zero(flt);                          \
            assert(res == RES);                                         \
            float_num_free(flt);                                        \
        }                                                               \
        TEST_CASE_CLOSE                                                 \
    }

    TEST_FLOAT_NUM_IS_ZERO(1, FLOAT_NUM_ZERO(1), true);
    TEST_FLOAT_NUM_IS_ZERO(2, (0, 1, POSITIVE, 1, 1), false);

    #undef TEST_FLOAT_NUM_IS_ZERO

    TEST_FN_CLOSE
}



void test_float()
{
    TEST_LIB

    bool show = true;

    test_int64_get_sign(show);
    test_int64_add(show);

    test_float_num_create(show);
    test_float_num_is_zero(show);

    TEST_ASSERT_MEM_EMPTY
}



int main()
{
    setbuf(stdout, NULL);
    test_float();
    printf("\n\n\tTest successful\n\n");
    return 0;
}
