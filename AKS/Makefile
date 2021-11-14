CC := gcc
EXE_NAME := aks
CFLAGS := -Wall -Werror -Wextra
LDFLAGS	:= -lgmp
PARALLEL ?= true

ifeq ($(PARALLEL),true)
CFLAGS += -D USE_PARALLEL
LDFLAGS += -fopenmp
EXE_NAME := aks_para
endif

$(EXE_NAME): aks.o poly.o sieve.o main.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

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
	$(RM)	*.o *.out