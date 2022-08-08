#pragma once

#include "Cube.h"



class Indexer
{
    
    public:
        Indexer();
        int getCornerIndex(Cube cube);

    private:
        
        std::array<uint8_t, 255> onesArray;
        std::array<int, Cube::NUM_CORNERS> factorial;
        std::array<int, Cube::NUM_CORNERS> power3;

        int getCornerPermRank(std::array<Cube::Cubie, 8>& corners);
        int getCornerOrientRank(std::array<Cube::Cubie, 8>& corners);

        std::array<uint8_t, Cube::NUM_CORNERS> getLehmerCode(Cube cube);
        int lehmerToBase10(std::array<uint8_t, Cube::NUM_CORNERS>& lehmerCode);

        int fac(int n);
};