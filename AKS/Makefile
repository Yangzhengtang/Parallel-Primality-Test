CC := g++
CFLAGS := -fopenmp
LDFLAGS	:= -lgmp

aks: aks.o poly.o sieve.o main.o
	$(CC) $(LDFLAGS) -o $@ $^ $(CFLAGS)

main.o: main.c
	$(CC) $(LDFLAGS) -c $< $(CFLAGS)

aks.o: aks.c
	$(CC) $(LDFLAGS) -c $< $(CFLAGS)

poly.o: poly.c
	$(CC) $(LDFLAGS) -c $< $(CFLAGS)

sieve.o: sieve.c
	$(CC) $(LDFLAGS) -c $< $(CFLAGS)


.PHONY: clean
clean: clean
	$(RM)	*.o