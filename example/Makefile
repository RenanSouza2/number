FLAGS = -fsanitize=address -Wall -Wextra -Werror -Wfatal-errors -O3 -c

LIB_ROOT = $(word 1,$(subst lib, ,$(CURDIR)))lib
DIR = lib$(word 2,$(subst lib, ,$(CURDIR)))

build: lib.o



lib.o: code.c
	echo "building num object $(DIR)"
	gcc -o $@ $^ $(FLAGS)

debug.o: code.c
	echo "building num debug $(DIR)"
	gcc -o $@ $^ $(FLAGS) -D DEBUG



clean c:
	$(MAKE) clean --directory=$(LIB_ROOT) -s

_clean:
	echo "cleaning num $(DIR)"
	rm -f *.o
	$(MAKE) clean --directory=test



.PHONY: test
test t:
	$(MAKE) dbg --directory=$(LIB_ROOT) -s
	$(MAKE) _test -s

_test:
	$(MAKE) --directory=test

export
