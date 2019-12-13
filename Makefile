CC=gcc
CFLAGS=-Wall -O2 -std=c99
PREFIX ?= usr/local
DESTDIR ?= /

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

union-find: union-find/quick-union.o union-find/quick-find.o
all: union-find

install: all
	install -d $(DESTDIR)/$(PREFIX)/bin
	install union-find/quick-find.o $(DESTDIR)/$(PREFIX)/bin
	install union-find/quick-union.o $(DESTDIR)/$(PREFIX)/bin

clean:
	rm */*.o bin/* -f
