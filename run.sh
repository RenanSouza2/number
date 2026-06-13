set -e
rm -rf thread_log/*
time make build
echo running
time ./src/main.out $@
echo
echo success
echo
