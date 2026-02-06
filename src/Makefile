ROOT = $(shell git rev-parse --show-toplevel)
include $(ROOT)/makefiles/flags.mk
include $(ROOT)/makefiles/vars.mk

FLAGS := $(subst -std=c11,-std=c2x,$(FLAGS))



build: main.o

dbg: debug.o



main.o: main.c $(LIB_FILE)
	echo "building num exec $(DIR)"
	gcc -o $@ $^ $(FLAGS) $(FLAGS_PRD)

debug.o: main.c $(DBG_FULL_FILE)
	echo "building num debug $(DIR)"
	gcc -o $@ $^ $(FLAGS) $(FLAGS_DBG)



.PHONY: $(LIB_FILE)
$(LIB_FILE):
	$(MAKE) --directory=$(dir $@)

.PHONY: $(DBG_FULL_FILE)
$(DBG_FULL_FILE):
	$(MAKE) dbg --directory=$(dir $@)



clean:
	echo "cleaning num $(DIR)"
	rm -rf *.o
	$(MAKE) clean --directory=$(LIB_DIR)
