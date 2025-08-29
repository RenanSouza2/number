FLAGS = -std=c11 -Wall -Wextra -Werror -Wfatal-errors -Wpedantic -Wshadow -Wpointer-arith -Wcast-qual -Wstrict-prototypes -Wformat=2 -Wnull-dereference -Wconversion -Wsign-conversion -fstack-protector-strong -O0 -g3 -ggdb -fno-omit-frame-pointer -fsanitize=address,undefined,leak -D _POSIX_C_SOURCE=199309L -D DEBUG
DEBUG_FILE = $(LIB_ROOT)/debug.o
CLU_FILE = $(LIB_ROOT)/../mods/clu/bin/bin.o

test: test.o
	./test.o

test.o: test.c $(DEBUG_FILE) $(CLU_FILE)
	echo "building num test $(DIR)"
	gcc $(FLAGS) -o $@ $^

clean:
	rm -rf test.o
