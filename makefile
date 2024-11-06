
export GCC=gcc
export CFLAGS=-Wall
# export CFLAGS=-Wall -g -Og #for debug build.
# export CFLAGS=-Wall -O3 -finline-functions #for release build.

all: dist/include/simple-cypher/simple-cypher.h dist/lib/libsimplecypher.a dist/lib/libsimplecypher.so bin/simple-cypher

.PHONY: clean
clean:
	rm -f src/simple-cypher.o
	rm -f src/main.o
	make -C test clean

.PHONY: test
test: all
	make -C test SIMPLE_CYPHER_INCLUDE=$(CURDIR)/dist/include SIMPLE_CYPHER_LIB=$(CURDIR)/dist/lib

.PHONY: test-bin
test-bin: all
	make -C test test-bin SIMPLE_CYPHER_BIN=$(CURDIR)/bin

src/simple-cypher.o: src/simple-cypher.c src/simple-cypher.h
	$(GCC) $(CFLAGS) -c -o src/simple-cypher.o src/simple-cypher.c

dist/include/simple-cypher:
	mkdir -p dist/include/simple-cypher

dist/include/simple-cypher/simple-cypher.h: src/simple-cypher.h | dist/include/simple-cypher
	cp src/simple-cypher.h dist/include/simple-cypher/simple-cypher.h

dist:
	mkdir -p dist

dist/lib:
	mkdir -p dist/lib

dist/lib/libsimplecypher.a: src/simple-cypher.o | dist/lib
	ar r dist/lib/libsimplecypher.a src/simple-cypher.o

dist/lib/libsimplecypher.so: src/simple-cypher.o | dist/lib
	$(GCC) $(CFLAGS) -shared -o dist/lib/libsimplecypher.so src/simple-cypher.o

bin:
	mkdir -p bin

src/main.o: src/main.c dist/include
	$(GCC) $(CFLAGS) -Idist/include -c -o src/main.o src/main.c

bin/simple-cypher: src/main.o dist/lib/libsimplecypher.a | bin
	$(GCC) $(CFLAGS) -Ldist/lib -o bin/simple-cypher src/main.o -lsimplecypher
