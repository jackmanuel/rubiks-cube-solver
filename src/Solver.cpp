#include <iostream>
#include <chrono>

#include "Solver.h"
#include "Cube.h"
#include "PDB.h"
#include "Indexer.h"
#include "TransitionTable.h"
#include "DatabaseConstants.h"

PDB* Solver::sharedPDB = nullptr;

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

const Solver::MoveList Solver::MoveTable[19] = {
    // lastMove = -1 (all moves allowed)
    { 18, { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17 } },
    // lastMove = 0, 1, 2 (R) -> prune R (0,1,2). keep U,F,D,B,L
    { 15, { 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 0, 0, 0 } },
    { 15, { 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 0, 0, 0 } },
    { 15, { 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 0, 0, 0 } },
    // lastMove = 3, 4, 5 (U) -> prune U (3,4,5). keep R,F,D,B,L
    { 15, { 0, 1, 2, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 0, 0, 0 } },
    { 15, { 0, 1, 2, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 0, 0, 0 } },
    { 15, { 0, 1, 2, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 0, 0, 0 } },
    // lastMove = 6, 7, 8 (F) -> prune F (6,7,8). keep R,U,D,B,L
    { 15, { 0, 1, 2, 3, 4, 5, 9, 10, 11, 12, 13, 14, 15, 16, 17, 0, 0, 0 } },
    { 15, { 0, 1, 2, 3, 4, 5, 9, 10, 11, 12, 13, 14, 15, 16, 17, 0, 0, 0 } },
    { 15, { 0, 1, 2, 3, 4, 5, 9, 10, 11, 12, 13, 14, 15, 16, 17, 0, 0, 0 } },
    // lastMove = 9, 10, 11 (D) -> prune D (9,10,11) and U (3,4,5). keep R,F,B,L
    { 12, { 0, 1, 2, 6, 7, 8, 12, 13, 14, 15, 16, 17, 0, 0, 0, 0, 0, 0 } },
    { 12, { 0, 1, 2, 6, 7, 8, 12, 13, 14, 15, 16, 17, 0, 0, 0, 0, 0, 0 } },
    { 12, { 0, 1, 2, 6, 7, 8, 12, 13, 14, 15, 16, 17, 0, 0, 0, 0, 0, 0 } },
    // lastMove = 12, 13, 14 (B) -> prune B (12,13,14) and F (6,7,8). keep R,U,D,L
    { 12, { 0, 1, 2, 3, 4, 5, 9, 10, 11, 15, 16, 17, 0, 0, 0, 0, 0, 0 } },
    { 12, { 0, 1, 2, 3, 4, 5, 9, 10, 11, 15, 16, 17, 0, 0, 0, 0, 0, 0 } },
    { 12, { 0, 1, 2, 3, 4, 5, 9, 10, 11, 15, 16, 17, 0, 0, 0, 0, 0, 0 } },
    // lastMove = 15, 16, 17 (L) -> prune L (15,16,17) and R (0,1,2). keep U,F,D,B
    { 12, { 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 0, 0, 0, 0, 0, 0 } },
    { 12, { 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 0, 0, 0, 0, 0, 0 } },
    { 12, { 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 0, 0, 0, 0, 0, 0 } }
};

bool Solver::dfs(
    uint32_t e1Idx, uint32_t e2Idx, uint32_t cState,
    int distance, int lastMove,
    SearchContext& ctx)
{
    // Extract properties instantly via registers
    uint32_t ePerm1 = e1Idx >> 7;
    uint8_t eOrient1 = e1Idx & 0x7F;
    uint32_t ePerm2 = e2Idx >> 7;
    uint8_t eOrient2 = e2Idx & 0x7F;
    uint16_t cPerm = cState >> 16;
    uint16_t cOrient = cState & 0xFFFF;

    // Heuristic Lookup: Edge databases are checked first as they are more likely to prune.
    uint8_t h = ctx.edge1DB[e1Idx];
    if (h > distance) return false;

    uint8_t h2 = ctx.edge2DB[e2Idx];
    if (h2 > distance) return false;
    if (h2 > h) h = h2;

    uint32_t cIdx  = (uint32_t)cPerm * 2187 + cOrient;
    uint8_t hCorner = ctx.cornerDB[cIdx];
    if (hCorner > distance) return false;
    if (hCorner > h) h = hCorner;

    ctx.statesChecked++;
    if ((ctx.statesChecked & 0xFFFFF) == 0)
    {
        std::cout << "\r\033[Ksearching depth " << ctx.maxDepth << "... (" << formatNumber(ctx.statesChecked) << " states checked)" << std::flush;
    }

    // Goal test: heuristic is 0 if and only if the cube is solved.
    // This works because the pattern databases collectively cover all 20 movable pieces.
    if (h == 0) return true;

    const MoveList& nextMoves = MoveTable[lastMove + 1];
    int currentDepth = ctx.maxDepth - distance;

    // Try all allowed next moves
    for (int i = 0; i < nextMoves.numMoves; i++)
    {
        int move = nextMoves.allowed[i];

        ctx.solution[currentDepth] = move;

        uint32_t nextE1Idx = (TransitionTable::edgePerm[ePerm1][move] << 7) |
                             (eOrient1 ^ TransitionTable::edgeFlipMask[ePerm1][move]);
        uint32_t nextE2Idx = (TransitionTable::edgePerm[ePerm2][move] << 7) |
                             (eOrient2 ^ TransitionTable::edgeFlipMask[ePerm2][move]);
        uint32_t nextCState = ((uint32_t)TransitionTable::cornerPerm[cPerm][move] << 16) |
                               TransitionTable::cornerOrient[cOrient][move];

        if (dfs(nextE1Idx, nextE2Idx, nextCState, distance - 1, move, ctx))
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

    return solveWithPDB(cube, &pdb);
}

void Solver::init()
{
    if (sharedPDB == nullptr) {
        sharedPDB = new PDB(DatabaseConstants::CORNER_DB, DatabaseConstants::EDGE1_DB,
                           DatabaseConstants::EDGE2_DB, DatabaseConstants::ORIENT_DB);
    }
}

void Solver::cleanup()
{
    if (sharedPDB != nullptr) {
        delete sharedPDB;
        sharedPDB = nullptr;
    }
}

std::string Solver::solveWithPDB(Cube cube)
{
    if (sharedPDB == nullptr) {
        throw std::runtime_error("Pattern databases not initialized. Call Solver::init() first.");
    }
    return solveWithPDB(cube, sharedPDB);
}

std::string Solver::solveWithPDB(Cube cube, PDB* pdb)
{
    // Extract decomposed coordinates from the scrambled cube (done ONCE)
    Indexer indexer;
    uint16_t cPerm    = (uint16_t)indexer.getCornerPermRankFromCube(cube);
    uint16_t cOrient  = (uint16_t)indexer.getCornerOrientRankFromCube(cube);
    uint32_t ePerm1   = (uint32_t)indexer.getEdgePermRank1FromCube(cube);
    uint8_t  eOrient1 = (uint8_t)indexer.getEdgeOrientRank1FromCube(cube);
    uint32_t ePerm2   = (uint32_t)indexer.getEdgePermRank2FromCube(cube);
    uint8_t  eOrient2 = (uint8_t)indexer.getEdgeOrientRank2FromCube(cube);

    uint32_t e1Idx = (ePerm1 << 7) | eOrient1;
    uint32_t e2Idx = (ePerm2 << 7) | eOrient2;
    uint32_t cState = ((uint32_t)cPerm << 16) | cOrient;
    uint32_t cIdx = cPerm * 2187 + cOrient;

    uint8_t h = pdb->getEdge1DB()[e1Idx];
    uint8_t h2 = pdb->getEdge2DB()[e2Idx];
    uint8_t hCorner = pdb->getCornerDB()[cIdx];

    if (h2 > h) h = h2;
    if (hCorner > h) h = hCorner;

    int maxDepth = (int)h;

    // Check if already solved
    if (h == 0)
    {
        std::cout << "The cube is already solved!" << std::endl;
        return "";
    }

    std::cout << "Starting the search at depth " << maxDepth << std::endl;

    int solution[MAX_MOVES];
    long long totalStatesChecked = 0;

    SearchContext ctx;
    ctx.edge1DB = pdb->getEdge1DB().data();
    ctx.edge2DB = pdb->getEdge2DB().data();
    ctx.cornerDB = pdb->getCornerDB().data();
    ctx.solution = solution;

    auto startTime = std::chrono::high_resolution_clock::now();

    // IDA* loop: iteratively deepen until solution found
    while (maxDepth <= MAX_MOVES)
    {
        std::cout << "\r\033[Ksearching depth " << maxDepth << "... (" << formatNumber(0) << " states checked)" << std::flush;

        ctx.statesChecked = 0;
        ctx.maxDepth = maxDepth;

        if (dfs(e1Idx, e2Idx, cState, maxDepth, -1, ctx))
        {
            totalStatesChecked += ctx.statesChecked;
            auto endTime = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> elapsed = endTime - startTime;
            double seconds = elapsed.count();
            double statesPerSec = (seconds > 0.0) ? (totalStatesChecked / seconds) : 0.0;

            std::cout << std::endl << "SOLUTION FOUND!" << std::endl;
            std::cout << "Total states: " << formatNumber(totalStatesChecked) << std::endl;
            std::cout << "Search time:  " << seconds << " seconds" << std::endl;
            std::cout << "Speed:        " << formatNumber(static_cast<long long>(statesPerSec)) << " states/sec" << std::endl;

            return movesToString(solution, maxDepth, cube);
        }

        totalStatesChecked += ctx.statesChecked;
        std::cout << "\r\033[Ksearching depth " << maxDepth << "... done (" << formatNumber(ctx.statesChecked) << " states checked)" << std::flush;
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

std::string Solver::movesToString(int* solution, int length, const Cube& cube)
{
    std::string result;
    for (int i = 0; i < length; i++)
    {
        if (i > 0) result.append(" ");
        int array_move = solution[i];
        
        // Extract the base Face (0-5) and rotation amount (0-2) from the move integer
        Cube::Face target_array_face = static_cast<Cube::Face>(array_move / 3);
        int amount = array_move % 3;

        // Find which physical Room Face currently houses this Array Face
        Cube::Face room_face = Cube::R_FACE;
        for (int f = 0; f < 6; f++) {
            if (cube.getMap()[f] == target_array_face) {
                room_face = static_cast<Cube::Face>(f);
                break;
            }
        }

        // Reconstruct the physical Room move
        int room_move = (static_cast<int>(room_face) * 3) + amount;
        result.append(MOVE_NAMES[room_move]);
    }
    return result;
}
