#pragma once

#include <string>
#include <cstdint>

#include <atomic>
#include <thread>
#include <vector>
#include "PDB.h"

// Removed CubeState. State is now bitpacked into 3 uint32_t registers to 
// hit the exact 6-argument limit of x64 SysV calling convention.

struct SearchContext {
    const uint8_t* edge1DB;
    const uint8_t* edge2DB;
    const uint8_t* cornerDB;
    int* solution;
    long long statesChecked;
    int maxDepth;
    std::atomic<bool>* found;
};

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

        // Pre-load pattern databases into memory. Must be called once before solveWithPDB.
        static void init();

        // Solve using already-loaded databases.
        static std::string solveWithPDB(Cube cube);
        
        static void cleanup();

    private:
        static std::string solveWithPDB(Cube cube, PDB* pdb);
        Solver(void);

        // Recursive IDA* DFS on decomposed coordinates
        static bool dfs(uint32_t e1Idx, uint32_t e2Idx, uint32_t cState, 
                        int distance, int lastMove, SearchContext& ctx);

        // Precomputed allowed moves table to reduce branching in hot loop
        struct MoveList {
            int numMoves;
            int allowed[NUM_MOVES];
        };
        static const MoveList MoveTable[NUM_MOVES + 1];

        static PDB* sharedPDB;

        // Convert solution (array of move indices) to string
        static std::string movesToString(int* solution, int length, const Cube& cube);
};