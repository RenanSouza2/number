set -e
rm -rf thread_log/*
make build
for (( i=1; i<=10000; i++ )); do
    time ./src/main.o $@
done

