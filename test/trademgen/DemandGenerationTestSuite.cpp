/*!
 * \page DemandGenerationTestSuite_cpp Command-Line Test to Demonstrate How To Use TraDemGen elements
 * \code
 */
// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <sstream>
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
#include <stdair/basic/ProgressStatusSet.hpp>
#include <stdair/bom/EventStruct.hpp>
#include <stdair/bom/EventQueue.hpp>
#include <stdair/bom/BookingRequestStruct.hpp>
#include <stdair/service/Logger.hpp>
// TraDemGen
#include <trademgen/TRADEMGEN_Service.hpp>
#include <trademgen/bom/DemandStreamKey.hpp>
#include <trademgen/config/trademgen-paths.hpp>

namespace boost_utf = boost::unit_test;

// (Boost) Unit Test XML Report
std::ofstream utfReportStream ("DemandGenerationTestSuite_utfresults.xml");

/**
 * Configuration for the Boost Unit Test Framework (UTF)
 */
struct UnitTestConfig {
  /** Constructor. */
  UnitTestConfig() {
    boost_utf::unit_test_log.set_stream (utfReportStream);
    boost_utf::unit_test_log.set_format (boost_utf::XML);
    boost_utf::unit_test_log.set_threshold_level (boost_utf::log_test_units);
    //boost_utf::unit_test_log.set_threshold_level (boost_utf::log_successful_tests);
  }
  
  /** Destructor. */
  ~UnitTestConfig() {
  }
};

// Specific type definitions
typedef std::pair<stdair::Count_T, stdair::Count_T> NbOfEventsPair_T;
typedef std::map<const stdair::DemandStreamKeyStr_T,
                 NbOfEventsPair_T> NbOfEventsByDemandStreamMap_T;


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

  /**
     Initialise the current number of generated events and the
     expected total numbers of requests to be generated, depending on
     the demand streams.
     <br>The current number of generated events starts at one, for each demand
     stream, because the initialisation step generates exactly one event
     for each demand stream.
  */
  NbOfEventsByDemandStreamMap_T lNbOfEventsMap;
  lNbOfEventsMap.insert (NbOfEventsByDemandStreamMap_T::
                         value_type ("SIN-HND 2010-Feb-08 Y",
                                     NbOfEventsPair_T (1, 10)));
  lNbOfEventsMap.insert (NbOfEventsByDemandStreamMap_T::
                         value_type ("SIN-HND 2010-Feb-09 Y",
                                     NbOfEventsPair_T (1, 10)));
  lNbOfEventsMap.insert (NbOfEventsByDemandStreamMap_T::
                         value_type ("SIN-BKK 2010-Feb-08 Y",
                                     NbOfEventsPair_T (1, 10)));
  lNbOfEventsMap.insert (NbOfEventsByDemandStreamMap_T::
                         value_type ("SIN-BKK 2010-Feb-09 Y",
                                     NbOfEventsPair_T (1, 10)));
  // Total number of events, for all the demand streams: 3
  stdair::Count_T lRefExpectedNbOfEvents (40);
  
  // Retrieve the expected (mean value of the) number of events to be
  // generated
  const stdair::Count_T& lExpectedNbOfEventsToBeGenerated =
    trademgenService.getExpectedTotalNumberOfRequestsToBeGenerated();

  BOOST_CHECK_EQUAL (lRefExpectedNbOfEvents,
                     std::floor (lExpectedNbOfEventsToBeGenerated));
  
  BOOST_CHECK_MESSAGE (lRefExpectedNbOfEvents ==
                       std::floor (lExpectedNbOfEventsToBeGenerated),
                       "Expected total number of requests to be generated: "
                       << lExpectedNbOfEventsToBeGenerated
                       << " (=> "
                       << std::floor (lExpectedNbOfEventsToBeGenerated)
                       << "). Reference value: " << lRefExpectedNbOfEvents);

  const bool lGenerateDemandWithStatisticOrder = true;

  /**
   * Initialisation step.
   *
   * Generate the first event for each demand stream.
   *
   * \note For that demand (CSV) file (i.e., demand01.csv), the
   *       expected and actual numbers of events to be generated are
   *       the same (and equal to 40).
   */
  const stdair::Count_T& lActualNbOfEventsToBeGenerated =
    trademgenService.generateFirstRequests(lGenerateDemandWithStatisticOrder);

  // DEBUG
  STDAIR_LOG_DEBUG ("Expected number of events: "
                    << lExpectedNbOfEventsToBeGenerated << ", actual: "
                    << lActualNbOfEventsToBeGenerated);
  
  // Total number of events, for all the demand streams: 40
  const stdair::Count_T lRefActualNbOfEvents (40);
  BOOST_CHECK_EQUAL (lRefActualNbOfEvents, lActualNbOfEventsToBeGenerated);
  
  BOOST_CHECK_MESSAGE (lRefActualNbOfEvents == lActualNbOfEventsToBeGenerated,
                       "Actual total number of requests to be generated: "
                       << lExpectedNbOfEventsToBeGenerated
                       << " (=> "
                       << std::floor (lExpectedNbOfEventsToBeGenerated)
                       << "). Reference value: " << lRefActualNbOfEvents);

  /** Is the queue empty? */
  const bool isQueueDone = trademgenService.isQueueDone();
  BOOST_REQUIRE_MESSAGE (isQueueDone == false,
                         "The event queue should not be empty. You may check "
                         << "the input file: '" << lInputFilename << "'");

  /**
     Main loop.
     <ul>
     <li>Pop a request and get its associated type/demand stream.</li>
     <li>Generate the next request for the same type/demand stream.</li>
     </ul>
  */
  stdair::Count_T idx = 1;
  while (trademgenService.isQueueDone() == false) {

    // Get the next event from the event queue
    stdair::EventStruct lEventStruct;
    stdair::ProgressStatusSet lPPS = trademgenService.popEvent (lEventStruct);

    // DEBUG
    STDAIR_LOG_DEBUG ("Poped event: '" << lEventStruct.describe() << "'.");
      
    // Extract the corresponding demand/booking request
    const stdair::BookingRequestStruct& lPoppedRequest =
      lEventStruct.getBookingRequest();
    
    // DEBUG
    STDAIR_LOG_DEBUG ("Poped booking request: '"
                      << lPoppedRequest.describe() << "'.");
    
    // Retrieve the corresponding demand stream
    const stdair::DemandGeneratorKey_T& lDemandStreamKey =
      lPoppedRequest.getDemandGeneratorKey();

    // Check that the number of booking requests to be generated are correct
    const NbOfEventsByDemandStreamMap_T::iterator itNbOfEventsMap =
      lNbOfEventsMap.find (lDemandStreamKey);
    BOOST_REQUIRE_MESSAGE (itNbOfEventsMap != lNbOfEventsMap.end(),
                           "The demand stream key '" << lDemandStreamKey
                           << "' is not expected in that test");

    /**
       For that demand stream, retrieve:
        <ul>
        <li>The current number of events</li>
        <li>The expected total number of events to be generated. That
        number is just hard coded for that test (it does not correspond
        to an automatically generated number)</li>
        </ul>
    */
    const NbOfEventsPair_T& lNbOfEventsPair = itNbOfEventsMap->second;
    stdair::Count_T lCurrentNbOfEvents = lNbOfEventsPair.first;
    const stdair::Count_T& lExpectedTotalNbOfEvents = lNbOfEventsPair.second;

    // Assess whether more events should be generated for that demand stream
    const bool stillHavingRequestsToBeGenerated = trademgenService.
      stillHavingRequestsToBeGenerated (lDemandStreamKey, lPPS,
                                        lGenerateDemandWithStatisticOrder);

    /**
       The first time an event is popped from the queue for that demand stream,
       check that the actual total number of requests to be generated (as
       calculated by the demand stream itself during the initialisation
       step), is equal to the expected number.
    */
    if (lCurrentNbOfEvents == 1) {
      /**
         Retrieve, from the demand stream, the total number of events
         to be generated, so that that number can be compared to the
         expected one.
      */
      const stdair::ProgressStatus& lDemandStreamProgressStatus =
        lPPS.getSpecificGeneratorStatus();
      const stdair::Count_T& lNbOfRequests =
        lDemandStreamProgressStatus.getExpectedNb();

      BOOST_CHECK_EQUAL (lNbOfRequests, lExpectedTotalNbOfEvents);
      BOOST_CHECK_MESSAGE (lNbOfRequests == lExpectedTotalNbOfEvents,
                           "[" << lDemandStreamKey
                           << "] Total number of requests to be generated: "
                           << lNbOfRequests << "). Expected value: "
                           << lExpectedTotalNbOfEvents);
    }

    // DEBUG
    STDAIR_LOG_DEBUG ("=> [" << lDemandStreamKey << "][" << lCurrentNbOfEvents
                      << "/" << lExpectedTotalNbOfEvents
                      << "] is now processed. "
                      << "Still generate events for that demand stream? "
                      << stillHavingRequestsToBeGenerated);

    // If there are still events to be generated for that demand stream,
    // generate and add them to the event queue
    if (stillHavingRequestsToBeGenerated == true) {
      const stdair::BookingRequestPtr_T lNextRequest_ptr =
        trademgenService.generateNextRequest (lDemandStreamKey,
                                              lGenerateDemandWithStatisticOrder);
      assert (lNextRequest_ptr != NULL);

      /**
         Sanity check
         <br>The date-time of the next event must be greater than
         the date-time of the current event.
      */
      const stdair::Duration_T lDuration =
        lNextRequest_ptr->getRequestDateTime()
        - lPoppedRequest.getRequestDateTime();
      BOOST_REQUIRE_GT (lDuration.total_milliseconds(), 0);
      BOOST_REQUIRE_MESSAGE (lDuration.total_milliseconds() > 0,
                             "[" << lDemandStreamKey
                             << "] The date-time of the generated event ("
                             << lNextRequest_ptr->getRequestDateTime()
                             << ") is lower than the date-time "
                             << "of the current event ("
                             << lPoppedRequest.getRequestDateTime() << ")");
      
      // DEBUG
      STDAIR_LOG_DEBUG ("[" << lDemandStreamKey << "][" << lCurrentNbOfEvents
                        << "/" << lExpectedTotalNbOfEvents
                        << "] Added request: '" << lNextRequest_ptr->describe()
                        << "'. Is queue done? "
                        << trademgenService.isQueueDone());

      // Keep, within the dedicated map, the current counters of events updated.
      ++lCurrentNbOfEvents;
      itNbOfEventsMap->second = NbOfEventsPair_T (lCurrentNbOfEvents,
                                                  lExpectedTotalNbOfEvents);
    }
    
    // Iterate
    ++idx;
  }
  // Compensate for the last iteration
  --idx;
  //
  BOOST_CHECK_EQUAL (idx, lRefActualNbOfEvents);
  BOOST_CHECK_MESSAGE (idx == lRefActualNbOfEvents,
                       "The total actual number of events is "
                       << lRefActualNbOfEvents << ", but " << idx
                       << " events have been generated");
  
  /** Reset the context of the demand streams for another demand generation
      without having to reparse the demand input file. */
  trademgenService.reset();

  // DEBUG
  STDAIR_LOG_DEBUG ("End of the simulation");

  // Close the log file
  logOutputFile.close();
}

// End the test suite
BOOST_AUTO_TEST_SUITE_END()

/*!
 * \endcode
 */
