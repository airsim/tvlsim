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
#define BOOST_TEST_MODULE TvlSimTest
#include <boost/test/unit_test.hpp>
// StdAir
#include <stdair/stdair_exceptions.hpp>
#include <stdair/stdair_json.hpp>
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
std::ofstream utfReportStream ("PartnerSimulationTestSuite_utfresults.xml");

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
BOOST_AUTO_TEST_CASE (partner_simulation_test) {

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

  // Demand input file name
  const stdair::Filename_T lDemandInputFilename (STDAIR_SAMPLE_DIR
                                                 "/IBP_study/demand01.csv");

  // Schedule input file name
  const stdair::Filename_T lScheduleInputFilename (STDAIR_SAMPLE_DIR
                                                   "/IBP_study/schedule01.csv");
    
  // O&D input file name
  const stdair::Filename_T lOnDInputFilename (STDAIR_SAMPLE_DIR "/IBP_study/ond01.csv");

  // FRAT5 curve input file name
  const stdair::Filename_T lFRAT5InputFilename (STDAIR_SAMPLE_DIR
                                               "/frat5.csv");

  // Fare family disutility curve input file name
  const stdair::Filename_T lFFDisutilityInputFilename (STDAIR_SAMPLE_DIR
                                                       "/ffDisutility.csv");

  // Yield input file name
  const stdair::Filename_T lYieldInputFilename (STDAIR_SAMPLE_DIR
                                               "/IBP_study/yield01.csv");
    
  // Fare input file name
  const stdair::Filename_T lFareInputFilename (STDAIR_SAMPLE_DIR
                                               "/IBP_study/fare01.csv");

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
  const stdair::Filename_T lLogFilename ("PartnerSimulationTestSuite.log");

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
  
  // Construct the JSON command string for the airline features
  std::ostringstream lMyCommandJSONstream;
  lMyCommandJSONstream << "{\"config\":"
                       << "{\"airline1\":"
                       << "{  \"airline_code\":\"" << "SQ"
                       << "\",\"unconstraining_method\":\"" << "E"
                       << "\",\"pre_optimisation_method\":\"N"
                       << "\",\"optimisation_method\":\"M"
                       << "\",\"partnership_technique\":\"i"
                       << "\",\"forecasting_method\":\"B"
                       << "\"},"
                       << "\"airline2\":"
                       << "{  \"airline_code\":\"" << "CX"
                       << "\",\"unconstraining_method\":\"" << "E"
                       << "\",\"pre_optimisation_method\":\"N"
                       << "\",\"optimisation_method\":\"M"
                       << "\",\"partnership_technique\":\"i"
                       << "\",\"forecasting_method\":\"B"
                       << "\"}}}";
      
  STDAIR_LOG_DEBUG(lMyCommandJSONstream.str());
  // Load the airlines features configuration
  const stdair::JSONString lJSONCommandString (lMyCommandJSONstream.str());
  const std::string& lCSVConfigDump =
    tvlsimService.jsonHandler (lJSONCommandString);
  
  STDAIR_LOG_DEBUG(lCSVConfigDump);
  /**BOOST_CHECK_MESSAGE (lCSVConfigDump == "ok",
                       "Impossible to understand the JSon command: '"
                       << lJSONCommandString.getString() << "'.");*/

  // Build the BOM tree from parsing input files
  const stdair::ScheduleFilePath lScheduleFilePath (lScheduleInputFilename);
  const stdair::ODFilePath lODFilePath (lOnDInputFilename);
  const stdair::FRAT5FilePath lFRAT5FilePath (lFRAT5InputFilename);
  const stdair::FFDisutilityFilePath lFFDisutilityFilePath (lFFDisutilityInputFilename);
  const SIMFQT::FareFilePath lFareFilePath (lFareInputFilename);
  const AIRRAC::YieldFilePath lYieldFilePath (lYieldInputFilename);
  const TRADEMGEN::DemandFilePath lDemandFilePath (lDemandInputFilename);

  // Load the input files
  BOOST_CHECK_NO_THROW (tvlsimService.setInputFiles(lScheduleFilePath, 
						  lODFilePath, 
						  lFRAT5FilePath,
						  lFFDisutilityFilePath, 
						  lYieldFilePath,
						  lFareFilePath, 
						  lDemandFilePath));
	 
  // Parse the input files 
  BOOST_CHECK_NO_THROW (tvlsimService.parseAndLoad ());

  // Initialise the snapshot and RM events
  BOOST_CHECK_NO_THROW (tvlsimService.initSnapshotAndRMEvents());
  
  // Perform a simulation
  // BOOST_CHECK_THROW (tvlsimService.simulate(), stdair::EventException);
  BOOST_CHECK_NO_THROW (tvlsimService.simulate ());

  // Close the log file
  logOutputFile.close();
}

// End the test suite
BOOST_AUTO_TEST_SUITE_END()

/*!
 * \endcode
 */
