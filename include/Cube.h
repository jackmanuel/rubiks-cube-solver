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
#include <cstring>
#include <array>
#include <string>
#include <vector>

class Cube
{
    public:

        static const int NUM_CORNERS = 8;
        static const int NUM_EDGES = 12;
        static const int NUM_MOVES = 18;

        bool operator==(const Cube &cube) const
        {
            return (this->edges == cube.edges) && (this->corners == cube.corners);
        }

        bool operator<(const Cube &cube) const
        {
            return this->depth < cube.depth;
        }

        enum move
        {
            NONE,
            R,
            R_PRIME,
            R2,
            U,
            U_PRIME,
            U2,
            F,
            F_PRIME,
            F2,
            D,
            D_PRIME,
            D2,
            B,
            B_PRIME,
            B2,
            L,
            L_PRIME,
            L2
        };

        enum layer
        {
            UP, DOWN, LEFT, RIGHT
        };

        enum Face { R_FACE = 0, U_FACE, F_FACE, D_FACE, B_FACE, L_FACE };
        
        struct Cubie
        {
            uint8_t index;
            uint8_t orientation;

            bool operator==(const Cubie &cubie) const
            {
                return (this->index == cubie.index) && (this->orientation == cubie.orientation);
            }

        };



        Cube();
        bool isSolved(void);
        bool isEdgeSolved(void);
        bool isCornerSolved(void);

        void printCubeState(void);
        void applyMoves(std::string moveList);

        std::vector<Cube> generateNeighbours2(void);
        std::vector<Cube> generateNeighbours(void);
        std::vector<Cube> generateNeighboursPruned(void);
        std::array<Cubie, Cube::NUM_CORNERS>& getCorners(void);
        std::array<Cubie, Cube::NUM_EDGES>& getEdges(void);
        Cube::move getLastMove(void);

        uint8_t getDepth(void);
        void setDepth(uint8_t newDepth);

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

        void x();
        void xPrime();
        void x2();

        void y();
        void yPrime();
        void y2();

        void z();
        void zPrime();
        void z2();

        void m();
        void mPrime();
        void m2();

        void e();
        void ePrime();
        void e2();

        void s();
        void sPrime();
        void s2();

        // Dispatch: apply move by integer index (0-17)
        // Order: R,R',R2, U,U',U2, F,F',F2, D,D',D2, B,B',B2, L,L',L2
        static void applyMoveByIndex(Cube& cube, int moveIndex);
        
        const Face* getMap() const { return curr_map; }


    private:

        Face curr_map[6];
        void applyMapped(Face f, int amount);

        void r_array();
        void r_arrayPrime();
        void r_array2();

        void u_array();
        void u_arrayPrime();
        void u_array2();

        void f_array();
        void f_arrayPrime();
        void f_array2();

        void d_array();
        void d_arrayPrime();
        void d_array2();

        void l_array();
        void l_arrayPrime();
        void l_array2();

        void b_array();
        void b_arrayPrime();
        void b_array2();


        std::array<Cubie, 12> edges;
        std::array<Cubie, 8> corners;
        move lastMove;
        uint8_t depth;

        static const std::array<Cubie, NUM_CORNERS> SOLVED_CORNERS;
        static const std::array<Cubie, NUM_EDGES> SOLVED_EDGES;

        void updateCornerOrientation(uint8_t corner_index, uint8_t change);
        void flipEdgeOrientation(uint8_t edge_index);

        void swapCornerCubies(uint8_t first_index, uint8_t second_index);
        void swapEdgeCubies(uint8_t first_index, uint8_t second_index);

        void fourCycleCorners(uint8_t c1, uint8_t c2, uint8_t c3, uint8_t c4);
        void fourCycleEdges(uint8_t e1, uint8_t e2, uint8_t e3, uint8_t e4);
};