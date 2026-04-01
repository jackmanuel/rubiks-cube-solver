#pragma once

#include <cstdint>
#include <string>

namespace TransitionTable {
    static constexpr int NUM_MOVES = 18;

    // State space sizes
    static constexpr int NUM_CORNER_PERMS = 40320;        // 8!
    static constexpr int NUM_CORNER_ORIENTS = 2187;       // 3^7
    static constexpr int NUM_EDGE_PERMS = 3991680;        // P(12,7)
    static constexpr int NUM_FULL_EDGE_ORIENTS = 2048;    // 2^11

    // Move index convention:
    //   R=0, R'=1, R2=2, U=3, U'=4, U2=5,
    //   F=6, F'=7, F2=8, D=9, D'=10, D2=11,
    //   B=12, B'=13, B2=14, L=15, L'=16, L2=17

    // Table pointers (initialized by init())

    // cornerPerm[permRank][move] = new permRank
    extern uint16_t (*cornerPerm)[NUM_MOVES];

    // cornerOrient[orientRank][move] = new orientRank
    extern uint16_t (*cornerOrient)[NUM_MOVES];

    // edgePerm[permRank][move] = new permRank
    // (shared between both edge groups — the position transformation is identical)
    extern uint32_t (*edgePerm)[NUM_MOVES];

    // edgeFlipMask[permRank][move] = 7-bit mask of which tracked edges flip
    // (shared between both edge groups — flip depends on position, not identity)
    extern uint8_t (*edgeFlipMask)[NUM_MOVES];

    // fullOrient[orientRank][move] = new orientRank
    extern uint16_t (*fullOrient)[NUM_MOVES];

    // Initialize all tables (load from disk if available, else generate + save)
    void init();

    // Force regeneration of all tables
    void generate();

    // Free all allocated memory
    void cleanup();
}
