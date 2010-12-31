// STL
#include <sstream>
#include <fstream>
#include <string>
// CPPUNIT
#include <extracppunit/CppUnitCore.hpp>
// StdAir
#include <stdair/STDAIR_Service.hpp>
// SimFQT
#include <simfqt/SIMFQT_Service.hpp>
// SimFQT Test Suite
#include <test/simfqt/FQTTestSuite.hpp>
#include <simfqt/config/simfqt-paths.hpp>

// //////////////////////////////////////////////////////////////////////
// Test is based on ...
// //////////////////////////////////////////////////////////////////////

// //////////////////////////////////////////////////////////////////////
void FQTTestSuite::simpleFQTHelper() {

  // Fare input file name
  const stdair::Filename_T lFareInputFilename (STDAIR_SAMPLE_DIR "/fare01.csv");
    
  // Output log File
  const std::string lLogFilename ("FQTTestSuite.log");

  // Set the log parameters
  std::ofstream logOutputFile;
  // Open and clean the log outputfile
  logOutputFile.open (lLogFilename.c_str());
  logOutputFile.clear();
    
  // Initialise the list of classes/buckets
  const stdair::BasLogParams lLogParams (stdair::LOG::DEBUG, logOutputFile);
  SIMFQT::SIMFQT_Service simfqtService (lLogParams, lFareInputFilename);

  // Perform a price quotation
  // const std::string lAirlineCode ("SV");
  // const STDAIR::PartySize_T lPartySize = 5;
  // simfqtService.priceQuote (lAirlineCode, lPartySize);
}

// //////////////////////////////////////////////////////////////////////
void FQTTestSuite::simpleFQT () {
  // TODO: Check that the FQT goes as expected
  CPPUNIT_ASSERT_NO_THROW ( simpleFQTHelper(););
}

// //////////////////////////////////////////////////////////////////////
// void FQTTestSuite::errorCase () {
//  CPPUNIT_ASSERT (false);
// }

// //////////////////////////////////////////////////////////////////////
FQTTestSuite::FQTTestSuite () {
  _describeKey << "Running test on FQT";
}

// /////////////// M A I N /////////////////
CPPUNIT_MAIN()

