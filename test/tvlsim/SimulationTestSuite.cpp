/*!
 * \page SimulationTestSuite_cpp Command-Line Test to Demonstrate How To Test TvlSim
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
#define BOOST_TEST_MODULE SimulationTestSuite
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
#include <tvlsim/TVLSIM_Types.hpp>
#include <tvlsim/TVLSIM_Service.hpp>
#include <tvlsim/config/tvlsim-paths.hpp>

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

// //////////////////////////////////////////////////////////////////////
/**
 * Perform a simple simulation
 */
void SimulationTest (const unsigned short iTestFlag,
                     const stdair::Filename_T iScheduleInputFilename,
                     const stdair::Filename_T iOnDInputFilename,
                     const stdair::Filename_T iFRAT5InputFilename,
                     const stdair::Filename_T iFFDisutilityInputFilename,
                     const stdair::Filename_T iYieldInputFilename,
                     const stdair::Filename_T iFareInputFilename,
                     const stdair::Filename_T iDemandInputFilename,
                     const bool isBuiltin) {

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
  const TVLSIM::NbOfRuns_T lNbOfRuns (1);

  // Output log File
  std::ostringstream oStr;
  oStr << "SimpleSimulationTestSuite_" << iTestFlag << ".log";
  const stdair::Filename_T lLogFilename (oStr.str());

  // Set the log parameters
  std::ofstream logOutputFile;
  // Open and clean the log outputfile
  logOutputFile.open (lLogFilename.c_str());
  logOutputFile.clear();
  
  // Initialise the simulation context
  const stdair::BasLogParams lLogParams (stdair::LOG::DEBUG, logOutputFile);
  
  TVLSIM::TVLSIM_Service tvlsimService (lLogParams, lStartDate, lEndDate,
                                  lRandomSeed, lOrderStatDemandGenMethod,
				  lNbOfRuns);

  // Check wether or not a (CSV) input file should be read
  if (isBuiltin == true) {

    // Build the default sample BOM tree (filled with fares) for Simfqt
    tvlsimService.buildSampleBom();

  } else {

    // Retrieve the input file path
    const stdair::ScheduleFilePath lScheduleFilePath (iScheduleInputFilename);
    const stdair::ODFilePath lODFilePath (iOnDInputFilename);
    const stdair::FRAT5FilePath lFRAT5FilePath (iFRAT5InputFilename);
    const stdair::FFDisutilityFilePath lFFDisutilityFilePath (iFFDisutilityInputFilename);
    const AIRRAC::YieldFilePath lYieldFilePath (iYieldInputFilename);
    const SIMFQT::FareFilePath lFareFilePath (iFareInputFilename);
    const TRADEMGEN::DemandFilePath lDemandFilePath (iDemandInputFilename);

    // Load the input files
    tvlsimService.setInputFiles(lScheduleFilePath, 
                              lODFilePath, 
                              lFRAT5FilePath,
                              lFFDisutilityFilePath, 
                              lYieldFilePath,
                              lFareFilePath,
                              lDemandFilePath);
	 
    // Parse the input files 
    tvlsimService.parseAndLoad ();   
  }
  
  // Initialise the snapshot and RM events
  tvlsimService.initSnapshotAndRMEvents();
  
  // Perform a simulation
  tvlsimService.simulate ();

  // Close the log file
  logOutputFile.close();

}

// /////////////// Main: Unit Test Suite //////////////

// Set the UTF configuration (re-direct the output to a specific file)
BOOST_GLOBAL_FIXTURE (UnitTestConfig);

// Start the test suite
BOOST_AUTO_TEST_SUITE (master_test_suite)

/**
 * Test a simple simulation
 */
BOOST_AUTO_TEST_CASE (simple_simulation_test) {

  // State whether the BOM tree should be built-in or parsed from input files
  const bool isBuiltin = false;

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
  
  // Demand input file name
  const stdair::Filename_T lDemandInputFilename (STDAIR_SAMPLE_DIR
                                                 "/rds01/demand.csv");

  // Simulate
  BOOST_CHECK_NO_THROW (SimulationTest(0,
                                       lScheduleInputFilename, 
                                       lOnDInputFilename, 
                                       lFRAT5InputFilename,
                                       lFFDisutilityInputFilename,
                                       lYieldInputFilename,
                                       lFareInputFilename,
                                       lDemandInputFilename,
                                       isBuiltin));
}

/**
 * Test a simple simulation
 */
BOOST_AUTO_TEST_CASE (default_bom_simulation_test) {

  // State whether the BOM tree should be built-in or parsed from input files
  const bool isBuiltin = true;

  // Simulate
  BOOST_CHECK_NO_THROW (SimulationTest(1,
                                       " ", " ", " ", " ", " ", " ", " ",
                                       isBuiltin));

 
}

// End the test suite
BOOST_AUTO_TEST_SUITE_END()

/*!
 * \endcode
 */
