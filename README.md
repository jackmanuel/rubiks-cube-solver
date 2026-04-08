# Optimal Rubik's Cube Solver

An optimal Rubik's Cube solver implementing Korf's IDA* Algorithm with Pattern Databases (PDBs). This program finds the shortest possible sequence of moves to solve any scrambled cube.

## Project Origin & History

This project was originally developed as a university group project for CMPT 417 (Intelligent Systems) at Simon Fraser University during the Summer 2022 semester. The original group submission by Jack Manuel, Keene Upathamp, and Tara H. Kazemi has been preserved under the [v1.1-archive](https://github.com/jackmanuel/rubiks-cube-solver/tree/v1.1-archive) tag. For a deep dive into the mathematical implementation, see the [original project report](docs/Korf_IDA_PDB_Report.pdf).

The core implementation was heavily inspired by Benjamin Botto's Medium article: [Implementing an Optimal Rubik’s Cube Solver using Korf’s Algorithm](https://medium.com/@benjamin.botto/implementing-an-optimal-rubiks-cube-solver-using-korf-s-algorithm-bf750b332cf9).

The original project has been completely overhauled and improved upon since the initial submission, including core architectural changes. This was solely developed by Jack Manuel.

## Technical Architecture

The solver guarantees an optimal solution using an Iterative Deepening A* (IDA*) search, guided by several precomputed Pattern Databases (PDBs). It tracks the corners (88 million states), two sets of seven edges (511 million states each), and edge orientations. The maximum distance from these databases provides an admissible heuristic to prune the search space.

The search relies on a transition table architecture. Instead of manipulating full cube structures, the state is abstracted into numeric values. Precomputed arrays map how a move transforms one state to another, allowing the algorithm to apply moves in constant time via array lookups. Search performance is further accelerated through multithreading support, enabling the program to utilize as many CPU cores as possible during the solve. An automated Python script is provided in the `benchmark` directory to track evaluation speeds and catch performance regressions.

## How to Run (Linux Only)

This project is built for Linux environments and requires `g++` and `make`. You will also need Python 3 to run the benchmarking script.

### Cloning the Repository

To get started, first download the code to your machine by cloning the repository using Git. Open your terminal and run the following command:

```bash
git clone https://github.com/jackmanuel/rubiks-cube-solver.git
cd rubiks-cube-solver
```

### Building the Project

Compile the `solver` executable using the provided Makefile by running the following command in your terminal:

```bash
make
```

### Generating Pattern Databases

The solver requires several database files to function effectively. These total approximately 1GB on disk. All the databases can be generated in less than one minute, depending on hardware. The transition tables must also be generated before searching, and this process is also quick.

You can generate all required data by running the transition table and database build commands:

```bash
./solver --build-tables
./solver --build all
```

Alternatively, you can target specific databases individually by replacing `all` with `corners`, `edge1`, `edge2`, or `orient`.

### Solving a Scramble

Run the solver by passing a scramble string:

```bash
./solver "D L B2 R2 B' R2 U2 L2 B2 U2 B D2 L2 R' U B2 L R' B2 F'"
```

The solver accepts all standard move notation, including wide moves (r), slice moves (S), and rotations (x).

Loading the pattern databases from disk into memory takes several seconds. To avoid this overhead when solving multiple cubes, use the `--continuous` flag to enter an interactive session to input scrambles one by one, or use the `--input` flag followed by a filename to process a list of scrambles sequentially.

## Performance Expectation

Search time scales exponentially with the complexity of the scramble. With multithreaded evaluation, a solution length of 14 moves or fewer is virtually instant. A scramble requiring 15 to 16 moves takes seconds to a minute, while 17 moves takes minutes. Scrambles requiring 18 to 20 moves may take hours or even days to compute.

## References

Korf, R. E. (1997). [Finding Optimal Solutions to Rubik's Cube Using Pattern Databases](https://www.cs.princeton.edu/courses/archive/fall06/cos402/papers/korfrubik.pdf). AAAI-1997.

Korf, R. E., & Felner, A. (2002). [Disjoint pattern database heuristics](https://doi.org/10.1016/S0004-3702(01)00092-3). Artificial Intelligence, 134(1-2), 9–22.

Botto, B. (2020). [Implementing an Optimal Rubik’s Cube Solver using Korf’s Algorithm](https://medium.com/@benjamin.botto/implementing-an-optimal-rubiks-cube-solver-using-korf-s-algorithm-bf750b332cf9). Medium.
