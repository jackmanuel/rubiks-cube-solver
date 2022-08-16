#include <iostream>
#include <queue>
#include <fstream>

#include "Cube.h"
#include "Solver2.h"
#include "Indexer.h"
#include "PDBBuilder.h"
#include "PDB.h"
#include "Solver.h"

int main(int argc, char const *argv[])
{
    std::cout << "begin!" << std::endl;

    Cube cube;
    
    cube.applyMoves(argv[1]);

    std::string solution = Solver::solve(cube);

    std::cout << "scramble: " << argv[1] << std::endl;
    std::cout << solution << std::endl;
    std::cout << "end!" << std::endl;

    return 0;
}