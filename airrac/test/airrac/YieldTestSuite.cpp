// STL
#include <sstream>
#include <fstream>
#include <string>
// CPPUNIT
#include <extracppunit/CppUnitCore.hpp>
// StdAir
#include <stdair/basic/BasLogParams.hpp>
#include <stdair/basic/BasDBParams.hpp>
#include <stdair/bom/TravelSolutionStruct.hpp>
// Airrac
#include <airrac/AIRRAC_Types.hpp>
#include <airrac/AIRRAC_Service.hpp>
// Airrac Test Suite
#include <test/airrac/YieldTestSuite.hpp>

// //////////////////////////////////////////////////////////////////////
// Test is based on ...
// //////////////////////////////////////////////////////////////////////

// //////////////////////////////////////////////////////////////////////
void YieldTestSuite::simpleYieldHelper() {

  try {
    
    // Airline code
    const std::string lAirlineCode ("SV");
    
    // Number of passengers in the travelling group
    // const stdair::PartySize_T lPartySize = 5;

    // Travel solution
    // stdair::TravelSolutionStruct lTravelSolution;
    
    // Input file name
    stdair::Filename_T lInputFilename ("../../test/samples/invdump01.csv");

    // Output log File
    stdair::Filename_T lLogFilename ("YieldTestSuite.log");

    // Set the log parameters
    std::ofstream logOutputFile;
    // Open and clean the log outputfile
    logOutputFile.open (lLogFilename.c_str());
    logOutputFile.clear();
    
    // Initialise the list of classes/buckets
    const stdair::BasLogParams lLogParams (stdair::LOG::DEBUG, logOutputFile);
    AIRRAC::AIRRAC_Service airracService (lLogParams, lAirlineCode,
                                          lInputFilename);

    // Make a booking
    // airracService.sell (lTravelSolution, lPartySize);
    
  } catch (const AIRRAC::RootException& otexp) {
    std::cerr << "Standard exception: " << otexp.what() << std::endl;
    return;
    
  } catch (const std::exception& stde) {
    std::cerr << "Standard exception: " << stde.what() << std::endl;
    return;
    
  } catch (...) {
    return;
  }
  
}

// //////////////////////////////////////////////////////////////////////
void YieldTestSuite::simpleYield () {
  // TODO: Check that the yield goes as expected
  CPPUNIT_ASSERT_NO_THROW ( simpleYieldHelper(););
}

// //////////////////////////////////////////////////////////////////////
// void YieldTestSuite::errorCase () {
//  CPPUNIT_ASSERT (false);
// }

// //////////////////////////////////////////////////////////////////////
YieldTestSuite::YieldTestSuite () {
  _describeKey << "Running test on yield";  
}

// /////////////// M A I N /////////////////
CPPUNIT_MAIN()

