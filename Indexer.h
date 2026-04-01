#pragma once

#include <array>
#include <cstdint>

#include "Cube.h"

class Indexer
{
    
    public:
        Indexer();

        static const int NUM_EDGES_IN_PDB = 7;

        // Combined PDB indices (original API, still used for PDB building and initial state)
        uint32_t getCornerIndex(Cube cube);
        uint32_t getEdgeIndex1(Cube cube);
        uint32_t getEdgeIndex2(Cube cube);
        uint16_t getEdgeOrientRankCube(Cube cube);

        // Decomposed ranking (public for transition table generation and initial state)
        int getCornerPermRank(std::array<Cube::Cubie, Cube::NUM_CORNERS>& corners);
        int getCornerOrientRank(std::array<Cube::Cubie, Cube::NUM_CORNERS>& corners);

        template<std::size_t SIZE>
        int getEdgePermRank(std::array<Cube::Cubie, Cube::NUM_EDGES>& edges, std::array<int, SIZE> edgeSet);

        template<std::size_t SIZE>
        int getEdgeOrientRank(std::array<Cube::Cubie, Cube::NUM_EDGES>& edges, std::array<int, SIZE> edgeSet);

        // Decomposed ranking from Cube (convenience wrappers)
        int getCornerPermRankFromCube(Cube cube);
        int getCornerOrientRankFromCube(Cube cube);
        int getEdgePermRank1FromCube(Cube cube);
        int getEdgeOrientRank1FromCube(Cube cube);
        int getEdgePermRank2FromCube(Cube cube);
        int getEdgeOrientRank2FromCube(Cube cube);

        // Unranking: reconstruct state from rank (for transition table generation)
        void unrankCornerPerm(int permRank, std::array<uint8_t, Cube::NUM_CORNERS>& perm);
        void unrankCornerOrient(int orientRank, std::array<uint8_t, Cube::NUM_CORNERS>& orient);
        void unrankEdgePerm(int permRank, std::array<int, NUM_EDGES_IN_PDB>& positions);
        void unrankFullEdgeOrient(uint16_t orientRank, std::array<uint8_t, Cube::NUM_EDGES>& orient);

    private:

        std::array<uint8_t, (1 << Cube::NUM_EDGES)> onesArray;
        std::array<int, Cube::NUM_CORNERS> factorial;
        std::array<int, Cube::NUM_CORNERS> power3;
        std::array<int, Indexer::NUM_EDGES_IN_PDB> perms;
        std::array<int, Indexer::NUM_EDGES_IN_PDB> edgeSet1;
        std::array<int, Indexer::NUM_EDGES_IN_PDB> edgeSet2;
        std::array<int, Cube::NUM_EDGES> edgeSetAll;

        std::array<uint8_t, Cube::NUM_CORNERS> getLehmerCode(Cube cube);
        int cornerLehmerToBase10(std::array<uint8_t, Cube::NUM_CORNERS>& lehmerCode);

        template<std::size_t SIZE>
        int edgeLehmerToBase10(std::array<uint8_t, SIZE>& lehmerCode);

        int fac(int n);
        int permutations(int n, int k);
};