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
#include <stdair/bom/BookingRequestStruct.hpp>
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

  // Create an empty booking request structure
  // TODO: fill the booking request structure from the input parameters
  const stdair::AirportCode_T lOrigin ("NCE");
  const stdair::AirportCode_T lDestination ("BOS");
  const stdair::AirportCode_T lPOS ("NYC");
  const stdair::Date_T lPreferredDepartureDate(2011, boost::gregorian::Jan, 20);
  const stdair::Date_T lRequestDate (2011, boost::gregorian::Jan, 12);
  const stdair::Duration_T lRequestTime (boost::posix_time::hours(10));
  const stdair::DateTime_T lRequestDateTime (lRequestDate, lRequestTime);
  const stdair::CabinCode_T lPreferredCabin ("Eco");
  const stdair::NbOfSeats_T lPartySize (1);
  const stdair::ChannelLabel_T lChannel ("D");
  const stdair::TripType_T lTripType ("RI");
  const stdair::DayDuration_T lStayDuration (7);
  const stdair::FrequentFlyer_T lFrequentFlyerType ("M");
  const stdair::Duration_T lPreferredDepartureTime (boost::posix_time::hours(10));
  const stdair::WTP_T lWTP (1000.0);
  const stdair::PriceValue_T lValueOfTime (100.0);
  const stdair::BookingRequestStruct lBookingRequest (lOrigin, lDestination,
                                                      lPOS,
                                                      lPreferredDepartureDate,
                                                      lRequestDateTime,
                                                      lPreferredCabin,
                                                      lPartySize, lChannel,
                                                      lTripType, lStayDuration,
                                                      lFrequentFlyerType,
                                                      lPreferredDepartureTime,
                                                      lWTP, lValueOfTime);
  const stdair::SegmentPathList_T lSegmentPath;
  
  // Price the travel solution
  stdair::TravelSolutionList_T lTravelSolutionList =
    simcrsService.getFareQuote (lBookingRequest, lSegmentPath);

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

