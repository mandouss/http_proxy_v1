CC=g++
CFLAGS=-std=c++11 -pedantic -Wall -ggdb3 -Werror #-pthread
SOURCES=socket.cpp 
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=socket test

#<<<<<<< HEAD
all: socket test log
#=======
#all: socket test parser cache
#>>>>>>> dcde491c3d87f492bf8ec085e36ab371fe559990

socket: socket.o threadcontrol.o parser.o cache.o time.o
	$(CC) $(CFLAGS) -o $@ socket.o threadcontrol.o parser.o cache.o -lpthread

test: test.o
	gcc -std=gnu99 -pedantic -Wall -ggdb3 -Werror -o $@ test.o

#parser: parser.o parser.h
#	$(CC) $(CFLAGS) -o $@ parser.o

#<<<<<<< HEAD
#socket.o: socket.cpp socket.h parser.h threadcontrol.h
#=======
#cache: cache.o
#	$(CC) $(CFLAGS) -o $@ cache.o
log: file_test.o time.o parser.o
	$(CC) $(CFLAGS) -o $@ file_test.o parser.o time.o 
socket.o: socket.cpp socket.h threadcontrol.h
	$(CC) $(CFLAGS) -c socket.cpp

test.o: test.c
	gcc -std=gnu99 -pedantic -Wall -ggdb3 -Werror -c test.c

parser.o: parser.cpp parser.h
	$(CC) $(CFLAGS) -c parser.cpp

#<<<<<<< HEAD
threadcontrol.o: threadcontrol.cpp threadcontrol.h parser.h
	$(CC) $(CFLAGS) -c threadcontrol.cpp


.PHONY:clean
#=======
cache.o: cache.cpp cache.h time.h parser.h  
	$(CC) $(CFLAGS) -c cache.cpp 

file_test.o: file_test.cpp log.h time.h parser.h
	$(CC) $(CFLAGS) -c file_test.cpp

time.o: time.cpp time.h
	$(CC) $(CFLAGS) -c time.cpp
#>>>>>>> dcde491c3d87f492bf8ec085e36ab371fe559990
clean:
	rm -f *~ *.o socket test parser threadcontrol
