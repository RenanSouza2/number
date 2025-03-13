#ifndef U64

#include <stdint.h>

#ifdef __linux__

#define U64P(C) "%" #C "lu"
#define U64PX "%016lx"

#elif defined __APPLE__

#define U64P(C) "%" #C "llu"
#define U64PX "%016llx"

#endif

#endif
