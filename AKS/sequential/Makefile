CC := g++
LDFLAGS	:= -lgmp

aks.o: main.o sieve.o poly.o
	$(CC) $(LDFLAGS) -o $@ $^

main.o: main.cpp
	$(CC) $(LDFLAGS) -c $< 

sieve.o: sieve.cpp
	$(CC) $(LDFLAGS) -c $< 

poly.o: poly.cpp
	$(CC) $(LDFLAGS) -c $< 

.PHONY: clean
clean: clean
	$(RM)	*.o