#include "../debug.h"
#include "../../../testrc.h"
#include "../../../mods/macros/test.h"

#include "../../num/debug.h"
#include "../../sig/debug.h"


#define FLOAT_NUM_ZERO(SIZE) (0, SIZE, ZERO, 0)

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
    TEST_FLOAT_NUM_CREATE(3, (0, 3, POSITIVE, 1, 1), -1, 1, (POSITIVE, 2, 1, 0));

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
