#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include <unistd.h>

#include "../lib/macros/assert.h"
#include "../lib/macros/U64.h"

#include "../lib/num/struct.h"
#include "../lib/mod/struct.h"
#include "../lib/fix/struct.h"

#include "../mods/clu/header.h"



uint64_t altutime()
{
    struct timespec time;
    clock_gettime(CLOCK_MONOTONIC, &time);
    return time.tv_sec * 1e3 + time.tv_nsec / 1e6;
}



num_t num_generate(uint64_t max, uint64_t salt)
{
    num_t num = num_wrap(2);
    for(uint64_t i=0; i<max; i++)
    {
        // printf("\n%lu", i);

        // uint64_t begin = altutime();
        num = num_add(num, num_wrap(salt));
        num = num_sqr(num);
        // uint64_t end = altutime();
        // printf("\t%10.3f", (end - begin) / 1e3);
    }
    return num;
}

num_t num_generate_2(uint64_t index)
{
    num_t num = num_wrap(0xe6503424c62eef89);
    for(uint64_t i=1; i<index; i++)
    {
        num = num_add(num, num_wrap(2));
        num = num_mul(num, num_wrap(0xe6503424c62eef89));
    }
    return num;
}

uint64_t rand_64()
{
    return (U64(rand()) << 32) | rand();
}

num_t num_rand(uint64_t count)
{
    num_t num = num_wrap(0);
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
    num_t num_1 = num_generate(begin, 1);
    num_t num_2 = num_generate(begin - 1, 2);
    for(uint64_t i=begin; i<end; i++)
    {
        printf("\n" U64P() "", i);

        uint64_t begin = altutime();
        num_1 = num_add(num_1, num_wrap(1));
        num_1 = num_sqr(num_1);
        uint64_t end = altutime();
        // printf("\t%10.3f", (end - begin) / 1e3);

        begin = altutime();
        num_2 = num_add(num_2, num_wrap(2));
        num_2 = num_sqr(num_2);
        end = altutime();
        // printf("\t%10.3f", (end - begin) / 1e3);

        num_t num_1_copy = num_copy(num_1);
        num_t num_2_copy = num_copy(num_2);

        begin = altutime();
        num_t num = num_div(num_1_copy, num_2_copy);
        end = altutime();
        printf("\t%10.3f", (end - begin) / 1e3);

        num_free(num);
    }
    num_free(num_1);
    num_free(num_2);
}

void time_2(int argc, char** argv, uint64_t max)
{
    assert(argc > 1);
    uint64_t id = atoi(argv[1]);
    printf("\nid: " U64P() "", id);

    num_t num_1 = num_generate(max, 2);
    num_display_tag("num_1", num_1);

    num_t num_2 = num_wrap(0xe6503424c62eef89);
    for(uint64_t i=1; num_cmp(num_1, num_2) > 0; i++)
    {
        num_2 = num_add(num_2, num_wrap(2));
        num_2 = num_mul(num_2, num_wrap(0xe6503424c62eef89));

        if(i % 8 != id)
            continue;

        printf("\n" U64P(5) "", i);

        num_t num_aux_1 = num_copy(num_1);
        num_t num_aux_2 = num_copy(num_2);

        uint64_t begin = altutime();
        num_aux_1 = num_div(num_aux_1, num_aux_2);
        uint64_t end = altutime();
        printf("\t%10.3lf", (end - begin) / 1e3);
    }
}



void fibonacci()
{
    num_t num_a = num_wrap(1);
    num_t num_b = num_wrap(1);

    uint64_t space = 1e4;
    for(uint64_t i=0; ; i++)
    {
        num_t num_c = num_add(num_a, num_copy(num_b));

        if(i%space == 0)
        {
            printf("\n" U64P() " num: ", i);
            num_display(num_c);
        }

        num_a = num_b;
        num_b = num_c;
    }
}

void fibonacci_2(uint64_t min, uint64_t max)
{
    num_t num_a = num_wrap(0);
    num_t num_b = num_wrap(1);
    num_t num_c = num_wrap(1);

    for(uint64_t i=0; i < max; i++)
    {
        if(i > min)
            printf("\n" U64P() "", i);

        uint64_t begin = altutime();
        num_t num_a_2 = num_sqr(num_copy(num_a));
        num_t num_b_2 = num_sqr(num_copy(num_b));
        num_t num_c_2 = num_sqr(num_copy(num_c));

        num_a = num_add(num_a, num_c);
        num_b = num_mul(num_b, num_a);

        num_a = num_add(num_a_2, num_copy(num_b_2));
        num_c = num_add(num_b_2, num_c_2);
        uint64_t end = altutime();

        if(i > min)
            printf("\t%10.3f", (end - begin) / 1e3);
    }
}

void factorial()
{
    num_t num = num_wrap(1);
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

num_t fib_1(uint64_t index)
{
    if(index < 2)
        return num_wrap(1);

    num_t num_a = num_wrap(1);
    num_t num_b = num_wrap(1);
    for(uint64_t i=1; i<index; i++)
    {
        num_t num_c = num_add(num_a, num_copy(num_b));
        num_a = num_b;
        num_b = num_c;
    }
    num_free(num_a);
    return num_b;
}

num_t fib_2(uint64_t index)
{
    num_t num_a = num_wrap(1);
    num_t num_b = num_wrap(0);
    num_t num_c = num_wrap(1);
    for(uint64_t mask = 0x8000000000000000; mask; mask >>= 1)
    {
        num_t num_a_2 = num_sqr(num_copy(num_a));
        num_t num_b_2 = num_sqr(num_copy(num_b));
        num_t num_c_2 = num_sqr(num_copy(num_c));

        num_a = num_add(num_a, num_c);
        num_b = num_mul(num_b, num_a);

        num_a = num_add(num_a_2, num_copy(num_b_2));
        num_c = num_add(num_b_2, num_c_2);

        if((mask & index) == 0)
            continue;

        num_c_2 = num_add(num_copy(num_b), num_copy(num_c));
        num_free(num_a);
        num_a = num_b;
        num_b = num_c;
        num_c = num_c_2;
    }
    num_free(num_a);
    num_free(num_b);
    return num_c;
}



void mod_fib(mod_p mod_a, mod_p mod_b)
{
    mod_t mod_c = mod_add(*mod_a, mod_copy(*mod_b));

    *mod_a = *mod_b;
    *mod_b = mod_c;
}

void time_dec()
{
    printf("\n");
    num_t num = num_generate(20, 2);
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
    fix_t fix = fix_wrap(0, pos);
    for(uint64_t i=1; ; i++)
    {
        int64_t base = 16 * i * i - 16 * i + 3;
        fix_t fix_1 = fix_wrap(8, pos);
        fix_t fix_2 = fix_wrap(base, pos);
        fix_1 = fix_div(fix_1, fix_2);
        fix = fix_add(fix, fix_copy(fix_1));

        if(i%1000000 == 0)
        {
            printf("\ni: " U64P() "\t", i);
            fix_display_dec("fix", fix);
            printf("\t");
            fix_display_dec("fix_1", fix_1);
            fix_free(fix_1);
        }
    }
}

void e()
{
    uint64_t pos = 1000;
    fix_t fix = fix_wrap(0, pos);
    fix_t base = fix_wrap(1, pos);
    for(uint64_t i=1; ; i++)
    {
        fix_t fix_2 = fix_wrap(i, pos);
        base = fix_div(base, fix_2);
        fix = fix_add(fix, fix_copy(base));

        if(i%1000 == 0)
        {
            fprintf(stderr, "\ni: " U64P() "", i);
            fix_display_dec("fix", fix);
        }
    }
}

void pi_2()
{
    uint64_t pos = 2;
    fix_t fix = fix_wrap(2, pos);
    for(uint64_t i=1; ; i++)
    {
        uint64_t base = 4 * i * i;
        fix = fix_mul(fix, fix_wrap(base, pos));
        fix = fix_div(fix, fix_wrap(base - 1, pos));

        if(i%1000000 == 0)
            fix_display_dec("fix", fix);
    }
}



void display_bit(uint64_t value)
{
    for(uint64_t i=0; i<64; i++)
    {
        printf(U64PX, value >> 63);
        value <<= 1;
    }
}


fix_t fix_step(fix_t fix, uint64_t pos)
{
    fix_t fix_a = fix_shr(fix_copy(fix), 1);
    fix = fix_div(fix_wrap(1, pos), fix);
    return fix_add(fix, fix_a);
}

void sqrt_2()
{
    fix_t fix_x = fix_wrap(1, 1);
    num_t num = num_wrap(10);

    for(uint64_t i=0; ; i++)
    {
        printf("\ni: " U64P() "", i);
        // fprintf(stderr, "\ni: %lu", i);

        fix_x = fix_step(fix_x, 1);

        fix_t fix_2 = fix_sqr(fix_copy(fix_x));
        fix_2 = fix_sub(fix_2, fix_wrap(2, 1));
        bool res = num_cmp(fix_2.snum.num, num) < 0;
        fix_free(fix_2);
        if(res)
            break;
    }

    for(uint64_t pos = 2; ; pos *= 2)
    {
        fix_x = fix_reposition(fix_x, pos);
        fix_x = fix_step(fix_x, pos);

        if(fork())
            continue;

        printf("\n\n");
        fix_display_full("hex", fix_x);
        fix_display_dec("res", fix_x);
        printf("\n\npos: " U64P() "", pos * 2);

        exit(EXIT_SUCCESS);
    }

    fix_display_dec("res", fix_x);

    fix_free(fix_x);
    num_free(num);
}



// int main(int argc, char** argv)
int main()
{
    setbuf(stdout, NULL);
    srand(time(NULL));

    // num_generate(21, 2);
    time_1(14, 17);
    // time_2(argc, argv, 19);
    // fibonacci_2(16, 23);
    // fibonacci();
    // sqrt_2();

    // uint64_t pos = 1e1;
    // fix_t fix = fix_wrap(1, pos);
    // fix = fix_div(fix, fix_wrap(10, pos));
    // fix = fix_mul(fix, fix_wrap(10, pos));
    // fix = fix_sub(fix, fix_wrap(1, pos));
    //
    // uint64_t begin = altutime();
    // fix_display_tag("res", fix);
    // uint64_t end = altutime();
    // printf("\n%10.3f", (end - begin) / 1e3);
    //
    // printf("\na");
    // num_t num = num_sub(num_wrap(2), num_wrap(3));
    // printf("\nb");
    // num_display(num);

    // num_t num = num_wrap(29);
    // mod_t mod_1 = mod_wrap(1, num);
    // mod_t mod_2 = mod_wrap(9, num);
    // mod_1 = mod_div(mod_1, mod_2);
    // mod_display("mod", mod_1);

    // assert(clu_mem_is_empty());

    printf("\n");
    return 0;
}
