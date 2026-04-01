#include <iostream>
#include <chrono>

#include "Solver.h"
#include "Cube.h"
#include "PDB.h"
#include "Indexer.h"
#include "TransitionTable.h"
#include "DatabaseConstants.h"

// Move names indexed by move ID (0-17)
static const char* MOVE_NAMES[18] = {
    "R", "R'", "R2",
    "U", "U'", "U2",
    "F", "F'", "F2",
    "D", "D'", "D2",
    "B", "B'", "B2",
    "L", "L'", "L2"
};

static std::string formatNumber(long long n)
{
    std::string s = std::to_string(n);
    int insertPosition = s.length() - 3;
    while (insertPosition > 0) {
        s.insert(insertPosition, ",");
        insertPosition -= 3;
    }
    return s;
}

Solver::Solver(void)
{
}

bool Solver::shouldPrune(int move, int lastMove)
{
    if (lastMove < 0) return false;

    int face = move / 3;
    int lastFace = lastMove / 3;

    // Same face: always prune (e.g. R after R')
    if (face == lastFace) return true;

    // Opposite faces: prune one ordering to avoid duplicates.
    // Face pairs: R(0)↔L(5), U(1)↔D(3), F(2)↔B(4)
    // Convention: if opposite face, only allow the lower-numbered face first.
    bool opposite = false;
    if ((face == 0 && lastFace == 5) || (face == 5 && lastFace == 0)) opposite = true;
    if ((face == 1 && lastFace == 3) || (face == 3 && lastFace == 1)) opposite = true;
    if ((face == 2 && lastFace == 4) || (face == 4 && lastFace == 2)) opposite = true;

    if (opposite && lastFace > face) return true;

    return false;
}

bool Solver::dfs(
    uint16_t cPerm, uint16_t cOrient,
    uint32_t ePerm1, uint8_t eOrient1,
    uint32_t ePerm2, uint8_t eOrient2,
    uint16_t orient,
    int depth, int maxDepth, int lastMove,
    int* solution,
    long long& statesChecked,
    const PDB& pdb)
{
    // Heuristic Lookup: Edge databases are checked first as they are more likely to prune.
    uint32_t e1Idx = (uint32_t)ePerm1 * 128 + eOrient1;
    uint8_t h = pdb.getEdge1DB()[e1Idx];
    if (depth + h > maxDepth) return false;

    uint32_t e2Idx = (uint32_t)ePerm2 * 128 + eOrient2;
    uint8_t h2 = pdb.getEdge2DB()[e2Idx];
    if (depth + h2 > maxDepth) return false;
    if (h2 > h) h = h2;

    uint32_t cIdx  = (uint32_t)cPerm * 2187 + cOrient;
    uint8_t hCorner = pdb.getCornerDB()[cIdx];
    if (depth + hCorner > maxDepth) return false;
    if (hCorner > h) h = hCorner;

    uint8_t hOrient = pdb.getOrientDB()[orient];
    if (depth + hOrient > maxDepth) return false;
    if (hOrient > h) h = hOrient;

    statesChecked++;
    if ((statesChecked & 0xFFFFF) == 0)
    {
        std::cout << "\rsearching depth " << maxDepth << "... (" << formatNumber(statesChecked) << " states checked)      " << std::flush;
    }

    // Goal test: heuristic is 0 if and only if the cube is solved.
    // This works because the pattern databases collectively cover all 20 movable pieces.
    if (h == 0) return true;

    // Try all 18 moves
    for (int move = 0; move < NUM_MOVES; move++)
    {
        if (shouldPrune(move, lastMove)) continue;

        solution[depth] = move;

        if (dfs(
            TransitionTable::cornerPerm[cPerm][move],
            TransitionTable::cornerOrient[cOrient][move],
            TransitionTable::edgePerm[ePerm1][move],
            eOrient1 ^ TransitionTable::edgeFlipMask[ePerm1][move],
            TransitionTable::edgePerm[ePerm2][move],
            eOrient2 ^ TransitionTable::edgeFlipMask[ePerm2][move],
            TransitionTable::fullOrient[orient][move],
            depth + 1, maxDepth, move,
            solution, statesChecked, pdb))
        {
            return true;
        }
    }

    return false;
}

std::string Solver::solve(Cube cube)
{
    // Load pattern databases
    PDB pdb(DatabaseConstants::CORNER_DB, DatabaseConstants::EDGE1_DB,
            DatabaseConstants::EDGE2_DB, DatabaseConstants::ORIENT_DB);

    // Extract decomposed coordinates from the scrambled cube (done ONCE)
    Indexer indexer;
    uint16_t cPerm    = (uint16_t)indexer.getCornerPermRankFromCube(cube);
    uint16_t cOrient  = (uint16_t)indexer.getCornerOrientRankFromCube(cube);
    uint32_t ePerm1   = (uint32_t)indexer.getEdgePermRank1FromCube(cube);
    uint8_t  eOrient1 = (uint8_t)indexer.getEdgeOrientRank1FromCube(cube);
    uint32_t ePerm2   = (uint32_t)indexer.getEdgePermRank2FromCube(cube);
    uint8_t  eOrient2 = (uint8_t)indexer.getEdgeOrientRank2FromCube(cube);
    uint16_t orient   = indexer.getEdgeOrientRankCube(cube);

    uint32_t e1Idx = (uint32_t)ePerm1 * 128 + eOrient1;
    uint32_t e2Idx = (uint32_t)ePerm2 * 128 + eOrient2;
    uint32_t cIdx  = (uint32_t)cPerm * 2187 + cOrient;

    uint8_t h = pdb.getEdge1DB()[e1Idx];
    uint8_t h2 = pdb.getEdge2DB()[e2Idx];
    uint8_t hCorner = pdb.getCornerDB()[cIdx];
    uint8_t hOrient = pdb.getOrientDB()[orient];

    if (h2 > h) h = h2;
    if (hCorner > h) h = hCorner;
    if (hOrient > h) h = hOrient;

    int maxDepth = (int)h;

    // Check if already solved
    if (h == 0)
    {
        std::cout << "The cube is already solved!" << std::endl;
        return "";
    }

    std::cout << "Starting the search at depth " << maxDepth << std::endl;

    int solution[MAX_MOVES];
    long long statesChecked = 0;
    long long totalStatesChecked = 0;

    auto startTime = std::chrono::high_resolution_clock::now();

    // IDA* loop: iteratively deepen until solution found
    while (maxDepth <= MAX_MOVES)
    {
        std::cout << "\rsearching depth " << maxDepth << "... (" << formatNumber(0) << " states checked)      " << std::flush;

        statesChecked = 0;

        if (dfs(cPerm, cOrient, ePerm1, eOrient1, ePerm2, eOrient2, orient,
                0, maxDepth, -1, solution, statesChecked, pdb))
        {
            totalStatesChecked += statesChecked;
            auto endTime = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> elapsed = endTime - startTime;
            double seconds = elapsed.count();
            double statesPerSec = (seconds > 0.0) ? (totalStatesChecked / seconds) : 0.0;

            std::cout << std::endl << "SOLUTION FOUND!" << std::endl;
            std::cout << "Total states: " << formatNumber(totalStatesChecked) << std::endl;
            std::cout << "Search time:  " << seconds << " seconds" << std::endl;
            std::cout << "Speed:        " << formatNumber(static_cast<long long>(statesPerSec)) << " states/sec" << std::endl;

            return movesToString(solution, maxDepth);
        }

        totalStatesChecked += statesChecked;
        std::cout << "\rsearching depth " << maxDepth << "... done (" << formatNumber(statesChecked) << " states checked)      " << std::flush;
        maxDepth++;
    }

    auto endTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = endTime - startTime;
    double seconds = elapsed.count();
    double statesPerSec = (seconds > 0.0) ? (totalStatesChecked / seconds) : 0.0;

    std::cout << std::endl << "No solution found within " << MAX_MOVES << " moves." << std::endl;
    std::cout << "Total states: " << formatNumber(totalStatesChecked) << std::endl;
    std::cout << "Search time:  " << seconds << " seconds" << std::endl;
    std::cout << "Speed:        " << formatNumber(static_cast<long long>(statesPerSec)) << " states/sec" << std::endl;
    return "";
}

std::string Solver::movesToString(int* solution, int length)
{
    std::string result;
    for (int i = 0; i < length; i++)
    {
        if (i > 0) result.append(" ");
        result.append(MOVE_NAMES[solution[i]]);
    }
    return result;
}
