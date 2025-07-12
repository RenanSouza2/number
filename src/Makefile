FLAGS = -Wall -Wextra -Werror -Wfatal-errors -O3 -pthread -std=c99 -lrt -D_GNU_SOURCE
FLAGS_DBG = -D DEBUG -fsanitize=address

LIB = ../lib/lib.o
DGB = ../lib/debug.o
CLU = ../mods/clu/bin/bin.o

DIR = src



build: main.o

dbg: debug.o



main.o: main.c $(LIB)
	echo "building num exec $(DIR)"
	gcc -o $@ $^ $(FLAGS)

debug.o: main.c $(DGB) $(CLU)
	echo "building num debug $(DIR)"
	gcc -o $@ $^ $(FLAGS) $(FLAGS_DBG)



.PHONY: $(LIB)  $(CLU)
$(LIB) $(CLU):
	$(MAKE) --directory=$(dir $@)

.PHONY: $(DGB)
$(DGB):
	$(MAKE) dbg --directory=$(dir $@)



clean:
	echo cleaning "num $(DIR)"
	rm -rf *.o
	$(MAKE) clean --directory=$(dir $(LIB))
