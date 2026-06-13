set -e
rm -rf thread_log/*
make dbg
echo running
time ./src/debug.out $@
echo
echo success
echo
