CFLAGS = -std=c99 -pedantic -pthread -D_XOPEN_SOURCE=600 -Wall -Werror
CFLAGS2 = -std=c99 -pedantic -D_XOPEN_SOURCE=600 -Wall -Werror
CC = gcc

.PHONY: all clean piper pipes cleanpipes

all: piper

clean:

piper.o: piper.c
	$(CC) -c $(CFLAGS) piper.c -o piper.o

piper: piper.o sbuf.o
	$(CC)  $(CFLAGS) piper.o sbuf.o -o piper

sbuf.o: sbuf.c sbuf.h
	$(CC)  -c $(CFLAGS2) sbuf.c -o sbuf.o

pipes:

cleanpipes:
