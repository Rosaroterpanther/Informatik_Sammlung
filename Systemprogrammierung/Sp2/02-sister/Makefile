AR      = ar
CC      = cc
CFLAGS  = -std=c99 -pedantic -D_XOPEN_SOURCE=600 -Wall -Werror -m32 -O3
LDFLAGS = -m32
RM      = rm -f

.PHONY: all doc clean

all: sister

doc:
	$(RM) -r html
	doxygen

clean:
	$(RM) sister libsister.a *.o

sister: libsister.a
	$(CC) -o $@ $(LDFLAGS) $^
	strip -S $@

libsister.a: cmdline.o connection-fork.o i4httools.o request-http.o sister.o
	$(AR) -rcs $@ $^

.c.o:
	$(CC) -c $(CFLAGS) $<
	strip -S $@

connection-fork.o: connection-fork.c connection.h request.h
request-http.o:    request-http.c request.h cmdline.h i4httools.h
sister.o:          sister.c connection.h cmdline.h request.h
