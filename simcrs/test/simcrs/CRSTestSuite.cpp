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
// SimCRS
#include <simcrs/SIMCRS_Service.hpp>
// SimCRS Test Suite
#include <test/simcrs/CRSTestSuite.hpp>

// //////////////////////////////////////////////////////////////////////
// Test is based on ...
// //////////////////////////////////////////////////////////////////////

// //////////////////////////////////////////////////////////////////////
void CRSTestSuite::simpleCRSHelper() {

  try {
    
    // CRS code
    std::string lCRSCode ("1P");
    
    // Airline code
    std::string lAirlineCode ("BA");
    
    // Number of passengers in the travelling group
    // stdair::PartySize_T lPartySize = 5;
    
    // Output log File
    std::string lLogFilename ("CRSTestSuite.log");

    // Schedule input filename
    std::string lScheduleFilename ("../samples/schedule01.csv");
    
    // O&D input filename
    std::string lOnDFilename ("../samples/ond01.csv");
    
    // Fare input filename
    std::string lFareFilename ("../samples/fare01.csv");
    
    // Set the log parameters
    std::ofstream logOutputFile;
    // Open and clean the log outputfile
    logOutputFile.open (lLogFilename.c_str());
    logOutputFile.clear();
    
    // Initialise the list of classes/buckets
    const stdair::BasLogParams lLogParams (stdair::LOG::DEBUG, logOutputFile);
    SIMCRS::SIMCRS_Service simcrsService (lLogParams, lCRSCode,
                                          lScheduleFilename, lOnDFilename,
                                          lFareFilename);

    // Make a booking
    //simcrsService.sell (lAirlineCode, lPartySize);
    
  } catch (const SIMCRS::RootException& otexp) {
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
void CRSTestSuite::simpleCRS () {
  // TODO: Check that the crs goes as expected
  CPPUNIT_ASSERT_NO_THROW ( simpleCRSHelper(););
}

// //////////////////////////////////////////////////////////////////////
// void CRSTestSuite::errorCase () {
//  CPPUNIT_ASSERT (false);
// }

// //////////////////////////////////////////////////////////////////////
CRSTestSuite::CRSTestSuite () {
  _describeKey << "Running test on CRS";
}

// /////////////// M A I N /////////////////
CPPUNIT_MAIN()

