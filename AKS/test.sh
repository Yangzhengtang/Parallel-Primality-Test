#!/bin/sh
mkdir ./test-res

#echo "n,logn,isPrime,time" > test-res/serial.csv
#aks -f selected.txt >> test-res/serial.csv

echo "n,logn,isPrime,time" > test-res/para-2.csv
OMP_NUM_THREADS=2 ./test.o >> test-res/para-2.csv

echo "n,logn,isPrime,time" > test-res/para-4.csv
OMP_NUM_THREADS=4 ./test.o >> test-res/para-4.csv

echo "n,logn,isPrime,time" > test-res/para-8.csv
OMP_NUM_THREADS=8 ./test.o >> test-res/para-8.csv

echo "n,logn,isPrime,time" > test-res/para-16.csv
OMP_NUM_THREADS=16 ./test.o >> test-res/para-16.csv

echo "n,logn,isPrime,time" > test-res/para-32.csv
OMP_NUM_THREADS=32 ./test.o >> test-res/para-32.csv

echo "n,logn,isPrime,time" > test-res/para-64.csv
OMP_NUM_THREADS=64 ./test.o >> test-res/para-64.csv