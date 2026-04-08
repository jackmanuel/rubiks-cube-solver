#pragma once

#include <string>

namespace DatabaseConstants {
    const std::string DB_DIR = "databases";

    // Pattern databases
    const std::string CORNER_DB = "databases/cornerDB.data";
    const std::string EDGE1_DB = "databases/edge1DB.data";
    const std::string EDGE2_DB = "databases/edge2DB.data";
    const std::string ORIENT_DB = "databases/edgeOrientDB.data";

    // Transition tables
    const std::string CORNER_PERM_TT = "databases/cornerPerm.tt";
    const std::string CORNER_ORIENT_TT = "databases/cornerOrient.tt";
    const std::string EDGE_PERM_TT = "databases/edgePerm.tt";
    const std::string EDGE_FLIP_TT = "databases/edgeFlip.tt";
}
