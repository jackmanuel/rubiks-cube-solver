#pragma once

#include <atomic>
#include <cstdint>
#include <string>

#include "PDB.h"

// State is bitpacked into 3 uint32_t registers to hit the exact 
// 6-argument limit of x64 SysV calling convention.

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