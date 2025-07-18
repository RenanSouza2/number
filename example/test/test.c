#include "../debug.h"
#include "../../../testrc.h"
#include "../../../mods/macros/test.h"



void test_example_fn(bool show)
{
    TEST_FN_OPEN

    TEST_CASE_OPEN(1)
    {

    }
    TEST_CASE_CLOSE

    TEST_FN_CLOSE
}



void test_example()
{
    TEST_LIB

    bool show = false;

    test_example_fn(show);

    TEST_ASSERT_MEM_EMPTY
}



int main()
{
    setbuf(stdout, NULL);
    test_example();
    printf("\n\n\tTest successful\n\n");
    return 0;
}
