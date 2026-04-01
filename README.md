# Optimal Rubik's Cube Solver

An optimal Rubik's Cube solver implementing **Korf's IDA\* Algorithm** with **Pattern Databases (PDBs)**. This program finds the shortest possible sequence of moves to solve any scrambled cube.

---

## Project Origin & History

This project was originally developed as a university group project for **CMPT 417** (Intelligent Systems) at **Simon Fraser University** during the **Summer 2022** semester, taught by **Hang Ma**.

### Original Collaboration
- **Jack Manuel**
- **Keene Upathamp**
- **Tara H. Kazemi**

The original state of the project as submitted has been preserved and can be found under the GitHub tag **[v1.1-archive](https://github.com/jackmanuel/rubiks-cube-solver/tree/v1.1-archive)**.

The core implementation was heavily inspired by Benjamin Botto's Medium article:  
[Implementing an Optimal Rubik’s Cube Solver using Korf’s Algorithm](https://medium.com/@benjamin.botto/implementing-an-optimal-rubiks-cube-solver-using-korf-s-algorithm-bf750b332cf9)

---

## 2026 Revival

As of **2026**, I have revived this repository to refine the codebase, fix original submission issues, and optimize the performance. This revival is an independent project undertaken solely by myself (Jack Manuel), without the involvement of the original teammates.

---

## Technical Overview

The solver guarantees an optimal solution by utilizing **Iterative Deepening A\* (IDA\*)** search. To effectively prune the massive search space of 43 quintillion permutations, it relies on several precomputed **Pattern Databases (PDBs)**.

### Heuristics & Databases
- **Corners PDB**: Indexes 8 corner cubies (88,179,840 states).
- **Edge Groups 1 & 2**: Two disjoint databases each covering 7 edges (~511M states each).
- **Edge Orientation**: A smaller database tracking the orientation of all 12 edges (2,048 states).

The solver calculates the maximum distance from all available PDBs to provide a strong, admissible heuristic for the search algorithm.

---

## How to Run (Linux Only)

This project is built for Linux environments and requires `g++` and `make`.

### 1. Build the Project
Compile the `solver` executable using the provided Makefile:
```bash
make
```

### 2. Generate Pattern Databases
The solver requires several PDB files to function effectively. These total approximately **1GB** on disk. 

**Generation Warning:** Building the edge databases is a heavy operation. It can take **3+ hours** for each edge database. 

You can build all required databases with:
```bash
./solver --build all
```
Or target specific databases individually:
- `./solver --build corners`
- `./solver --build edge1`
- `./solver --build edge2`
- `./solver --build orient`

### 3. Solve a Scramble
Run the solver by passing a scramble string in standard Rubik's notation:
```bash
./solver "D L B2 R2 B' R2 U2 L2 B2 U2 B D2 L2 R' U B2 L R' B2 F'"
```

---

## Performance Expectation

Search time scales exponentially with the complexity of the scramble:

| Solution Length | Typical Solve Time |
| :--- | :--- |
| **0 - 14 Moves** | Virtually Instant |
| **15 - 16 Moves** | Minutes |
| **17 - 18 Moves** | Hours |

---

## Documentation
For a deep dive into the mathematical implementation and Lehmer indexing, see the [report](Korf_IDA_PDB_Report.pdf) included in this repository. The report was submitted as part of the original group project, and was contributed to by all three original members.

---

## References

- **Korf, R. E. (1997).** [Finding Optimal Solutions to Rubik's Cube Using Pattern Databases](https://www.cs.princeton.edu/courses/archive/fall06/cos402/papers/korfrubik.pdf). *AAAI-1997*.
- **Korf, R. E., & Felner, A. (2002).** [Disjoint pattern database heuristics](https://doi.org/10.1016/S0004-3702(01)00092-3). *Artificial Intelligence*, 134(1-2), 9–22.
- **Botto, B. (2020).** [Implementing an Optimal Rubik’s Cube Solver using Korf’s Algorithm](https://medium.com/@benjamin.botto/implementing-an-optimal-rubiks-cube-solver-using-korf-s-algorithm-bf750b332cf9). *Medium*.
