CC := gcc
MY_GMP_FLAG := -lgmp -I/usr/local/Cellar/gmp/6.2.1_1/include -L/usr/local/Cellar/gmp/6.2.1_1/lib/
CFLAGS := -Wall -Ofast -fopenmp $(MY_GMP_FLAG)

all: test.o benchmarking.o

test.o: test.c
	$(CC) $(CFLAGS) $< -o $@

benchmarking.o: benchmarking.c
	$(CC) $(CFLAGS) $< -o $@

.PHONY: clean
clean:
	rm benchmarking.o test.o