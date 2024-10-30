
export GCC=gcc
export CFLAGS+=-I$(SIMPLE_CYPHER_INCLUDE) -L$(SIMPLE_CYPHER_LIB)

.PHONY: all
all: bin/test
	gdb bin/test

.PHONY: clean
clean: 
	rm -f src/test-simple-cypher-encrypt-decrypt.o
	rm -f src/main.o

src/test-simple-cypher-encrypt-decrypt.o: src/test-simple-cypher-encrypt-decrypt.c src/test-simple-cypher-encrypt-decrypt.h src/test.h $(SIMPLE_CYPHER_INCLUDE)
	$(GCC) $(CFLAGS) -c -o src/test-simple-cypher-encrypt-decrypt.o src/test-simple-cypher-encrypt-decrypt.c

src/main.o: src/main.o src/test-simple-cypher-encrypt-decrypt.h $(SIMPLE_CYPHER_INCLUDE)
	$(GCC) $(CFLAGS) -c -o src/main.o src/main.c

bin: 
	mkdir -p bin

bin/test: src/test-simple-cypher-encrypt-decrypt.o src/main.o $(SIMPLE_CYPHER_LIB) | bin
	$(GCC) $(CFLAGS) -o bin/test src/test-simple-cypher-encrypt-decrypt.o src/main.o -lsimplecypher
