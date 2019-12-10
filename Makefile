CC=gcc
CFLAGS=-Wall -O2
PREFIX ?= usr/local
DESTDIR ?= /

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

union-find: union-find/union-find.o
all: union-find

install: all
	install -d $(DESTDIR)/$(PREFIX)/bin
	install union-find/union-find.o $(DESTDIR)/$(PREFIX)/bin/union-find

clean:
	rm */*.o bin/* -f
