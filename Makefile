CC=g++
CFLAGS=-std=c++11 -pedantic -Wall -ggdb3 -Werror #-pthread
SOURCES=socket.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=socket test


all: socket test  

socket: socket.o threadcontrol.o parser.o cache.o log.o
	$(CC) $(CFLAGS) -o $@ socket.o threadcontrol.o parser.o cache.o log.o -lpthread

test: test.o
	$(CC) $(CFLAGS) -o $@ test.o

#parser: parser.o parser.h
#	$(CC) $(CFLAGS) -o $@ parser.o



#socket.o: socket.cpp socket.h parser.h threadcontrol.h

#cache: cache.o
#	$(CC) $(CFLAGS) -o $@ cache.o
#log: file_test.o parser.o
#	$(CC) $(CFLAGS) -o $@ file_test.o parser.o 

socket.o: socket.cpp socket.h 
	$(CC) $(CFLAGS) -c socket.cpp

test.o: test.c
	$(CC) $(CFLAGS) -c test.c

parser.o: parser.cpp parser.h
	$(CC) $(CFLAGS) -c parser.cpp

threadcontrol.o: threadcontrol.cpp threadcontrol.h parser.h
	$(CC) $(CFLAGS) -c threadcontrol.cpp

cache.o: cache.cpp cache.h time.h parser.h  
	$(CC) $(CFLAGS) -c cache.cpp 
log.o: log.cpp log.h time.h parser.h
	$(CC) $(CFLAGS) -c log.cpp 

#file_test.o: file_test.cpp log.h time.h parser.h
#	$(CC) $(CFLAGS) -c file_test.cpp

.PHONY:clean

clean:
	rm -f *~ *.o socket test parser threadcontrol cache
