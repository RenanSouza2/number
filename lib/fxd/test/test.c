#include "../debug.h"
#include "../../../testrc.h"
#include "../../../mods/macros/test.h"



void test_fxd_div(bool show)
{
    TEST_FN_OPEN

    #define TEST_FXD_NUM_DIV(TAG, FXD_1, FXD_2, RES)                \
    {                                                               \
        TEST_CASE_OPEN(TAG)                                         \
        {                                                           \
            fxd_num_t fxd_1 = fxd_num_create_immed(ARG_OPEN FXD_1); \
            fxd_num_t fxd_2 = fxd_num_create_immed(ARG_OPEN FXD_2); \
            fxd_1 = fxd_num_div(fxd_1, fxd_2);                      \
            assert(fxd_num_immed(fxd_1, ARG_OPEN RES))              \
        }                                                           \
        TEST_CASE_CLOSE                                             \
    }

    TEST_FXD_NUM_DIV(1,
        (1, POSITIVE, 2, 6, 0),
        (1, POSITIVE, 2, 3, 0),
        (1, POSITIVE, 2, 2, 0)
    );

    TEST_FN_CLOSE
}



void test_fxd(void)
{
    TEST_LIB

    bool show = false;

    test_fxd_div(show);

    TEST_ASSERT_MEM_EMPTY
}



int main(void)
{
    setbuf(stdout, NULL);
    test_fxd();
    printf("\n\n\tTest successful\n\n");
    return 0;
}
