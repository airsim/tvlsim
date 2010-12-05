// STL
#include <iostream>
#include <string>
// CPPUNIT
#include <extracppunit/CppUnitCore.hpp>
// TRAVELCCM
#include <travelccm/TRAVELCCM_Service.hpp>
// TRAVEL-CCM Test Suite
#include <test/travelccm/TravelChoiceTestSuite.hpp>

// //////////////////////////////////////////////////////////////////////
void testTravelChoiceHelper() {

  try {
    
    // Output log File
    std::string lLogFilename ("TravelChoiceTestSuite.log");
    
    // Number of random draws to be generated (best if greater than 100)
    // const int K = 100000;
    
    // Input file name
    // const std::string inputFileName ("samples/sample2.csv");
    // const bool hasInputFile = true;
    
    // Set the log parameters
    std::ofstream logOutputFile;
    // open and clean the log outputfile
    logOutputFile.open (lLogFilename.c_str());
    logOutputFile.clear();
    
    // Initialise the service context
    const stdair::BasLogParams lLogParams (stdair::LOG::DEBUG, logOutputFile);
    const TRAVELCCM::TravelCCMType lCCMType (TRAVELCCM::TravelCCMType::PREF_BASED);
    TRAVELCCM::TRAVELCCM_Service travelccmService (lLogParams, lCCMType);
    
    // Start a mini-simulation
    travelccmService.simulate();
    
  } catch (const std::exception& stde) {
    std::cerr << "Standard exception: " << stde.what() << std::endl;
    
  } catch (...) {
    std::cerr << "Unknown exception" << std::endl;
  }
}

// //////////////////////////////////////////////////////////////////////
void TravelChoiceTestSuite::testTravelChoice() {
  CPPUNIT_ASSERT_NO_THROW (testTravelChoiceHelper(););
}

// //////////////////////////////////////////////////////////////////////
// void TravelChoiceTestSuite::errorCase () {
//  CPPUNIT_ASSERT (false);
// }

// //////////////////////////////////////////////////////////////////////
TravelChoiceTestSuite::TravelChoiceTestSuite () {
  _describeKey << "Running test on TRAVEL-CCM Optimisation function";  
}

// /////////////// M A I N /////////////////
CPPUNIT_MAIN()
