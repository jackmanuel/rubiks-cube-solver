#include <unordered_set>
#include <queue>
#include <iostream>
#include <vector>
#include <fstream>
#include <stack>

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

    // int counter = 0;

    while (!open.empty()){
        Cube current = open.front();
        open.pop();

        // if (counter & 1048576)
        // {
        //     std::cout << "current depth: " << current.getDepth() << std::endl;
        //     // std::cout << "nodes popped: " << counter << std::endl;
        //     counter = 0;
        // }
        // counter++;
        
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

    std::ofstream writer("cornerDB.data", std::ios::out | std::ios::binary | std::ios::trunc);
    writer.write((char*)database, sizeof(uint8_t) * NUM_CORNER_RANKS);
    writer.close();

    delete[] database;
}

void PDBBuilder::buildEdges1(void)
{
    Indexer indexer;

    std::unordered_set<int32_t> closed;
    std::stack<Cube> open;
    std::vector<Cube> neighbours;
    std::array<uint8_t, NUM_EDGE_RANKS>* database = new std::array<uint8_t, NUM_EDGE_RANKS>{0};

    Cube cube;
    open.push(cube);

    closed.insert(indexer.getEdgeIndex1(cube));

    int counter = 0;

    while (!open.empty()){
        Cube current = open.top();
        open.pop();

        if (counter & 1048576)
        {
            std::cout << "current depth: " << current.getDepth() << std::endl;
            std::cout << "open size: " << open.size() << std::endl;
            std::cout << "closed size: " << closed.size() << std::endl;

            // std::cout << "nodes popped: " << counter << std::endl;
            counter = 0;
        }
        counter++;
        
        uint32_t rank = indexer.getEdgeIndex1(current);
        uint32_t databaseVal = (*database)[rank];
        if (databaseVal == 0 || databaseVal > current.getDepth())
        {
            (*database)[rank] = current.getDepth();
        }

        neighbours = current.generateNeighbours();

        for (Cube neighbour : neighbours)
        {
            uint32_t neighbourRank = indexer.getEdgeIndex1(neighbour);

            // only push neighbours if not visited
            // and it's less than 12 moves in
            if (!closed.count(neighbourRank) && neighbour.getDepth() <= 11)
            {
                open.push(neighbour);
                closed.insert(neighbourRank);
            }
        }
    }

    std::ofstream writer("edge1DB.data", std::ios::out | std::ios::binary | std::ios::trunc);
    writer.write((char*)database, sizeof(uint8_t) * NUM_EDGE_RANKS);
    writer.close();

    delete[] database;
}


std::array<uint8_t, PDBBuilder::NUM_CORNER_RANKS>* PDBBuilder::getPDB(std::string filename)
{   

    std::array<uint8_t, NUM_CORNER_RANKS>* database = new std::array<uint8_t, NUM_CORNER_RANKS>{0};

    std::ifstream reader(filename, std::ios::in);

    if (!reader.is_open())
    {
        throw std::exception();
    }

    reader.seekg(0, std::ios::beg);
    reader.read(
      (char*)database,
      sizeof(uint8_t) * NUM_CORNER_RANKS);
    reader.close();

    return database;
}
