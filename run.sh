set -e
rm -rf thread_log/*
make build
echo running
time ./src/main.o $@
echo
echo success
echo
