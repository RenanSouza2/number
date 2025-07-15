#include "../debug.h"
#include "../../../testrc.h"
#include "../../../mods/macros/test.h"



void test_uint_from_char(bool show)
{
    TEST_FN_OPEN

    #define TEST_UINT_FROM_CHAR(TAG, CHAR, UINT)    \
    {                                               \
        TEST_CASE_OPEN(TAG)                         \
        {                                           \
            uint64_t res = uint_from_char(CHAR);    \
            assert(uint64(res, UINT));              \
        }                                           \
        TEST_CASE_CLOSE                             \
    }

    TEST_UINT_FROM_CHAR(1, '0',  0);
    TEST_UINT_FROM_CHAR(2, '1',  1);
    TEST_UINT_FROM_CHAR(3, '9',  9);
    TEST_UINT_FROM_CHAR(4, 'a', 10);
    TEST_UINT_FROM_CHAR(5, 'b', 11);
    TEST_UINT_FROM_CHAR(6, 'f', 15);
    TEST_UINT_FROM_CHAR(7, 'A', 10);
    TEST_UINT_FROM_CHAR(8, 'B', 11);
    TEST_UINT_FROM_CHAR(9, 'F', 15);

    #undef TEST_UINT_FROM_CHAR

    TEST_CASE_OPEN(10)
    {
        TEST_REVERT_OPEN
        {
            uint_from_char('g');
        }
        TEST_REVERT_CLOSE
    }
    TEST_CASE_CLOSE

    TEST_FN_CLOSE
}

void test_uint128(bool show)
{
    TEST_FN_OPEN

    TEST_CASE_OPEN(1)
    {
        uint128_t u = 1;
        assert(uint128_immed(u, 0, 1));
    }
    TEST_CASE_CLOSE

    TEST_CASE_OPEN(2)
    {
        uint128_t u = UINT64_MAX;
        assert(uint128_immed(u, 0, UINT64_MAX));
    }
    TEST_CASE_CLOSE

    TEST_CASE_OPEN(3)
    {
        uint128_t u = U128_IMMED(1, 0);
        assert(uint128_immed(u, 1, 0));
    }
    TEST_CASE_CLOSE

    TEST_CASE_OPEN(4)
    {
        uint128_t u = U128_IMMED(1, 2);
        assert(uint128_immed(u, 1, 2));
    }
    TEST_CASE_CLOSE

    TEST_CASE_OPEN(5)
    {
        uint128_t u = U128_IMMED(UINT64_MAX, UINT64_MAX);
        assert(uint128_immed(u, UINT64_MAX, UINT64_MAX));
    }
    TEST_CASE_CLOSE

    TEST_CASE_OPEN(6)
    {
        uint128_t u = UINT64_MAX;
        u += 1;
        assert(uint128_immed(u, 1, 0));
    }
    TEST_CASE_CLOSE

    TEST_CASE_OPEN(7)
    {
        uint128_t u = U128_IMMED(1, 0);
        u -= 1;
        assert(uint128_immed(u, 0, UINT64_MAX));
    }
    TEST_CASE_CLOSE

    TEST_CASE_OPEN(8)
    {
        uint128_t u = UINT64_MAX;
        u *= UINT64_MAX;
        assert(uint128_immed(u, UINT64_MAX - 1, 1));
    }
    TEST_CASE_CLOSE

    TEST_CASE_OPEN(9)
    {
        uint128_t u = U128_IMMED(1, 0);
        u <<= 1;
        assert(uint128_immed(u, 2, 0));
    }
    TEST_CASE_CLOSE

    TEST_CASE_OPEN(10)
    {
        uint128_t u = U128_IMMED(1, 0);
        u >>= 1;
        assert(uint128_immed(u, 0, 0x8000000000000000));
    }
    TEST_CASE_CLOSE

    TEST_CASE_OPEN(11)
    {
        uint128_t u = U128_IMMED(1, 0);
        u /= 1;
        assert(uint128_immed(u, 1, 0));
    }
    TEST_CASE_CLOSE

    TEST_CASE_OPEN(12)
    {
        uint128_t u = U128_IMMED(1, 0);
        u /= 2;
        assert(uint128_immed(u, 0, 0x8000000000000000));
    }
    TEST_CASE_CLOSE

    TEST_CASE_OPEN(13)
    {
        uint128_t u = U128_IMMED(UINT64_MAX, UINT64_MAX);
        u /= 1;
        assert(uint128_immed(u, UINT64_MAX, UINT64_MAX));
    }
    TEST_CASE_CLOSE

    TEST_CASE_OPEN(14)
    {
        uint128_t u = U128_IMMED(UINT64_MAX / 2, UINT64_MAX);
        u /= 2;
        assert(uint128_immed(u, UINT64_MAX >> 2, UINT64_MAX));
    }
    TEST_CASE_CLOSE

    TEST_CASE_OPEN(15)
    {
        uint128_t u = U128_IMMED(UINT64_MAX, UINT64_MAX - 1);
        u /= 2;
        assert(uint128_immed(u, UINT64_MAX >> 1, UINT64_MAX));
    }
    TEST_CASE_CLOSE

    TEST_FN_CLOSE
}





void test_num_create(bool show)
{
    TEST_FN_OPEN

    #define TEST_NUM_CREATE(TAG, SIZE, COUNT, res_size) \
    {                                                   \
        TEST_CASE_OPEN(TAG)                             \
        {                                               \
            num_p num = num_create(SIZE, COUNT);        \
            assert(uint64(num->size, res_size));        \
            assert(uint64(num->count, COUNT));          \
            assert(num->chunk != NULL);                 \
            num_free(num);                              \
        }                                               \
        TEST_CASE_CLOSE                                 \
    }

    TEST_NUM_CREATE(1, 0, 0, 1);
    TEST_NUM_CREATE(2, 1, 1, 1);
    TEST_NUM_CREATE(3, 2, 2, 2);
    TEST_NUM_CREATE(4, 3, 2, 3);

    #undef TEST_NUM_CREATE

    TEST_CASE_OPEN(5)
    {
        TEST_REVERT_OPEN
        {
            num_create(0, 1);
        }
        TEST_REVERT_CLOSE
    }
    TEST_CASE_CLOSE

    TEST_FN_CLOSE
}

void test_num_expand_to(bool show)
{
    TEST_FN_OPEN

    #define TEST_NUM_EXPAND_TO(TAG, NUM, TARGET, SIZE, RES) \
    {                                                       \
        TEST_CASE_OPEN(TAG)                                 \
        {                                                   \
            num_p num = num_create_immed(ARG_OPEN NUM);     \
            num = num_expand_to(num, TARGET);               \
            assert(uint64(num->size, SIZE));                \
            assert(num_immed(num, ARG_OPEN RES));           \
        }                                                   \
        TEST_CASE_CLOSE                                     \
    }

    TEST_NUM_EXPAND_TO(1, (0), 2, 4, (0));
    TEST_NUM_EXPAND_TO(1, (2, 1, 2), 2, 4, (2, 1, 2));
    TEST_NUM_EXPAND_TO(1, (2, 1, 2), 10, 20, (2, 1, 2));

    #undef TEST_NUM_EXPAND_TO

    TEST_FN_CLOSE
}

void test_num_chunk_get(bool show)
{
    TEST_FN_OPEN

    #define TEST_NUM_CHUNK_GET(TAG, NUM, POS, RES)      \
    {                                                   \
        TEST_CASE_OPEN(TAG)                             \
        {                                               \
            num_p num = num_create_immed(ARG_OPEN NUM); \
            uint64_t value = num_chunk_get(num, POS);   \
            assert(uint64(value, RES));                 \
            num_free(num);                              \
        }                                               \
        TEST_CASE_CLOSE                                 \
    }

    TEST_NUM_CHUNK_GET(1, (0), 0, 0);
    TEST_NUM_CHUNK_GET(2, (0), 1, 0);
    TEST_NUM_CHUNK_GET(3, (2, 1, 2), 0, 2);
    TEST_NUM_CHUNK_GET(4, (2, 1, 2), 1, 1);
    TEST_NUM_CHUNK_GET(5, (2, 1, 2), 2, 0);

    #undef TEST_NUM_CHUNK_GET

    TEST_FN_CLOSE
}

void test_num_chunk_set(bool show)
{
    TEST_FN_OPEN

    #define TEST_NUM_CHUNK_SET(TAG, NUM, POS, VALUE, RES)   \
    {                                                       \
        TEST_CASE_OPEN(TAG)                                 \
        {                                                   \
            num_p num = num_create_immed(ARG_OPEN NUM);     \
            num = num_chunk_set(num, POS, VALUE);           \
            assert(num_immed(num, ARG_OPEN RES));           \
        }                                                   \
        TEST_CASE_CLOSE                                     \
    }

    TEST_NUM_CHUNK_SET(1, (0), 0, 0, (0));
    TEST_NUM_CHUNK_SET(2, (0), 0, 1, (1, 1));
    TEST_NUM_CHUNK_SET(3, (1, 1), 0, 2, (1, 2));
    // TEST_NUM_CHUNK_SET(4, (1, 1), 5, 2, (6, 2, 0, 0, 0, 0, 1));
    // TEST_NUM_CHUNK_SET(5, (1, 1), 5, 0, (1, 1));

    #undef TEST_NUM_CHUNK_SET

    TEST_FN_CLOSE
}

void test_num_normalize(bool show)
{
    TEST_FN_OPEN

    #define TEST_NUM_NORMALIZE(TAG, NUM_BEF, RES, NUM_AFT)  \
    {                                                       \
        TEST_CASE_OPEN(TAG)                                 \
        {                                                   \
            num_p num = num_create_immed(ARG_OPEN NUM_BEF); \
            bool res = num_normalize(num);                  \
            assert(res == RES);                             \
            assert(num_immed(num, ARG_OPEN NUM_AFT));       \
        }                                                   \
        TEST_CASE_CLOSE                                     \
    }

    TEST_NUM_NORMALIZE(1,
        (0),
        false,
        (0)
    );
    TEST_NUM_NORMALIZE(2,
        (1, 1),
        false,
        (1, 1)
    );
    TEST_NUM_NORMALIZE(3,
        (1, 0),
        true,
        (0)
    );
    TEST_NUM_NORMALIZE(4,
        (2, 0, 1),
        true,
        (1, 1)
    );
    TEST_NUM_NORMALIZE(5,
        (2, 0, 0),
        true,
        (1, 0)
    );

    #undef TEST_NUM_NORMALIZE

    TEST_FN_CLOSE
}

void test_num_break(bool show)
{
    TEST_FN_OPEN

    #define TEST_NUM_BREAK(TAG, NUM, COUNT, NUM_HI, NUM_LO)             \
    {                                                                   \
        TEST_CASE_OPEN(TAG)                                             \
        {                                                               \
            num_p num_hi, num_lo, num = num_create_immed(ARG_OPEN NUM); \
            num_break(&num_hi, &num_lo, num, COUNT);                    \
            assert(num_immed(num_hi, ARG_OPEN NUM_HI));                 \
            assert(num_immed(num_lo, ARG_OPEN NUM_LO));                 \
        }                                                               \
        TEST_CASE_CLOSE                                                 \
    }

    TEST_NUM_BREAK(1,
        (0),
        0,
        (0),
        (0)
    );
    TEST_NUM_BREAK(2,
        (0),
        1,
        (0),
        (0)
    );
    TEST_NUM_BREAK(3,
        (1, 1),
        0,
        (1, 1),
        (0)
    );
    TEST_NUM_BREAK(4,
        (1, 1),
        1,
        (0),
        (1, 1)
    );
    TEST_NUM_BREAK(5,
        (1, 1),
        2,
        (0),
        (1, 1)
    );
    TEST_NUM_BREAK(6,
        (2, 1, 2),
        0,
        (2, 1, 2),
        (0)
    );
    TEST_NUM_BREAK(7,
        (2, 1, 2),
        1,
        (1, 1),
        (1, 2)
    );
    TEST_NUM_BREAK(8,
        (2, 1, 2),
        2,
        (0),
        (2, 1, 2)
    );
    TEST_NUM_BREAK(9,
        (2, 1, 2),
        3,
        (0),
        (2, 1, 2)
    );
    TEST_NUM_BREAK(10,
        (3, 1, 0, 2),
        2,
        (1, 1),
        (1, 2)
    );
    TEST_NUM_BREAK(11,
        (3, 1, 0, 0),
        2,
        (1, 1),
        (0)
    );

    #undef TEST_NUM_BREAK

    TEST_FN_CLOSE
}



void test_num_wrap(bool show)
{
    TEST_FN_OPEN

    #define TEST_NUM_WRAP(TAG, NUM, ...)            \
    {                                               \
        TEST_CASE_OPEN(TAG)                         \
        {                                           \
            num_p num = num_wrap(NUM);              \
            assert(num_immed(num, __VA_ARGS__));    \
        }                                           \
        TEST_CASE_CLOSE                             \
    }

    TEST_NUM_WRAP(1, 0, 0);
    TEST_NUM_WRAP(2, 2, 1, 2);
    TEST_NUM_WRAP(3, UINT64_MAX, 1, UINT64_MAX);

    #undef TEST_NUM_WRAP

    TEST_FN_CLOSE
}

void test_num_wrap_dec(bool show)
{
    TEST_FN_OPEN

    #define TEST_NUM_WRAP_DEC(TAG, STR, ...)        \
    {                                               \
        TEST_CASE_OPEN(TAG)                         \
        {                                           \
            num_p num = num_wrap_dec(STR);          \
            assert(num_immed(num, __VA_ARGS__));    \
        }                                           \
        TEST_CASE_CLOSE                             \
    }

    TEST_NUM_WRAP_DEC(1, "0", 0);
    TEST_NUM_WRAP_DEC(2, "1", 1, 1);
    TEST_NUM_WRAP_DEC(3, "9", 1, 9);
    TEST_NUM_WRAP_DEC(4, "10", 1, 10);
    TEST_NUM_WRAP_DEC(5, "18446744073709551615", 1, UINT64_MAX);
    TEST_NUM_WRAP_DEC(6, "18446744073709551616", 2, 1, 0);
    TEST_NUM_WRAP_DEC(7, "0000", 0);
    TEST_NUM_WRAP_DEC(8, "00001", 1, 1);
    TEST_NUM_WRAP_DEC(9, "", 0);

    #undef TEST_NUM_WRAP_DEC

    TEST_FN_CLOSE
}

void test_num_wrap_hex(bool show)
{
    TEST_FN_OPEN

    #define TEST_NUM_WRAP_HEX(TAG, STR, ...)        \
    {                                               \
        TEST_CASE_OPEN(TAG)                         \
        {                                           \
            num_p num = num_wrap_hex(STR);          \
            assert(num_immed(num, __VA_ARGS__));    \
        }                                           \
        TEST_CASE_CLOSE                             \
    }

    TEST_NUM_WRAP_HEX( 1, "0x0", 0);
    TEST_NUM_WRAP_HEX( 2, "0x1", 1, 1);
    TEST_NUM_WRAP_HEX( 3, "0x9", 1, 9);
    TEST_NUM_WRAP_HEX( 4, "0xa", 1, 10);
    TEST_NUM_WRAP_HEX( 5, "0xA", 1, 10);
    TEST_NUM_WRAP_HEX( 6, "0x10", 1, 16);
    TEST_NUM_WRAP_HEX( 7, "0xffffffffffffffff", 1, UINT64_MAX);
    TEST_NUM_WRAP_HEX( 8, "0x10000000000000000", 2, 1, 0);
    TEST_NUM_WRAP_HEX( 9, "0x0000", 0);
    TEST_NUM_WRAP_HEX(10, "0x00001", 1, 1);
    TEST_NUM_WRAP_HEX(11, "0x", 0);

    #undef TEST_NUM_WRAP_HEX

    #define TEST_NUM_WRAP_HEX(TAG, STR) \
    {                                   \
        TEST_CASE_OPEN(TAG)             \
        {                               \
            TEST_REVERT_OPEN            \
            {                           \
                num_wrap_hex(STR);      \
            }                           \
            TEST_REVERT_CLOSE           \
        }                               \
        TEST_CASE_CLOSE                 \
    }

    TEST_NUM_WRAP_HEX(12, "");
    TEST_NUM_WRAP_HEX(13, "0");
    TEST_NUM_WRAP_HEX(14, "ab");
    TEST_NUM_WRAP_HEX(14, "0b");

    #undef TEST_NUM_WRAP_HEX

    TEST_FN_CLOSE
}

void test_num_wrap_str(bool show)
{
    TEST_FN_OPEN

    #define TEST_NUM_WRAP_STR(TAG, STR, ...)        \
    {                                               \
        TEST_CASE_OPEN(TAG)                         \
        {                                           \
            num_p num = num_wrap_str(STR);          \
            assert(num_immed(num, __VA_ARGS__));    \
        }                                           \
        TEST_CASE_CLOSE                             \
    }

    TEST_NUM_WRAP_STR( 1, "0", 0);
    TEST_NUM_WRAP_STR( 2, "1", 1, 1);
    TEST_NUM_WRAP_STR( 3, "9", 1, 9);
    TEST_NUM_WRAP_STR( 4, "10", 1, 10);
    TEST_NUM_WRAP_STR( 5, "18446744073709551615", 1, UINT64_MAX);
    TEST_NUM_WRAP_STR( 6, "18446744073709551616", 2, 1, 0);
    TEST_NUM_WRAP_STR( 7, "0000", 0);
    TEST_NUM_WRAP_STR( 8, "00001", 1, 1);
    TEST_NUM_WRAP_STR( 9, "0x0", 0);
    TEST_NUM_WRAP_STR(10, "0x1", 1, 1);
    TEST_NUM_WRAP_STR(11, "0x9", 1, 9);
    TEST_NUM_WRAP_STR(12, "0xa", 1, 10);
    TEST_NUM_WRAP_STR(13, "0xA", 1, 10);
    TEST_NUM_WRAP_STR(14, "0x10", 1, 16);
    TEST_NUM_WRAP_STR(15, "0xffffffffffffffff", 1, UINT64_MAX);
    TEST_NUM_WRAP_STR(16, "0x10000000000000000", 2, 1, 0);
    TEST_NUM_WRAP_STR(17, "0x0000", 0);
    TEST_NUM_WRAP_STR(18, "0x00001", 1, 1);

    #undef TEST_NUM_WRAP_STR

    TEST_FN_CLOSE
}

void test_num_read_dec(bool show)
{
    TEST_FN_OPEN

    #define TEST_NUM_READ_DEC(TAG, ...)                             \
    {                                                               \
        TEST_CASE_OPEN(TAG)                                         \
        {                                                           \
            num_p num = num_read_dec("numbers/num_" #TAG ".txt");   \
            assert(num_immed(num, __VA_ARGS__));                    \
        }                                                           \
        TEST_CASE_CLOSE                                             \
    }

    TEST_NUM_READ_DEC(1, 0);
    TEST_NUM_READ_DEC(2, 1, 1);
    TEST_NUM_READ_DEC(3, 1, 10);
    TEST_NUM_READ_DEC(4, 1, 12345678901234567);
    TEST_NUM_READ_DEC(5, 1, 123456789012345678);
    TEST_NUM_READ_DEC(6, 1, 1234567890123456789);
    TEST_NUM_READ_DEC(7, 2, 6, 0xb14e9f812f366c35);

    #undef TEST_NUM_READ_DEC

    TEST_CASE_OPEN(8)
    {
        TEST_REVERT_OPEN
        {
            num_read_dec("numbers/num_8.txt");
        }
        TEST_REVERT_CLOSE
    }
    TEST_CASE_CLOSE

    TEST_FN_CLOSE
}

void test_num_unwrap(bool show)
{
    TEST_FN_OPEN

    #define TEST_NUM_UNWRAP(TAG, RES, NUM)              \
    {                                                   \
        TEST_CASE_OPEN(TAG)                             \
        {                                               \
            num_p num = num_create_immed(ARG_OPEN NUM); \
            uint64_t res = num_unwrap(num);             \
            assert(res == RES);                         \
        }                                               \
        TEST_CASE_CLOSE                                 \
    }

    TEST_NUM_UNWRAP(1, 0, (0));
    TEST_NUM_UNWRAP(2, 1, (1, 1));
    TEST_NUM_UNWRAP(3, UINT64_MAX, (1, UINT64_MAX));

    #undef TEST_NUM_UNWRAP

    TEST_CASE_OPEN(4)
    {
        num_p num = num_create_immed(2, 3, 4);
        TEST_REVERT_OPEN
        {
            num_unwrap(num);
        }
        TEST_REVERT_CLOSE
    }
    TEST_CASE_CLOSE

    TEST_FN_CLOSE
}

void test_num_copy(bool show)
{
    TEST_FN_OPEN

    #define TEST_NUM_COPY(TAG, NUM)                     \
    {                                                   \
        TEST_CASE_OPEN(TAG)                             \
        {                                               \
            num_p num = num_create_immed(ARG_OPEN NUM); \
            num_p num_res = num_copy(num);              \
            assert(num_eq_dbg(num_res, num));           \
        }                                               \
        TEST_CASE_CLOSE                                 \
    }

    TEST_NUM_COPY(1, (0));
    TEST_NUM_COPY(2, (1, 1));
    TEST_NUM_COPY(3, (2, 1, 2));

    #undef TEST_NUM_COPY

    TEST_FN_CLOSE
}



void test_num_cmp_offset(bool show)
{
    TEST_FN_OPEN

    #define TEST_NUM_CMP(TAG, NUM_1, POS, RELATION, NUM_2)      \
    {                                                           \
        TEST_CASE_OPEN(TAG)                                     \
        {                                                       \
            num_p num_1 = num_create_immed(ARG_OPEN NUM_1);     \
            num_p num_2 = num_create_immed(ARG_OPEN NUM_2);     \
            int64_t cmp = num_cmp_offset(num_1, POS, num_2, 0); \
            assert(cmp RELATION 0);                             \
            num_free(num_1);                                    \
            num_free(num_2);                                    \
        }                                                       \
        TEST_CASE_CLOSE                                         \
    }

    TEST_NUM_CMP(1, (0), 0, ==, (0));
    TEST_NUM_CMP(2, (1, 1), 0, >, (0));
    TEST_NUM_CMP(3, (0), 0, <, (1, 1));
    TEST_NUM_CMP(4, (1, 1), 0, <, (1, 2));
    TEST_NUM_CMP(5, (1, 2), 0, ==, (1, 2));
    TEST_NUM_CMP(6, (1, 3), 0, >, (1, 2));
    TEST_NUM_CMP(7, (2, 2, 0), 0, >, (2, 1, 0));
    TEST_NUM_CMP(8, (2, 2, 0), 0, ==, (2, 2, 0));
    TEST_NUM_CMP(9, (2, 2, 0), 0, <, (2, 3, 0));
    TEST_NUM_CMP(10,
        (2, 0x8000000000000000, 0), 0,
        <,
        (2, 0x8000000000000000, UINT64_MAX >> 1)
    );
    TEST_NUM_CMP(11,
        (3, 0x8000000000000000, 0, 0), 0,
        <,
        (3, 0x8000000000000000, UINT64_MAX >> 1, 0x8000000000000000)
    );

    #undef TEST_NUM_CMP

    TEST_FN_CLOSE
}

void test_num_add_uint_offset(bool show)
{
    TEST_FN_OPEN

    #define TEST_NUM_ADD_UINT_OFFSET(TAG, NUM_BEF, OFFSET, VALUE, NUM_AFT)  \
    {                                                                       \
        TEST_CASE_OPEN(TAG)                                                 \
        {                                                                   \
            num_p num = num_create_immed(ARG_OPEN NUM_BEF);                 \
            num = num_add_uint_offset(num, OFFSET, VALUE);                  \
            assert(num_immed(num, ARG_OPEN NUM_AFT));                       \
        }                                                                   \
        TEST_CASE_CLOSE                                                     \
    }

    TEST_NUM_ADD_UINT_OFFSET(1, (0), 0, 0, (0));
    TEST_NUM_ADD_UINT_OFFSET(2, (0), 0, 1, (1, 1));
    TEST_NUM_ADD_UINT_OFFSET(3, (1, 1), 0, 2, (1, 3));
    TEST_NUM_ADD_UINT_OFFSET(4, (1, UINT64_MAX), 0, 3, (2, 1, 2));
    TEST_NUM_ADD_UINT_OFFSET(5, (0), 1, 0, (0));
    TEST_NUM_ADD_UINT_OFFSET(6, (0), 1, 1, (2, 1, 0));
    TEST_NUM_ADD_UINT_OFFSET(7, (1, 1), 1, 2, (2, 2, 1));
    TEST_NUM_ADD_UINT_OFFSET(8, (1, UINT64_MAX), 1, 3, (2, 3, UINT64_MAX));

    #undef TEST_NUM_ADD_UINT_OFFSET

    TEST_FN_CLOSE
}

void test_num_sub_uint_offset(bool show)
{
    TEST_FN_OPEN

    #define TEST_NUM_SUB_UINT_OFFSET(TAG, NUM, POS, VALUE, RES) \
    {                                                           \
        TEST_CASE_OPEN(TAG)                                     \
        {                                                       \
            num_p num = num_create_immed(ARG_OPEN NUM);         \
            num = num_sub_uint_offset(num, POS, VALUE);         \
            assert(num_immed(num, ARG_OPEN RES));               \
        }                                                       \
        TEST_CASE_CLOSE                                         \
    }

    TEST_NUM_SUB_UINT_OFFSET(1, (0), 0, 0, (0));
    TEST_NUM_SUB_UINT_OFFSET(2, (1, 1), 0, 0, (1, 1));
    TEST_NUM_SUB_UINT_OFFSET(3, (1, 1), 0, 1, (1, 0));
    TEST_NUM_SUB_UINT_OFFSET(4, (1, 2), 0, 1, (1, 1));
    TEST_NUM_SUB_UINT_OFFSET(5, (2, 1, 0), 0, 1, (2, 0, UINT64_MAX));
    TEST_NUM_SUB_UINT_OFFSET(6,
        (5, 1, 0, 0, 0, 0), 0,
        0x8000000000000000,
        (5, 0, UINT64_MAX, UINT64_MAX, UINT64_MAX, 0x8000000000000000)
    );
    TEST_NUM_SUB_UINT_OFFSET(7, (2, 2, 3), 0, 1, (2, 2, 2));
    TEST_NUM_SUB_UINT_OFFSET(8, (2, 2, 3), 1, 1, (2, 1, 3));
    TEST_NUM_SUB_UINT_OFFSET(9, (2, 1, 3), 1, 1, (2, 0, 3));
    TEST_NUM_SUB_UINT_OFFSET(10, (2, 1, 0), 1, 1, (2, 0, 0));
    TEST_NUM_SUB_UINT_OFFSET(11, (3, 1, 0, 1), 2, 1, (3, 0, 0, 1));
    TEST_NUM_SUB_UINT_OFFSET(12, (3, 1, 0, 1), 1, 1, (3, 0, UINT64_MAX, 1));
    TEST_NUM_SUB_UINT_OFFSET(13, (2, 1, 0), 1, 1, (2, 0, 0));
    TEST_NUM_SUB_UINT_OFFSET(14, (0), 0, 0, (0));

    #undef TEST_NUM_SUB_UINT_OFFSET

    #define TEST_NUM_SUB_UINT_OFFSET(TAG, NUM, POS, VALUE)  \
    {                                                       \
        TEST_CASE_OPEN(TAG)                                 \
        {                                                   \
            num_p num = num_create_immed(ARG_OPEN NUM);     \
            TEST_REVERT_OPEN                                \
            {                                               \
                num_sub_uint_offset(num, POS, VALUE);       \
            }                                               \
            TEST_REVERT_CLOSE                               \
        }                                                   \
        TEST_CASE_CLOSE                                     \
    }

    TEST_NUM_SUB_UINT_OFFSET(9, (0), 0, 1);
    TEST_NUM_SUB_UINT_OFFSET(10, (1, 2), 0, 3);
    TEST_NUM_SUB_UINT_OFFSET(11, (1, 2), 1, 1);

    #undef TEST_NUM_SUB_UINT_OFFSET

    TEST_FN_CLOSE
}

void test_num_sub_offset(bool show)
{
    TEST_FN_OPEN

    #define TEST_NUM_SUB_OFFSET(TAG, NUM_1, POS, NUM_2, RES)    \
    {                                                           \
        TEST_CASE_OPEN(TAG)                                     \
        {                                                       \
            num_p num_1 = num_create_immed(ARG_OPEN NUM_1);     \
            num_p num_2 = num_create_immed(ARG_OPEN NUM_2);     \
            num_1 = num_sub_offset(num_1, POS, num_2);          \
            assert(num_immed(num_1, ARG_OPEN RES));             \
            num_free(num_2);                                    \
        }                                                       \
        TEST_CASE_CLOSE                                         \
    }

    TEST_NUM_SUB_OFFSET(1, (0), 0, (0), (0));
    TEST_NUM_SUB_OFFSET(2, (0), 0, (0), (0));
    TEST_NUM_SUB_OFFSET(3, (2, 1, 2), 1, (1, 1), (1, 2));
    TEST_NUM_SUB_OFFSET(4, (2, 1, 0), 1, (1, 1), (1, 0));
    TEST_NUM_SUB_OFFSET(5, (3, 1, 2, 3), 1, (2, 1, 2), (1, 3));
    TEST_NUM_SUB_OFFSET(6, (3, 1, 2, 3), 1, (2, 1, 1), (2, 1, 3));

    #undef TEST_NUM_SUB_OFFSET

    #define TEST_NUM_SUB_OFFSET(TAG, NUM_1, POS, NUM_2)     \
    {                                                       \
        TEST_CASE_OPEN(TAG)                                 \
        {                                                   \
            num_p num_1 = num_create_immed(ARG_OPEN NUM_1); \
            num_p num_2 = num_create_immed(ARG_OPEN NUM_2); \
            TEST_REVERT_OPEN                                \
            {                                               \
                num_sub_offset(num_1, POS, num_2);         \
            }                                               \
            TEST_REVERT_CLOSE                               \
        }                                                   \
        TEST_CASE_CLOSE                                     \
    }

    TEST_NUM_SUB_OFFSET(7, (0), 0, (1, 1));
    TEST_NUM_SUB_OFFSET(8, (2, 2, 1), 1, (1, 3));
    TEST_NUM_SUB_OFFSET(9, (2, 2, 1), 1, (2, 1, 2));

    #undef TEST_NUM_SUB_OFFSET

    TEST_FN_CLOSE
}



void test_num_shl_uint(bool show)
{
    TEST_FN_OPEN

    #define TEST_NUM_SHL_UINT(TAG, NUM_BEF, BITS, NUM_AFT)  \
    {                                                       \
        TEST_CASE_OPEN(TAG)                                 \
        {                                                   \
            num_p num = num_create_immed(ARG_OPEN NUM_BEF); \
            num = num_shl_uint(num, BITS);                  \
            assert(num_immed(num, ARG_OPEN NUM_AFT));       \
        }                                                   \
        TEST_CASE_CLOSE                                     \
    }

    TEST_NUM_SHL_UINT(1, (0), 0, (0));
    TEST_NUM_SHL_UINT(2, (0), 63, (0));
    TEST_NUM_SHL_UINT(3, (1, 1), 1, (1, 2));
    TEST_NUM_SHL_UINT(4, (1, 1), 1, (1, 2));
    TEST_NUM_SHL_UINT(5, (1, 1), 63, (1, 0x8000000000000000));
    TEST_NUM_SHL_UINT(6, (1, 2), 63, (2, 1, 0));
    TEST_NUM_SHL_UINT(7, (2, 1, 2), 63, (2, 0x8000000000000001, 0));

    #undef TEST_NUM_SHL_UINT

    TEST_CASE_OPEN(8)
    {
        num_p num = num_create_immed(1, 1);
        TEST_REVERT_OPEN
        {
            num_shl_uint(num, 64);
        }
        TEST_REVERT_CLOSE
    }
    TEST_CASE_CLOSE

    TEST_FN_CLOSE
}

void test_num_shr_uint(bool show)
{
    TEST_FN_OPEN

    #define TEST_NUM_SHR_UINT(TAG, NUM_BEF, BITS, NUM_AFT)  \
    {                                                       \
        TEST_CASE_OPEN(TAG)                                 \
        {                                                   \
            num_p num = num_create_immed(ARG_OPEN NUM_BEF); \
            num = num_shr_uint(num, BITS);                  \
            assert(num_immed(num, ARG_OPEN NUM_AFT));       \
        }                                                   \
        TEST_CASE_CLOSE                                     \
    }

    TEST_NUM_SHR_UINT(1, (0), 1, (0));
    TEST_NUM_SHR_UINT(2, (1, 1), 0, (1, 1));
    TEST_NUM_SHR_UINT(3, (1, 1), 1, (0));
    TEST_NUM_SHR_UINT(4, (1, 2), 1, (1, 1));
    TEST_NUM_SHR_UINT(5, (1, UINT64_MAX), 63, (1, 1));
    TEST_NUM_SHR_UINT(6, (2, 1, 0x8000000000000000), 63, (1, 3));
    TEST_NUM_SHR_UINT(7,
        (4, 4, UINT64_MAX, UINT64_MAX, UINT64_MAX - 3),
        0,
        (4, 4, UINT64_MAX, UINT64_MAX, UINT64_MAX - 3)
    );

    #undef TEST_NUM_SHR_UINT

    TEST_CASE_OPEN(8)
    {
        num_p num = num_create_immed(1, 1);
        TEST_REVERT_OPEN
        {
            num_shr_uint(num, 64);
        }
        TEST_REVERT_CLOSE
    }
    TEST_CASE_CLOSE

    TEST_FN_CLOSE
}

void test_num_mul_uint(bool show)
{
    TEST_FN_OPEN

    #define TEST_NUM_MUL_UINT(TAG, NUM_BEF, VALUE, NUM_AFT) \
    {                                                       \
        TEST_CASE_OPEN(TAG)                                 \
        {                                                   \
            num_p num = num_create_immed(ARG_OPEN NUM_BEF); \
            num_p num_res = num_create(num->count + 1, 0);    \
            num_mul_uint(num_res, num, VALUE);       \
            assert(num_immed(num_res, ARG_OPEN NUM_AFT));   \
            num_free(num);                                  \
        }                                                   \
        TEST_CASE_CLOSE                                     \
    }

    TEST_NUM_MUL_UINT(1, (0), 0, (0));
    TEST_NUM_MUL_UINT(2, (0), 1, (0));
    TEST_NUM_MUL_UINT(3, (1, 2), 0, (0));
    TEST_NUM_MUL_UINT(4, (1, 2), 3, (1, 6));
    TEST_NUM_MUL_UINT(5, (1, 2), UINT64_MAX, (2, 1, UINT64_MAX - 1));
    TEST_NUM_MUL_UINT(6, (2, 2, 3), 4, (2, 8, 12));
    TEST_NUM_MUL_UINT(7, (2, UINT64_MAX, UINT64_MAX), UINT64_MAX, (3, UINT64_MAX - 1, UINT64_MAX, 1));
    TEST_NUM_MUL_UINT(8, (2, 2, 0), 3, (2, 6, 0));
    TEST_NUM_MUL_UINT(9, (2, 6, 0xea7db545decb57a4), 123427205323, (2, 0xc6bfba060d, 0xdf4c110fd2f33f0c));

    #undef TEST_NUM_MUL_UINT

    TEST_FN_CLOSE
}

void test_num_add_mul_uint(bool show)
{
    TEST_FN_OPEN

    #define TEST_NUM_ADD_MUL_UINT(TAG, NUM_BEF, VALUE, NUM_AFT) \
    {                                                           \
        TEST_CASE_OPEN(TAG)                                     \
        {                                                       \
            num_p num = num_create_immed(ARG_OPEN NUM_BEF);     \
            num_p num_res = num_create_immed(1, 1);             \
            num_res = num_add_mul_uint(num_res, num, VALUE);    \
            assert(num_immed(num_res, ARG_OPEN NUM_AFT));       \
            num_free(num);                                      \
        }                                                       \
        TEST_CASE_CLOSE                                         \
    }

    TEST_NUM_ADD_MUL_UINT(1, (0), 0, (1, 1));
    TEST_NUM_ADD_MUL_UINT(2, (0), 1, (1, 1));
    TEST_NUM_ADD_MUL_UINT(3, (1, 2), 0, (1, 1));
    TEST_NUM_ADD_MUL_UINT(4, (1, 2), 3, (1, 7));
    TEST_NUM_ADD_MUL_UINT(5, (1, 2), UINT64_MAX, (2, 1, UINT64_MAX));
    TEST_NUM_ADD_MUL_UINT(6, (2, 2, 3), 4, (2, 8, 13));
    TEST_NUM_ADD_MUL_UINT(7, (2, UINT64_MAX, UINT64_MAX), UINT64_MAX, (3, UINT64_MAX - 1, UINT64_MAX, 2));
    TEST_NUM_ADD_MUL_UINT(8, (2, 2, 0), 3, (2, 6, 1));
    TEST_NUM_ADD_MUL_UINT(9, (2, 6, 0xea7db545decb57a4), 123427205323, (2, 0xc6bfba060d, 0xdf4c110fd2f33f0d));

    #undef TEST_NUM_ADD_MUL_UINT

    TEST_FN_CLOSE
}



void test_num_pad(bool show)
{
    TEST_FN_OPEN

    #define TEST_NUM_PAD(TAG, NUM_BEF, NUM_AFT)             \
    {                                                       \
        TEST_CASE_OPEN(TAG)                                 \
        {                                                   \
            num_p num = num_create_immed(ARG_OPEN NUM_BEF); \
            num = num_pad(num);                             \
            assert(num_immed(num, ARG_OPEN NUM_AFT));       \
        }                                                   \
        TEST_CASE_CLOSE                                     \
    }

    TEST_NUM_PAD(1, (0), (0));
    TEST_NUM_PAD(2, (1, 1), (1, 1));
    TEST_NUM_PAD(3, (1, 0xddddccccbbbbaaaa), (4, 0xdddd, 0xcccc, 0xbbbb, 0xaaaa));
    TEST_NUM_PAD(4,
        (2, 0xddddccccbbbbaaaa, 0x4444333322221111),
        (8, 0xdddd, 0xcccc, 0xbbbb, 0xaaaa, 0x4444, 0x3333, 0x2222, 0x1111)
    );

    #undef TEST_NUM_PAD

    TEST_FN_CLOSE
}

void test_num_depad(bool show)
{
    TEST_FN_OPEN

    #define TEST_NUM_DEPAD(TAG, NUM_BEF, NUM_AFT)           \
    {                                                       \
        TEST_CASE_OPEN(TAG)                                 \
        {                                                   \
            num_p num = num_create_immed(ARG_OPEN NUM_BEF); \
            num = num_depad(num);                           \
            assert(num_immed(num, ARG_OPEN NUM_AFT));       \
        }                                                   \
        TEST_CASE_CLOSE                                     \
    }

    TEST_NUM_DEPAD(1, (0), (0));
    TEST_NUM_DEPAD(2, (1, 1), (1, 1));
    TEST_NUM_DEPAD(3, (4, 0xdddd, 0xcccc, 0xbbbb, 0xaaaa), (1, 0xddddccccbbbbaaaa));
    TEST_NUM_DEPAD(4,
        (8, 0xdddd, 0xcccc, 0xbbbb, 0xaaaa, 0x4444, 0x3333, 0x2222, 0x1111),
        (2, 0xddddccccbbbbaaaa, 0x4444333322221111)
    );
    TEST_NUM_DEPAD(5,
        (4, 0, 0, 0x22222222, 0x11111111),
        (1, 0x222233331111)
    );

    #undef TEST_NUM_DEPAD

    TEST_FN_CLOSE
}

void test_num_fft(bool show)
{
    TEST_FN_OPEN

    uint64_t p = 4179340454199820289;

    #define TEST_NUM_FFT(TAG, NUM_BEF, N, NUM_AFT)          \
    {                                                       \
        TEST_CASE_OPEN(TAG)                                 \
        {                                                   \
            num_p num = num_create_immed(ARG_OPEN NUM_BEF); \
            num = num_fft(num, N);                          \
            assert(num_immed(num, ARG_OPEN NUM_AFT));       \
        }                                                   \
        TEST_CASE_CLOSE                                     \
    }

    TEST_NUM_FFT(1, (1, 1), 1, (1, 1));
    TEST_NUM_FFT(2, (2, 1, 0), 2, (2, p - 1, 1));
    TEST_NUM_FFT(3, (2, 1, 2), 2, (2, 1, 3));
    TEST_NUM_FFT(4,
        (4, 0, 0, 3, 4), 4,
        (4, 0x221bf1403ca1e969, 1, 0x17e40ebfc35e16a0, 7)
    );

    #undef TEST_NUM_FFT

    TEST_FN_CLOSE
}

void test_num_fft_inv(bool show)
{
    TEST_FN_OPEN

    uint64_t p = 4179340454199820289;

    #define TEST_NUM_FFT_INV(TAG, NUM_BEF, N, NUM_AFT)      \
    {                                                       \
        TEST_CASE_OPEN(TAG)                                 \
        {                                                   \
            num_p num = num_create_immed(ARG_OPEN NUM_BEF); \
            num = num_fft_inv(num, N);                      \
            assert(num_immed(num, ARG_OPEN NUM_AFT));       \
        }                                                   \
        TEST_CASE_CLOSE                                     \
    }

    TEST_NUM_FFT_INV(1, (1, 1), 1, (1, 1));
    TEST_NUM_FFT_INV(2, (2, p - 1, 1), 2, (2, 1, 0));
    TEST_NUM_FFT_INV(3, (2, 1, 3), 2, (2, 1, 2));
    TEST_NUM_FFT_INV(4,
        (4, 0x221bf1403ca1e969, 1, 0x17e40ebfc35e16a0, 7), 4,
        (4, 0, 0, 3, 4)
    );

    #undef TEST_NUM_FFT_INV

    TEST_FN_CLOSE
}




void test_num_is_zero(bool show)
{
    TEST_FN_OPEN

    #define TEST_NUM_IS_ZERO(TAG, NUM, IS_ZERO)         \
    {                                                   \
        TEST_CASE_OPEN(TAG)                             \
        {                                               \
            num_p num = num_create_immed(ARG_OPEN NUM); \
            bool res = num_is_zero(num);                \
            assert(res == IS_ZERO);                     \
            num_free(num);                              \
        }                                               \
        TEST_CASE_CLOSE                                 \
    }

    TEST_NUM_IS_ZERO(1, (0), true);
    TEST_NUM_IS_ZERO(1, (1, 1), false);
    TEST_NUM_IS_ZERO(1, (2, 1, 2), false);

    #undef TEST_NUM_IS_ZERO

    TEST_FN_CLOSE
}



void test_num_shl(bool show)
{
    TEST_FN_OPEN

    #define TEST_NUM_SHL(TAG, NUM_IN, BITS, NUM_OUT)        \
    {                                                       \
        TEST_CASE_OPEN(TAG)                                 \
        {                                                   \
            num_p num = num_create_immed(ARG_OPEN NUM_IN);  \
            num = num_shl(num, BITS);                       \
            assert(num_immed(num, ARG_OPEN NUM_OUT));       \
        }                                                   \
        TEST_CASE_CLOSE                                     \
    }

    TEST_NUM_SHL(1, (0), 0, (0));
    TEST_NUM_SHL(2, (0), 1, (0));
    TEST_NUM_SHL(3, (0), 64, (0));
    TEST_NUM_SHL(4, (1, 1), 1, (1, 2));
    TEST_NUM_SHL(5, (1, 1), 64, (2, 1, 0));
    TEST_NUM_SHL(6, (1, 1), 65, (2, 2, 0));

    #undef TEST_NUM_SHL

    TEST_FN_CLOSE
}

void test_num_shr(bool show)
{
    TEST_FN_OPEN

    #define TEST_NUM_SHR(TAG, NUM_IN, BITS, NUM_OUT)        \
    {                                                       \
        TEST_CASE_OPEN(TAG)                                 \
        {                                                   \
            num_p num = num_create_immed(ARG_OPEN NUM_IN);  \
            num = num_shr(num, BITS);                       \
            assert(num_immed(num, ARG_OPEN NUM_OUT));       \
        }                                                   \
        TEST_CASE_CLOSE                                     \
    }

    TEST_NUM_SHR(1, (0), 0, (0));
    TEST_NUM_SHR(2, (0), 1, (0));
    TEST_NUM_SHR(3, (1, 1), 1, (0));
    TEST_NUM_SHR(4, (1, 2), 1, (1, 1));
    TEST_NUM_SHR(5, (2, 1, 0), 64, (1, 1));
    TEST_NUM_SHR(6, (2, 1, 0), 65, (0));
    TEST_NUM_SHR(7, (2, 2, 0), 65, (1, 1));

    #undef TEST_NUM_SHR

    TEST_FN_CLOSE
}




void test_num_add(bool show)
{
    TEST_FN_OPEN

    #define TEST_NUM_ADD(TAG, NUM_1, NUM_2, NUM_OUT)        \
    {                                                       \
        TEST_CASE_OPEN(TAG)                                 \
        {                                                   \
            num_p num_1 = num_create_immed(ARG_OPEN NUM_1); \
            num_p num_2 = num_create_immed(ARG_OPEN NUM_2); \
            num_1 = num_add(num_1, num_2);                  \
            assert(num_immed(num_1, ARG_OPEN NUM_OUT));     \
        }                                                   \
        TEST_CASE_CLOSE                                     \
    }

    TEST_NUM_ADD(1,
        (0),
        (0),
        (0)
    );
    TEST_NUM_ADD(2,
        (1, 1),
        (0),
        (1, 1)
    );
    TEST_NUM_ADD(3,
        (0),
        (1, 1),
        (1, 1)
    );
    TEST_NUM_ADD(4,
        (1, 1),
        (1, 2),
        (1, 3)
    );
    TEST_NUM_ADD(5,
        (2, 2, 1),
        (1, 2),
        (2, 2, 3)
    );
    TEST_NUM_ADD(6,
        (1, 1),
        (2, 2, 3),
        (2, 2, 4)
    );
    TEST_NUM_ADD(7,
        (2, UINT64_MAX, UINT64_MAX),
        (1, 1),
        (3, 1, 0, 0)
    );
    TEST_NUM_ADD(8,
        (1, 1),
        (2, UINT64_MAX, UINT64_MAX),
        (3, 1, 0, 0)
    );

    #undef TEST_NUM_ADD

    TEST_FN_CLOSE
}

void test_num_sub(bool show)
{
    TEST_FN_OPEN

    #define TEST_NUM_SUB(TAG, NUM_1, NUM_2, NUM_OUT)        \
    {                                                       \
        TEST_CASE_OPEN(TAG)                                 \
        {                                                   \
            num_p num_1 = num_create_immed(ARG_OPEN NUM_1); \
            num_p num_2 = num_create_immed(ARG_OPEN NUM_2); \
            num_1 = num_sub(num_1, num_2);                  \
            assert(num_immed(num_1, ARG_OPEN NUM_OUT));     \
        }                                                   \
        TEST_CASE_CLOSE                                     \
    }

    TEST_NUM_SUB(1,
        (0),
        (0),
        (0)
    );
    TEST_NUM_SUB(2,
        (1, 1),
        (0),
        (1, 1)
    );
    TEST_NUM_SUB(3,
        (1, 2),
        (1, 1),
        (1, 1)
    );
    TEST_NUM_SUB(4,
        (1, 2),
        (1, 2),
        (0)
    );
    TEST_NUM_SUB(5,
        (2, 1, 0),
        (1, 1),
        (1, UINT64_MAX)
    );
    TEST_NUM_SUB(6,
        (2, 4, 3),
        (2, 1, 2),
        (2, 3, 1)
    );
    TEST_NUM_SUB(7,
        (2, 1, 0),
        (2, 1, 0),
        (0)
    );
    TEST_NUM_SUB(8,
        (5, 1, 0, 0, 0, 0),
        (4, 0x8000000000000000, 0, 0, 0x8000000000000000),
        (4, UINT64_MAX >> 1, UINT64_MAX, UINT64_MAX, 0x8000000000000000)
    );

    #undef TEST_NUM_SUB

    TEST_FN_CLOSE
}

void test_num_mul(bool show)
{
    TEST_FN_OPEN

    #define TEST_NUM_MUL(TAG, NUM_1, NUM_2, NUM_OUT)        \
    {                                                       \
        TEST_CASE_OPEN(TAG)                                 \
        {                                                   \
            num_p num_1 = num_create_immed(ARG_OPEN NUM_1); \
            num_p num_2 = num_create_immed(ARG_OPEN NUM_2); \
            num_1 = num_mul(num_1, num_2);                  \
            assert(num_immed(num_1, ARG_OPEN NUM_OUT));     \
        }                                                   \
        TEST_CASE_CLOSE                                     \
    }

    TEST_NUM_MUL(1,
        (0),
        (0),
        (0)
    );
    TEST_NUM_MUL(2,
        (1, 1),
        (0),
        (0)
    );
    TEST_NUM_MUL(3,
        (0),
        (1, 1),
        (0)
    );
    TEST_NUM_MUL(4,
        (1, 2),
        (1, 3),
        (1, 6)
    );
    TEST_NUM_MUL(5,
        (1, UINT64_MAX),
        (1, UINT64_MAX),
        (2, UINT64_MAX - 1, 1)
    );
    TEST_NUM_MUL(6,
        (2, 2, 3),
        (1, 4),
        (2, 8, 12)
    );
    TEST_NUM_MUL(7,
        (1, 2),
        (2, 3, 4),
        (2, 6, 8)
    );
    TEST_NUM_MUL(8,
        (2, 2, 3),
        (2, 4, 5),
        (3, 8, 22, 15)
    );
    TEST_NUM_MUL(9,
        (2, 1, 0),
        (1, 1),
        (2, 1, 0)
    );
    TEST_NUM_MUL(10,
        (1, 1),
        (2, 1, 0),
        (2, 1, 0)
    );
    TEST_NUM_MUL(11,
        (2, 1, 0),
        (2, 1, 0),
        (3, 1, 0, 0)
    );
    TEST_NUM_MUL(12,
        (1, 2),
        (2, 2, UINT64_MAX),
        (2, 5, UINT64_MAX - 1)
    );
    TEST_NUM_MUL(13,
        (2, UINT64_MAX, UINT64_MAX),
        (2, UINT64_MAX, UINT64_MAX),
        (4, UINT64_MAX, UINT64_MAX - 1, 0, 1)
    );
    TEST_NUM_MUL(14,
        (3, 2, 3, 4),
        (3, 5, 6, 7),
        (5, 10, 27, 52, 45, 28)
    );
    TEST_NUM_MUL(14,
        (3, 2, 3, 4),
        (3, 5, 6, 7),
        (5, 10, 27, 52, 45, 28)
    );

    #undef TEST_NUM_MUL

    TEST_FN_CLOSE
}

void test_num_sqr(bool show)
{
    TEST_FN_OPEN

    #define TEST_NUM_SQR(TAG, NUM_IN, NUM_OUT)              \
    {                                                       \
        TEST_CASE_OPEN(TAG)                                 \
        {                                                   \
            num_p num = num_create_immed(ARG_OPEN NUM_IN);  \
            num = num_sqr(num);                             \
            assert(num_immed(num, ARG_OPEN NUM_OUT));       \
        }                                                   \
        TEST_CASE_CLOSE                                     \
    }

    TEST_NUM_SQR(1,
        (0),
        (0)
    );
    TEST_NUM_SQR(2,
        (1, 1),
        (1, 1)
    );
    TEST_NUM_SQR(3,
        (1, 2),
        (1, 4)
    );
    TEST_NUM_SQR(4,
        (1, (uint64_t)(UINT32_MAX)),
        (1, 0xfffffffe00000001)
    );
    TEST_NUM_SQR(5,
        (1, (uint64_t)1 << 32),
        (2, 1, 0)
    );
    TEST_NUM_SQR(6,
        (2, 1, 0),
        (3, 1, 0, 0)
    );
    TEST_NUM_SQR(7,
        (2, 2, 3),
        (3, 4, 12, 9)
    );
    TEST_NUM_SQR(8,
        (2, UINT64_MAX, UINT64_MAX),
        (4, UINT64_MAX, UINT64_MAX - 1, 0, 1)
    );

    #undef TEST_NUM_SQR

    TEST_FN_CLOSE
}

void test_num_div_mod(bool show)
{
    TEST_FN_OPEN

    #define TEST_NUM_DIV_MOD(TAG, NUM_1, NUM_2, NUM_Q, NUM_R)   \
    {                                                           \
        TEST_CASE_OPEN(TAG)                                     \
        {                                                       \
            num_p num_q, num_r;                                 \
            num_p num_1 = num_create_immed(ARG_OPEN NUM_1);     \
            num_p num_2 = num_create_immed(ARG_OPEN NUM_2);     \
            num_div_mod(&num_q, &num_r, num_1, num_2);          \
            assert(num_immed(num_q, ARG_OPEN NUM_Q));           \
            assert(num_immed(num_r, ARG_OPEN NUM_R));           \
        }                                                       \
        TEST_CASE_CLOSE                                         \
    }

    TEST_NUM_DIV_MOD(1,
        (0),
        (1, 1),
        (0),
        (0)
    );
    TEST_NUM_DIV_MOD(2,
        (1, 4),
        (1, 2),
        (1, 2),
        (0)
    );
    TEST_NUM_DIV_MOD(3,
        (1, 5),
        (1, 2),
        (1, 2),
        (1, 1)
    );
    TEST_NUM_DIV_MOD(4,
        (1, 5),
        (1, 5),
        (1, 1),
        (0)
    );
    TEST_NUM_DIV_MOD(5,
        (1, 9),
        (1, 3),
        (1, 3),
        (0)
    );
    TEST_NUM_DIV_MOD(6,
        (3, 1, 0, 0),
        (2, 1, 0),
        (2, 1, 0),
        (0)
    );
    TEST_NUM_DIV_MOD(7,
        (1, 1),
        (2, 1, 0),
        (0),
        (1, 1)
    );
    TEST_NUM_DIV_MOD(8,
        (2, 4, UINT64_MAX),
        (2, 2, 0),
        (1, 2),
        (1, UINT64_MAX)
    );
    TEST_NUM_DIV_MOD(9,
        (2, 4, 0),
        (2, 2, UINT64_MAX),
        (1, 1),
        (2, 1, 1)
    );
    TEST_NUM_DIV_MOD(10,
        (2, 1, 0),
        (1, UINT64_MAX),
        (1, 1),
        (1, 1)
    );
    TEST_NUM_DIV_MOD(11,
        (2, UINT64_MAX, 0),
        (2, 1, UINT64_MAX),
        (1, UINT64_MAX >> 1),
        (2, 1, UINT64_MAX >> 1)
    );
    TEST_NUM_DIV_MOD(12,
        (2, 0xc929d7d593, 0xb7090a859117cfa4),
        (2, 6, 0xea7db545decb57a4),
        (1, 0x0000001d1635b735),
        (1, 0x88c80995d8646eb0)
    );
    TEST_NUM_DIV_MOD(13,
        (3, UINT64_MAX, 0, UINT64_MAX),
        (1, UINT64_MAX),
        (3, 1, 0, 1),
        (0)
    );
    TEST_NUM_DIV_MOD(14,
        (5, UINT64_MAX, 0, 0, 0, UINT64_MAX),
        (1, UINT64_MAX),
        (5, 1, 0, 0, 0, 1),
        (0)
    );
    TEST_NUM_DIV_MOD(15,
        (6, 4, 0, 8, 4, 0, 0),
        (4, 4, 0, 0, 4),
        (3, 1, 0, 1),
        (4, 3, UINT64_MAX, UINT64_MAX, UINT64_MAX - 3)
    );
    TEST_NUM_DIV_MOD(16,
        (3, 1, 1, 0),
        (2, 1, 2),
        (1, UINT64_MAX),
        (1, 2)
    );
    TEST_NUM_DIV_MOD(17,
        (3, UINT64_MAX, UINT64_MAX, UINT64_MAX),
        (2, UINT64_MAX, UINT64_MAX),
        (2, 1, 0),
        (1, UINT64_MAX)
    );
    TEST_NUM_DIV_MOD(18,
        (3, 1, 0, 0),
        (2, 1, UINT64_MAX),
        (1, 0x8000000000000000),
        (1, 0x8000000000000000)
    );
    TEST_NUM_DIV_MOD(19,
        (4, 1, 0, 0, 0),
        (3, 1, 0, UINT64_MAX),
        (1, UINT64_MAX),
        (2, 1, UINT64_MAX)
    );
    TEST_NUM_DIV_MOD(20,
        (3, 1, UINT64_MAX - 1, 0),
        (2, 1, UINT64_MAX),
        (1, UINT64_MAX),
        (1, UINT64_MAX)
    );
    TEST_NUM_DIV_MOD(21,
        (4, 1, UINT64_MAX, UINT64_MAX - 1, 0),
        (3, 1, UINT64_MAX, UINT64_MAX),
        (1, UINT64_MAX),
        (3, 1, UINT64_MAX - 1, UINT64_MAX)
    );
    TEST_NUM_DIV_MOD(22,
        (3, UINT64_MAX, 0, 0),
        (2, UINT64_MAX, UINT64_MAX),
        (1, UINT64_MAX),
        (1, UINT64_MAX)
    );
    TEST_NUM_DIV_MOD(23,
        (3, 2, 0, 0),
        (2, 2, UINT64_MAX),
        (1, 0xAAAAAAAAAAAAAAAA),
        (2, 2, 0xAAAAAAAAAAAAAAAA)
    );
    TEST_NUM_DIV_MOD(24,
        (5, 1, 0, 0, 0, 0),
        (4, 0x8000000000000000, 0, 0, 0x8000000000000000),
        (1, 1),
        (4, UINT64_MAX >> 1, UINT64_MAX, UINT64_MAX, 0x8000000000000000)
    );
    TEST_NUM_DIV_MOD(25,
        (6, 0x8000000000000000, 1, 0, 0x8000000000000000, 0, 0),
        (4, 0x8000000000000000, 0, 0, 0x8000000000000000),
        (3, 1, 0, 1),
        (4, UINT64_MAX >> 1, UINT64_MAX, UINT64_MAX, 0x8000000000000000)
    );

    #undef TEST_NUM_DIV_MOD

    TEST_CASE_OPEN(25)
    {
        num_p num_1 = num_create_immed(1, 1);
        num_p num_2 = num_create_immed(0);
        num_p num_q, num_r;
        TEST_REVERT_OPEN
        {
            num_div_mod(&num_q, &num_r, num_1, num_2);
        }
        TEST_REVERT_CLOSE
    }
    TEST_CASE_CLOSE

    TEST_FN_CLOSE
}

void test_num_gcd(bool show)
{
    TEST_FN_OPEN

    #define TEST_NUM_GDC(TAG, NUM_1, NUM_2, NUM_OUT)        \
    {                                                       \
        TEST_CASE_OPEN(TAG)                                 \
        {                                                   \
            num_p num_1 = num_create_immed(ARG_OPEN NUM_1); \
            num_p num_2 = num_create_immed(ARG_OPEN NUM_2); \
            num_1 = num_gcd(num_1, num_2);                  \
            assert(num_immed(num_1, ARG_OPEN NUM_OUT));     \
        }                                                   \
        TEST_CASE_CLOSE                                     \
    }

    TEST_NUM_GDC(1,
        (1, 3),
        (1, 2),
        (1, 1)
    );
    TEST_NUM_GDC(2,
        (1, 4),
        (1, 2),
        (1, 2)
    );
    TEST_NUM_GDC(3,
        (1, 6),
        (1, 4),
        (1, 2)
    );

    #undef TEST_NUM_GDC

    TEST_FN_CLOSE
}



void test_num_base_to(bool show)
{
    TEST_FN_OPEN

#define TEST_NUM_BASE_TO(TAG, NUM_IN, NUM_OUT)          \
{                                                       \
    TEST_CASE_OPEN(TAG)                                 \
    {                                                   \
        num_p num = num_create_immed(ARG_OPEN NUM_IN);  \
        num = num_base_to(num, 10);                     \
        assert(num_immed(num, ARG_OPEN NUM_OUT));       \
    }                                                   \
    TEST_CASE_CLOSE                                     \
}

    TEST_NUM_BASE_TO(1,
        (0),
        (0)
    );
    TEST_NUM_BASE_TO(2,
        (1, 1),
        (1, 1)
    );
    TEST_NUM_BASE_TO(3,
        (1, 9),
        (1, 9)
    );
    TEST_NUM_BASE_TO(4,
        (1, 10),
        (2, 1, 0)
    );
    TEST_NUM_BASE_TO(5,
        (1, 100),
        (3, 1, 0, 0)
    );
    TEST_NUM_BASE_TO(6,
        (1, 99),
        (2, 9, 9)
    );
    TEST_NUM_BASE_TO(7,
        (2, 5, 0x6bc75e2d63100000),
        (21, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
    );

    #undef TEST_NUM_BASE_TO

    TEST_FN_CLOSE
}

void test_num_base_from(bool show)
{
    TEST_FN_OPEN

#define TEST_NUM_BASE_FROM(TAG, NUM_IN, NUM_OUT)        \
{                                                       \
    TEST_CASE_OPEN(TAG)                                 \
    {                                                   \
        num_p num = num_create_immed(ARG_OPEN NUM_IN);  \
        num = num_base_from(num, 10);                   \
        assert(num_immed(num, ARG_OPEN NUM_OUT));       \
    }                                                   \
    TEST_CASE_CLOSE                                     \
}

    TEST_NUM_BASE_FROM(1,
        (0),
        (0)
    );
    TEST_NUM_BASE_FROM(2,
        (1, 1),
        (1, 1)
    );
    TEST_NUM_BASE_FROM(3,
        (1, 9),
        (1, 9)
    );
    TEST_NUM_BASE_FROM(4,
        (2, 1, 0),
        (1, 10)
    );
    TEST_NUM_BASE_FROM(5,
        (21, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
        (2, 5, 0x6bc75e2d63100000)
    );

    #undef TEST_NUM_BASE_FROM

    TEST_FN_CLOSE
}



void test_num()
{
    TEST_LIB

    bool show = true;

    test_uint_from_char(show);
    test_uint128(show);

    test_num_create(show);
    test_num_expand_to(show);
    test_num_chunk_get(show);
    test_num_chunk_set(show);

    test_num_normalize(show);
    test_num_break(show);

    test_num_wrap(show);
    test_num_wrap_dec(show);
    test_num_wrap_hex(show);
    test_num_wrap_str(show);
    test_num_read_dec(show);
    test_num_unwrap(show);
    test_num_copy(show);

    test_num_add_uint_offset(show);
    test_num_sub_uint_offset(show);
    test_num_cmp_offset(show);
    test_num_sub_offset(show);

    test_num_shl_uint(show);
    test_num_shr_uint(show);
    test_num_mul_uint(show);
    test_num_add_mul_uint(show);

    test_num_pad(show);
    test_num_depad(show);
    test_num_fft(show);
    test_num_fft_inv(show);

    test_num_is_zero(show);

    test_num_shl(show);
    test_num_shr(show);

    test_num_add(show);
    test_num_sub(show);
    test_num_mul(show);
    test_num_sqr(show);
    test_num_div_mod(show);
    test_num_gcd(show);

    test_num_base_to(show);
    test_num_base_from(show);

    TEST_ASSERT_MEM_EMPTY
}



int main()
{
    setbuf(stdout, NULL);
    test_num();
    printf("\n\n\tTest successful\n\n");
    return 0;
}
