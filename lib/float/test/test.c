#include "../debug.h"
#include "../../../testrc.h"
#include "../../../mods/macros/test.h"



void test_float_num_is_zero(bool show)
{
    TEST_FN_OPEN

    #define TEST_FLOAT_NUM_IS_ZERO(TAG, FLOAT, RES) \
    {   \
        TEST_CASE_OPEN(TAG) \
        {   \
            float_num_t flt = float_num_create_immed(ARG_OPEN FLOAT);   \
            bool res = float_num_is_zero(flt);  \
            assert(res == RES); \
            float_num_free(flt);    \
        }   \
        TEST_CASE_CLOSE \
    }

    TEST_FLOAT_NUM_IS_ZERO(1, (0), true);

    #undef TEST_FLOAT_NUM_IS_ZERO

    TEST_FN_CLOSE
}



void test_float()
{
    TEST_LIB

    bool show = true;

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
