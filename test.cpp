#include <iostream>

#include "Cube.h"

int main(int argc, char const *argv[])
{
    std::cout << "begin!" << std::endl;

    Cube cube;

    cube.r();

    cube.printCubeState();

    cube.r();
    cube.r();
    cube.r();

    cube.printCubeState();

    return 0;
}