set -e
rm -rf thread_log/*
make dbg
echo running
time ./src/debug.o $@
echo
echo success
echo
