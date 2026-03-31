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
    closed.insert(indexer.getCornerIndex(cube));

    int counter = 0;
    int populated = 0;
    while (!open.empty()){
        Cube current = open.front();
        open.pop();


        if (counter & (1 << 20))
        {
            std::cout << "open size: " << open.size() << std::endl;
            std::cout << "populated: " << populated << std::endl;
            counter = 0;
        }
        counter++;

        uint32_t rank = indexer.getCornerIndex(current);
        uint32_t databaseVal = (*database)[rank];

        if (databaseVal == 0)
        {
            (*database)[rank] = current.getDepth();
            populated++;
        }
        
        if (databaseVal != 0 || current.getDepth() == 11)
        {   
            // don't generate neighbours if at max depth 
            // or if the state was reached before
            continue;
        }

        neighbours = current.generateNeighbours();

        for (Cube neighbour : neighbours)
        {   
            uint32_t rank = indexer.getCornerIndex(neighbour);
            
            if (!closed.count(rank))
            {
                open.push(neighbour);
                closed.insert(rank);
            }

        }
    }

    std::ofstream writer("databases/cornerDB new.data", std::ios::out | std::ios::binary | std::ios::trunc);
    writer.write((char*)database, sizeof(uint8_t) * NUM_CORNER_RANKS);
    writer.close();

    delete database;
}

void PDBBuilder::buildEdges1(void)
{
    Indexer indexer;
    std::array<uint8_t, NUM_EDGE_RANKS>* database = new std::array<uint8_t, NUM_EDGE_RANKS>{0};

    for (uint8_t currDepth = 0; currDepth <= 10; currDepth++)
    {
        DFS(indexer, currDepth, database);
    }

    // all database values not filled in must be 11 moves
    for (int i = 0; i < NUM_EDGE_RANKS; i++)
    {
        if ((*database)[i] == 0)
        {
            (*database)[i] = 11;
        }
    }

    // add 0 for solved cube, since it's overwritten
    Cube cube;
    (*database)[indexer.getEdgeIndex1(cube)] = 0;


    std::ofstream writer("databases/edge1DB.data", std::ios::out | std::ios::binary | std::ios::trunc);
    writer.write((char*)database, sizeof(uint8_t) * NUM_EDGE_RANKS);
    writer.close();

    delete database;
}

void PDBBuilder::buildEdges2(void)
{
    Indexer indexer;
    std::array<uint8_t, NUM_EDGE_RANKS>* database = new std::array<uint8_t, NUM_EDGE_RANKS>{0};

    for (uint8_t currDepth = 0; currDepth <= 10; currDepth++)
    {
        DFS2(indexer, currDepth, database);
    }

    // all database values not filled in must be 11 moves
    for (int i = 0; i < NUM_EDGE_RANKS; i++)
    {
        if ((*database)[i] == 0)
        {
            (*database)[i] = 11;
        }
    }

    // add 0 for solved cube, since it's overwritten
    Cube cube;
    (*database)[indexer.getEdgeIndex2(cube)] = 0;

    std::ofstream writer("databases/edge2DB.data", std::ios::out | std::ios::binary | std::ios::trunc);
    writer.write((char*)database, sizeof(uint8_t) * NUM_EDGE_RANKS);
    writer.close();

    delete database;
}

std::array<uint8_t, PDBBuilder::NUM_EDGE_RANKS>* PDBBuilder::testDFS(int depth)
{
    Indexer indexer;
    std::array<uint8_t, NUM_EDGE_RANKS>* database = new std::array<uint8_t, NUM_EDGE_RANKS>{0};

    for (uint8_t currDepth = 0; currDepth <= depth; currDepth++)
    {
        DFS(indexer, currDepth, database);
    }

    // all database values not filled in must be 11 moves
    for (int i = 0; i < NUM_EDGE_RANKS; i++)
    {
        if ((*database)[i] == 0)
        {
            (*database)[i] = 11;
        }
    }

    // add 0 for solved cube, since it's overwritten
    Cube cube;
    (*database)[indexer.getEdgeIndex1(cube)] = 0;


    return database;
}


void PDBBuilder::DFS(Indexer indexer, uint8_t maxDepth, std::array<uint8_t, NUM_EDGE_RANKS>* database)
{
    std::cout << "~~~~~~~~~~~~STARTING DEPTH " << (int)maxDepth << std::endl;

    std::stack<Cube> open;
    std::vector<Cube> neighbours;

    Cube cube;
    open.push(cube);

    int counter = 0;
    int populated = 0;

    while (!open.empty()){
        Cube current = open.top();
        open.pop();

        if (counter & 8388608)
        {
            std::cout << "open size: " << open.size() << std::endl;
            std::cout << "populated: " << populated << std::endl;
            std::cout << "depth: " << (int)maxDepth << std::endl;
            counter = 0;
        }
        counter++;
        
        uint32_t rank = indexer.getEdgeIndex1(current);
        uint32_t databaseVal = (*database)[rank];

        if (databaseVal == 0)
        {
            populated++;
            (*database)[rank] = current.getDepth();
        }
        
        // dont generate neighbours if max depth is reached
        // or database already has entry
        if (current.getDepth() == maxDepth || databaseVal < current.getDepth())
        {
            continue;
        }

        neighbours = current.generateNeighbours();

        for (Cube neighbour : neighbours)
        {
            open.push(neighbour);
        }
    }

    std::cout << "Searched depth: " << (int)maxDepth << std::endl;
    std::cout << "Database entries filled: " << populated << std::endl;
}


void PDBBuilder::DFS2(Indexer indexer, uint8_t maxDepth, std::array<uint8_t, NUM_EDGE_RANKS>* database)
{
    std::cout << "~~~~~~~~~~~~STARTING DEPTH " << (int)maxDepth << std::endl;

    std::stack<Cube> open;
    std::vector<Cube> neighbours;

    Cube cube;
    open.push(cube);

    int counter = 0;
    int populated = 0;

    while (!open.empty()){
        Cube current = open.top();
        open.pop();

        if (counter & 8388608)
        {
            std::cout << "open size: " << open.size() << std::endl;
            std::cout << "populated: " << populated << std::endl;
            std::cout << "depth: " << (int)maxDepth << std::endl;
            counter = 0;
        }
        counter++;
        
        uint32_t rank = indexer.getEdgeIndex2(current);
        uint32_t databaseVal = (*database)[rank];

        if (databaseVal == 0)
        {
            populated++;
            (*database)[rank] = current.getDepth();
        }
        
        // dont generate neighbours if max depth is reached
        // or database already has entry
        if (current.getDepth() == maxDepth || databaseVal < current.getDepth())
        {
            continue;
        }

        neighbours = current.generateNeighbours();

        for (Cube neighbour : neighbours)
        {
            open.push(neighbour);
        }
    }

    std::cout << "Searched depth: " << (int)maxDepth << std::endl;
    std::cout << "Database entries filled: " << populated << std::endl;
}




void PDBBuilder::buildEdgeOrient(void)
{   
    Indexer indexer;

    std::unordered_set<int16_t> closed;
    std::queue<Cube> open;
    std::vector<Cube> neighbours;
    std::array<uint8_t, NUM_EDGE_ORIENTATIONS>* database = new std::array<uint8_t, NUM_EDGE_ORIENTATIONS>{0};

    Cube cube;
    open.push(cube);
    closed.insert(indexer.getEdgeOrientRankCube(cube));

    int populated = 0;

    while (!open.empty())
    {
        Cube current = open.front();
        open.pop();

        uint16_t rank = indexer.getEdgeOrientRankCube(current);
        uint16_t databaseVal = (*database)[rank];

        if (databaseVal == 0)
        {
            (*database)[rank] = current.getDepth();
            populated++;
        }
        
        if (databaseVal != 0)
        {   
            // don't generate neighbours if state reached before
            continue;
        }

        neighbours = current.generateNeighbours();

        for (Cube neighbour : neighbours)
        {   
            uint32_t rank = indexer.getEdgeOrientRankCube(neighbour);
            
            if (!closed.count(rank))
            {
                open.push(neighbour);
                closed.insert(rank);
            }
        }
    }

    printf("populated: %d\n", populated);

    std::ofstream writer("databases/edgeOrientDB safe.data", std::ios::out | std::ios::binary | std::ios::trunc);
    writer.write((char*)database, sizeof(uint8_t) * NUM_EDGE_ORIENTATIONS);
    writer.close();

    delete database;
}