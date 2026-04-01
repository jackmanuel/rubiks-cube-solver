CC = g++
CFLAGS  = -g -Wall -O3
BUILDDIR = build

OBJS = $(BUILDDIR)/main.o $(BUILDDIR)/Cube.o $(BUILDDIR)/Solver2.o \
       $(BUILDDIR)/Indexer.o $(BUILDDIR)/PDB.o $(BUILDDIR)/Solver.o \
       $(BUILDDIR)/PDBBuilder.o $(BUILDDIR)/TransitionTable.o


all: solver


solver: $(BUILDDIR) $(OBJS)
	$(CC) $(CFLAGS) -o solver $(OBJS)

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

$(BUILDDIR)/main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp -o $@

$(BUILDDIR)/Cube.o: Cube.cpp Cube.h
	$(CC) $(CFLAGS) -c Cube.cpp -o $@

$(BUILDDIR)/Solver2.o: Solver2.cpp Solver2.h
	$(CC) $(CFLAGS) -c Solver2.cpp -o $@

$(BUILDDIR)/Indexer.o: Indexer.cpp Indexer.h
	$(CC) $(CFLAGS) -c Indexer.cpp -o $@

$(BUILDDIR)/PDB.o: PDB.cpp PDB.h
	$(CC) $(CFLAGS) -c PDB.cpp -o $@

$(BUILDDIR)/Solver.o: Solver.cpp Solver.h
	$(CC) $(CFLAGS) -c Solver.cpp -o $@

$(BUILDDIR)/PDBBuilder.o: PDBBuilder.cpp PDBBuilder.h
	$(CC) $(CFLAGS) -c PDBBuilder.cpp -o $@

$(BUILDDIR)/TransitionTable.o: TransitionTable.cpp TransitionTable.h
	$(CC) $(CFLAGS) -c TransitionTable.cpp -o $@

clean:
	$(RM) solver
	$(RM) -r $(BUILDDIR)