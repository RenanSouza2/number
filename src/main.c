#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../lib/num/header.h"
#include "../utils/assert.h"
#include "../utils/U64.h"



uint64_t altutime()
{
    struct timespec time;
    clock_gettime(CLOCK_MONOTONIC, &time);
    return time.tv_sec * 1e6 + time.tv_nsec / 1e3;
}



num_p num_generate(uint64_t max, uint64_t salt)
{
    uint64_t begin = altutime();

    num_p num = num_wrap(2);
    for(uint64_t i=0; i<max; i++)
    {
        printf("\ni: %lu", i);

        num = num_add(num, num_wrap(salt));
        num = num_mul(num, num_copy(num));

        uint64_t end = altutime();
        num_display_full("num", num);
        printf("\n%lu: %10.3f", i, (end - begin) / 1e6);
    }
    return num;
}

num_p num_generate_2(uint64_t index)
{
    num_p num = num_wrap(0xe6503424c62eef89);
    for(uint64_t i=1; i<index; i++)
    {
        num = num_add(num, num_wrap(2));
        num = num_mul(num, num_wrap(0xe6503424c62eef89));
    }
    return num;
}

uint64_t rand_64()
{
    return ((uint64_t)rand()) << 32 | rand();
}

num_p num_rand(uint64_t count)
{
    num_p num = num_wrap(0);
    for(uint64_t i=0; i<count; i++)
    {
        uint64_t value = rand_64();
        num = num_shl(num, 64);
        num = num_add(num, num_wrap(value));
    }
    return num;
}



void time_1(uint64_t begin, uint64_t end)
{
    assert(begin);
    num_p num_1 = num_generate(begin, 1);
    num_p num_2 = num_generate(begin - 1, 2);
    for(uint64_t i=begin; i<end; i++)
    {
        printf("\n" U64P() "", i);

        num_1 = num_add(num_1, num_wrap(1));
        num_1 = num_mul(num_1, num_copy(num_1));

        num_2 = num_add(num_2, num_wrap(2));
        num_2 = num_mul(num_2, num_copy(num_2));

        num_p num_1_copy = num_copy(num_1);
        num_p num_2_copy = num_copy(num_2);

        uint64_t begin = altutime();
        num_p num = num_div(num_1_copy, num_2_copy);
        uint64_t end = altutime();
        printf("\t%10.3f", (end - begin) / 1e6);

        num_free(num);
    }
}

void time_2(int argc, char** argv, uint64_t max)
{
    assert(argc > 1);
    uint64_t id = atoi(argv[1]);
    printf("\nid: " U64P() "", id);

    num_p num_1 = num_generate(max, 2);
    num_display_tag("num_1", num_1);

    num_p num_2 = num_wrap(0xe6503424c62eef89);
    for(uint64_t i=1; num_cmp(num_1, num_2) > 0; i++)
    {
        num_2 = num_add(num_2, num_wrap(2));
        num_2 = num_mul(num_2, num_wrap(0xe6503424c62eef89));

        if(i % 8 != id)
            continue;

        printf("\n" U64P(5) "", i);

        num_p num_aux_1 = num_copy(num_1);
        num_p num_aux_2 = num_copy(num_2);

        uint64_t begin = altutime();
        num_aux_1 = num_div(num_aux_1, num_aux_2);
        uint64_t end = altutime();
        printf("\t%10.3lf", (end - begin) / 1e6);
    }
}



void fibonacci()
{
    num_p num_a = num_wrap(1);
    num_p num_b = num_wrap(1);

    uint64_t space = 1e4;
    for(uint64_t i=0; ; i++)
    {
        num_p num_c = num_add(num_a, num_copy(num_b));

        if(i%space == 0)
        {
            printf("\n" U64P() " num: ", i);
            num_display(num_c);
        }

        num_a = num_b;
        num_b = num_c;
    }
}

void fibonacci_2()
{
    num_p num_a = num_wrap(1);
    num_p num_b = num_wrap(0);
    num_p num_c = num_wrap(1);

    uint64_t space = 1;
    for(uint64_t i=0; ; i++)
    {
        num_display_full("num_b", num_b);

        if(i%space == 0)
        {
            printf("\n" U64P() " num: ", i);
            num_display(num_c);
        }

        num_p num_b_2 = num_mul(num_copy(num_b), num_copy(num_b));
        num_p num_a_2 = num_mul(num_copy(num_a), num_copy(num_a));
        num_p num_a_new = num_add(num_a_2, num_copy(num_b_2));

        num_p num_b_new = num_add(num_a, num_copy(num_c));
        num_b_new = num_mul(num_b_new, num_copy(num_c));

        num_p num_c_2 = num_mul(num_c, num_copy(num_c));
        num_p num_c_new = num_add(num_b_2, num_c_2);

        num_a = num_a_new;
        num_b = num_b_new;
        num_c = num_c_new;
    }
}

void factorial()
{
    num_p num = num_wrap(1);
    for(uint64_t i=1; ; i++)
    {
        num = num_mul(num, num_wrap(i));

        if(i%1000 == 0)
        {
            num_display_full("num", num);
            printf("\ni: %lu\t", i);
        }
    }
}

int main(int argc, char** argv)
{
    setbuf(stdout, NULL);
    srand(time(NULL));

    // time_1(13, 30);
    // time_2(argc, argv, 18);
    // num_generate(30, 2);

    // fibonacci();
    // fibonacci_2();
    // factorial();

    

    num_p num_1 = num_wrap_str("10384593717069655257060992658440192");
    num_p num_2 = num_wrap_str("5192296858534827628530496329220096");
    num_p num_3 = num_wrap_str("15576890575604482885591488987660288000000000000000000");
    num_p num_4 = num_wrap_str("3000000000000000000");

    num_display_tag("num_1", num_1);
    num_display_tag("num_2", num_2);
    num_display_tag("num_3", num_3);
    num_display_tag("num_4", num_4);

    num_p num_5 = num_shl(num_wrap(1), 112);
    num_display_tag("num_5", num_5);

    printf("\n");
    return 0;
}
