ROOT = $(shell git rev-parse --show-toplevel)
include $(ROOT)/makefiles/flags.mk
include $(ROOT)/makefiles/vars.mk



build b: lib.o

dbg d: debug.o



lib.o: code.c
	echo "building $(PROJECT) object $(DIR)"
	gcc -o $@ $^ $(FLAGS) $(FLAGS_PRD) -c

debug.o: code.c
	echo "building $(PROJECT) debug $(DIR)"
	gcc -o $@ $^ $(FLAGS) $(FLAGS_DBG) -c



clean c:
	$(MAKE) clean --directory=$(LIB_ROOT) -s

_clean:
	echo "cleaning $(PROJECT) $(DIR)"
	rm -f *.o
	$(MAKE) clean --directory=test



.PHONY: test
test t:
	$(MAKE) dbg --directory=$(LIB_ROOT) -s
	$(MAKE) _test -s

_test:
	$(MAKE) --directory=test

export
