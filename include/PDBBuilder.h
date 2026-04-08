#pragma once

#include <array>

#include <cstdint>



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



    private:
        PDBBuilder();
        static const int MAX_DEPTH = 11;
        static const uint8_t UNVISITED = 255;
        

        // Frontier-less parallel BFS
        static void fillEdgesParallel(std::array<uint8_t, NUM_EDGE_RANKS>* database, int edgeGroup, uint32_t numThreads);
        static void fillCornersParallel(std::array<uint8_t, NUM_CORNER_RANKS>* database, uint32_t numThreads);
};