CC := g++
CFLAGS = -std=c++11 -pthread -I ./ntl-11.5.1/include
LDFLAGS	:= -lgmp -lntl

aks: main.o aks.o
	$(CC) $(LDFLAGS) -o $@ $^ $(CFLAGS)

main.o: main.cpp
	$(CC) $(LDFLAGS) -c $< $(CFLAGS)

aks.o: aks.cpp aks.h
	$(CC) $(LDFLAGS) -c $< $(CFLAGS)

.PHONY: clean
clean: clean
	$(RM)	*.o