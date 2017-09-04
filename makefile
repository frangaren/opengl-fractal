CC=gcc
CFLAGS=-Wall -g -c
LDFLAGS=-lGL -lglfw -lGLEW

PROGRAM_NAME=fractal

.PHONY:default
default: build

.PHONY:build
build: bin/$(PROGRAM_NAME)

.PHONY:clean
clean:
	rm -f bin/$(PROGRAM_NAME)
	rm -f src/*.o

bin/$(PROGRAM_NAME): src/main.o src/app.o
	mkdir -p bin
	$(CC) $(LDFLAGS) -o $@ $^

src/main.o: src/main.c src/app.h
	$(CC) $(CFLAGS) -o $@ $<

src/app.o: src/app.c src/app.h
	$(CC) $(CFLAGS) -o $@ $<
