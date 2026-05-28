SRC = src
LIB = lib

build b:
	$(MAKE) build --directory=$(SRC) -s -j

dbg d:
	$(MAKE) dbg --directory=$(SRC) -s -j

clean c _clean:
	$(MAKE) clean --directory=$(SRC) -s -j

test t:
	$(MAKE) test --directory=$(LIB) -s
