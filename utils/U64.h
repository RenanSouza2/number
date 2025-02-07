#ifndef U64

#ifdef __linux__

#define U64P  "%lu"
#define U64PX "%016lx"

#elif defined __APPLE__

#define U64P  "%llu"
#define U64PX "%016llx"

#endif

#endif