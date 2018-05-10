CFLAGS = -std=c99 -pedantic -D_XOPEN_SOURCE=600 -Wall -Werror
CC = gcc

.PHONY: all clean

all: crawl

clean:

crawl: crawl.c
	$(CC) $(CFLAGS) crawl.c -o crawl 
