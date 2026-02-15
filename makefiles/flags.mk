FLAGS = -std=c11 -Wall -Wextra -Wpedantic -Werror -Wfatal-errors -Wshadow -Wpointer-arith -Wcast-qual -Wstrict-prototypes -Wwrite-strings -Wundef -Wformat=2 -Wnull-dereference -Wconversion -Wsign-conversion -D_POSIX_C_SOURCE=200809L -Wimplicit-fallthrough -Wfloat-equal -Wredundant-decls -Wdouble-promotion -Wmissing-include-dirs -Wswitch-enum -Wnested-externs

FLAGS_PRD = -O2 -march=native -fstack-protector-strong -D_FORTIFY_SOURCE=2 -flto
FLAGS_DBG = -D DEBUG -O0 -g3 -ggdb -fno-omit-frame-pointer -fsanitize=address,undefined

FLAGS_CMP = -c
FLAGS_LNK = -r -nostdlib
FLAGS_EXE = 

ifeq ($(shell uname -s),Linux)
	FLAGS += -Wduplicated-cond -Wduplicated-branches -Wlogical-op
	FLAGS += -fanalyzer

    FLAGS_PRD += -fstack-clash-protection
	FLAGS_DBG += -fsanitize=leak

	FLAGS_CMP += -fPIE
	FLAGS_EXE += -pie -Wl,-z,relro -Wl,-z,now -Wl,-z,defs
endif

ifeq ($(shell uname -s),Darwin)
    FLAGS += -Wunreachable-code -Wunreachable-code-break -Wconditional-uninitialized -Wmissing-variable-declarations

    FLAGS_EXE += -Wl,-fatal_warnings -Wl,-dead_strip
endif
