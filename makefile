CC = g++
CFLAGS  = -g -Wall -O3


all: solver


solver:  main.o Cube.o Solver2.o Indexer.o PDB.o Solver.o PDBBuilder.o
	$(CC) $(CFLAGS) -o solver main.o Cube.o Solver2.o Indexer.o PDB.o Solver.o PDBBuilder.o

main.o:  main.cpp
	$(CC) $(CFLAGS) -c  main.cpp

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

PDBBuilder.o: PDBBuilder.cpp PDBBuilder.h
	$(CC) $(CFLAGS) -c PDBBuilder.cpp

clean: 
	$(RM) solver *.o *~