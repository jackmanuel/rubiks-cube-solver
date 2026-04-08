#pragma once

#include <array>
#include <cstdint>
#include <string>

#include "Indexer.h"
#include "PDBBuilder.h"

class PDB
{
    public:
        PDB(std::string cornerFilename, std::string edge1Filename, 
            std::string edge2Filename, std::string orientFilename);
        ~PDB(void);



        // Direct array access for the coordinate-level solver
        const std::array<uint8_t, PDBBuilder::NUM_CORNER_RANKS>& getCornerDB() const { return *cornerDB; }
        const std::array<uint8_t, PDBBuilder::NUM_EDGE_RANKS>& getEdge1DB() const { return *edge1DB; }
        const std::array<uint8_t, PDBBuilder::NUM_EDGE_RANKS>& getEdge2DB() const { return *edge2DB; }


    private:

        Indexer indexer;

        std::array<uint8_t, PDBBuilder::NUM_CORNER_RANKS>* cornerDB;
        std::array<uint8_t, PDBBuilder::NUM_EDGE_RANKS>* edge1DB;
        std::array<uint8_t, PDBBuilder::NUM_EDGE_RANKS>* edge2DB;
        std::array<uint8_t, PDBBuilder::NUM_EDGE_ORIENTATIONS>* orientDB;

        template<std::size_t SIZE>
        std::array<uint8_t, SIZE>* loadPDB(std::string filename);
};       
