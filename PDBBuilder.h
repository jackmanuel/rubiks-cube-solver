#pragma once

/* ideas:

iterate through ~420M states that are 11 moves or fewer from solved

for every unique state, add 24 entries to the PDB for 24 symmetries


*/

class PDBBuilder
{
    public:
        static const int NUM_CORNER_RANKS = 88179840;
        static const int NUM_EDGE_RANKS = 510935040;

        static void buildCorners(void);
        static void buildEdges1(void);
        static std::array<uint8_t, NUM_CORNER_RANKS>* getPDB(std::string filename);

    private:
        PDBBuilder();
        static const int MAX_CORNER_DEPTH = 11;
};