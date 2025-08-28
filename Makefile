SRC = src
LIB = lib

build b:
	$(MAKE) build --directory=$(SRC)

dbg d:
	$(MAKE) dbg --directory=$(SRC) -s

clean c _clean:
	$(MAKE) clean --directory=$(SRC) -s

test t:
	$(MAKE) test --directory=$(LIB) -s
