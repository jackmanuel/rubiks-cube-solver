#pragma once

#include <string>
#include <cstdint>

#include "PDB.h"

class Solver
{
    public:
        static const int MAX_MOVES = 20;
        static const int NUM_MOVES = 18;

        // The solved state permutation rank for edge group 2 (edges 5-11) is 1831445,
        // because unlike group 1, it tracks edges positioned at indices 5-11.
        // e2Idx = ePerm2 * 128 + eOrient2 = 1831445 * 128 + 0 = 234424960
        static const uint32_t SOLVED_E2_IDX = 234424960;

        // Solve using coordinate-level IDA* with transition tables.
        // The Cube is used only once to extract initial coordinates.
        static std::string solve(Cube cube);

    private:
        Solver(void);

        // Recursive IDA* DFS on decomposed coordinates
        static bool dfs(
            uint16_t cPerm, uint16_t cOrient,
            uint32_t ePerm1, uint8_t eOrient1,
            uint32_t ePerm2, uint8_t eOrient2,
            uint16_t orient,
            int depth, int maxDepth, int lastMove,
            int* solution,
            long long& statesChecked,
            const PDB& pdb);

        // Precomputed allowed moves table to reduce branching in hot loop
        struct MoveList {
            int numMoves;
            int allowed[NUM_MOVES];
        };
        static const MoveList MoveTable[NUM_MOVES + 1];

        // Convert solution (array of move indices) to string
        static std::string movesToString(int* solution, int length);
};