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
    fourCycleCorners(2, 7, 6, 1);

    updateCornerOrientation(2, 2);
    updateCornerOrientation(6, 2);

    updateCornerOrientation(1, 1);
    updateCornerOrientation(7, 1);

    fourCycleEdges(1, 4, 11, 7);
}

void Cube::rPrime()
{
    fourCycleCorners(2, 1, 6, 7);

    updateCornerOrientation(1, 1);
    updateCornerOrientation(7, 1);

    updateCornerOrientation(2, 2);
    updateCornerOrientation(6, 2);

    fourCycleEdges(1, 7, 11, 4);

}

void Cube::r2()
{
    swapCornerCubies(2, 6);
    swapCornerCubies(1, 7);

    swapEdgeCubies(1, 11);
    swapEdgeCubies(4, 7);
}



void Cube::u()
{
    fourCycleCorners(2, 1, 0, 3);
    fourCycleEdges(1, 0, 3, 2);
}

void Cube::uPrime()
{
    fourCycleCorners(2, 3, 0, 1);
    fourCycleEdges(1, 2, 3, 0);
}

void Cube::u2()
{
    swapCornerCubies(0, 2);
    swapCornerCubies(3, 1);

    swapEdgeCubies(0, 2);
    swapEdgeCubies(3, 1);
}

void Cube::f()
{
    fourCycleCorners(2, 3, 4, 7);

    updateCornerOrientation(2, 1);
    updateCornerOrientation(4, 1);

    updateCornerOrientation(3, 2);
    updateCornerOrientation(7, 2);

    fourCycleEdges(2, 5, 8, 4);

    flipEdgeOrientation(2);
    flipEdgeOrientation(5);
    flipEdgeOrientation(8);
    flipEdgeOrientation(4);
}

void Cube::fPrime()
{
    fourCycleCorners(2, 7, 4, 3);

    updateCornerOrientation(2, 1);
    updateCornerOrientation(4, 1);

    updateCornerOrientation(3, 2);
    updateCornerOrientation(7, 2);

    fourCycleEdges(2, 4, 8, 5);

    flipEdgeOrientation(2);
    flipEdgeOrientation(4);
    flipEdgeOrientation(8);
    flipEdgeOrientation(5);
}


void Cube::f2()
{
    swapCornerCubies(2, 4);
    swapCornerCubies(3, 7);

    swapEdgeCubies(2, 8);
    swapEdgeCubies(5, 4);
}

void Cube::d()
{
    fourCycleCorners(7, 4, 5, 6);
    fourCycleEdges(8, 9, 10, 11);
}

void Cube::dPrime()
{
    fourCycleCorners(4, 7, 6, 5);
    fourCycleEdges(9, 8, 11, 10);
}

void Cube::d2()
{
    swapCornerCubies(4, 6);
    swapCornerCubies(7, 5);

    swapEdgeCubies(9, 11);
    swapEdgeCubies(8, 10);
}

void Cube::l()
{
    fourCycleCorners(3, 0, 5, 4);

    updateCornerOrientation(3, 1);
    updateCornerOrientation(5, 1);

    updateCornerOrientation(0, 2);
    updateCornerOrientation(4, 2);

    fourCycleEdges(5, 3, 6, 9);
}

void Cube::lPrime()
{
    fourCycleCorners(0, 3, 4, 5);

    updateCornerOrientation(3, 1);
    updateCornerOrientation(5, 1);

    updateCornerOrientation(0, 2);
    updateCornerOrientation(4, 2);

    fourCycleEdges(3, 5, 9, 6);
}

void Cube::l2()
{
    swapCornerCubies(3, 5);
    swapCornerCubies(0, 4);

    swapEdgeCubies(3, 9);
    swapEdgeCubies(6, 5);
}

void Cube::b()
{   
    fourCycleCorners(0, 1, 6, 5);

    updateCornerOrientation(0, 1);
    updateCornerOrientation(6, 1);

    updateCornerOrientation(1, 2);
    updateCornerOrientation(5, 2);

    fourCycleEdges(0, 7, 10, 6);

    flipEdgeOrientation(0);
    flipEdgeOrientation(7);
    flipEdgeOrientation(10);
    flipEdgeOrientation(6);
}

void Cube::bPrime()
{
    fourCycleCorners(1, 0, 5, 6);

    updateCornerOrientation(0, 1);
    updateCornerOrientation(6, 1);

    updateCornerOrientation(1, 2);
    updateCornerOrientation(5, 2);

    fourCycleEdges(7, 0, 6, 10);

    flipEdgeOrientation(0);
    flipEdgeOrientation(7);
    flipEdgeOrientation(10);
    flipEdgeOrientation(6);
}

void Cube::b2()
{
    swapCornerCubies(0, 6);
    swapCornerCubies(1, 5);
}

// outcome of the function:
// all argurments shift to the left by 1
// c1, c2, c3, c4 --> c2, c3, c4, c1
void Cube::fourCycleCorners(uint8_t c1, uint8_t c2, uint8_t c3, uint8_t c4)
{
    Cubie temp = this->corners[c1];

    this->corners[c1] = this->corners[c2];
    this->corners[c2] = this->corners[c3];
    this->corners[c3] = this->corners[c4];
    this->corners[c4] = temp;
}

// outcome of the function:
// all argurments shift to the left by 1
// e1, e2, e3, e4 --> e2, e3, e4, e1
void Cube::fourCycleEdges(uint8_t e1, uint8_t e2, uint8_t e3, uint8_t e4)
{
    Cubie temp = this->edges[e1];

    this->edges[e1] = this->edges[e2];
    this->edges[e2] = this->edges[e3];
    this->edges[e3] = this->edges[e4];
    this->edges[e4] = temp;
}

void Cube::swapCornerCubies(uint8_t first_index, uint8_t second_index)
{
    Cubie temp = this->corners[first_index];

    this->corners[first_index] = this->corners[second_index];
    this->corners[second_index] = temp;
}

void Cube::swapEdgeCubies(uint8_t first_index, uint8_t second_index)
{
    Cubie temp = this->edges[first_index];

    this->edges[first_index] = this->edges[second_index];
    this->edges[second_index] = temp;
}

void Cube::updateCornerOrientation(uint8_t corner_index, uint8_t change)
{
    Cubie& corner = this->corners[corner_index];
    corner.orientation = (corner.orientation + change) % 3;
}

void Cube::flipEdgeOrientation(uint8_t edge_index)
{
    Cubie& edge = this->edges[edge_index];

    // flip bit
    edge.orientation ^= 1;
}



