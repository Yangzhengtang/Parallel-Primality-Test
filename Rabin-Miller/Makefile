CC := gcc
CFLAGS := -Wall -Ofast -fopenmp

all: test.o benchmarking.o

test.o: test.c
	$(CC) $(CFLAGS) $< -o $@

benchmarking.o: benchmarking.c
	$(CC) $(CFLAGS) $< -o $@

.PHONY: clean
clean:
	rm benchmarking.o test.o