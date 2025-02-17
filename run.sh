make clean
make build
time ./src/main.o 0 > out_0.txt &
time ./src/main.o 1 > out_1.txt &
time ./src/main.o 2 > out_2.txt &
time ./src/main.o 3 > out_3.txt &
time ./src/main.o 4 > out_4.txt &
time ./src/main.o 5 > out_5.txt &
