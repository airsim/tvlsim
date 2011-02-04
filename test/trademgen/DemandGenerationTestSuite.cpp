/*!
 * \page DemandGenerationTestSuite_cpp Command-Line Test to Demonstrate How To Use TraDemGen elements
 * \code
 */
// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <iostream>
#include <fstream>
#include <map>
#include <cmath>
// Boost Unit Test Framework (UTF)
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#define BOOST_TEST_MODULE DemandGenerationTest
#include <boost/test/unit_test.hpp>
// StdAir
#include <stdair/stdair_basic_types.hpp>
#include <stdair/basic/BasLogParams.hpp>
#include <stdair/basic/BasDBParams.hpp>
#include <stdair/basic/BasFileMgr.hpp>
#include <stdair/bom/EventStruct.hpp>
#include <stdair/bom/EventQueue.hpp>
#include <stdair/bom/BookingRequestStruct.hpp>
#include <stdair/service/Logger.hpp>
// TraDemGen
#include <trademgen/TRADEMGEN_Service.hpp>
#include <trademgen/bom/DemandStreamKey.hpp>
#include <trademgen/config/trademgen-paths.hpp>

namespace boost_utf = boost::unit_test;

/**
 * Configuration for the Boost Unit Test Framework (UTF)
 */
struct UnitTestConfig {
  /** Constructor. */
  UnitTestConfig() {
    static std::ofstream _test_log ("DemandGenerationTestSuite_utfresults.xml");
    boost_utf::unit_test_log.set_stream (_test_log);
    boost_utf::unit_test_log.set_format (boost_utf::XML);
    boost_utf::unit_test_log.set_threshold_level (boost_utf::log_test_units);
    //boost_utf::unit_test_log.set_threshold_level (boost_utf::log_successful_tests);
  }
  
  /** Destructor. */
  ~UnitTestConfig() {
  }
};

// Specific type definitions
typedef std::map<const stdair::DemandStreamKeyStr_T,
                 const unsigned int> DemandStreamKeyMap_T;


// /////////////// Main: Unit Test Suite //////////////

// Set the UTF configuration (re-direct the output to a specific file)
BOOST_GLOBAL_FIXTURE (UnitTestConfig);

// Start the test suite
BOOST_AUTO_TEST_SUITE (master_test_suite)

/**
 * Test a simple simulation
 */
BOOST_AUTO_TEST_CASE (trademgen_simple_simulation_test) {

  // Input file name
  const stdair::Filename_T lInputFilename (STDAIR_SAMPLE_DIR "/demand01.csv");

  // Check that the file path given as input corresponds to an actual file
  const bool doesExistAndIsReadable =
    stdair::BasFileMgr::doesExistAndIsReadable (lInputFilename);
  BOOST_CHECK_MESSAGE (doesExistAndIsReadable == true,
                       "The '" << lInputFilename
                       << "' input file can not be open and read");
  
  // Output log File
  const stdair::Filename_T lLogFilename ("DemandGenerationTestSuite.log");
  
  // Set the log parameters
  std::ofstream logOutputFile;
  // open and clean the log outputfile
  logOutputFile.open (lLogFilename.c_str());
  logOutputFile.clear();
  
  // Initialise the TraDemGen service object
  const stdair::BasLogParams lLogParams (stdair::LOG::DEBUG, logOutputFile);
  TRADEMGEN::TRADEMGEN_Service trademgenService (lLogParams, lInputFilename);

  // Numbers of requests to be generated, depending on the demand streams
  DemandStreamKeyMap_T lDemandStreamKeyMap;
  lDemandStreamKeyMap.insert (DemandStreamKeyMap_T::
                              value_type ("SIN-HND 2010-Feb-08 Y", 11));
  lDemandStreamKeyMap.insert (DemandStreamKeyMap_T::
                              value_type ("SIN-BKK 2010-Feb-08 Y", 10));
  
  // /////////////////////////////////////////////////////
  // Event queue
  stdair::EventQueue lEventQueue = stdair::EventQueue();
  // Browse the list of DemandStreams and Generate the first event for each
  // DemandStream.
  trademgenService.generateFirstRequests (lEventQueue);

  /** Is queue empty */
  const bool isQueueEmpty = lEventQueue.isQueueEmpty();
  BOOST_CHECK_MESSAGE (isQueueEmpty == false,
                       "The event queue should be full. You may check the "
                       << "input file: '" << lInputFilename << "'");
  
  /** Queue size */
  const stdair::Count_T lQueueSize = lEventQueue.getQueueSize();
  BOOST_CHECK_EQUAL (lQueueSize, 2);
  
  // Pop requests, get type, and generate next request of same type
  stdair::Count_T i = 0;
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
    STDAIR_LOG_DEBUG ("Poped booking request: " << lPoppedRequest.describe());
    
    // Retrieve the corresponding demand stream
    const stdair::DemandStreamKeyStr_T& lDemandStreamKey =
      lEventStruct.getDemandStreamKey ();

    // Get the total number of requests to be generated by the demand
    // stream which corresponds to the given key
    const stdair::NbOfRequests_T& lNbOfRequests =
      trademgenService.getTotalNumberOfRequestsToBeGenerated (lDemandStreamKey);

    // DEBUG
    STDAIR_LOG_DEBUG ("TotalNumberOfRequestsToBeGenerated: "
                      << lNbOfRequests << " for demand stream '"
                      << lDemandStreamKey << "'");

    // Check that the number of booking requests to be generated are correct
    const DemandStreamKeyMap_T::const_iterator itDemandStream =
      lDemandStreamKeyMap.find (lDemandStreamKey);
    BOOST_REQUIRE_MESSAGE (itDemandStream != lDemandStreamKeyMap.end(),
                           "The demand stream key '" << lDemandStreamKey
                           << "' is not expected in that test");

    BOOST_CHECK_EQUAL (std::floor (lNbOfRequests + 0.5),
                       itDemandStream->second);
    BOOST_CHECK_MESSAGE (std::floor (lNbOfRequests + 0.5)
                         == itDemandStream->second,
                         "TotalNumberOfRequestsToBeGenerated: "
                         << lNbOfRequests << " (=> "
                         << std::floor (lNbOfRequests + 0.5)
                         << ") for demand stream '" << lDemandStreamKey
                         << "'. Expected value: " << itDemandStream->second);
  
    // Generate next request
    const bool stillHavingRequestsToBeGenerated = 
      trademgenService.stillHavingRequestsToBeGenerated (lDemandStreamKey);
    STDAIR_LOG_DEBUG ("stillHavingRequestsToBeGenerated: "
                      << stillHavingRequestsToBeGenerated);
    
    if (stillHavingRequestsToBeGenerated) {
      const stdair::BookingRequestPtr_T lNextRequest_ptr =
        trademgenService.generateNextRequest (lDemandStreamKey);
      assert (lNextRequest_ptr != NULL);
      // DEBUG
      STDAIR_LOG_DEBUG ("Added request: " << lNextRequest_ptr->describe());
      
      stdair::EventStruct lNextEventStruct ("Request", lDemandStreamKey,
                                            lNextRequest_ptr);
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

  /** Reset the context of the demand streams for another demand generation
      without having to reparse the demand input file. */
  trademgenService.reset ();

  // Close the log file
  logOutputFile.close();
}

// End the test suite
BOOST_AUTO_TEST_SUITE_END()

/*!
 * \endcode
 */
