#include <iterator>
#include <sstream>

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

    for (int i = 0; i < 6; i++) {
        this->curr_map[i] = (Face)i;
    }

    this->lastMove = NONE;
    this->depth = 0;
}



std::array<Cube::Cubie, Cube::NUM_CORNERS>& Cube::getCorners(void)
{
    return this->corners;
}

uint8_t Cube::getDepth(void)
{
    return this->depth;
}



std::array<Cube::Cubie, Cube::NUM_EDGES>& Cube::getEdges(void)
{
    return this->edges;
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


void Cube::r_array()
{
    this->lastMove = R;

    fourCycleCorners(2, 7, 6, 1);

    updateCornerOrientation(2, 2);
    updateCornerOrientation(6, 2);

    updateCornerOrientation(1, 1);
    updateCornerOrientation(7, 1);

    fourCycleEdges(1, 4, 11, 7);
}

void Cube::r_arrayPrime()
{

    this->lastMove = R_PRIME;

    fourCycleCorners(2, 1, 6, 7);

    updateCornerOrientation(1, 1);
    updateCornerOrientation(7, 1);

    updateCornerOrientation(2, 2);
    updateCornerOrientation(6, 2);

    fourCycleEdges(1, 7, 11, 4);

}

void Cube::r_array2()
{
    this->lastMove = R2;

    swapCornerCubies(2, 6);
    swapCornerCubies(1, 7);

    swapEdgeCubies(1, 11);
    swapEdgeCubies(4, 7);
}



void Cube::u_array()
{
    this->lastMove = U;

    fourCycleCorners(2, 1, 0, 3);
    fourCycleEdges(1, 0, 3, 2);
}

void Cube::u_arrayPrime()
{
    this->lastMove = U_PRIME;

    fourCycleCorners(2, 3, 0, 1);
    fourCycleEdges(1, 2, 3, 0);
}

void Cube::u_array2()
{
    this->lastMove = U2;

    swapCornerCubies(0, 2);
    swapCornerCubies(3, 1);

    swapEdgeCubies(0, 2);
    swapEdgeCubies(3, 1);
}

void Cube::f_array()
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

void Cube::f_arrayPrime()
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


void Cube::f_array2()
{
    this->lastMove = F2;

    swapCornerCubies(2, 4);
    swapCornerCubies(3, 7);

    swapEdgeCubies(2, 8);
    swapEdgeCubies(5, 4);
}

void Cube::d_array()
{
    this->lastMove = D;

    fourCycleCorners(7, 4, 5, 6);
    fourCycleEdges(8, 9, 10, 11);
}

void Cube::d_arrayPrime()
{
    this->lastMove = D_PRIME;

    fourCycleCorners(4, 7, 6, 5);
    fourCycleEdges(9, 8, 11, 10);
}

void Cube::d_array2()
{
    this->lastMove = D2;

    swapCornerCubies(4, 6);
    swapCornerCubies(7, 5);

    swapEdgeCubies(9, 11);
    swapEdgeCubies(8, 10);
}

void Cube::l_array()
{
    this->lastMove = L;

    fourCycleCorners(3, 0, 5, 4);

    updateCornerOrientation(3, 1);
    updateCornerOrientation(5, 1);

    updateCornerOrientation(0, 2);
    updateCornerOrientation(4, 2);

    fourCycleEdges(5, 3, 6, 9);
}

void Cube::l_arrayPrime()
{
    this->lastMove = L_PRIME;

    fourCycleCorners(0, 3, 4, 5);

    updateCornerOrientation(3, 1);
    updateCornerOrientation(5, 1);

    updateCornerOrientation(0, 2);
    updateCornerOrientation(4, 2);

    fourCycleEdges(3, 5, 9, 6);
}

void Cube::l_array2()
{
    this->lastMove = L2;

    swapCornerCubies(3, 5);
    swapCornerCubies(0, 4);

    swapEdgeCubies(3, 9);
    swapEdgeCubies(6, 5);
}

void Cube::b_array()
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

void Cube::b_arrayPrime()
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

void Cube::b_array2()
{
    this->lastMove = B2;

    swapCornerCubies(0, 6);
    swapCornerCubies(1, 5);

    swapEdgeCubies(0, 10);
    swapEdgeCubies(6, 7);
}

void Cube::x() {
    Face tempU = curr_map[U_FACE], tempB = curr_map[B_FACE], tempD = curr_map[D_FACE], tempF = curr_map[F_FACE];
    curr_map[U_FACE] = tempF;
    curr_map[B_FACE] = tempU;
    curr_map[D_FACE] = tempB;
    curr_map[F_FACE] = tempD;
}
void Cube::xPrime() { x(); x(); x(); }
void Cube::x2() { x(); x(); }

void Cube::y() {
    Face tempL = curr_map[L_FACE], tempB = curr_map[B_FACE], tempR = curr_map[R_FACE], tempF = curr_map[F_FACE];
    curr_map[L_FACE] = tempF;
    curr_map[B_FACE] = tempL;
    curr_map[R_FACE] = tempB;
    curr_map[F_FACE] = tempR;
}
void Cube::yPrime() { y(); y(); y(); }
void Cube::y2() { y(); y(); }

void Cube::z() {
    Face tempR = curr_map[R_FACE], tempD = curr_map[D_FACE], tempL = curr_map[L_FACE], tempU = curr_map[U_FACE];
    curr_map[R_FACE] = tempU;
    curr_map[D_FACE] = tempR;
    curr_map[L_FACE] = tempD;
    curr_map[U_FACE] = tempL;
}
void Cube::zPrime() { z(); z(); z(); }
void Cube::z2() { z(); z(); }

void Cube::applyMapped(Face f, int amount) {
    Face target = this->curr_map[f];
    switch (target) {
        case U_FACE: amount == 1 ? u_array() : amount == 2 ? u_arrayPrime() : u_array2(); break;
        case D_FACE: amount == 1 ? d_array() : amount == 2 ? d_arrayPrime() : d_array2(); break;
        case F_FACE: amount == 1 ? f_array() : amount == 2 ? f_arrayPrime() : f_array2(); break;
        case B_FACE: amount == 1 ? b_array() : amount == 2 ? b_arrayPrime() : b_array2(); break;
        case L_FACE: amount == 1 ? l_array() : amount == 2 ? l_arrayPrime() : l_array2(); break;
        case R_FACE: amount == 1 ? r_array() : amount == 2 ? r_arrayPrime() : r_array2(); break;
    }
}

void Cube::r() { applyMapped(R_FACE, 1); }
void Cube::rPrime() { applyMapped(R_FACE, 2); }
void Cube::r2() { applyMapped(R_FACE, 3); }

void Cube::u() { applyMapped(U_FACE, 1); }
void Cube::uPrime() { applyMapped(U_FACE, 2); }
void Cube::u2() { applyMapped(U_FACE, 3); }

void Cube::f() { applyMapped(F_FACE, 1); }
void Cube::fPrime() { applyMapped(F_FACE, 2); }
void Cube::f2() { applyMapped(F_FACE, 3); }

void Cube::d() { applyMapped(D_FACE, 1); }
void Cube::dPrime() { applyMapped(D_FACE, 2); }
void Cube::d2() { applyMapped(D_FACE, 3); }

void Cube::b() { applyMapped(B_FACE, 1); }
void Cube::bPrime() { applyMapped(B_FACE, 2); }
void Cube::b2() { applyMapped(B_FACE, 3); }

void Cube::l() { applyMapped(L_FACE, 1); }
void Cube::lPrime() { applyMapped(L_FACE, 2); }
void Cube::l2() { applyMapped(L_FACE, 3); }

void Cube::m() { r(); lPrime(); xPrime(); }
void Cube::mPrime() { rPrime(); l(); x(); }
void Cube::m2() { r2(); l2(); x2(); }

void Cube::e() { u(); dPrime(); yPrime(); }
void Cube::ePrime() { uPrime(); d(); y(); }
void Cube::e2() { u2(); d2(); y2(); }

void Cube::s() { fPrime(); b(); z(); }
void Cube::sPrime() { f(); bPrime(); zPrime(); }
void Cube::s2() { f2(); b2(); z2(); }

void Cube::rw() { l(); x(); }
void Cube::rwPrime() { lPrime(); xPrime(); }
void Cube::rw2() { l2(); x2(); }

void Cube::lw() { r(); xPrime(); }
void Cube::lwPrime() { rPrime(); x(); }
void Cube::lw2() { r2(); x2(); }

void Cube::uw() { d(); y(); }
void Cube::uwPrime() { dPrime(); yPrime(); }
void Cube::uw2() { d2(); y2(); }

void Cube::dw() { u(); yPrime(); }
void Cube::dwPrime() { uPrime(); y(); }
void Cube::dw2() { u2(); y2(); }

void Cube::fw() { b(); z(); }
void Cube::fwPrime() { bPrime(); zPrime(); }
void Cube::fw2() { b2(); z2(); }

void Cube::bw() { f(); zPrime(); }
void Cube::bwPrime() { fPrime(); z(); }
void Cube::bw2() { f2(); z2(); }


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
        else if (moveStr == "M")  m();
        else if (moveStr == "M'") mPrime();
        else if (moveStr == "M2") m2();
        else if (moveStr == "E")  e();
        else if (moveStr == "E'") ePrime();
        else if (moveStr == "E2") e2();
        else if (moveStr == "S")  s();
        else if (moveStr == "S'") sPrime();
        else if (moveStr == "S2") s2();
        else if (moveStr == "x")  x();
        else if (moveStr == "x'") xPrime();
        else if (moveStr == "x2") x2();
        else if (moveStr == "y")  y();
        else if (moveStr == "y'") yPrime();
        else if (moveStr == "y2") y2();
        else if (moveStr == "z")  z();
        else if (moveStr == "z'") zPrime();
        else if (moveStr == "z2") z2();
        else if (moveStr == "Rw" || moveStr == "r") rw();
        else if (moveStr == "Rw'" || moveStr == "r'") rwPrime();
        else if (moveStr == "Rw2" || moveStr == "r2") rw2();
        else if (moveStr == "Lw" || moveStr == "l") lw();
        else if (moveStr == "Lw'" || moveStr == "l'") lwPrime();
        else if (moveStr == "Lw2" || moveStr == "l2") lw2();
        else if (moveStr == "Uw" || moveStr == "u") uw();
        else if (moveStr == "Uw'" || moveStr == "u'") uwPrime();
        else if (moveStr == "Uw2" || moveStr == "u2") uw2();
        else if (moveStr == "Dw" || moveStr == "d") dw();
        else if (moveStr == "Dw'" || moveStr == "d'") dwPrime();
        else if (moveStr == "Dw2" || moveStr == "d2") dw2();
        else if (moveStr == "Fw" || moveStr == "f") fw();
        else if (moveStr == "Fw'" || moveStr == "f'") fwPrime();
        else if (moveStr == "Fw2" || moveStr == "f2") fw2();
        else if (moveStr == "Bw" || moveStr == "b") bw();
        else if (moveStr == "Bw'" || moveStr == "b'") bwPrime();
        else if (moveStr == "Bw2" || moveStr == "b2") bw2();
        else
        {
            throw std::runtime_error("Invalid move encountered in scramble: \"" + moveStr + "\"");
        }
    }

    this->lastMove = NONE;
    this->depth = 0;
}


