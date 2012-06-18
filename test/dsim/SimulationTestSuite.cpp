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
#include <stdair/basic/BasConst_General.hpp>
#include <stdair/basic/BasLogParams.hpp>
#include <stdair/basic/BasFileMgr.hpp>
#include <stdair/basic/DemandGenerationMethod.hpp>
#include <stdair/service/Logger.hpp>
// SimFQT
#include <simfqt/SIMFQT_Types.hpp>
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

  // Method for the demand generation (here, statistics order)
  const stdair::DemandGenerationMethod lOrderStatDemandGenMethod =
    stdair::DemandGenerationMethod::STA_ORD;

  // Start date
  const stdair::Date_T lStartDate (2009, boost::gregorian::Jan, 01);
  
  // End date
  const stdair::Date_T lEndDate (2011, boost::gregorian::Jan, 01);

  // Random generation seed
  const stdair::RandomSeed_T lRandomSeed (stdair::DEFAULT_RANDOM_SEED);
  
  // Number of simulation runs to be performed
  const DSIM::NbOfRuns_T lNbOfRuns (1);

  // Demand input file name
  const stdair::Filename_T lDemandInputFilename (STDAIR_SAMPLE_DIR
                                                 "/rds01/demand.csv");

  // Schedule input file name
  const stdair::Filename_T lScheduleInputFilename (STDAIR_SAMPLE_DIR
                                                   "/rds01/schedule.csv");
    
  // O&D input file name
  const stdair::Filename_T lOnDInputFilename (STDAIR_SAMPLE_DIR "/ond01.csv");

  // FRAT5 curve input file name
  const stdair::Filename_T lFRAT5InputFilename (STDAIR_SAMPLE_DIR
                                               "/frat5.csv");

  // Fare family disutility curve input file name
  const stdair::Filename_T lFFDisutilityInputFilename (STDAIR_SAMPLE_DIR
                                                       "/ffDisutility.csv");

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
    stdair::BasFileMgr::doesExistAndIsReadable (lFRAT5InputFilename);
  BOOST_CHECK_MESSAGE (doesExistAndIsReadable == true,
                       "The '" << lFRAT5InputFilename
                       << "' input file can not be open and read");
  
  // Check that the file path given as input corresponds to an actual file
  doesExistAndIsReadable =
    stdair::BasFileMgr::doesExistAndIsReadable (lFFDisutilityInputFilename);
  BOOST_CHECK_MESSAGE (doesExistAndIsReadable == true,
                       "The '" << lFFDisutilityInputFilename
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
  
  DSIM::DSIM_Service dsimService (lLogParams, lStartDate, lEndDate,
                                  lRandomSeed);

  // Build the BOM tree from parsing input files
  stdair::ScheduleFilePath lScheduleFilePath (lScheduleInputFilename);
  stdair::ODFilePath lODFilePath (lOnDInputFilename);
  stdair::FRAT5FilePath lFRAT5FilePath (lFRAT5InputFilename);
  stdair::FFDisutilityFilePath lFFDisutilityFilePath (lFFDisutilityInputFilename);
  const SIMFQT::FareFilePath lFareFilePath (lFareInputFilename);
  const AIRRAC::YieldFilePath lYieldFilePath (lYieldInputFilename);
  const TRADEMGEN::DemandFilePath lDemandFilePath (lDemandInputFilename);
  BOOST_CHECK_NO_THROW (dsimService.parseAndLoad (lScheduleFilePath,
                                                  lODFilePath,
                                                  lFRAT5FilePath,
                                                  lFFDisutilityFilePath,
                                                  lYieldFilePath ,
                                                  lFareFilePath,
                                                  lDemandFilePath));

  // Initialise the snapshot and RM events
  BOOST_CHECK_NO_THROW (dsimService.initSnapshotAndRMEvents());
  
  // Perform a simulation
  // BOOST_CHECK_THROW (dsimService.simulate(), stdair::EventException);
  BOOST_CHECK_NO_THROW (dsimService.simulate (lNbOfRuns,
                                              lOrderStatDemandGenMethod));

  // Close the log file
  logOutputFile.close();
}

// End the test suite
BOOST_AUTO_TEST_SUITE_END()

/*!
 * \endcode
 */
