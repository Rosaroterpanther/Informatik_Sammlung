CFLAGS = -std=c99 -pedantic -D_XOPEN_SOURCE=600 -Wall -Werror
CC = gcc

.PHONY: all

all: lilo

lilo: lilo.c
	$(CC) $(CFLAGS) -o $@ $^
