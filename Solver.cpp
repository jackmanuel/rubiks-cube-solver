#include <stack>
#include <algorithm>
#include <iostream>

#include "Solver.h"
#include "Cube.h"
#include "PDB.h"
#include "DatabaseConstants.h"

Solver::Solver(void)
{
}

std::string Solver::solve(Cube cube)
{
    PDB PDB(DatabaseConstants::CORNER_DB, DatabaseConstants::EDGE1_DB, 
            DatabaseConstants::EDGE2_DB, DatabaseConstants::ORIENT_DB);

    Cube startingCube = cube;
    std::stack<Cube> open;
    std::vector<Cube> neighbours;
    std::array<Cube::move, MAX_MOVES> movesApplied{Cube::move::NONE};

    // cube will take at least this many moves, start here for search
    int maxDepth = PDB.getMoveCountLowerBound(cube);

    std::cout << "Starting the search at depth " << maxDepth << std::endl;
    std::cout << "Searching depth " << maxDepth << "..." << std::flush;

    open.push(cube);
    long long statesChecked = 0;

    while (true)
    {
        if (open.empty())
        {
            // didn't find a solution at that depth
            maxDepth++;
            statesChecked = 0;
            
            // Pad with spaces to overwrite the progress from the previous depth
            std::cout << "\rSearching depth " << maxDepth << "...                                                           " << std::flush;

            open.push(startingCube);
        }
        
        Cube current = open.top();
        open.pop();
        statesChecked++;

        // 0x7FFFF is 524,287 in decimal
        if ((statesChecked & 0x7FFFF) == 0)
        {
            std::cout << "\rSearching depth " << maxDepth << "... (States checked: " << statesChecked << ", Queue size: " << open.size() << ")" << std::flush;
        }
        

        if (current.isSolved())
        {
            if (current.getDepth() > 0)
            {
                movesApplied[current.getDepth() - 1] = current.getLastMove();
                std::cout << std::endl << "SOLUTION FOUND!" << std::endl;
            }
            else
            {
                std::cout << "The cube is already solved!" << std::endl;
            }
            break;
        }

        if (current.getDepth() != 0)
        {   
            // stores the moves to get to current state
            movesApplied[current.getDepth() - 1] = current.getLastMove();
        }

        neighbours = current.generateNeighboursPruned();

        for (std::vector<Cube>::iterator it = neighbours.begin(); it != neighbours.end();)
        {
            int fVal = (*it).getDepth() + PDB.getMoveCountLowerBound((*it));

            // neighbour f value is still within depth of search
            if (fVal <= maxDepth)
            {
                // temporarily set depth as f, to sort neighbours by f
                (*it).setDepth(fVal);
                ++it;
            }
            else
            {
                it = neighbours.erase(it);
            }
        }
        
        // sort neighbours by f value.
        std::sort(neighbours.begin(), neighbours.end());

        for (Cube neighbour : neighbours)
        {
            // reset depth value to its proper value (g)
            neighbour.setDepth(current.getDepth() + 1);

            // push onto stack in order of smallest f value
            open.push(neighbour);
        }
    }

    return movesToString(movesApplied);
}


std::string Solver::movesToString(std::array<Cube::move, Solver::MAX_MOVES> movesApplied)
{
    std::string result;

    for (int move : movesApplied)
    {
        switch (move) 
        {
            case (Cube::move::NONE):
                break;
            case (Cube::move::R):
                result.append("R ");
                break;
            case (Cube::move::R_PRIME):
                result.append("R' ");
                break;
            case (Cube::move::R2):
                result.append("R2 ");
                break;
            case (Cube::move::U):
                result.append("U ");
                break;
            case (Cube::move::U_PRIME):
                result.append("U' ");
                break;
            case (Cube::move::U2):
                result.append("U2 ");
                break;
            case (Cube::move::F):
                result.append("F ");
                break;
            case (Cube::move::F_PRIME):
                result.append("F' ");
                break;
            case (Cube::move::F2):
                result.append("F2 ");
                break;
            case (Cube::move::D):
                result.append("D ");
                break;
            case (Cube::move::D_PRIME):
                result.append("D' ");
                break;
            case (Cube::move::D2):
                result.append("D2 ");
                break;
            case (Cube::move::B):
                result.append("B ");
                break;
            case (Cube::move::B_PRIME):
                result.append("B' ");
                break;
            case (Cube::move::B2):
                result.append("B2 ");
                break;
            case (Cube::move::L):
                result.append("L ");
                break;
            case (Cube::move::L_PRIME):
                result.append("L' ");
                break;
            case (Cube::move::L2):
                result.append("L2 ");
                break;
        }   
    }
    return result;
}
