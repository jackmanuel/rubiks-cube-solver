#include <unordered_set>
#include <queue>
#include <iostream>
#include <vector>
#include <fstream>

#include "PDBBuilder.h"
#include "Cube.h"
#include "Indexer.h"

PDBBuilder::PDBBuilder()
{
}


void PDBBuilder::buildCorners(void)
{
    Indexer indexer;

    std::unordered_set<int32_t> closed;
    std::queue<Cube> open;
    std::vector<Cube> neighbours;
    std::array<uint8_t, NUM_CORNER_RANKS>* database = new std::array<uint8_t, NUM_CORNER_RANKS>{0};

    Cube cube;
    open.push(cube);
    closed.insert(0);

    int counter = 0;

    while (!open.empty()){
        Cube current = open.front();
        open.pop();

        if (counter & 1048576)
        {
            std::cout << "current depth: " << current.getDepth() << std::endl;
            // std::cout << "nodes popped: " << counter << std::endl;
            counter = 0;
        }
        counter++;
        
        uint32_t rank = indexer.getCornerIndex(current);
        uint32_t databaseVal = (*database)[rank];
        if (databaseVal == 0 || databaseVal > current.getDepth())
        {
            (*database)[rank] = current.getDepth();
        }

        neighbours = current.generateNeighbours();

        for (Cube neighbour : neighbours)
        {
            uint32_t neighbourRank = indexer.getCornerIndex(neighbour);

            // only push neighbours if not visited
            // and it's less than 12 moves in
            if (!closed.count(neighbourRank) && neighbour.getDepth() <= 11)
            {
                open.push(neighbour);
                closed.insert(neighbourRank);
            }
        }
    }

    std::ofstream writer("cornerDB.data", std::ios::out | std::ios::binary);
    writer.write((char*)database, sizeof(uint32_t) * NUM_CORNER_RANKS);
    writer.close();

    delete[] database;
}
