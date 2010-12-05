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

// //////////////////////////////////////////////////////////////////////
// Test is based on ...
// //////////////////////////////////////////////////////////////////////

// //////////////////////////////////////////////////////////////////////
void FQTTestSuite::simpleFQTHelper() {

  try {
    
    // Fare input file name
    stdair::Filename_T lFareInputFilename ("../samples/fare01.csv");
    
    // Airline code
    //std::string lAirlineCode ("SV");
    
    // Number of passengers in the travelling group
    //SIMFQT::PartySize_T lPartySize = 5;
    
    // Output log File
    std::string lLogFilename ("FQTTestSuite.log");

    // Set the log parameters
    std::ofstream logOutputFile;
    // Open and clean the log outputfile
    logOutputFile.open (lLogFilename.c_str());
    logOutputFile.clear();
    
    // Initialise the list of classes/buckets
    const stdair::BasLogParams lLogParams (stdair::LOG::DEBUG, logOutputFile);
    SIMFQT::SIMFQT_Service simfqtService (lLogParams, lFareInputFilename);

    // Perform a price quotation
    //simfqtService.priceQuote (lAirlineCode, lPartySize);
    
  } catch (const SIMFQT::RootException& otexp) {
    std::cerr << "SimFQT exception: " << otexp.what() << std::endl;
    
  } catch (const std::exception& stde) {
    std::cerr << "Standard exception: " << stde.what() << std::endl;
  }
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

