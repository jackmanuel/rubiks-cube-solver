#include <iostream>
#include <fstream>
#include <cstring>

#include "TransitionTable.h"
#include "Cube.h"
#include "Indexer.h"
#include "DatabaseConstants.h"
#include "ProgressBar.h"

namespace TransitionTable {

// Global table pointers
uint16_t (*cornerPerm)[NUM_MOVES] = nullptr;
uint16_t (*cornerOrient)[NUM_MOVES] = nullptr;
uint32_t (*edgePerm)[NUM_MOVES] = nullptr;
uint8_t  (*edgeFlipMask)[NUM_MOVES] = nullptr;
uint16_t (*fullOrient)[NUM_MOVES] = nullptr;

// ======================== Move Definitions ========================
// For each move, define how corner/edge positions are permuted and
// what orientation changes occur. This avoids using Cube objects for
// the small tables (corners, orient).

// Corner position permutation: cornerPosPerm[move][old_pos] = new_pos
// Derived from Cube.cpp's fourCycleCorners and swapCornerCubies calls.
// fourCycle(a,b,c,d): new[a]=old[b], new[b]=old[c], new[c]=old[d], new[d]=old[a]
// swap(a,b): new[a]=old[b], new[b]=old[a]
static const int CORNER_SOURCE[NUM_MOVES][8] = {
    // R: fourCycle(2,7,6,1)
    {0, 2, 7, 3, 4, 5, 1, 6},
    // R': fourCycle(2,1,6,7)
    {0, 6, 1, 3, 4, 5, 7, 2},
    // R2: swap(2,6), swap(1,7)
    {0, 7, 6, 3, 4, 5, 2, 1},
    // U: fourCycle(2,1,0,3)
    {3, 0, 1, 2, 4, 5, 6, 7},
    // U': fourCycle(2,3,0,1)
    {1, 2, 3, 0, 4, 5, 6, 7},
    // U2: swap(0,2), swap(3,1)
    {2, 3, 0, 1, 4, 5, 6, 7},
    // F: fourCycle(2,3,4,7)
    {0, 1, 3, 4, 7, 5, 6, 2},
    // F': fourCycle(2,7,4,3)
    {0, 1, 7, 2, 3, 5, 6, 4},
    // F2: swap(2,4), swap(3,7)
    {0, 1, 4, 7, 2, 5, 6, 3},
    // D: fourCycle(7,4,5,6)
    {0, 1, 2, 3, 5, 6, 7, 4},
    // D': fourCycle(4,7,6,5)
    {0, 1, 2, 3, 7, 4, 5, 6},
    // D2: swap(4,6), swap(7,5)
    {0, 1, 2, 3, 6, 7, 4, 5},
    // B: fourCycle(0,1,6,5)
    {1, 6, 2, 3, 4, 0, 5, 7},
    // B': fourCycle(1,0,5,6)
    {5, 0, 2, 3, 4, 6, 1, 7},
    // B2: swap(0,6), swap(1,5)
    {6, 5, 2, 3, 4, 1, 0, 7},
    // L: fourCycle(3,0,5,4)
    {5, 1, 2, 0, 3, 4, 6, 7},
    // L': fourCycle(0,3,4,5)
    {3, 1, 2, 4, 5, 0, 6, 7},
    // L2: swap(3,5), swap(0,4)
    {4, 1, 2, 5, 0, 3, 6, 7},
};

// Corner twist amounts applied at each position AFTER the cycle.
// Value is the amount added (mod 3) to the NEW occupant's orientation.
static const int CORNER_TWIST[NUM_MOVES][8] = {
    // R: pos 2+2, pos 6+2, pos 1+1, pos 7+1
    {0, 1, 2, 0, 0, 0, 2, 1},
    // R': same twist amounts
    {0, 1, 2, 0, 0, 0, 2, 1},
    // R2: no twist
    {0, 0, 0, 0, 0, 0, 0, 0},
    // U: no twist
    {0, 0, 0, 0, 0, 0, 0, 0},
    // U': no twist
    {0, 0, 0, 0, 0, 0, 0, 0},
    // U2: no twist
    {0, 0, 0, 0, 0, 0, 0, 0},
    // F: pos 2+1, pos 4+1, pos 3+2, pos 7+2
    {0, 0, 1, 2, 1, 0, 0, 2},
    // F': same twist amounts
    {0, 0, 1, 2, 1, 0, 0, 2},
    // F2: no twist
    {0, 0, 0, 0, 0, 0, 0, 0},
    // D: no twist
    {0, 0, 0, 0, 0, 0, 0, 0},
    // D': no twist
    {0, 0, 0, 0, 0, 0, 0, 0},
    // D2: no twist
    {0, 0, 0, 0, 0, 0, 0, 0},
    // B: pos 0+1, pos 6+1, pos 1+2, pos 5+2
    {1, 2, 0, 0, 0, 2, 1, 0},
    // B': same twist amounts
    {1, 2, 0, 0, 0, 2, 1, 0},
    // B2: no twist
    {0, 0, 0, 0, 0, 0, 0, 0},
    // L: pos 3+1, pos 5+1, pos 0+2, pos 4+2
    {2, 0, 0, 1, 2, 1, 0, 0},
    // L': same twist amounts
    {2, 0, 0, 1, 2, 1, 0, 0},
    // L2: no twist
    {0, 0, 0, 0, 0, 0, 0, 0},
};

// Edge position permutation: EDGE_SOURCE[move][new_pos] = old_pos
// Same derivation as corners, but for edges.
static const int EDGE_SOURCE[NUM_MOVES][12] = {
    // R: fourCycle(1,4,11,7): new[1]=old[4], new[4]=old[11], new[11]=old[7], new[7]=old[1]
    {0, 4, 2, 3, 11, 5, 6, 1, 8, 9, 10, 7},
    // R': fourCycle(1,7,11,4)
    {0, 7, 2, 3, 1, 5, 6, 11, 8, 9, 10, 4},
    // R2: swap(1,11), swap(4,7)
    {0, 11, 2, 3, 7, 5, 6, 4, 8, 9, 10, 1},
    // U: fourCycle(1,0,3,2)
    {3, 0, 1, 2, 4, 5, 6, 7, 8, 9, 10, 11},
    // U': fourCycle(1,2,3,0)
    {1, 2, 3, 0, 4, 5, 6, 7, 8, 9, 10, 11},
    // U2: swap(0,2), swap(3,1)
    {2, 3, 0, 1, 4, 5, 6, 7, 8, 9, 10, 11},
    // F: fourCycle(2,5,8,4)
    {0, 1, 5, 3, 2, 8, 6, 7, 4, 9, 10, 11},
    // F': fourCycle(2,4,8,5)
    {0, 1, 4, 3, 8, 2, 6, 7, 5, 9, 10, 11},
    // F2: swap(2,8), swap(5,4)
    {0, 1, 8, 3, 5, 4, 6, 7, 2, 9, 10, 11},
    // D: fourCycle(8,9,10,11)
    {0, 1, 2, 3, 4, 5, 6, 7, 9, 10, 11, 8},
    // D': fourCycle(9,8,11,10)
    {0, 1, 2, 3, 4, 5, 6, 7, 11, 8, 9, 10},
    // D2: swap(9,11), swap(8,10)
    {0, 1, 2, 3, 4, 5, 6, 7, 10, 11, 8, 9},
    // B: fourCycle(0,7,10,6)
    {7, 1, 2, 3, 4, 5, 0, 10, 8, 9, 6, 11},
    // B': fourCycle(7,0,6,10)
    {6, 1, 2, 3, 4, 5, 10, 0, 8, 9, 7, 11},
    // B2: swap(0,10), swap(6,7)
    {10, 1, 2, 3, 4, 5, 7, 6, 8, 9, 0, 11},
    // L: fourCycle(5,3,6,9)
    {0, 1, 2, 6, 4, 3, 9, 7, 8, 5, 10, 11},
    // L': fourCycle(3,5,9,6)
    {0, 1, 2, 5, 4, 9, 3, 7, 8, 6, 10, 11},
    // L2: swap(3,9), swap(6,5)
    {0, 1, 2, 9, 4, 6, 5, 7, 8, 3, 10, 11},
};

// Edge positions where orientation flips for each move.
// Only F, F', B, B' flip edge orientations. F2, B2 do not.
static const bool EDGE_FLIP_POS[NUM_MOVES][12] = {
    // R
    {0,0,0,0,0,0,0,0,0,0,0,0},
    // R'
    {0,0,0,0,0,0,0,0,0,0,0,0},
    // R2
    {0,0,0,0,0,0,0,0,0,0,0,0},
    // U
    {0,0,0,0,0,0,0,0,0,0,0,0},
    // U'
    {0,0,0,0,0,0,0,0,0,0,0,0},
    // U2
    {0,0,0,0,0,0,0,0,0,0,0,0},
    // F: positions 2,4,5,8 flip
    {0,0,1,0,1,1,0,0,1,0,0,0},
    // F': positions 2,4,5,8 flip
    {0,0,1,0,1,1,0,0,1,0,0,0},
    // F2: no flip
    {0,0,0,0,0,0,0,0,0,0,0,0},
    // D
    {0,0,0,0,0,0,0,0,0,0,0,0},
    // D'
    {0,0,0,0,0,0,0,0,0,0,0,0},
    // D2
    {0,0,0,0,0,0,0,0,0,0,0,0},
    // B: positions 0,6,7,10 flip
    {1,0,0,0,0,0,1,1,0,0,1,0},
    // B': positions 0,6,7,10 flip
    {1,0,0,0,0,0,1,1,0,0,1,0},
    // B2: no flip
    {0,0,0,0,0,0,0,0,0,0,0,0},
    // L
    {0,0,0,0,0,0,0,0,0,0,0,0},
    // L'
    {0,0,0,0,0,0,0,0,0,0,0,0},
    // L2
    {0,0,0,0,0,0,0,0,0,0,0,0},
};


// ======================== Generation ========================

static void generateCornerPermTable(Indexer& indexer)
{
    std::cout << "  Generating corner permutation table (" << NUM_CORNER_PERMS << " states)..." << std::flush;

    cornerPerm = new uint16_t[NUM_CORNER_PERMS][NUM_MOVES];

    for (int rank = 0; rank < NUM_CORNER_PERMS; rank++)
    {
        // Unrank to get the permutation
        std::array<uint8_t, Cube::NUM_CORNERS> perm;
        indexer.unrankCornerPerm(rank, perm);

        // Build a Cube with this corner permutation (identity orientation)
        Cube cube;
        auto& corners = cube.getCorners();
        for (int i = 0; i < Cube::NUM_CORNERS; i++)
        {
            corners[i].index = perm[i];
            corners[i].orientation = 0;
        }

        // Apply each move and get new perm rank
        for (int m = 0; m < NUM_MOVES; m++)
        {
            Cube moved = cube;
            Cube::applyMoveByIndex(moved, m);
            cornerPerm[rank][m] = (uint16_t)indexer.getCornerPermRank(moved.getCorners());
        }
    }

    std::cout << " done." << std::endl;
}

static void generateCornerOrientTable(Indexer& indexer)
{
    std::cout << "  Generating corner orientation table (" << NUM_CORNER_ORIENTS << " states)..." << std::flush;

    cornerOrient = new uint16_t[NUM_CORNER_ORIENTS][NUM_MOVES];

    for (int rank = 0; rank < NUM_CORNER_ORIENTS; rank++)
    {
        // Decode orientation at each position (position-ordered)
        std::array<uint8_t, Cube::NUM_CORNERS> orient;
        indexer.unrankCornerOrient(rank, orient);

        // For each move, compute new orientation using the move definitions
        for (int m = 0; m < NUM_MOVES; m++)
        {
            std::array<uint8_t, Cube::NUM_CORNERS> newOrient;
            for (int pos = 0; pos < Cube::NUM_CORNERS; pos++)
            {
                int sourcePos = CORNER_SOURCE[m][pos];
                newOrient[pos] = (orient[sourcePos] + CORNER_TWIST[m][pos]) % 3;
            }

            // Re-encode (only positions 0-6)
            // Encoding: orient[6]*3^0 + orient[5]*3^1 + ... + orient[0]*3^6
            int newRank = 0;
            int pow3 = 1;
            for (int i = Cube::NUM_CORNERS - 2; i >= 0; i--)
            {
                newRank += newOrient[i] * pow3;
                pow3 *= 3;
            }
            cornerOrient[rank][m] = (uint16_t)newRank;
        }
    }

    std::cout << " done." << std::endl;
}

static void generateEdgePermTable(Indexer& indexer)
{
    ProgressBar progress("  Edge perm  ", NUM_EDGE_PERMS);

    edgePerm = new uint32_t[NUM_EDGE_PERMS][NUM_MOVES];

    // For each of 3,991,680 partial permutations (7-from-12):
    // unrank → apply each move's position permutation → re-rank
    for (int rank = 0; rank < NUM_EDGE_PERMS; rank++)
    {
        std::array<int, Indexer::NUM_EDGES_IN_PDB> positions;
        indexer.unrankEdgePerm(rank, positions);

        // Build a minimal Cube with edges at these positions for ranking
        // We need edges where edgeSet[identity] = position,
        // which means: for each identity i, the edge with index=i is at positions[i].
        // In Cube terms: edges[positions[i]].index = i
        Cube cube;
        auto& edges = cube.getEdges();
        // Clear all edge indices to something > 6 (not tracked)
        for (int i = 0; i < Cube::NUM_EDGES; i++)
        {
            edges[i].index = Cube::NUM_EDGES; // sentinel: not in our group
            edges[i].orientation = 0;
        }
        // Place tracked edges at their positions
        for (int i = 0; i < Indexer::NUM_EDGES_IN_PDB; i++)
        {
            edges[positions[i]].index = i;
        }

        for (int m = 0; m < NUM_MOVES; m++)
        {
            // Apply the move's position permutation
            // New position of edge identity i = EDGE_SOURCE inverse at positions[i]
            // Actually: EDGE_SOURCE[m][new_pos] = old_pos means:
            // "the piece at new_pos came from old_pos"
            // We need: "where does the piece at old_pos go?" i.e. the inverse map
            // If old_pos = positions[i], we find new_pos such that EDGE_SOURCE[m][new_pos] = positions[i]

            // Build inverse: posMap[old_pos] = new_pos
            int posMap[12];
            for (int p = 0; p < 12; p++)
            {
                posMap[EDGE_SOURCE[m][p]] = p;
            }

            std::array<int, Indexer::NUM_EDGES_IN_PDB> newPositions;
            for (int i = 0; i < Indexer::NUM_EDGES_IN_PDB; i++)
            {
                newPositions[i] = posMap[positions[i]];
            }

            // Build a Cube for ranking
            Cube movedCube;
            auto& movedEdges = movedCube.getEdges();
            for (int i = 0; i < Cube::NUM_EDGES; i++)
            {
                movedEdges[i].index = Cube::NUM_EDGES;
                movedEdges[i].orientation = 0;
            }
            for (int i = 0; i < Indexer::NUM_EDGES_IN_PDB; i++)
            {
                movedEdges[newPositions[i]].index = i;
            }

            // Compute perm rank using the standard edgeSet construction
            std::array<int, Indexer::NUM_EDGES_IN_PDB> edgeSet;
            int found = 0;
            for (int j = 0; j < Cube::NUM_EDGES && found < Indexer::NUM_EDGES_IN_PDB; j++)
            {
                if (movedEdges[j].index < Indexer::NUM_EDGES_IN_PDB)
                {
                    edgeSet[movedEdges[j].index] = j;
                    found++;
                }
            }
            edgePerm[rank][m] = (uint32_t)indexer.getEdgePermRank(movedEdges, edgeSet);
        }

        if ((rank & 0xFFFFF) == 0)
        {
            progress.update(rank);
        }
    }

    progress.finish();
}

static void generateEdgeFlipMaskTable(Indexer& indexer)
{
    ProgressBar progress("  Edge flip  ", NUM_EDGE_PERMS);

    edgeFlipMask = new uint8_t[NUM_EDGE_PERMS][NUM_MOVES];

    for (int rank = 0; rank < NUM_EDGE_PERMS; rank++)
    {
        std::array<int, Indexer::NUM_EDGES_IN_PDB> positions;
        indexer.unrankEdgePerm(rank, positions);

        for (int m = 0; m < NUM_MOVES; m++)
        {
            // For each of the 7 tracked edges (identity i at positions[i]),
            // check if positions[i] is a position that gets flipped by this move.
            // The flip happens BEFORE the cycle (at the old position).
            // Actually in Cube.cpp, flips happen AFTER the cycle, so the flip
            // applies to whatever edge is now at the destination position.
            // But since EDGE_FLIP_POS marks the positions involved in the flip,
            // and the edge at old position positions[i] either stays or moves,
            // the flip check should be at the OLD position for each edge.
            //
            // Looking at Cube::f(): fourCycleEdges(2,5,8,4) then flipEdge(2,5,8,4)
            // After the cycle, the edge NOW at position 2 (which came from pos 5)
            // gets flipped. So the flip is on the POST-cycle position.
            // Since we're tracking by identity, edge i was at positions[i],
            // after the move it's at posMap[positions[i]].
            // The flip happens at the destination position.
            int posMap[12];
            for (int p = 0; p < 12; p++)
            {
                posMap[EDGE_SOURCE[m][p]] = p;
            }

            uint8_t mask = 0;
            for (int i = 0; i < Indexer::NUM_EDGES_IN_PDB; i++)
            {
                int newPos = posMap[positions[i]];
                if (EDGE_FLIP_POS[m][newPos])
                {
                    mask |= (1 << i);
                }
            }
            edgeFlipMask[rank][m] = mask;
        }

        if ((rank & 0xFFFFF) == 0)
        {
            progress.update(rank);
        }
    }

    progress.finish();
}

static void generateFullOrientTable(Indexer& indexer)
{
    std::cout << "  Generating full edge orientation table (" << NUM_FULL_EDGE_ORIENTS << " states)..." << std::flush;

    fullOrient = new uint16_t[NUM_FULL_EDGE_ORIENTS][NUM_MOVES];

    for (int rank = 0; rank < NUM_FULL_EDGE_ORIENTS; rank++)
    {
        // Decode orientation bits (position-ordered)
        std::array<uint8_t, Cube::NUM_EDGES> orient;
        indexer.unrankFullEdgeOrient((uint16_t)rank, orient);

        for (int m = 0; m < NUM_MOVES; m++)
        {
            // Apply the move: new orient at pos = old orient at source pos, XOR flip
            std::array<uint8_t, Cube::NUM_EDGES> newOrient;
            for (int pos = 0; pos < Cube::NUM_EDGES; pos++)
            {
                int sourcePos = EDGE_SOURCE[m][pos];
                newOrient[pos] = orient[sourcePos] ^ (EDGE_FLIP_POS[m][pos] ? 1 : 0);
            }

            // Re-encode: first 11 bits
            uint16_t newRank = 0;
            for (int i = 0; i < Cube::NUM_EDGES - 1; i++)
            {
                newRank |= newOrient[i] << i;
            }
            fullOrient[rank][m] = newRank;
        }
    }

    std::cout << " done." << std::endl;
}


// ======================== Persistence ========================

template<typename T>
static bool loadTable(const std::string& filename, T* table, size_t count)
{
    std::ifstream reader(filename, std::ios::in | std::ios::binary);
    if (!reader.is_open()) return false;

    reader.read(reinterpret_cast<char*>(table), count * sizeof(T));
    bool ok = !reader.fail();
    reader.close();
    return ok;
}

template<typename T>
static void saveTable(const std::string& filename, const T* table, size_t count)
{
    std::ofstream writer(filename, std::ios::out | std::ios::binary | std::ios::trunc);
    writer.write(reinterpret_cast<const char*>(table), count * sizeof(T));
    writer.close();
}

// ======================== Public API ========================

void generate()
{
    Indexer indexer;

    std::cout << "Generating transition tables..." << std::endl;

    generateCornerPermTable(indexer);
    generateCornerOrientTable(indexer);
    generateEdgePermTable(indexer);
    generateEdgeFlipMaskTable(indexer);
    generateFullOrientTable(indexer);

    // Save to disk
    std::cout << "Saving transition tables to disk..." << std::flush;
    saveTable(DatabaseConstants::CORNER_PERM_TT,   cornerPerm,   (size_t)NUM_CORNER_PERMS);
    saveTable(DatabaseConstants::CORNER_ORIENT_TT, cornerOrient, (size_t)NUM_CORNER_ORIENTS);
    saveTable(DatabaseConstants::EDGE_PERM_TT,     edgePerm,     (size_t)NUM_EDGE_PERMS);
    saveTable(DatabaseConstants::EDGE_FLIP_TT,     edgeFlipMask, (size_t)NUM_EDGE_PERMS);
    saveTable(DatabaseConstants::FULL_ORIENT_TT,   fullOrient,   (size_t)NUM_FULL_EDGE_ORIENTS);
    std::cout << " done." << std::endl;
}

void init()
{
    // Try loading from disk first
    std::cout << "Loading transition tables..." << std::endl;

    bool allLoaded = true;

    // Corner perm
    cornerPerm = new uint16_t[NUM_CORNER_PERMS][NUM_MOVES];
    if (!loadTable(DatabaseConstants::CORNER_PERM_TT, cornerPerm, (size_t)NUM_CORNER_PERMS))
    {
        allLoaded = false;
    }

    // Corner orient
    if (allLoaded)
    {
        cornerOrient = new uint16_t[NUM_CORNER_ORIENTS][NUM_MOVES];
        if (!loadTable(DatabaseConstants::CORNER_ORIENT_TT, cornerOrient, (size_t)NUM_CORNER_ORIENTS))
        {
            allLoaded = false;
        }
    }

    // Edge perm
    if (allLoaded)
    {
        edgePerm = new uint32_t[NUM_EDGE_PERMS][NUM_MOVES];
        if (!loadTable(DatabaseConstants::EDGE_PERM_TT, edgePerm, (size_t)NUM_EDGE_PERMS))
        {
            allLoaded = false;
        }
    }

    // Edge flip mask
    if (allLoaded)
    {
        edgeFlipMask = new uint8_t[NUM_EDGE_PERMS][NUM_MOVES];
        if (!loadTable(DatabaseConstants::EDGE_FLIP_TT, edgeFlipMask, (size_t)NUM_EDGE_PERMS))
        {
            allLoaded = false;
        }
    }

    // Full orient
    if (allLoaded)
    {
        fullOrient = new uint16_t[NUM_FULL_EDGE_ORIENTS][NUM_MOVES];
        if (!loadTable(DatabaseConstants::FULL_ORIENT_TT, fullOrient, (size_t)NUM_FULL_EDGE_ORIENTS))
        {
            allLoaded = false;
        }
    }

    if (allLoaded)
    {
        std::cout << "Transition tables loaded from disk." << std::endl;
    }
    else
    {
        std::cout << "Transition tables not found on disk. Generating..." << std::endl;
        // Free partially loaded tables and regenerate
        cleanup();
        generate();
    }
}

void cleanup()
{
    delete[] cornerPerm;    cornerPerm = nullptr;
    delete[] cornerOrient;  cornerOrient = nullptr;
    delete[] edgePerm;      edgePerm = nullptr;
    delete[] edgeFlipMask;  edgeFlipMask = nullptr;
    delete[] fullOrient;    fullOrient = nullptr;
}

} // namespace TransitionTable
