CC = g++
CFLAGS  = -g -Wall


all: solver


solver:  test.o Cube.o Solver2.o
	$(CC) $(CFLAGS) -o solver test.o Cube.o Solver2.o

test.o:  test.cpp
	$(CC) $(CFLAGS) -c test.cpp

Cube.o:  Cube.cpp Cube.h
	$(CC) $(CFLAGS) -c Cube.cpp

Solver2.o: Solver2.cpp Solver2.h
	$(CC) $(CFLAGS) -c Solver2.cpp

clean: 
	$(RM) solver *.o *~