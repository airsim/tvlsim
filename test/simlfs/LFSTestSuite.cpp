// STL
#include <sstream>
#include <fstream>
#include <string>
// CPPUNIT
#include <extracppunit/CppUnitCore.hpp>
// StdAir
#include <stdair/basic/BasLogParams.hpp>
#include <stdair/basic/BasDBParams.hpp>
#include <stdair/service/Logger.hpp>
// SimLFS
#include <simlfs/SIMLFS_Service.hpp>
#include <simlfs/config/simlfs-paths.hpp>
// SimLFS Test Suite
#include <test/simlfs/LFSTestSuite.hpp>

// //////////////////////////////////////////////////////////////////////
// Test is based on ...
// //////////////////////////////////////////////////////////////////////

// //////////////////////////////////////////////////////////////////////
void LFSTestSuite::simpleLFSHelper() {

  // Fare input file name
  const std::string lFareInputFilename (STDAIR_SAMPLE_DIR "/fare01.csv");
    
  // Output log File
  const std::string lLogFilename ("LFSTestSuite.log");

  // Set the log parameters
  std::ofstream logOutputFile;
  // Open and clean the log outputfile
  logOutputFile.open (lLogFilename.c_str());
  logOutputFile.clear();
  
  // Initialise the list of classes/buckets
  const stdair::BasLogParams lLogParams (stdair::LOG::DEBUG, logOutputFile);
  SIMLFS::SIMLFS_Service simlfsService (lLogParams, lFareInputFilename);
  
  // Perform a price quotation
  // Airline code
  const stdair::AirlineCode_T lAirlineCode ("SV");
  // Number of passengers in the travelling group
  const SIMLFS::PartySize_T lPartySize = 5;
  simlfsService.priceQuote (lAirlineCode, lPartySize);
  
  // DEBUG
  STDAIR_LOG_DEBUG ("Sample data directory: " << STDAIR_SAMPLE_DIR);
}

// //////////////////////////////////////////////////////////////////////
void LFSTestSuite::simpleLFS () {
  // TODO: Check that the LFS goes as expected
  CPPUNIT_ASSERT_NO_THROW ( simpleLFSHelper(););
}

// //////////////////////////////////////////////////////////////////////
// void LFSTestSuite::errorCase () {
//  CPPUNIT_ASSERT (false);
// }

// //////////////////////////////////////////////////////////////////////
LFSTestSuite::LFSTestSuite () {
  _describeKey << "Running test on LFS";
}

// /////////////// M A I N /////////////////
CPPUNIT_MAIN()
