/*!
 * \page SimulationTestSuite_cpp Command-Line Test to Demonstrate How To Test DSim
 * \code
 */
// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <sstream>
#include <fstream>
#include <string>
// Boost Unit Test Framework (UTF)
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#define BOOST_TEST_MODULE DSimTest
#include <boost/test/unit_test.hpp>
// StdAir
#include <stdair/stdair_exceptions.hpp>
#include <stdair/basic/BasLogParams.hpp>
#include <stdair/basic/BasDBParams.hpp>
#include <stdair/basic/BasFileMgr.hpp>
#include <stdair/basic/ForecastingMethod.hpp>
#include <stdair/basic/DemandGenerationMethod.hpp>
#include <stdair/service/Logger.hpp>
// Dsim
#include <dsim/DSIM_Types.hpp>
#include <dsim/DSIM_Service.hpp>
#include <dsim/config/dsim-paths.hpp>

namespace boost_utf = boost::unit_test;

// (Boost) Unit Test XML Report
std::ofstream utfReportStream ("SimulationTestSuite_utfresults.xml");

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


// /////////////// Main: Unit Test Suite //////////////

// Set the UTF configuration (re-direct the output to a specific file)
BOOST_GLOBAL_FIXTURE (UnitTestConfig);

// Start the test suite
BOOST_AUTO_TEST_SUITE (master_test_suite)

/**
 * Test a simple simulation
 */
BOOST_AUTO_TEST_CASE (simple_simulation_test) {

  // Number of simulation runs to be performed
  const DSIM::NbOfRuns_T lNbOfRuns = 1;

  const stdair::DemandGenerationMethod lOrderStatDemandGenMethod =
    stdair::DemandGenerationMethod::STA_ORD;
  const stdair::ForecastingMethod lAdditiveForecastMethod =
    stdair::ForecastingMethod::ADD_PK;

  // Start date
  stdair::Date_T lStartDate (2009, boost::gregorian::Jan, 01);
  
  // End date
  stdair::Date_T lEndDate (2011, boost::gregorian::Jan, 01);

  // Demand input file name
  const stdair::Filename_T lDemandInputFilename (STDAIR_SAMPLE_DIR
                                                 "/rds01/demand.csv");

  // Schedule input file name
  const stdair::Filename_T lScheduleInputFilename (STDAIR_SAMPLE_DIR
                                                   "/rds01/schedule.csv");
    
  // O&D input file name
  const stdair::Filename_T lOnDInputFilename (STDAIR_SAMPLE_DIR "/ond01.csv");

  // Yield input file name
  const stdair::Filename_T lYieldInputFilename (STDAIR_SAMPLE_DIR
                                               "/rds01/yield.csv");
    
  // Fare input file name
  const stdair::Filename_T lFareInputFilename (STDAIR_SAMPLE_DIR
                                               "/rds01/fare.csv");

  // Check that the file path given as input corresponds to an actual file
  bool doesExistAndIsReadable =
    stdair::BasFileMgr::doesExistAndIsReadable (lScheduleInputFilename);
  BOOST_CHECK_MESSAGE (doesExistAndIsReadable == true,
                       "The '" << lScheduleInputFilename
                       << "' input file can not be open and read");

  // Check that the file path given as input corresponds to an actual file
  doesExistAndIsReadable =
    stdair::BasFileMgr::doesExistAndIsReadable (lOnDInputFilename);
  BOOST_CHECK_MESSAGE (doesExistAndIsReadable == true,
                       "The '" << lOnDInputFilename
                       << "' input file can not be open and read");

  // Check that the file path given as input corresponds to an actual file
  doesExistAndIsReadable =
    stdair::BasFileMgr::doesExistAndIsReadable (lDemandInputFilename);
  BOOST_CHECK_MESSAGE (doesExistAndIsReadable == true,
                       "The '" << lDemandInputFilename
                       << "' input file can not be open and read");

  // Check that the file path given as input corresponds to an actual file
  doesExistAndIsReadable =
    stdair::BasFileMgr::doesExistAndIsReadable (lFareInputFilename);
  BOOST_CHECK_MESSAGE (doesExistAndIsReadable == true,
                       "The '" << lFareInputFilename
                       << "' input file can not be open and read");

  // Check that the file path given as input corresponds to an actual file
  doesExistAndIsReadable =
    stdair::BasFileMgr::doesExistAndIsReadable (lYieldInputFilename);
  BOOST_CHECK_MESSAGE (doesExistAndIsReadable == true,
                       "The '" << lYieldInputFilename
                       << "' input file can not be open and read");

  // Output log File
  const stdair::Filename_T lLogFilename ("SimulationTestSuite.log");

  // Set the log parameters
  std::ofstream logOutputFile;
  // Open and clean the log outputfile
  logOutputFile.open (lLogFilename.c_str());
  logOutputFile.clear();
  
  // Initialise the simulation context
  const stdair::BasLogParams lLogParams (stdair::LOG::DEBUG, logOutputFile);
  const stdair::BasDBParams lDBParams ("dsim", "dsim", "localhost", "3306",
                                       "sim_dsim");
  DSIM::DSIM_Service dsimService (lLogParams, lDBParams, lStartDate, lEndDate);

  // Build the BOM tree from parsing input files
  BOOST_CHECK_NO_THROW (dsimService.parseAndLoad (lScheduleInputFilename,
                                                  lOnDInputFilename,
                                                  lYieldInputFilename, 
                                                  lFareInputFilename,
                                                  lDemandInputFilename));

  // Initialise the snapshot and RM events
  BOOST_CHECK_NO_THROW (dsimService.initSnapshotAndRMEvents());
  
  // Perform a simulation
  // BOOST_CHECK_THROW (dsimService.simulate(), stdair::EventException);
  BOOST_CHECK_NO_THROW (dsimService.simulate (lNbOfRuns,
                                              lOrderStatDemandGenMethod,
                                              lAdditiveForecastMethod));

  // Close the log file
  logOutputFile.close();
}

// End the test suite
BOOST_AUTO_TEST_SUITE_END()

/*!
 * \endcode
 */
