#pragma once

/* ideas:

iterate through ~420M states that are 11 moves or fewer from solved

for every unique state, add 24 entries to the PDB for 24 symmetries


*/

class PDBBuilder
{
    public:
        PDBBuilder();
        void buildCorners(void);


    private:
        static const int NUM_CORNER_RANKS = 88179840;
        static const int MAX_CORNER_DEPTH = 11;
};