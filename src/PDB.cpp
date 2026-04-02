#include <iostream>
#include <fstream>
#include <stdexcept>

#include "PDB.h"

PDB::PDB(std::string cornerFilename, std::string edge1Filename, 
         std::string edge2Filename, std::string orientFilename)
{
    this->indexer = Indexer();

    std::cout << "Loading pattern databases..." << std::endl;
    cornerDB = loadPDB<PDBBuilder::NUM_CORNER_RANKS>(cornerFilename);
    edge1DB = loadPDB<PDBBuilder::NUM_EDGE_RANKS>(edge1Filename);
    edge2DB = loadPDB<PDBBuilder::NUM_EDGE_RANKS>(edge2Filename);
    orientDB = loadPDB<PDBBuilder::NUM_EDGE_ORIENTATIONS>(orientFilename);
    std::cout << "Successfully loaded PDBs!" << std::endl;
}

PDB::~PDB(void)
{
    delete cornerDB;
    delete edge1DB;
    delete edge2DB;
    delete orientDB;
}

int PDB::getMoveCountLowerBound(Cube cube)
{
    uint8_t cornerMoves = (*cornerDB)[indexer.getCornerIndex(cube)];
    uint8_t edgeMoves1 = (*edge1DB)[indexer.getEdgeIndex1(cube)];
    uint8_t edgeMoves2 = (*edge2DB)[indexer.getEdgeIndex2(cube)];
    uint8_t orientMoves = (*orientDB)[indexer.getEdgeOrientRankCube(cube)];

    uint8_t max = cornerMoves;

    if (edgeMoves1 > max) { max = edgeMoves1; }
    if (edgeMoves2 > max) { max = edgeMoves2; }
    if (orientMoves > max) { max = orientMoves; }

    return (int)max;
}



template<std::size_t SIZE>
std::array<uint8_t, SIZE>* PDB::loadPDB(std::string filename)
{   
    std::array<uint8_t, SIZE>* database = new std::array<uint8_t, SIZE>{0};

    std::ifstream reader(filename, std::ios::in);

    if (!reader.is_open())
    {
        throw std::runtime_error("Failed to open pattern database file: " + filename);
    }

    reader.seekg(0, std::ios::beg);
    reader.read(
      (char*)database,
      sizeof(uint8_t) * SIZE);
    reader.close();

    return database;
}



