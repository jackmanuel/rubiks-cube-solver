//    * Edge indexes.
//    *
//    * 0  1  2  3  4  5  6  7  8  9  10 11
//    * UB UR UF UL FR FL BL BR DF DL DB DR
//    * BO BY BR BW RY RW OW OY GR GW GO GY
//    *
//    * Corner indexes.
//    *
//    * 0   1   2   3   4   5   6   7
//    * ULB URB URF ULF DLF DLB DRB DRF
//    * BWO BOY BYR BRW GWR GOW GYO GRY


#pragma once

#include <cstdint>

class Cube
{
public:
    
    struct Cubie
    {
        uint8_t index;
        uint8_t orientation;
    };

    Cube();
    void printCubeState();

    void r();

private:

    Cubie edges[12];
    Cubie corners[8];

    void updateCornerOrientation(uint8_t corner_index, uint change);

};