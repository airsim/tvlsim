// STL
#include <cassert>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
// StdAir
#include <stdair/stdair_basic_types.hpp>
#include <stdair/basic/BasLogParams.hpp>
#include <stdair/basic/BasDBParams.hpp>
#include <stdair/bom/TravelSolutionStruct.hpp>
// Airrac
#include <airrac/AIRRAC_Service.hpp>
#include <airrac/config/airrac-paths.hpp>

// ///////// M A I N ////////////
int main (int argc, char* argv[]) {

  try {
    
    // Airline code
    const std::string lAirlineCode ("SV");
    
    // Travel solution
    // stdair::TravelSolutionStruct lTravelSolution;
    
    // Input file name
    stdair::Filename_T lInputFilename ("../../test/samples/yieldstore01.csv");

    // Output log File
    stdair::Filename_T lLogFilename ("airrac.log");

    // Set the log parameters
    std::ofstream logOutputFile;
    // Open and clean the log outputfile
    logOutputFile.open (lLogFilename.c_str());
    logOutputFile.clear();
    
    // Initialise the list of classes/buckets
    const stdair::BasLogParams lLogParams (stdair::LOG::DEBUG, logOutputFile);
    AIRRAC::AIRRAC_Service airracService (lLogParams, lAirlineCode,
                                          lInputFilename);

    // Calculate a yield
    // airracService.calculateYield (lTravelSolution);
    
  } catch (const std::exception& stde) {
    std::cerr << "Standard exception: " << stde.what() << std::endl;
    return -1;
    
  } catch (...) {
    return -1;
  }
  
  return 0;	
}
