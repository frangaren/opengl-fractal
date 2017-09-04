CC=gcc
CFLAGS=-Wall -g -c
LDFLAGS=-lGL -lglfw -lGLEW

PROGRAM_NAME=fractal

.PHONY:default
default: build

.PHONY:build
build: bin/$(PROGRAM_NAME) bin/vertex_shader.glsl bin/fragment_shader.glsl

.PHONY:clean
clean:
	rm -f bin/$(PROGRAM_NAME)
	rm -f src/*.o

bin/$(PROGRAM_NAME): src/main.o src/app.o src/fractal.o
	mkdir -p bin
	$(CC) $(LDFLAGS) -o $@ $^

src/main.o: src/main.c src/app.h src/fractal.h
	$(CC) $(CFLAGS) -o $@ $<

src/app.o: src/app.c src/app.h
	$(CC) $(CFLAGS) -o $@ $<

src/fractal.o: src/fractal.c src/fractal.h
	$(CC) $(CFLAGS) -o $@ $<

bin/vertex_shader.glsl: src/vertex_shader.glsl
	mkdir -p bin
	cp $< $@

bin/fragment_shader.glsl: src/fragment_shader.glsl
	mkdir -p bin
	cp $< $@
