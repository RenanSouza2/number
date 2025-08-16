#include "../debug.h"
#include "../../../testrc.h"
#include "../../../mods/macros/test.h"



void test_fxd_fn(bool show)
{
    TEST_FN_OPEN

    TEST_CASE_OPEN(1)
    {

    }
    TEST_CASE_CLOSE

    TEST_FN_CLOSE
}



void test_fxd()
{
    TEST_LIB

    bool show = false;

    test_fxd_fn(show);

    TEST_ASSERT_MEM_EMPTY
}



int main()
{
    setbuf(stdout, NULL);
    test_fxd();
    printf("\n\n\tTest successful\n\n");
    return 0;
}
