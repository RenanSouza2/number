FLAGS = -Wall -Wextra -Werror -Wfatal-errors -O3 -fsanitize=address
DEBUG_FILE = $(LIB_ROOT)/debug.o
CLU_FILE = $(LIB_ROOT)/../mods/clu/bin/bin.o

test: test.o
	./test.o

test.o: test.c $(DEBUG_FILE) $(CLU_FILE)
	echo "building num test $(DIR)"
	gcc -o $@ $^ $(FLAGS) -D DEBUG

clean:
	rm -rf test.o
