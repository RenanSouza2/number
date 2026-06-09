set -e
rm -rf thread_log/*
time make build
echo running
time ./src/main.o $@
echo
echo success
echo
