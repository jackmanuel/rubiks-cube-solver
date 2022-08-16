#pragma once

#include <vector>

#include "Cube.h"

class Solver
{
    public:
        static const int MAX_MOVES = 20;
        static std::string solve(Cube cube);

    private:

        static std::string movesToString(std::array<Cube::move, Solver::MAX_MOVES> movesApplied);

        Solver(void);
};