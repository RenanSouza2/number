#include "../debug.h"
#include "../../../testrc.h"
#include "../../../mods/macros/test.h"



void test_float_fn(bool show)
{
    TEST_FN_OPEN

    TEST_CASE_OPEN(1)
    {

    }
    TEST_CASE_CLOSE

    TEST_FN_CLOSE
}



void test_float()
{
    TEST_LIB

    bool show = false;

    test_float_fn(show);

    TEST_ASSERT_MEM_EMPTY
}



int main()
{
    setbuf(stdout, NULL);
    test_float();
    printf("\n\n\tTest successful\n\n");
    return 0;
}
