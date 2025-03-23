FLAGS = -Wall -Wextra -Werror -Wfatal-errors -O3

LIB = ../lib/lib.o
DGB = ../lib/debug.o



build: main.o

dbg: debug.o



main.o: main.c $(LIB)
	gcc -o $@ $^ $(FLAGS)

debug.o: main.c $(DGB)
	gcc -o $@ $^ $(FLAGS) -D DEBUG



.PHONY: $(LIB)
$(LIB):
	$(MAKE) --directory=$(dir $@)

.PHONY: $(DGB)
$(DGB):
	$(MAKE) debug.o --directory=$(dir $@)

clean:
	rm -rf *.o
