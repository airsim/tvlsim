// STL
#include <iostream>
#include <string>
// CPPUNIT
#include <extracppunit/CppUnitCore.hpp>
// TravelCCM
#include <travelccm/TRAVELCCM_Service.hpp>
#include <travelccm/config/travelccm-paths.hpp>
// TravelCCM Test Suite
#include <test/travelccm/TravelChoiceTestSuite.hpp>

// //////////////////////////////////////////////////////////////////////
void testTravelChoiceHelper() {

  // Output log File
  const std::string lLogFilename ("TravelChoiceTestSuite.log");
    
  // Number of random draws to be generated (best if greater than 100)
  // const int K = 100000;
  
  // Input file name
  // const std::string inputFileName (STDAIR_SAMPLE_DIR "/ccm_02.csv");
  // const bool hasInputFile = true;
  
  // Set the log parameters
  std::ofstream logOutputFile;
  // Open and clean the log outputfile
  logOutputFile.open (lLogFilename.c_str());
  logOutputFile.clear();
  
  // Initialise the service context
  const stdair::BasLogParams lLogParams (stdair::LOG::DEBUG, logOutputFile);
  const TRAVELCCM::TravelCCMType lCCMType(TRAVELCCM::TravelCCMType::PREF_BASED);
  TRAVELCCM::TRAVELCCM_Service travelccmService (lLogParams, lCCMType);
  
  // Start a mini-simulation
  travelccmService.simulate();
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
