CC=g++
CFLAGS=-std=c++11 -pedantic -Wall -ggdb3 -Werror #-pthread
SOURCES=socket.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=socket test

all: socket test

socket: socket.o threadcontrol.o parser.o cache.o
	$(CC) $(CFLAGS) -o $@ socket.o threadcontrol.o parser.o cache.o -lpthread

test: test.o
	gcc -std=gnu99 -pedantic -Wall -ggdb3 -Werror -o $@ test.o

#parser: parser.o parser.h
#	$(CC) $(CFLAGS) -o $@ parser.o

socket.o: socket.cpp socket.h threadcontrol.h
	$(CC) $(CFLAGS) -c socket.cpp

test.o: test.c
	gcc -std=gnu99 -pedantic -Wall -ggdb3 -Werror -c test.c

parser.o: parser.cpp parser.h
	$(CC) $(CFLAGS) -c parser.cpp

threadcontrol.o: threadcontrol.cpp threadcontrol.h parser.h
	$(CC) $(CFLAGS) -c threadcontrol.cpp

.PHONY:clean
cache.o: cache.cpp cache.h
	$(CC) $(CFLAGS) -c cache.cpp

clean:
	rm -f *~ *.o socket test parser threadcontrol cache
