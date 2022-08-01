#include <iostream>
#include <cstdio>

#include "Cube.h"

// constructs solved cube
Cube::Cube()
{
    // cube is in solved state when cubies indices are sequential
    // and orientations are all zero
    for (int i = 0; i < 8; i++){
        this->corners[i].index = i;
        this->corners[i].orientation = 0;
    }

    for (int i = 0; i < 12; i++){
        this->edges[i].index = i;
        this->edges[i].orientation = 0;
    }

}

void Cube::printCubeState()
{   
    std::cout << "Corners: " << std::endl;
    for (int i = 0; i < 8; i++){

        Cubie corner = this->corners[i];
        printf("(%u, %u) ", corner.index, corner.orientation);
    }

    printf("\n");

    std::cout << "Edges: " << std::endl;
    for (int i = 0; i < 12; i++){

        Cubie edge = this->edges[i];
        printf("(%u, %u) ", edge.index, edge.orientation);
    }

    printf("\n");
}


void Cube::r()
{

    // R move - 90 turn of R face
    // Corner index changes: 
    // 2 -> 1
    // 1 -> 6
    // 6 -> 7
    // 7 -> 2

    Cubie temp = this->corners[2];

    this->corners[2] = this->corners[7];
    this->corners[7] = this->corners[6];
    this->corners[6] = this->corners[1];
    this->corners[1] = temp;

    updateCornerOrientation(2, 2);
    updateCornerOrientation(6, 2);

    updateCornerOrientation(1, 1);
    updateCornerOrientation(7, 1);


    // Edge index changes: 
    // 1 -> 7
    // 7 -> 11
    // 11 -> 4
    // 4 -> 1

    temp = this->edges[1];

    this->edges[1] = this->edges[4];
    this->edges[4] = this->edges[11];
    this->edges[11] = this->edges[7];
    this->edges[7] = temp;

    // TODO - edge orientation isn't addressed yet


}

void Cube::updateCornerOrientation(uint8_t corner_index, uint8_t change)
{
    Cubie& corner = this->corners[corner_index];
    corner.orientation = (corner.orientation + change) % 3;
}



