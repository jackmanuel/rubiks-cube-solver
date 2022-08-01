CC = g++
CFLAGS  = -g -Wall


all: solver


solver:  test.o Cube.o
	$(CC) $(CFLAGS) -o solver test.o Cube.o

test.o:  test.cpp
	$(CC) $(CFLAGS) -c test.cpp

cube.o:  Cube.cpp Cube.h
	$(CC) $(CFLAGS) -c Cube.cpp


clean: 
	$(RM) solver *.o *~