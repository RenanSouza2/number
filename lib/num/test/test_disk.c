#include "behavior.c"


static void test_num_disk()
{
    TEST_LIB

    bool show = false;

    num_config_t config = {
        .disk_path = "./cache",
        .disk_threshold = 0
    };
    num_config_set(&config);

    test_all(show);

    TEST_ASSERT_MEM_EMPTY
}



int main()
{
    setvbuf(stdout, nullptr, _IONBF, 0);
    srand((unsigned int)time(nullptr));
    test_num_disk();
    printf("\n\n\tTest successful\n\n");
    return 0;
}
