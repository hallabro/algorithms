CC=gcc
CFLAGS=-Wall -O2

all: union-find
union-find: union-find/union-find.o
	$(CC) $(CFLAGS) -o bin/$@ $<

clean:
	rm */*.o bin/* -f
