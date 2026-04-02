#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdexcept>

#ifdef _WIN32
#include <direct.h>
#define MKDIR(dir) _mkdir(dir)
#else
#include <sys/stat.h>
#define MKDIR(dir) mkdir(dir, 0755)
#endif

#include "Cube.h"
#include "PDBBuilder.h"
#include "Solver.h"
#include "TransitionTable.h"
#include "DatabaseConstants.h"

using namespace DatabaseConstants;

static bool fileExists(const std::string& filename)
{
    std::ifstream f(filename);
    return f.good();
}

static std::vector<std::string> getMissingDatabases()
{
    std::vector<std::string> missing;
    if (!fileExists(CORNER_DB))  missing.push_back(CORNER_DB);
    if (!fileExists(EDGE1_DB))   missing.push_back(EDGE1_DB);
    if (!fileExists(EDGE2_DB))   missing.push_back(EDGE2_DB);
    if (!fileExists(ORIENT_DB))  missing.push_back(ORIENT_DB);
    return missing;
}

static void generateDatabases(const std::vector<std::string>& missing)
{
    // Create the Databases directory if it doesn't exist
    MKDIR(DB_DIR.c_str());

    size_t total = missing.size();
    size_t current = 1;

    for (const std::string& db : missing)
    {
        if (db == CORNER_DB)
        {
            std::cout << "\n[" << current << "/" << total << "] Generating corner pattern database..." << std::endl;
            std::cout << "(this is the smallest, ~88M entries)" << std::endl;
            PDBBuilder::buildCorners();
            std::cout << "Done!" << std::endl;
        }
        else if (db == EDGE1_DB)
        {
            std::cout << "\n[" << current << "/" << total << "] Generating edge group 1 pattern database..." << std::endl;
            std::cout << "(This is large at ~511M entries. It may take 3+ hours)" << std::endl;
            PDBBuilder::buildEdges1();
            std::cout << "Done!" << std::endl;
        }
        else if (db == EDGE2_DB)
        {
            std::cout << "\n[" << current << "/" << total << "] Generating edge group 2 pattern database..." << std::endl;
            std::cout << "(This is large at ~511M entries. It may take 3+ hours)" << std::endl;
            PDBBuilder::buildEdges2();
            std::cout << "Done!" << std::endl;
        }
        else if (db == ORIENT_DB)
        {
            std::cout << "\n[" << current << "/" << total << "] Generating edge orientation pattern database..." << std::endl;
            std::cout << "(this is the smallest, 2048 entries — very fast)" << std::endl;
            PDBBuilder::buildEdgeOrient();
            std::cout << "Done!" << std::endl;
        }
        
        current++;
    }
}

int main(int argc, char const *argv[])
{
    if (argc < 2)
    {
        std::cerr << "Usage: ./solver \"<scramble>\"" << std::endl;
        std::cerr << "       ./solver --build <corners|edge1|edge2|orient|all>" << std::endl;
        std::cerr << "       ./solver --build-tables" << std::endl;
        std::cerr << "Example: ./solver \"D L B2 R2 B' R2 U2 L2 B2 U2 B D2 L2 R' U B2 L R' B2 F'\"" << std::endl;
        return 1;
    }

    if (std::string(argv[1]) == "--build")
    {
        if (argc < 3)
        {
            std::cerr << "Error: Target required for --build." << std::endl;
            std::cerr << "Usage: ./solver --build <corners|edge1|edge2|orient|all>" << std::endl;
            return 1;
        }

        std::string target = argv[2];
        std::vector<std::string> toBuild;
        
        if (target == "corners")     toBuild.push_back(CORNER_DB);
        else if (target == "edge1")  toBuild.push_back(EDGE1_DB);
        else if (target == "edge2")  toBuild.push_back(EDGE2_DB);
        else if (target == "orient") toBuild.push_back(ORIENT_DB);
        else if (target == "all") 
        {
            toBuild.push_back(CORNER_DB);
            toBuild.push_back(EDGE1_DB);
            toBuild.push_back(EDGE2_DB);
            toBuild.push_back(ORIENT_DB);
        }
        else
        {
            std::cerr << "Invalid build target. Options: corners, edge1, edge2, orient, all" << std::endl;
            return 1;
        }

        try
        {
            generateDatabases(toBuild);
            std::cout << "\nSuccessfully built target: " << target << std::endl;
        }
        catch (const std::exception& e)
        {
            std::cerr << "\nError during database generation: " << e.what() << std::endl;
            return 1;
        }
        return 0;
    }

    if (std::string(argv[1]) == "--build-tables")
    {
        MKDIR(DB_DIR.c_str());
        try
        {
            TransitionTable::generate();
            std::cout << "\nTransition tables generated and saved." << std::endl;
        }
        catch (const std::exception& e)
        {
            std::cerr << "\nError during table generation: " << e.what() << std::endl;
            return 1;
        }
        return 0;
    }

    // Check for missing pattern databases before doing anything
    std::vector<std::string> missing = getMissingDatabases();

    if (!missing.empty())
    {
        std::cerr << "========================================" << std::endl;
        std::cerr << " Pattern databases not found!" << std::endl;
        std::cerr << "========================================" << std::endl;
        std::cerr << std::endl;
        std::cerr << "The solver requires 4 pattern database files in the '" << DB_DIR << "/' directory." << std::endl;
        std::cerr << "The following file(s) are missing:" << std::endl;
        std::cerr << std::endl;
        for (const std::string& db : missing)
        {
            std::cerr << "  - " << db << std::endl;
        }
        std::cerr << std::endl;
        std::cerr << "These databases can be generated automatically, but be aware:" << std::endl;
        std::cerr << "  * The edge databases (~511M entries each) may take 3+ HOURS to generate." << std::endl;
        std::cerr << "  * Generation requires significant memory (1GB+)." << std::endl;
        std::cerr << "  * The total database size on disk is ~1GB." << std::endl;
        std::cerr << std::endl;
        std::cerr << "Would you like to generate the missing databases now? [y/N]: ";

        std::string response;
        std::getline(std::cin, response);

        if (response == "y" || response == "Y" || response == "yes" || response == "Yes")
        {
            std::cout << std::endl;
            std::cout << "Generating missing pattern databases..." << std::endl;
            std::cout << "This may take a very long time. Do not interrupt the process." << std::endl;

            try
            {
                generateDatabases(missing);
                std::cout << std::endl;
                std::cout << "All databases generated successfully!" << std::endl;
                std::cout << "Proceeding with solve..." << std::endl;
                std::cout << std::endl;
            }
            catch (const std::exception& e)
            {
                std::cerr << "Error during database generation: " << e.what() << std::endl;
                return 1;
            }
        }
        else
        {
            std::cerr << std::endl;
            std::cerr << "Cannot solve without pattern databases. Exiting." << std::endl;
            return 1;
        }
    }

    try
    {
        std::cout << "Starting program." << std::endl;

        // Initialize transition tables (load from disk or generate)
        MKDIR(DB_DIR.c_str());
        TransitionTable::init();

        Cube cube;
        cube.applyMoves(argv[1]);

        std::string solution = Solver::solve(cube);

        std::cout << "Scramble: " << argv[1] << std::endl;
        if (!solution.empty())
        {
            std::cout << "Solution: " << solution << std::endl;
        }

        TransitionTable::cleanup();
        std::cout << "Exiting program." << std::endl;
    }
    catch (const std::runtime_error& e)
    {
        std::cerr << std::endl;
        std::cerr << "Error: " << e.what() << std::endl;
        TransitionTable::cleanup();
        return 1;
    }
    catch (const std::exception& e)
    {
        std::cerr << std::endl;
        std::cerr << "An unexpected error occurred: " << e.what() << std::endl;
        TransitionTable::cleanup();
        return 1;
    }

    return 0;
}