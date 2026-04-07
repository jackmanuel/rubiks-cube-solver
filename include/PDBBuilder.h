#pragma once

/* ideas:

iterate through ~420M states that are 11 moves or fewer from solved

for every unique state, add 24 entries to the PDB for 24 symmetries


*/

#include <string>
#include <cstdint>

#include "Indexer.h"

class ProgressBar;

class PDBBuilder
{
    public:
        static const int NUM_CORNER_RANKS = 88179840;
        static const int NUM_EDGE_RANKS = 510935040;
        static const int NUM_EDGE_ORIENTATIONS = 2048;

        static void buildCorners(uint32_t numThreads = 1);
        static void buildEdges1(uint32_t numThreads = 0);
        static void buildEdges2(uint32_t numThreads = 0);
        static void buildEdgeOrient(uint32_t numThreads = 1);

        static std::array<uint8_t, NUM_CORNER_RANKS>* getCornerPDB();
        static std::array<uint8_t, NUM_EDGE_RANKS>* getEdgePDB(std::string filename);

        static std::array<uint8_t, NUM_EDGE_RANKS>* testDFS(int depth);

    private:
        PDBBuilder();
        static const int MAX_DEPTH = 11;
        static const uint8_t UNVISITED = 255;
        
        static void DFS(Indexer indexer, uint8_t maxDepth, std::array<uint8_t, NUM_EDGE_RANKS>* database,
                        ProgressBar* progress = nullptr, uint64_t* totalPopulated = nullptr);
        static void DFS2(Indexer indexer, uint8_t maxDepth, std::array<uint8_t, NUM_EDGE_RANKS>* database,
                         ProgressBar* progress = nullptr, uint64_t* totalPopulated = nullptr);

        // Frontier-less parallel BFS
        // Frontier-less parallel BFS
        static void fillEdgesParallel(std::array<uint8_t, NUM_EDGE_RANKS>* database, int edgeGroup, uint32_t numThreads);
        static void fillCornersParallel(std::array<uint8_t, NUM_CORNER_RANKS>* database, uint32_t numThreads);
};