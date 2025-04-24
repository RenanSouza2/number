#include "../debug.h"
#include "../../../mods/macros/test.h"
#include "../../../testrc.h"



void test_fixed_fn(bool show)
{
    TEST_FN_OPEN

    TEST_CASE_OPEN(1)
    {

    }
    TEST_CASE_CLOSE

    TEST_FN_CLOSE
}



void test_fixed()
{
    TEST_LIB

    bool show = false;

    test_fixed_fn(show);

    TEST_ASSERT_MEM_EMPTY
}



int main()
{
    setbuf(stdout, NULL);
    TEST_TIMEOUT_OPEN(5)
    test_fixed();
    TEST_TIMEOUT_CLOSE
    printf("\n\n\tTest successful\n\n");
    return 0;
}
