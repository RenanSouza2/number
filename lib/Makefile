include $(shell git rev-parse --show-toplevel)/makefiles/bundle.mk


build b: $(LIB_FULL_FILE)
dbg d: $(DBG_FULL_FILE)



$(DBG_FULL_FILE): $(DBG_FILE) $(CLU_FILE)
	echo " linking $(PRJ_NAME) full debug $(DIR)"
	gcc $(FLAGS) $(FLAGS_DBG) $(FLAGS_LNK) -o $@ $^



.PHONY: $(CLU_FILE)
$(NUM_FILE) $(CLU_FILE):
	$(MAKE) --directory=$(dir $@)



_clean::
	$(MAKE) clean --directory $(dir ${CLU_FILE})
