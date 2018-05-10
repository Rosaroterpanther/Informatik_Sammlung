CFLAGS = -std=c99 -pedantic -D_XOPEN_SOURCE=700 -Wall -Werror
CC = gcc

.PHONY: all clean

all: snail

clean:

snail: snail.c
	$(CC) $(CFLAGS) snail.c -o snail
