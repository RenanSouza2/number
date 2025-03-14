#ifndef __TEST_REVERT_H__
#define __TEST_REVERT_H__

#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

#define TEST_REVERT_OPEN                                \
    {                                                   \
        int pid = fork();                               \
        assert(pid >= 0);                               \
        if(pid)                                         \
        {                                               \
            int status;                                 \
            waitpid(pid, &status, 0);                   \
            assert(status != 0);                        \
        }                                               \
        else                                            \
        {                                               \
            assert(freopen("/dev/null", "w", stdout));  \
            assert(freopen("/dev/null", "w", stderr));

#define TEST_REVERT_CLOSE   \
            exit(0);        \
        }                   \
    }

#endif
