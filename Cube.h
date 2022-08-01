//    * Edge indices.
//    *
//    * 0  1  2  3  4  5  6  7  8  9  10 11
//    * UB UR UF UL FR FL BL BR DF DL DB DR
//    * BO BY BR BW RY RW OW OY GR GW GO GY
//    *
//    * Corner indices.
//    *
//    * 0   1   2   3   4   5   6   7
//    * ULB URB URF ULF DLF DLB DRB DRF
//    * BWO BOY BYR BRW GWR GOW GYO GRY

// Cube is configured in orientation with blue up, red front


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
    void rPrime();
    void r2();

    void u();
    void uPrime();
    void u2();

    void f();
    void fPrime();
    void f2();

    void d();
    void dPrime();
    void d2();

    void l();
    void lPrime();
    void l2();

    void b();
    void bPrime();
    void b2();


private:

    Cubie edges[12];
    Cubie corners[8];

    void updateCornerOrientation(uint8_t corner_index, uint8_t change);
    void flipEdgeOrientation(uint8_t edge_index);

    void swapCornerCubies(uint8_t first_index, uint8_t second_index);
    void swapEdgeCubies(uint8_t first_index, uint8_t second_index);

    void fourCycleCorners(uint8_t c1, uint8_t c2, uint8_t c3, uint8_t c4);
    void fourCycleEdges(uint8_t e1, uint8_t e2, uint8_t e3, uint8_t e4);

};