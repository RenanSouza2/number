set -e
rm -rf dbg/*
make build
time ./src/main.o $@
