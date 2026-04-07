#include <thread>
#include <atomic>
#include <mutex>
#include <algorithm>
#include <unordered_set>
#include <queue>
#include <stack>
#include <fstream>
#include <iostream>
#include <vector>

#include "PDBBuilder.h"
#include "Cube.h"
#include "Indexer.h"
#include "ProgressBar.h"
#include "DatabaseConstants.h"
#include "TransitionTable.h"

PDBBuilder::PDBBuilder()
{
}


void PDBBuilder::buildCorners(uint32_t numThreads)
{
    std::array<uint8_t, NUM_CORNER_RANKS>* database = new std::array<uint8_t, NUM_CORNER_RANKS>;
    fillCornersParallel(database, numThreads);

    std::ofstream writer(DatabaseConstants::CORNER_DB, std::ios::out | std::ios::binary | std::ios::trunc);
    writer.write((char*)database, sizeof(uint8_t) * NUM_CORNER_RANKS);
    writer.close();

    delete database;
}

void PDBBuilder::buildEdges1(uint32_t numThreads)
{
    std::array<uint8_t, NUM_EDGE_RANKS>* database = new std::array<uint8_t, NUM_EDGE_RANKS>;
    fillEdgesParallel(database, 1, numThreads);

    std::ofstream writer(DatabaseConstants::EDGE1_DB, std::ios::out | std::ios::binary | std::ios::trunc);
    writer.write((char*)database, sizeof(uint8_t) * NUM_EDGE_RANKS);
    writer.close();

    delete database;
}

void PDBBuilder::buildEdges2(uint32_t numThreads)
{
    std::array<uint8_t, NUM_EDGE_RANKS>* database = new std::array<uint8_t, NUM_EDGE_RANKS>;
    fillEdgesParallel(database, 2, numThreads);

    std::ofstream writer(DatabaseConstants::EDGE2_DB, std::ios::out | std::ios::binary | std::ios::trunc);
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




void PDBBuilder::buildEdgeOrient(uint32_t numThreads)
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

    std::ofstream writer(DatabaseConstants::ORIENT_DB, std::ios::out | std::ios::binary | std::ios::trunc);
    writer.write((char*)database, sizeof(uint8_t) * NUM_EDGE_ORIENTATIONS);
    writer.close();

    delete database;
}

void PDBBuilder::fillEdgesParallel(std::array<uint8_t, NUM_EDGE_RANKS>* database, int edgeGroup, uint32_t numThreads)
{
    // Initialize with UNVISITED
    std::fill(database->begin(), database->end(), UNVISITED);

    Indexer indexer;
    Cube solved;
    uint32_t solvedRank = (edgeGroup == 1) ? indexer.getEdgeIndex1(solved) : indexer.getEdgeIndex2(solved);
    (*database)[solvedRank] = 0;

    if (numThreads == 0)
    {
        numThreads = std::thread::hardware_concurrency();
        if (numThreads == 0) numThreads = 1;
    }

    ProgressBar progress(edgeGroup == 1 ? "Edge 1 " : "Edge 2 ", NUM_EDGE_RANKS);
    uint64_t totalPopulated = 1;

    for (uint8_t d = 0; d < 10; d++)
    {
        progress.setDepth(d, 10);
        std::vector<std::thread> threads;
        uint32_t chunkSize = NUM_EDGE_RANKS / numThreads;
        std::atomic<uint64_t> newlyPopulated{0};

        for (uint32_t t = 0; t < numThreads; t++)
        {
            uint32_t start = t * chunkSize;
            uint32_t end = (t == numThreads - 1) ? NUM_EDGE_RANKS : (t + 1) * chunkSize;

            threads.emplace_back([&, start, end, d]() {
                uint64_t localPopulated = 0;
                for (uint32_t i = start; i < end; i++)
                {
                    if ((*database)[i] == d)
                    {
                        uint32_t permRank = i >> 7;
                        uint8_t orientRank = i & 0x7F;

                        for (int move = 0; move < 18; move++)
                        {
                            uint32_t nextPermRank = TransitionTable::edgePerm[permRank][move];
                            uint8_t flipMask = TransitionTable::edgeFlipMask[permRank][move];
                            uint8_t nextOrientRank = orientRank ^ flipMask;
                            uint32_t nextRank = (nextPermRank << 7) | nextOrientRank;

                            std::atomic<uint8_t>* atomicDb = reinterpret_cast<std::atomic<uint8_t>*>(database->data());
                            uint8_t expected = UNVISITED;
                            if (atomicDb[nextRank].compare_exchange_strong(expected, d + 1))
                            {
                                localPopulated++;
                            }
                        }
                    }
                }
                newlyPopulated += localPopulated;
            });
        }

        for (auto& thread : threads) thread.join();
        totalPopulated += newlyPopulated;
        progress.update(totalPopulated);
    }

    // Set remaining UNVISITED to 11
    for (uint32_t i = 0; i < NUM_EDGE_RANKS; i++)
    {
        if ((*database)[i] == UNVISITED)
        {
            (*database)[i] = 11;
        }
    }
    progress.update(NUM_EDGE_RANKS);
    progress.finish();
}

void PDBBuilder::fillCornersParallel(std::array<uint8_t, NUM_CORNER_RANKS>* database, uint32_t numThreads)
{
    // Initialize with UNVISITED
    std::fill(database->begin(), database->end(), UNVISITED);

    Indexer indexer;
    Cube solved;
    uint32_t solvedRank = indexer.getCornerIndex(solved);
    (*database)[solvedRank] = 0;

    if (numThreads == 0)
    {
        numThreads = std::thread::hardware_concurrency();
        if (numThreads == 0) numThreads = 1;
    }
    ProgressBar progress("Corners ", NUM_CORNER_RANKS);
    uint64_t totalPopulated = 1;

    for (uint8_t d = 0; d < 10; d++)
    {
        progress.setDepth(d, 10);
        std::vector<std::thread> threads;
        uint32_t chunkSize = NUM_CORNER_RANKS / numThreads;
        std::atomic<uint64_t> newlyPopulated{0};

        for (uint32_t t = 0; t < numThreads; t++)
        {
            uint32_t start = t * chunkSize;
            uint32_t end = (t == numThreads - 1) ? NUM_CORNER_RANKS : (t + 1) * chunkSize;

            threads.emplace_back([&, start, end, d]() {
                uint64_t localPopulated = 0;
                std::atomic<uint8_t>* atomicDb = reinterpret_cast<std::atomic<uint8_t>*>(database->data());
                for (uint32_t i = start; i < end; i++)
                {
                    if ((*database)[i] == d)
                    {
                        uint32_t permRank = i / TransitionTable::NUM_CORNER_ORIENTS;
                        uint16_t orientRank = i % TransitionTable::NUM_CORNER_ORIENTS;

                        for (int move = 0; move < 18; move++)
                        {
                            uint32_t nextPermRank = TransitionTable::cornerPerm[permRank][move];
                            uint16_t nextOrientRank = TransitionTable::cornerOrient[orientRank][move];
                            uint32_t nextRank = nextPermRank * TransitionTable::NUM_CORNER_ORIENTS + nextOrientRank;
                            uint8_t expected = UNVISITED;
                            if (atomicDb[nextRank].compare_exchange_strong(expected, d + 1))
                            {
                                localPopulated++;
                            }
                        }
                    }
                }
                newlyPopulated += localPopulated;
            });
        }

        for (auto& thread : threads) thread.join();
        totalPopulated += newlyPopulated;
        progress.update(totalPopulated);
    }

    // Set remaining UNVISITED to 11
    for (uint32_t i = 0; i < NUM_CORNER_RANKS; i++)
    {
        if ((*database)[i] == UNVISITED)
        {
            (*database)[i] = 11;
        }
    }
    progress.update(NUM_CORNER_RANKS);
    progress.finish();
}