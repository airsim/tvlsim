// STL
#include <cassert>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
// StdAir
#include <stdair/STDAIR_Types.hpp>
#include <stdair/basic/BasLogParams.hpp>
#include <stdair/basic/BasDBParams.hpp>
// DSIM
#include <dsim/DSIM_Service.hpp>
#include <dsim/config/dsim-paths.hpp>

// ///////// M A I N ////////////
int main (int argc, char* argv[]) {

  try {
    
    // Schedule input file name
    const std::string lScheduleInputFilename ("../samples/schedule01.csv");

    // Demand input file name
    const stdair::Filename_T lDemandInputFilename ("../samples/demand01.csv");
    
    // Output log File
    const std::string lLogFilename ("simulate.log");

    // Set the log parameters
    std::ofstream logOutputFile;
    // Open and clean the log outputfile
    logOutputFile.open (lLogFilename.c_str());
    logOutputFile.clear();
    
    // Initialise the simulation context
    const stdair::BasLogParams lLogParams (stdair::LOG::DEBUG, logOutputFile);
    const stdair::BasDBParams lDBParams ("dsim", "dsim", "localhost", "3306",
                                         "dsim");
    DSIM::DSIM_Service dsimService (lLogParams, lDBParams,
                                    lScheduleInputFilename,
                                    lDemandInputFilename);
    // Perform a simulation
    dsimService.simulate();
    
  } catch (const DSIM::RootException& otexp) {
    std::cerr << "Standard exception: " << otexp.what() << std::endl;
    return -1;
    
  } catch (const std::exception& stde) {
    std::cerr << "Standard exception: " << stde.what() << std::endl;
    return -1;
    
  } catch (...) {
    return -1;
  }
  
  return 0;	
}

