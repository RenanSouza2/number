#ifndef __TEST_REVERT_H__
#define __TEST_REVERT_H__

#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

#include "./assert.h"

#define TEST_REVERT_OPEN                            \
    {                                               \
        int pid = fork();                           \
        assert(pid >= 0);                           \
        if(pid)                                     \
        {                                           \
            int status;                             \
            assert(waitpid(pid, &status, 0) >= 0);  \
            assert(status != EXIT_SUCCESS);         \
        }                                           \
        else                                        \
        {                                           \
            freopen("/dev/null", "w", stdout);      \
            freopen("/dev/null", "w", stderr);      \

#define TEST_REVERT_CLOSE       \
            exit(EXIT_SUCCESS); \
        }                       \
    }

#define TEST_TIMEOUT_OPEN(TIMEOUT)  \
    {                               \
        pid_t pid_timeout = fork(); \
        assert(pid_timeout >= 0);   \
        if(pid_timeout == 0)        \
        {                           \
            sleep(TIMEOUT);         \
            exit(EXIT_SUCCESS);     \
        }                           \
        pid_t pid_test = fork();    \
        assert(pid_test >= 0);      \
        if(pid_test == 0)           \
        {

#define TEST_TIMEOUT_CLOSE                          \
            exit(EXIT_SUCCESS);                     \
        }                                           \
        int status;                                 \
        pid_t pid_return = waitpid(0, &status, 0);  \
        if(pid_return == pid_timeout)               \
        {                                           \
            kill(pid_test, SIGKILL);                \
            printf("\n\n\tTest timeout\n\n");       \
            return 1;                               \
        }                                           \
        kill(pid_timeout, SIGKILL);                 \
        if(status)                                  \
        {                                           \
            printf("\n\n\tTest faillure\n\n");      \
            return 1;                               \
        }                                           \
    }

#endif
