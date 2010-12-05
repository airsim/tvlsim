// STL
#include <sstream>
#include <fstream>
#include <string>
// CPPUNIT
#include <extracppunit/CppUnitCore.hpp>
// StdAir
#include <stdair/basic/BasLogParams.hpp>
#include <stdair/basic/BasDBParams.hpp>
// SimLFS
#include <simlfs/SIMLFS_Service.hpp>
// SimLFS Test Suite
#include <test/simlfs/LFSTestSuite.hpp>

// //////////////////////////////////////////////////////////////////////
// Test is based on ...
// //////////////////////////////////////////////////////////////////////

// //////////////////////////////////////////////////////////////////////
void LFSTestSuite::simpleLFSHelper() {

  try {
    
    // Airline code
    std::string lAirlineCode ("SV");
    
    // Number of passengers in the travelling group
    SIMLFS::PartySize_T lPartySize = 5;
    
    // Output log File
    std::string lLogFilename ("LFSTestSuite.log");

    // Set the log parameters
    std::ofstream logOutputFile;
    // Open and clean the log outputfile
    logOutputFile.open (lLogFilename.c_str());
    logOutputFile.clear();
    
    // Initialise the list of classes/buckets
    const stdair::BasLogParams lLogParams (stdair::LOG::DEBUG, logOutputFile);
    SIMLFS::SIMLFS_Service simlfsService (lLogParams, lAirlineCode);

    // Perform a price quotation
    simlfsService.priceQuote (lAirlineCode, lPartySize);
    
  } catch (const SIMLFS::RootException& otexp) {
    std::cerr << "SimLFS exception: " << otexp.what() << std::endl;
    return;
    
  } catch (const std::exception& stde) {
    std::cerr << "Standard exception: " << stde.what() << std::endl;
    return;
    
  } catch (...) {
    return;
  }
  
}

// //////////////////////////////////////////////////////////////////////
void LFSTestSuite::simpleLFS () {
  // TODO: Check that the lfs goes as expected
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

