// STL
#include <cassert>
#include <sstream>
#include <fstream>
#include <string>
// StdAir
#include <stdair/stdair_basic_types.hpp>
#include <stdair/basic/BasLogParams.hpp>
#include <stdair/basic/BasDBParams.hpp>
#include <stdair/bom/TravelSolutionStruct.hpp>
#include <stdair/service/Logger.hpp>
// Airrac
#include <airrac/AIRRAC_Service.hpp>
#include <airrac/config/airrac-paths.hpp>

// ///////// M A I N ////////////
int main (int argc, char* argv[]) {

  // Airline code
  const stdair::AirlineCode_T lAirlineCode ("SV");
    
  // Travel solution
  stdair::TravelSolutionStruct lTravelSolution;
    
  // Input file name
  const stdair::Filename_T lInputFilename (STDAIR_SAMPLE_DIR
                                           "/yieldstore01.csv");

  // Output log File
  const stdair::Filename_T lLogFilename ("airrac.log");

  // Set the log parameters
  std::ofstream logOutputFile;
  // Open and clean the log outputfile
  logOutputFile.open (lLogFilename.c_str());
  logOutputFile.clear();
  
  // Initialise the list of classes/buckets
  const stdair::BasLogParams lLogParams (stdair::LOG::DEBUG, logOutputFile);
  AIRRAC::AIRRAC_Service airracService (lLogParams, lAirlineCode,
                                        lInputFilename);
  
  // Calculate the yields for the given travel solution
  airracService.calculateYield (lTravelSolution);
  
  // Close the log file
  logOutputFile.close();
  
  return 0;	
}
