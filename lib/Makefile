DIRS := $(wildcard */.)

LIB_FILES = $(foreach DIR,$(DIRS),$(dir $(DIR))lib.o)
DGB_FILES = $(foreach DIR,$(DIRS),$(dir $(DIR))debug.o)
CLU_FILE = ../mods/clu/bin/bin.o

DIR = lib$(word 2,$(subst lib, ,$(CURDIR)))

build: lib.o

dbg: debug.o $(CLU_FILE)



lib.o: $(LIB_FILES)
	echo " linking num object $(DIR)"
	ld -r -o $@ $^

debug.o: $(DGB_FILES)
	echo " linking num debug $(DIR)"
	ld -r -o $@ $^



.PHONY: $(LIB_FILES) $(CLU_FILE)
$(LIB_FILES) $(CLU_FILE):
	$(MAKE) --directory=$(dir $@)

.PHONY: $(DGB_FILES)
$(DGB_FILES):
	$(MAKE) dbg --directory=$(dir $@)



clean c:
	$(MAKE) _clean -s

_clean:
	echo "cleaning num $(DIR)"
	rm -rf *.o
	for DIR in $(DIRS) ; do \
		( $(MAKE) _clean --directory=$$DIR ) || exit $$? ; \
	done
	$(MAKE) clean --directory=$(dir $(CLU_FILE))



.PHONY: test
test t:
	$(MAKE) dbg -s
	$(MAKE) _test -s

_test:
	for DIR in $(DIRS) ; do \
		( $(MAKE) _test --directory=$$DIR ) || exit $$? ; \
	done
