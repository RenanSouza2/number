PRJ_DIR = $(shell git rev-parse --show-toplevel)
include $(PRJ_DIR)/makefiles/flags.mk
include $(PRJ_DIR)/makefiles/vars.mk



DIRS := $(wildcard */.)

LIB_FILES = $(foreach DIR,$(DIRS),$(dir $(DIR))lib.o)
DGB_FILES = $(foreach DIR,$(DIRS),$(dir $(DIR))debug.o)



build: $(LIB_FILE)

dbg: $(DBG_FULL_FILE) 



$(LIB_FILE): $(LIB_FILES)
	echo " linking num object $(DIR)"
	gcc $(FLAGS) $(FLAGS_PRD) $(FLAGS_LNK) -o $@ $^

$(DBG_FULL_FILE): $(DBG_FILE) $(CLU_FILE)
	echo " linking num debug full $(DIR)"
	gcc $(FLAGS) $(FLAGS_DBG) $(FLAGS_LNK) -o $@ $^

$(DBG_FILE): $(DGB_FILES)
	echo " linking num debug light $(DIR)"
	gcc $(FLAGS) $(FLAGS_DBG) $(FLAGS_LNK) -o $@ $^



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
	$(MAKE) _test

_test:
	for DIR in $(DIRS) ; do \
		( $(MAKE) _test --directory=$$DIR ) || exit $$? ; \
	done
