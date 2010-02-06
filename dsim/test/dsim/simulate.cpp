// STL
#include <cassert>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
// DSIM
#include <dsim/DSIM_Service.hpp>
#include <dsim/config/dsim-paths.hpp>

// ///////// M A I N ////////////
int main (int argc, char* argv[]) {

    // Schedule input file name
    std::string lScheduleInputFilename ("../samples/schedule01.csv");

    // Output log File
    std::string lLogFilename ("simulate.log");

    // Set the log parameters
    std::ofstream logOutputFile;
    // Open and clean the log outputfile
    logOutputFile.open (lLogFilename.c_str());
    logOutputFile.clear();
    
    // Initialise the list of classes/buckets
    const stdair::BasLogParams lLogParams (stdair::LOG::DEBUG, logOutputFile);
    DSIM::DSIM_Service dsimService (lLogParams, lScheduleInputFilename);

    // Perform a simulation
    dsimService.simulate();
    
  return 0;	
}

