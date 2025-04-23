#include "../debug.h"

#include "../../../mods/clu/header.h"
#include "../../../mods/macros/test.h"



#define TEST_ASSERT_EMPTY assert(clu_mem_is_empty());

void test_example()
{
    TEST_LIB

    bool show = true;

    TEST_ASSERT_EMPTY
}



int main()
{
    setbuf(stdout, NULL);
    TEST_TIMEOUT_OPEN_DEFAULT
    test_example();
    TEST_TIMEOUT_CLOSE
    printf("\n\n\tTest successful\n\n");
    return 0;
}
