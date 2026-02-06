PROJECT = clu
LIB_ROOT = $(shell git rev-parse --show-toplevel)/lib
DBG_FILE = $(LIB_ROOT)/debug.o
LIB_FILE = $(LIB_ROOT)/lib.o
DIR = $(shell basename $(CURDIR))
