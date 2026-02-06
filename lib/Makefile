PRJ_DIR = $(shell git rev-parse --show-toplevel)
include $(PRJ_DIR)/makefiles/flags.mk
include $(PRJ_DIR)/makefiles/vars.mk



DIRS := $(wildcard */.)

LIB_FILES = $(foreach DIR,$(DIRS),$(dir $(DIR))lib.o)
DGB_FILES = $(foreach DIR,$(DIRS),$(dir $(DIR))debug.o)



build: lib.o

dbg: debug.o



lib.o: $(LIB_FILES)
	echo " linking num object $(DIR)"
	gcc -r $(FLAGS) $(FLAGS_PRD) -o $@ $^ -flto -nostdlib

debug.o: $(DGB_FILES) $(CLU_FILE)
	echo " linking num debug $(DIR)"
	gcc -r $(FLAGS) $(FLAGS_DBG)  -o $@ $^ -flinker-output=nolto-rel -nostdlib



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
	$(MAKE) clean --directory $(dir ${CLU_FILE}) 



.PHONY: test
test t:
	$(MAKE) dbg
	$(MAKE) _test -s

_test:
	for DIR in $(DIRS) ; do \
		( $(MAKE) _test --directory=$$DIR ) || exit $$? ; \
	done
