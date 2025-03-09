#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

#include "../utils/assert.h"
#include "../utils/U64.h"

#include "../lib/num/header.h"
#include "../lib/mod/header.h"
#include "../lib/fix/header.h"

#include "../utils/clu/header.h"


uint64_t altutime()
{
    struct timespec time;
    clock_gettime(CLOCK_MONOTONIC, &time);
    return time.tv_sec * 1e3 + time.tv_nsec / 1e6;
}



num_p num_generate(uint64_t max, uint64_t salt)
{
    num_p num = num_wrap(2);
    for(uint64_t i=0; i<max; i++)
    {
        num = num_add(num, num_wrap(salt));
        num = num_mul(num, num_copy(num));
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
        printf("\t%10.3f", (end - begin) / 1e3);

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
        printf("\t%10.3lf", (end - begin) / 1e3);
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
            printf("\ni: " U64P() "\t", i);
        }
    }
}



void mod_fib(mod_p *mod_a, mod_p *mod_b)
{
    mod_p mod_c = mod_add(*mod_a, mod_copy(*mod_b));

    *mod_a = *mod_b;
    *mod_b = mod_c;
}

void time_dec()
{
    printf("\n");
    num_p num = num_generate(20, 2);
    printf("\ngenerated\n");
    printf("\n");
    uint64_t begin = altutime();
    num_display_dec(num);
    uint64_t end = altutime();
    printf("\n");
    printf("\ntime: %.3f", (end - begin) / 1e3);
}


void pi_1()
{
    uint64_t pos = 3;
    fix_p fix = fix_wrap(0, pos);
    for(uint64_t i=1; ; i++)
    {
        int64_t base = 16 * i * i - 16 * i + 3;
        fix_p fix_1 = fix_wrap(8, pos);
        fix_p fix_2 = fix_wrap(base, pos);
        fix_1 = fix_div(fix_1, fix_2);
        fix = fix_add(fix, fix_copy(fix_1));

        if(i%1000000 == 0)
        {
            printf("\ni: %llu\t", i);
            fix_display_dec(fix);
            printf("\t");
            fix_display_dec(fix_1);
            fix_free(fix_1);
        }
    }
}

void e()
{
    uint64_t pos = 1000;
    fix_p fix = fix_wrap(0, pos);
    fix_p base = fix_wrap(1, pos);
    for(uint64_t i=1; ; i++)
    {
        fix_p fix_2 = fix_wrap(i, pos);
        base = fix_div(base, fix_2);
        fix = fix_add(fix, fix_copy(base));

        if(i%1000 == 0)
        {
            fprintf(stderr, "\ni: %llu", i);
            printf("\n");
            fix_display_dec(fix);
        }
    }
}


int main(int argc, char** argv)
{
    setbuf(stdout, NULL);
    srand(time(NULL));

    // uint64_t pos = 2;
    // fix_p fix = fix_wrap(2, pos);
    // for(uint64_t i=1; ; i++)
    // {
    //     uint64_t base = 4 * i * i;
    //     fix = fix_mul(fix, fix_wrap(base, pos));
    //     fix = fix_div(fix, fix_wrap(base - 1, pos));
    //
    //     if(i%1000000 == 0)
    //     {
    //         printf("\n");
    //         fix_display_dec(fix);
    //     }
    // }

    // double fix = 2;
    // for(uint64_t i=1; ; i++)
    // {

    //     double base = 4 * i * i;
    //     fix *= base / (base - 1);

    //     if(i%1000000 == 0)
    //     {
    //         printf("\n%.16f %.0f %.16f %f", fix, base, base / (base - 1), (double)(4 * i * i) / INT_MAX);
    //     }
    // }

    time_1(13, 30);

    printf("\n");
    return 0;
}
