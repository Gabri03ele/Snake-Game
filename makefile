OPTIONS=-O3 -DNDEBUG -Wall -Wextra

all: build/theanswer

debug: OPTIONS=-O0 -g -DDEBUG -Wall -Wextra
debug: build/theanswer

build/theanswer: build/functions.o tools/main.c
	gcc ${OPTIONS} tools/main.c -o build/theanswer -I include/ build/functions.o

build/functions.o: src/functions.c include/myfunctions.h
	gcc ${OPTIONS} -c src/functions.c -o build/functions.o -I include/

clean:
	rm -rf build/*.o build/theanswer

