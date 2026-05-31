ROOT = $(shell git rev-parse --show-toplevel)
include $(ROOT)/makefiles/flags.mk
include $(ROOT)/makefiles/vars.mk



build b: main.o
dbg d: debug.o



main.o: main.c $(LIB_FILE)
	echo "building $(PRJ_NAME) exec $(DIR)"
	gcc -o $@ $^ $(FLAGS) $(FLAGS_PRD) $(FLAGS_EXE)
	echo "finished $(PRJ_NAME) exec $(DIR)"

debug.o: main.c $(DBG_FULL_FILE)
	echo "building $(PRJ_NAME) debug $(DIR)"
	gcc -o $@ $^ $(FLAGS) $(FLAGS_DBG) $(FLAGS_EXE)



.PHONY: $(LIB_FILE)
$(LIB_FILE):
	$(MAKE) --directory=$(dir $@)

.PHONY: $(DBG_FULL_FILE)
$(DBG_FULL_FILE):
	$(MAKE) dbg --directory=$(dir $@)



clean:
	echo "cleaning $(PRJ_NAME) $(DIR)"
	rm -rf *.o
	$(MAKE) clean --directory=$(LIB_DIR)
