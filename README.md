# Parallel-Primality-Test
Final Project for Parallel Algorithms (ECE 382V)

## Rabin-Miller Algo

### Compile and Run: 
gcc -fopenmp ./rm_test.c -o rabin_miller.out

./rabin_miller.out

### TODO:
    1. Optimize the power/exponent function
    2. Rewrite the random number generation function
    3. More benchmarking

## AKS Algo
To use OpenMP support:
```
make
aks -n [number]
or
aks -f selected.txt
```

To disable OpenMP, and build single-threaded:
```
make clean
PARALLEL=false make
aks_para -n [number]
or
aks_para -f selected.txt
```

To experiment with different thread counts, build with OpenMP, and set the following environment 
variable when running:
```
OMP_NUM_THREADS=4 ./aks selected.txt
```

When `OMP_NUM_THREADS` is not set, it defaults to the number returned by the command:
```
grep -c ^processor /proc/cpuinfo
```

