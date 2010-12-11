// STL
#include <sstream>
#include <fstream>
#include <string>
// CPPUNIT
#include <extracppunit/CppUnitCore.hpp>
// StdAir
#include <stdair/stdair_basic_types.hpp>
#include <stdair/bom/EventStruct.hpp>
#include <stdair/bom/EventQueue.hpp>
#include <stdair/bom/BookingRequestStruct.hpp>
#include <stdair/service/Logger.hpp>
// Trademgen
#include <trademgen/TRADEMGEN_Service.hpp>
// TraDemGen Test Suite
#include <test/trademgen/DemandGenerationTestSuite.hpp>

// //////////////////////////////////////////////////////////////////////
// Test is based on ...
// //////////////////////////////////////////////////////////////////////

// //////////////////////////////////////////////////////////////////////
void DemandGenerationTestSuite::simpleEventGenerationHelper() {

  // Input file name
  stdair::Filename_T lInputFilename ("../samples/demand01.csv");

  // Output log File
  std::string lLogFilename ("DemandGenerationTestSuite.log");
  
  // Airline code
  stdair::AirlineCode_T lAirlineCode ("BA");
    
  // Set the log parameters
  std::ofstream logOutputFile;
  // open and clean the log outputfile
  logOutputFile.open (lLogFilename.c_str());
  logOutputFile.clear();
  
  // Initialise the TraDemGen service object
  const stdair::BasLogParams lLogParams (stdair::LOG::DEBUG, logOutputFile);
  TRADEMGEN::TRADEMGEN_Service trademgenService (lLogParams, lInputFilename);

  // /////////////////////////////////////////////////////
  // Event queue
  stdair::EventQueue lEventQueue = stdair::EventQueue ();
  // Browse the list of DemandStreams and Generate the first event for each
  // DemandStream.
  trademgenService.generateFirstRequests (lEventQueue);
  
  // Pop requests, get type, and generate next request of same type
  int i = 0;
  while (lEventQueue.isQueueDone() == false && i < 20) {
    // DEBUG
    STDAIR_LOG_DEBUG ("Before popping (" << i << ")" );
    STDAIR_LOG_DEBUG ("Queue size: " << lEventQueue.getQueueSize () );
    STDAIR_LOG_DEBUG ("Is queue done? " << lEventQueue.isQueueDone () );
    
    stdair::EventStruct& lEventStruct = lEventQueue.popEvent ();

    // DEBUG
    STDAIR_LOG_DEBUG ("After popping" );
    STDAIR_LOG_DEBUG ("Queue size: " << lEventQueue.getQueueSize ());
    STDAIR_LOG_DEBUG ("Is queue done? " << lEventQueue.isQueueDone ());

    STDAIR_LOG_DEBUG ("Popped request " << i );
    
    const stdair::BookingRequestStruct& lPoppedRequest =
      lEventStruct.getBookingRequest ();
    
    // DEBUG
    STDAIR_LOG_DEBUG (lPoppedRequest.describe());
    
    // Retrieve the corresponding demand stream
    const stdair::DemandStreamKeyStr_T& lDemandStreamKey =
      lEventStruct.getDemandStreamKey ();
    // generate next request
    bool stillHavingRequestsToBeGenerated = 
      trademgenService.stillHavingRequestsToBeGenerated(lDemandStreamKey);
    STDAIR_LOG_DEBUG ("stillHavingRequestsToBeGenerated: " << stillHavingRequestsToBeGenerated );
    if (stillHavingRequestsToBeGenerated) {
      stdair::BookingRequestPtr_T lNextRequest =
        trademgenService.generateNextRequest (lDemandStreamKey);
      assert (lNextRequest != NULL);
      // DEBUG
      STDAIR_LOG_DEBUG ("Added request: " << lNextRequest->describe());
      
      stdair::EventStruct lNextEventStruct ("Request", lDemandStreamKey,
                                            lNextRequest);
      lEventQueue.eraseLastUsedEvent ();
      lEventQueue.addEvent (lNextEventStruct);
      
      // DEBUG
      STDAIR_LOG_DEBUG ("After adding");
      STDAIR_LOG_DEBUG ("Queue size: " << lEventQueue.getQueueSize ());
      STDAIR_LOG_DEBUG ("Is queue done? " << lEventQueue.isQueueDone ());
    
    }

    // DEBUG
    STDAIR_LOG_DEBUG (std::endl);
    
    // Iterate
    ++i;
  }
}

// //////////////////////////////////////////////////////////////////////
void DemandGenerationTestSuite::simpleEventGeneration () {
  // TODO: Check that the generated events follow the law given as input
  CPPUNIT_ASSERT_NO_THROW ( simpleEventGenerationHelper(););
}

// //////////////////////////////////////////////////////////////////////
// void DemandGenerationTestSuite::errorCase () {
//  CPPUNIT_ASSERT (false);
// }

// //////////////////////////////////////////////////////////////////////
DemandGenerationTestSuite::DemandGenerationTestSuite () {
  _describeKey << "Running test on RMOL Optimisation function";  
}

// /////////////// M A I N /////////////////
CPPUNIT_MAIN()

