CC = g++
CFLAGS  = -g -Wall -O3 -Iinclude
SRCDIR = src
BUILDDIR = build

OBJS = $(BUILDDIR)/main.o $(BUILDDIR)/Cube.o $(BUILDDIR)/Solver2.o \
       $(BUILDDIR)/Indexer.o $(BUILDDIR)/PDB.o $(BUILDDIR)/Solver.o \
       $(BUILDDIR)/PDBBuilder.o $(BUILDDIR)/TransitionTable.o


all: solver


solver: $(BUILDDIR) $(OBJS)
	$(CC) $(CFLAGS) -o solver $(OBJS)

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

$(BUILDDIR)/main.o: $(SRCDIR)/main.cpp
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/Cube.o: $(SRCDIR)/Cube.cpp include/Cube.h
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/Solver2.o: $(SRCDIR)/Solver2.cpp include/Solver2.h
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/Indexer.o: $(SRCDIR)/Indexer.cpp include/Indexer.h
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/PDB.o: $(SRCDIR)/PDB.cpp include/PDB.h
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/Solver.o: $(SRCDIR)/Solver.cpp include/Solver.h
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/PDBBuilder.o: $(SRCDIR)/PDBBuilder.cpp include/PDBBuilder.h
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/TransitionTable.o: $(SRCDIR)/TransitionTable.cpp include/TransitionTable.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) solver
	$(RM) -r $(BUILDDIR)