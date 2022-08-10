#pragma once

#include "Cube.h"



class Indexer
{
    
    public:
        Indexer();
        uint32_t getCornerIndex(Cube cube);
        uint32_t getEdgeIndex1(Cube cube);
        uint32_t getEdgeIndex2(Cube cube);

    private:

        static const int NUM_EDGES_IN_PDB = 7;

        static const int E1_START_IND = 0;
        static const int E1_END_IND = 6;

        static const int E2_START_IND = 5;
        static const int E2_END_IND = 11;

        std::array<uint8_t, (1 << Cube::NUM_EDGES)> onesArray;
        std::array<int, Cube::NUM_CORNERS> factorial;
        std::array<int, Cube::NUM_CORNERS> power3;
        std::array<int, Indexer::NUM_EDGES_IN_PDB> perms;

        int getCornerPermRank(std::array<Cube::Cubie, Cube::NUM_CORNERS>& corners);
        int getCornerOrientRank(std::array<Cube::Cubie, Cube::NUM_CORNERS>& corners);

        int getEdgeOrientRank(std::array<Cube::Cubie, Cube::NUM_EDGES>& edges,  int startInd, int endInd);
        int getEdgePermRank(std::array<Cube::Cubie, Cube::NUM_EDGES>& edges, int startInd, int endInd);

        std::array<uint8_t, Cube::NUM_CORNERS> getLehmerCode(Cube cube);
        int cornerLehmerToBase10(std::array<uint8_t, Cube::NUM_CORNERS>& lehmerCode);
        int edgeLehmerToBase10(std::array<uint8_t, Indexer::NUM_EDGES_IN_PDB>& lehmerCode);

        int fac(int n);
        int permutations(int n, int k);
};