#include <iostream>
#include <cstdio>
#include <string>
#include <sstream>
#include <iterator>
#include <vector>

#include "Cube.h"

// Pre-computed solved states for memcmp-based goal detection.
// Solved state: sequential indices (0..N-1) with all orientations 0.
const std::array<Cube::Cubie, Cube::NUM_CORNERS> Cube::SOLVED_CORNERS = {{
    {0,0}, {1,0}, {2,0}, {3,0}, {4,0}, {5,0}, {6,0}, {7,0}
}};
const std::array<Cube::Cubie, Cube::NUM_EDGES> Cube::SOLVED_EDGES = {{
    {0,0}, {1,0}, {2,0}, {3,0}, {4,0}, {5,0},
    {6,0}, {7,0}, {8,0}, {9,0}, {10,0}, {11,0}
}};

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

    this->lastMove = NONE;
    this->depth = 0;
}

void Cube::printCubeState(void)
{   
    std::cout << "Corners: " << std::endl;
    for (int i = 0; i < Cube::NUM_CORNERS; i++){

        Cubie corner = this->corners[i];
        printf("(%u, %u) ", corner.index, corner.orientation);
    }

    printf("\n");

    std::cout << "Edges: " << std::endl;
    for (int i = 0; i < Cube::NUM_EDGES; i++){

        Cubie edge = this->edges[i];
        printf("(%u, %u) ", edge.index, edge.orientation);
    }

    printf("\n");
}

std::array<Cube::Cubie, Cube::NUM_CORNERS>& Cube::getCorners(void)
{
    return this->corners;
}

uint8_t Cube::getDepth(void)
{
    return this->depth;
}

void Cube::setDepth(uint8_t newDepth)
{
    this->depth = newDepth;
}

std::array<Cube::Cubie, Cube::NUM_EDGES>& Cube::getEdges(void)
{
    return this->edges;
}

Cube::move Cube::getLastMove(void)
{
    return this->lastMove;
}

std::vector<Cube> Cube::generateNeighbours2(void)
{
    std::vector<Cube> result;

    if (this->lastMove != R && this->lastMove != R_PRIME && this->lastMove != R2)
    {   
        Cube rCube = *this;
        Cube rPrimeCube = *this;
        Cube r2Cube = *this;

        rCube.r();
        result.push_back(rCube);

        rPrimeCube.rPrime();
        result.push_back(rPrimeCube);

        r2Cube.r2();
        result.push_back(r2Cube);
    }

    
    if (this->lastMove != U && this->lastMove != U_PRIME && this->lastMove != U2)
    {   
        Cube uCube = *this;
        Cube uPrimeCube = *this;
        Cube u2Cube = *this;

        uCube.u();
        result.push_back(uCube);

        uPrimeCube.uPrime();
        result.push_back(uPrimeCube);

        u2Cube.u2();
        result.push_back(u2Cube);
    }

    if (this->lastMove != F && this->lastMove != F_PRIME && this->lastMove != F2)
    {
        Cube fCube = *this;
        Cube fPrimeCube = *this;
        Cube f2Cube = *this;

        fCube.f();
        result.push_back(fCube);

        fPrimeCube.fPrime();
        result.push_back(fPrimeCube);

        f2Cube.f2();
        result.push_back(f2Cube);
    }
    
    return result;
}

std::vector<Cube> Cube::generateNeighbours(void)
{
    std::vector<Cube> result;

    this->depth++;

    if (this->lastMove != R && this->lastMove != R_PRIME && this->lastMove != R2)
    {   
        Cube rCube = *this;
        Cube rPrimeCube = *this;
        Cube r2Cube = *this;

        rCube.r();
        result.push_back(rCube);

        rPrimeCube.rPrime();
        result.push_back(rPrimeCube);

        r2Cube.r2();
        result.push_back(r2Cube);
    }

    
    if (this->lastMove != U && this->lastMove != U_PRIME && this->lastMove != U2)
    {   
        Cube uCube = *this;
        Cube uPrimeCube = *this;
        Cube u2Cube = *this;

        uCube.u();
        result.push_back(uCube);

        uPrimeCube.uPrime();
        result.push_back(uPrimeCube);

        u2Cube.u2();
        result.push_back(u2Cube);
    }

    if (this->lastMove != F && this->lastMove != F_PRIME && this->lastMove != F2)
    {
        Cube fCube = *this;
        Cube fPrimeCube = *this;
        Cube f2Cube = *this;

        fCube.f();
        result.push_back(fCube);

        fPrimeCube.fPrime();
        result.push_back(fPrimeCube);

        f2Cube.f2();
        result.push_back(f2Cube);
    }

    if (this->lastMove != L && this->lastMove != L_PRIME && this->lastMove != L2)
    {
        Cube lCube = *this;
        Cube lPrimeCube = *this;
        Cube l2Cube = *this;

        lCube.l();
        result.push_back(lCube);

        lPrimeCube.lPrime();
        result.push_back(lPrimeCube);

        l2Cube.l2();
        result.push_back(l2Cube);
    }

    if (this->lastMove != B && this->lastMove != B_PRIME && this->lastMove != B2)
    {
        Cube bCube = *this;
        Cube bPrimeCube = *this;
        Cube b2Cube = *this;

        bCube.b();
        result.push_back(bCube);

        bPrimeCube.bPrime();
        result.push_back(bPrimeCube);

        b2Cube.b2();
        result.push_back(b2Cube);
    }

    if (this->lastMove != D && this->lastMove != D_PRIME && this->lastMove != D2)
    {
        Cube dCube = *this;
        Cube dPrimeCube = *this;
        Cube d2Cube = *this;

        dCube.d();
        result.push_back(dCube);

        dPrimeCube.dPrime();
        result.push_back(dPrimeCube);

        d2Cube.d2();
        result.push_back(d2Cube);
    }
    
    // neighbour cubes are generated with incremented depth
    // must decrement again so cube function was called on
    // has its old depth again
    this->depth--;

    return result;
}

std::vector<Cube> Cube::generateNeighboursPruned(void)
{
    std::vector<Cube> result;

    this->depth++;

    if (this->lastMove != R && this->lastMove != R_PRIME && this->lastMove != R2
     && this->lastMove != L && this->lastMove != L_PRIME && this->lastMove != L2)
    {   
        Cube rCube = *this;
        Cube rPrimeCube = *this;
        Cube r2Cube = *this;

        rCube.r();
        result.push_back(rCube);

        rPrimeCube.rPrime();
        result.push_back(rPrimeCube);

        r2Cube.r2();
        result.push_back(r2Cube);
    }

    
    if (this->lastMove != U && this->lastMove != U_PRIME && this->lastMove != U2
     && this->lastMove != D && this->lastMove != D_PRIME && this->lastMove != D2)
    {   
        Cube uCube = *this;
        Cube uPrimeCube = *this;
        Cube u2Cube = *this;

        uCube.u();
        result.push_back(uCube);

        uPrimeCube.uPrime();
        result.push_back(uPrimeCube);

        u2Cube.u2();
        result.push_back(u2Cube);
    }

    if (this->lastMove != F && this->lastMove != F_PRIME && this->lastMove != F2
     && this->lastMove != B && this->lastMove != B_PRIME && this->lastMove != B2)
    {
        Cube fCube = *this;
        Cube fPrimeCube = *this;
        Cube f2Cube = *this;

        fCube.f();
        result.push_back(fCube);

        fPrimeCube.fPrime();
        result.push_back(fPrimeCube);

        f2Cube.f2();
        result.push_back(f2Cube);
    }

    if (this->lastMove != L && this->lastMove != L_PRIME && this->lastMove != L2)
    {
        Cube lCube = *this;
        Cube lPrimeCube = *this;
        Cube l2Cube = *this;

        lCube.l();
        result.push_back(lCube);

        lPrimeCube.lPrime();
        result.push_back(lPrimeCube);

        l2Cube.l2();
        result.push_back(l2Cube);
    }

    if (this->lastMove != B && this->lastMove != B_PRIME && this->lastMove != B2)
    {
        Cube bCube = *this;
        Cube bPrimeCube = *this;
        Cube b2Cube = *this;

        bCube.b();
        result.push_back(bCube);

        bPrimeCube.bPrime();
        result.push_back(bPrimeCube);

        b2Cube.b2();
        result.push_back(b2Cube);
    }

    if (this->lastMove != D && this->lastMove != D_PRIME && this->lastMove != D2)
    {
        Cube dCube = *this;
        Cube dPrimeCube = *this;
        Cube d2Cube = *this;

        dCube.d();
        result.push_back(dCube);

        dPrimeCube.dPrime();
        result.push_back(dPrimeCube);

        d2Cube.d2();
        result.push_back(d2Cube);
    }
    
    this->depth--;

    return result;
}

void Cube::r()
{
    this->lastMove = R;

    fourCycleCorners(2, 7, 6, 1);

    updateCornerOrientation(2, 2);
    updateCornerOrientation(6, 2);

    updateCornerOrientation(1, 1);
    updateCornerOrientation(7, 1);

    fourCycleEdges(1, 4, 11, 7);
}

void Cube::rPrime()
{

    this->lastMove = R_PRIME;

    fourCycleCorners(2, 1, 6, 7);

    updateCornerOrientation(1, 1);
    updateCornerOrientation(7, 1);

    updateCornerOrientation(2, 2);
    updateCornerOrientation(6, 2);

    fourCycleEdges(1, 7, 11, 4);

}

void Cube::r2()
{
    this->lastMove = R2;

    swapCornerCubies(2, 6);
    swapCornerCubies(1, 7);

    swapEdgeCubies(1, 11);
    swapEdgeCubies(4, 7);
}



void Cube::u()
{
    this->lastMove = U;

    fourCycleCorners(2, 1, 0, 3);
    fourCycleEdges(1, 0, 3, 2);
}

void Cube::uPrime()
{
    this->lastMove = U_PRIME;

    fourCycleCorners(2, 3, 0, 1);
    fourCycleEdges(1, 2, 3, 0);
}

void Cube::u2()
{
    this->lastMove = U2;

    swapCornerCubies(0, 2);
    swapCornerCubies(3, 1);

    swapEdgeCubies(0, 2);
    swapEdgeCubies(3, 1);
}

void Cube::f()
{
    this->lastMove = F; 

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
    this->lastMove = F_PRIME;

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
    this->lastMove = F2;

    swapCornerCubies(2, 4);
    swapCornerCubies(3, 7);

    swapEdgeCubies(2, 8);
    swapEdgeCubies(5, 4);
}

void Cube::d()
{
    this->lastMove = D;

    fourCycleCorners(7, 4, 5, 6);
    fourCycleEdges(8, 9, 10, 11);
}

void Cube::dPrime()
{
    this->lastMove = D_PRIME;

    fourCycleCorners(4, 7, 6, 5);
    fourCycleEdges(9, 8, 11, 10);
}

void Cube::d2()
{
    this->lastMove = D2;

    swapCornerCubies(4, 6);
    swapCornerCubies(7, 5);

    swapEdgeCubies(9, 11);
    swapEdgeCubies(8, 10);
}

void Cube::l()
{
    this->lastMove = L;

    fourCycleCorners(3, 0, 5, 4);

    updateCornerOrientation(3, 1);
    updateCornerOrientation(5, 1);

    updateCornerOrientation(0, 2);
    updateCornerOrientation(4, 2);

    fourCycleEdges(5, 3, 6, 9);
}

void Cube::lPrime()
{
    this->lastMove = L_PRIME;

    fourCycleCorners(0, 3, 4, 5);

    updateCornerOrientation(3, 1);
    updateCornerOrientation(5, 1);

    updateCornerOrientation(0, 2);
    updateCornerOrientation(4, 2);

    fourCycleEdges(3, 5, 9, 6);
}

void Cube::l2()
{
    this->lastMove = L2;

    swapCornerCubies(3, 5);
    swapCornerCubies(0, 4);

    swapEdgeCubies(3, 9);
    swapEdgeCubies(6, 5);
}

void Cube::b()
{
    this->lastMove = B;

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
    this->lastMove = B_PRIME;

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
    this->lastMove = B2;

    swapCornerCubies(0, 6);
    swapCornerCubies(1, 5);

    swapEdgeCubies(0, 10);
    swapEdgeCubies(6, 7);
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

// memcmp-based goal test: compares the entire corners/edges arrays
// against pre-computed solved state in a single operation per array.
bool Cube::isSolved(void)
{
    return isCornerSolved() && isEdgeSolved();
}

bool Cube::isCornerSolved(void)
{
    return std::memcmp(corners.data(), SOLVED_CORNERS.data(),
                       NUM_CORNERS * sizeof(Cubie)) == 0;
}

bool Cube::isEdgeSolved(void)
{
    return std::memcmp(edges.data(), SOLVED_EDGES.data(),
                       NUM_EDGES * sizeof(Cubie)) == 0;
}

// Function pointer table for fast move dispatch by index (0-17)
// Order: R,R',R2, U,U',U2, F,F',F2, D,D',D2, B,B',B2, L,L',L2
static void (Cube::*MOVE_FUNCS[Cube::NUM_MOVES])() = {
    &Cube::r, &Cube::rPrime, &Cube::r2,
    &Cube::u, &Cube::uPrime, &Cube::u2,
    &Cube::f, &Cube::fPrime, &Cube::f2,
    &Cube::d, &Cube::dPrime, &Cube::d2,
    &Cube::b, &Cube::bPrime, &Cube::b2,
    &Cube::l, &Cube::lPrime, &Cube::l2
};

void Cube::applyMoveByIndex(Cube& cube, int moveIndex)
{
    (cube.*MOVE_FUNCS[moveIndex])();
}

void Cube::applyMoves(std::string moveList)
{
    std::istringstream iss(moveList);
    std::vector<std::string> tokens{std::istream_iterator<std::string>{iss},
                                    std::istream_iterator<std::string>{}};

    for (const std::string& moveStr : tokens)
    {
        if (moveStr == "F")      f();
        else if (moveStr == "F'") fPrime();
        else if (moveStr == "F2") f2();
        else if (moveStr == "U")  u();
        else if (moveStr == "U'") uPrime();
        else if (moveStr == "U2") u2();
        else if (moveStr == "D")  d();
        else if (moveStr == "D'") dPrime();
        else if (moveStr == "D2") d2();
        else if (moveStr == "R")  r();
        else if (moveStr == "R'") rPrime();
        else if (moveStr == "R2") r2();
        else if (moveStr == "L")  l();
        else if (moveStr == "L'") lPrime();
        else if (moveStr == "L2") l2();
        else if (moveStr == "B")  b();
        else if (moveStr == "B'") bPrime();
        else if (moveStr == "B2") b2();
        else
        {
            throw std::runtime_error("Invalid move encountered in scramble: \"" + moveStr + "\"");
        }
    }

    this->lastMove = NONE;
    this->depth = 0;
}


