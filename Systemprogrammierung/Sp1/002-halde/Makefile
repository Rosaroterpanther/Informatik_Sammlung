CFLAGS = -std=c99 -pedantic -D_XOPEN_SOURCE=600 -Wall -Werror
CC = gcc

.PHONY: all clean

all: halde.o test test-ref

test: test.o halde.o

test-ref: test-ref.o halde-ref.o

clean:
	rm -f *.o test test-ref

halde.o: halde.c halde.h
	$(CC) $(CFLAGS) -c $< -o $@

test.o: test.c halde.h
	$(CC) $(CFLAGS) -c $< -o $@

test-ref.o: test-ref.c halde.h
	$(CC) $(CFLAGS) -c $< -o $@
