CFLAGS = -std=c99 -pedantic -D_XOPEN_SOURCE=600 -Wall -Werror
CC = gcc

.PHONY: all clean clash

all: clash

clean:

clash: clash.c plist.o
	$(CC) $(CFLAGS) -c $< -o $@
