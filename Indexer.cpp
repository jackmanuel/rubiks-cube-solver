#include <bitset>
#include <array>
#include <cstdio>
#include <cmath>

#include "Indexer.h"
#include "Cube.h"

// this is number of different corner orientations there are
// it is 3^7
static constexpr const int NUM_CORNER_ORIENT_RANKS = 2187;

Indexer::Indexer()
{
    // precompute number of 1s in integers up to 2^8
    for (int i = 0; i <= 255; i++)
    {
        std::bitset<Cube::NUM_CORNERS> bits(i);
        onesArray[i] = bits.count();
    }

    // precompute factorials
    for (int i = 0; i < Cube::NUM_CORNERS; i++)
    {
        factorial[i] = fac(i);
    }
    
    // precompute powers of 3
    power3[0] = 1;
    for (int i = 1; i < Cube::NUM_CORNERS; i++)
    {
        power3[i] = 3 * power3[i - 1];
    }
}

int Indexer::getCornerIndex(Cube cube)
{
    std::array<Cube::Cubie, Cube::NUM_CORNERS>& corners = cube.getCorners();

    return (getCornerPermRank(corners) * NUM_CORNER_ORIENT_RANKS) + getCornerOrientRank(corners);
}


int Indexer::getCornerOrientRank(std::array<Cube::Cubie, Cube::NUM_CORNERS>& corners)
{
    int res = 0;

    // -1 because orientation of 7 corners determines the 8th.
    // A single corner cannot be rotated on the Rubiks cube
    for (int i = 0; i < Cube::NUM_CORNERS - 1; i++)
    {   
        res += corners[Cube::NUM_CORNERS - i - 2].orientation * power3[i];
    }

    return res;
}


int Indexer::getCornerPermRank(std::array<Cube::Cubie, Cube::NUM_CORNERS>& corners)
{

    // since the last number of code is always 0
    // could probably just make this NUM_CORNERS - 1 size for speed
    std::array<uint8_t, Cube::NUM_CORNERS> lehmerCode;
    std::bitset<Cube::NUM_CORNERS> bits;
    
    // first value of code is always first value of permutation
    lehmerCode[0] = corners[0].index;
    bits.set(Cube::NUM_CORNERS - corners[0].index - 1);

    // last value of code always 0
    lehmerCode[Cube::NUM_CORNERS - 1] = 0;

    for (int i = 1; i <= Cube::NUM_CORNERS - 2; i++){

        bits.set(Cube::NUM_CORNERS - corners[i].index - 1);
        lehmerCode[i] = corners[i].index - onesArray[bits.to_ulong() >> (Cube::NUM_CORNERS - corners[i].index)];
    }

    return lehmerToBase10(lehmerCode);
}


int Indexer::lehmerToBase10(std::array<uint8_t, Cube::NUM_CORNERS>& lehmerCode)
{
    int res = 0;

    for (int i = 0; i < Cube::NUM_CORNERS; i++)
    {
        res += lehmerCode[Cube::NUM_CORNERS - i - 1] * factorial[i];
    }

    return res;
}

int Indexer::fac(int n){
    if (n == 0 || n == 1) { return 1; }
    else return n * fac(n - 1);
}
