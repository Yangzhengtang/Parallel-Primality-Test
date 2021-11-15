#!/bin/sh
mkdir ./test-res

echo "n,logn,isPrime,time" > test-res/serial.csv
aks -f selected.txt >> test-res/serial.csv

echo "n,logn,isPrime,time" > test-res/para-2.csv
OMP_NUM_THREADS=2 aks_para -f selected.txt >> test-res/para-2.csv

echo "n,logn,isPrime,time" > test-res/para-4.csv
OMP_NUM_THREADS=4 aks_para -f selected.txt >> test-res/para-4.csv

echo "n,logn,isPrime,time" > test-res/para-8.csv
OMP_NUM_THREADS=8 aks_para -f selected.txt >> test-res/para-8.csv

echo "n,logn,isPrime,time" > test-res/para-16.csv
OMP_NUM_THREADS=16 aks_para -f selected.txt >> test-res/para-16.csv
