#include "./mods/clu/header.h"
#include "./mods/macros/test.h"
#include "./mods/macros/assert.h"

#define TEST_ASSERT_MEM_EMPTY assert(clu_mem_is_empty());
// #define TEST_TIMEOUT 5