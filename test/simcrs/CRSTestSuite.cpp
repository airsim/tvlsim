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
#include <stdair/service/Logger.hpp>
// SimCRS
#include <simcrs/SIMCRS_Service.hpp>
#include <simcrs/config/simcrs-paths.hpp>
// SimCRS Test Suite
#include <test/simcrs/CRSTestSuite.hpp>

// //////////////////////////////////////////////////////////////////////
// Test is based on ...
// //////////////////////////////////////////////////////////////////////

// //////////////////////////////////////////////////////////////////////
void CRSTestSuite::simpleCRSHelper() {

  // CRS code
  const std::string lCRSCode ("1P");
    
  // Output log File
  const std::string lLogFilename ("CRSTestSuite.log");

  // Schedule input filename
  const std::string lScheduleFilename (STDAIR_SAMPLE_DIR "/schedule01.csv");
    
  // O&D input filename
  const std::string lOnDFilename (STDAIR_SAMPLE_DIR "/ond01.csv");
    
  // Fare input filename
  const std::string lFareFilename (STDAIR_SAMPLE_DIR "/fare01.csv");
    
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

  // Create an empty travel solution
  // TODO: fill the travel solution from the input parameters
  stdair::TravelSolutionList_T lTravelSolutionList;  
  stdair::TravelSolutionStruct lEmptyTS;
  lTravelSolutionList.push_back (lEmptyTS);  

  // Price the travel solution
  simcrsService.getFareQuote (lTravelSolutionList); 

  // DEBUG
  const stdair::TravelSolutionStruct& lTravelSolution =
    lTravelSolutionList.front();
  STDAIR_LOG_DEBUG ("The price given by the fare quoter is: "
                    << lTravelSolution.getFare() << " Euros");

  // Make a booking
  // const std::string lAirlineCode ("SV");
  // const stdair::PartySize_T lPartySize = 5;
  // simcrsService.sell (lAirlineCode, lPartySize);
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

