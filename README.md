# Optimal Rubik's Cube Solver

An optimal Rubik's Cube solver implementing Korf's IDA* Algorithm with Pattern Databases (PDBs). This program finds the shortest possible sequence of moves to solve any scrambled cube.

## Project Origin & History

This project was originally developed as a university group project for CMPT 417 (Intelligent Systems) at Simon Fraser University during the Summer 2022 semester, taught by Hang Ma. The original collaboration was between Jack Manuel, Keene Upathamp, and Tara H. Kazemi. The original state of the project as submitted has been preserved and can be found under the GitHub tag [v1.1-archive](https://github.com/jackmanuel/rubiks-cube-solver/tree/v1.1-archive).

The core implementation was heavily inspired by Benjamin Botto's Medium article: [Implementing an Optimal Rubik’s Cube Solver using Korf’s Algorithm](https://medium.com/@benjamin.botto/implementing-an-optimal-rubiks-cube-solver-using-korf-s-algorithm-bf750b332cf9).

## 2026 Revival and Architecture

As of 2026, I have revived this repository to refine the codebase, fix original submission issues, and heavily optimize the performance. This revival is an independent project undertaken solely by myself (Jack Manuel).

The solver utilizes a high-performance transition table architecture. During the hot loop of the search algorithm, the program does not instantiate or manipulate full "Cube" objects. Instead, the cube's state is abstracted entirely into numeric values representing the permutation and orientation of its pieces (corners and edges). Arrays (transition tables) are precomputed to map how applying a specific move transforms one numeric state immediately into another. Because these tables are mathematically generated once and then saved to disk, the search algorithm can apply moves in constant time using simple array lookups, dramatically accelerating the solution time.

To ensure performance remains high, the project also includes an automated Python benchmarking suite in the `benchmark` directory. The benchmark script runs the solver against 10 simple scrambles, measuring the total elapsed time, the total number of states evaluated, and the average evaluation speed (states per second). You can save a run as a baseline and compare future modifications against it to instantly spot performance regressions or optimizations.

## Technical Overview

The solver guarantees an optimal solution by utilizing Iterative Deepening A* (IDA*) search. To effectively prune the massive search space of 43 quintillion permutations, it relies on several precomputed Pattern Databases (PDBs). A database is used for the corners, which indexes over 88 million states. The solver also uses two disjoint databases for the edges, each covering seven edges and containing around 511 million states. A smaller database tracks the orientation of all twelve edges. The solver calculates the maximum distance from all available databases to provide a strong, admissible heuristic for the search algorithm.

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

The solver requires several database files to function effectively. These total approximately 1GB on disk. Building the edge databases is a heavy operation and can take over three hours for each edge database. The transition tables must also be generated before searching, though this process is much faster.

You can build all required databases by running:

```bash
./solver --build all
```

Alternatively, you can target specific databases individually by replacing `all` with `corners`, `edge1`, `edge2`, or `orient`.

### Solving a Scramble

Run the solver by passing a scramble string in standard Rubik's notation:

```bash
./solver "D L B2 R2 B' R2 U2 L2 B2 U2 B D2 L2 R' U B2 L R' B2 F'"
```

## Performance Expectation

Search time scales exponentially with the complexity of the scramble. A solution length of 0 to 14 moves is virtually instant. A scramble requiring 15 to 16 moves will typically take minutes to solve, and scrambles requiring 17 to 18 moves will take several hours.

## Documentation

For a deep dive into the mathematical implementation and Lehmer indexing, see the [report](docs/Korf_IDA_PDB_Report.pdf) included in this repository. The report was submitted as part of the original group project, and was contributed to by all three original members.

## References

Korf, R. E. (1997). [Finding Optimal Solutions to Rubik's Cube Using Pattern Databases](https://www.cs.princeton.edu/courses/archive/fall06/cos402/papers/korfrubik.pdf). AAAI-1997.

Korf, R. E., & Felner, A. (2002). [Disjoint pattern database heuristics](https://doi.org/10.1016/S0004-3702(01)00092-3). Artificial Intelligence, 134(1-2), 9–22.

Botto, B. (2020). [Implementing an Optimal Rubik’s Cube Solver using Korf’s Algorithm](https://medium.com/@benjamin.botto/implementing-an-optimal-rubiks-cube-solver-using-korf-s-algorithm-bf750b332cf9). Medium.
