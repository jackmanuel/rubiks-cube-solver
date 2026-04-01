#include <unordered_set>
#include <queue>
#include <iostream>
#include <vector>
#include <fstream>
#include <stack>

#include "PDBBuilder.h"
#include "Cube.h"
#include "Indexer.h"
#include "ProgressBar.h"

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

    uint64_t populated = 0;
    ProgressBar progress("Corners", NUM_CORNER_RANKS);

    while (!open.empty()){
        Cube current = open.front();
        open.pop();

        uint32_t rank = indexer.getCornerIndex(current);
        uint32_t databaseVal = (*database)[rank];

        if (databaseVal == 0)
        {
            (*database)[rank] = current.getDepth();
            populated++;
            progress.update(populated);
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

    progress.finish();

    std::ofstream writer("databases/cornerDB.data", std::ios::out | std::ios::binary | std::ios::trunc);
    writer.write((char*)database, sizeof(uint8_t) * NUM_CORNER_RANKS);
    writer.close();

    delete database;
}

void PDBBuilder::buildEdges1(void)
{
    Indexer indexer;
    std::array<uint8_t, NUM_EDGE_RANKS>* database = new std::array<uint8_t, NUM_EDGE_RANKS>{0};

    uint64_t totalPopulated = 0;
    ProgressBar progress("Edge 1 ", NUM_EDGE_RANKS);

    for (uint8_t currDepth = 0; currDepth <= 10; currDepth++)
    {
        progress.setDepth(currDepth, 10);
        DFS(indexer, currDepth, database, &progress, &totalPopulated);
    }

    progress.finish();

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

    uint64_t totalPopulated = 0;
    ProgressBar progress("Edge 2 ", NUM_EDGE_RANKS);

    for (uint8_t currDepth = 0; currDepth <= 10; currDepth++)
    {
        progress.setDepth(currDepth, 10);
        DFS2(indexer, currDepth, database, &progress, &totalPopulated);
    }

    progress.finish();

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


void PDBBuilder::DFS(Indexer indexer, uint8_t maxDepth, std::array<uint8_t, NUM_EDGE_RANKS>* database,
                     ProgressBar* progress, uint64_t* totalPopulated)
{
    std::stack<Cube> open;
    std::vector<Cube> neighbours;

    Cube cube;
    open.push(cube);

    while (!open.empty()){
        Cube current = open.top();
        open.pop();
        
        uint32_t rank = indexer.getEdgeIndex1(current);
        uint32_t databaseVal = (*database)[rank];

        if (databaseVal == 0)
        {
            (*database)[rank] = current.getDepth();
            if (totalPopulated && progress)
            {
                (*totalPopulated)++;
                progress->update(*totalPopulated);
            }
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
}


void PDBBuilder::DFS2(Indexer indexer, uint8_t maxDepth, std::array<uint8_t, NUM_EDGE_RANKS>* database,
                      ProgressBar* progress, uint64_t* totalPopulated)
{
    std::stack<Cube> open;
    std::vector<Cube> neighbours;

    Cube cube;
    open.push(cube);

    while (!open.empty()){
        Cube current = open.top();
        open.pop();
        
        uint32_t rank = indexer.getEdgeIndex2(current);
        uint32_t databaseVal = (*database)[rank];

        if (databaseVal == 0)
        {
            (*database)[rank] = current.getDepth();
            if (totalPopulated && progress)
            {
                (*totalPopulated)++;
                progress->update(*totalPopulated);
            }
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

    uint64_t populated = 0;
    ProgressBar progress("Orient ", NUM_EDGE_ORIENTATIONS);

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
            progress.update(populated);
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

    progress.finish();

    std::ofstream writer("databases/edgeOrientDB.data", std::ios::out | std::ios::binary | std::ios::trunc);
    writer.write((char*)database, sizeof(uint8_t) * NUM_EDGE_ORIENTATIONS);
    writer.close();

    delete database;
}