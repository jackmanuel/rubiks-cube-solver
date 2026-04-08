#!/bin/bash
# Splits c7762fc into two commits:
#   Commit A: CRLF → LF normalization only (+ .gitattributes)
#   Commit B: actual code changes (dead code removal, comment cleanup)
set -e

PARENT="c7762fc~1"
TARGET="c7762fc"

echo "=== Step 1: Mixed reset to parent (undo c7762fc, keep working tree) ==="
git reset --mixed "$PARENT"
# HEAD and index now at c7762fc~1 (CRLF files, original code)
# Working tree still has LF files + code changes from c7762fc

echo "=== Step 2: Restore working tree to c7762fc~1 (CRLF, original code) ==="
git checkout -- .
# Working tree now matches c7762fc~1 exactly: CRLF, original code

echo "=== Step 3: Add .gitattributes (text=auto) ==="
git checkout "$TARGET" -- .gitattributes

echo "=== Step 4: Re-normalize all files (CRLF → LF in index, working tree kept as-is) ==="
git add --renormalize .
# Index now has LF files with ORIGINAL code. Working tree still CRLF.

echo "=== Step 5: Commit A — normalization only ==="
git commit -m "Normalize line endings (CRLF to LF)"

echo "=== Step 6: Restore code-changed files from c7762fc ==="
# All files that had real code changes beyond CRLF
git checkout "$TARGET" -- \
    include/Cube.h \
    include/DatabaseConstants.h \
    include/Indexer.h \
    include/PDB.h \
    include/PDBBuilder.h \
    include/Solver.h \
    include/TransitionTable.h \
    src/Cube.cpp \
    src/Indexer.cpp \
    src/PDB.cpp \
    src/PDBBuilder.cpp \
    src/Solver.cpp \
    src/TransitionTable.cpp \
    src/main.cpp

echo "=== Step 7: Stage code changes ==="
git add .

echo "=== Step 8: Commit B — actual code changes ==="
git commit -m "Remove dead code and unnecessary comments"

echo "=== Done! New commits: ==="
git log --oneline -3
