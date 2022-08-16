CC = g++
CFLAGS  = -g -Wall


all: solver


solver:  test.o Cube.o Solver2.o Indexer.o PDBBuilder.o PDB.o Solver.o
	$(CC) $(CFLAGS) -o solver test.o Cube.o Solver2.o Indexer.o PDBBuilder.o PDB.o Solver.o

test.o:  test.cpp
	$(CC) $(CFLAGS) -c test.cpp

Cube.o:  Cube.cpp Cube.h
	$(CC) $(CFLAGS) -c Cube.cpp

Solver2.o: Solver2.cpp Solver2.h
	$(CC) $(CFLAGS) -c Solver2.cpp

Indexer.o: Indexer.cpp Indexer.h
	$(CC) $(CFLAGS) -c Indexer.cpp

PDB.o: PDB.cpp PDB.h
	$(CC) $(CFLAGS) -c PDB.cpp

Solver.o: Solver.cpp Solver.h
	$(CC) $(CFLAGS) -c Solver.cpp

clean: 
	$(RM) solver *.o *~