CC=gcc
CFLAGS=-Wall -g -c
LDFLAGS=

PROGRAM_NAME=fractal

.PHONY:default
default: build

.PHONY:build
build: bin/$(PROGRAM_NAME)

.PHONY:clean
clean:
	rm -f bin/$(PROGRAM_NAME)
	rm -f src/*.o

bin/$(PROGRAM_NAME): src/main.o
	mkdir -p bin
	$(CC) $(LDFLAGS) -o $@ $^

src/main.o: src/main.c
	$(CC) $(CFLAGS) -o $@ $<
