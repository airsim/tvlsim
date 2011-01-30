/*!
 * \page SimulationTestSuite_cpp Command-Line Test to Demonstrate How To Use DSim
 * \code
 */
// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
// Boost Unit Test Framework (UTF)
#define BOOST_TEST_MODULE StdAirTest
#include <boost/test/unit_test.hpp>
// StdAir
#include <stdair/basic/BasLogParams.hpp>
#include <stdair/basic/BasDBParams.hpp>
#include <stdair/bom/BomRoot.hpp>
// Dsim
#include <dsim/DSIM_Types.hpp>
#include <dsim/DSIM_Service.hpp>
#include <dsim/config/dsim-paths.hpp>

namespace boost_utf = boost::unit_test;

/**
 * Configuration for the Boost Unit Test Framework (UTF)
 */
struct UnitTestConfig {
  /** Constructor. */
  UnitTestConfig() : _test_log ("SimulationTestSuite_results.xml")  {
    boost_utf::unit_test_log.set_stream (_test_log);
    boost_utf::unit_test_log.set_format (boost_utf::XML);
  }
  /** Destructor. */
  ~UnitTestConfig() {
    boost_utf::unit_test_log.set_stream (std::cout);
  }
  /** Log file */  
  std::ofstream _test_log;
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

  // Schedule input file name
  const stdair::Filename_T lScheduleInputFilename (STDAIR_SAMPLE_DIR
                                                   "/schedule01.csv");
    
  // O&D input file name
  const stdair::Filename_T lODInputFilename (STDAIR_SAMPLE_DIR "/ond01.csv");

  // Demand input file name
  const stdair::Filename_T lDemandInputFilename (STDAIR_SAMPLE_DIR
                                                 "/demand01.csv");

  // Fare input file name
  const stdair::Filename_T lFareInputFilename (STDAIR_SAMPLE_DIR "/fare01.csv");
    
  // Check that the file path given as input corresponds to an actual file
  bool doesExistAndIsReadable =
    stdair::BasFileMgr::doesExistAndIsReadable (lScheduleInputFilename);
  BOOST_CHECK_MESSAGE (doesExistAndIsReadable == true,
                       "The '" << lScheduleInputFilename
                       << "' input file can not be open and read");

  // Check that the file path given as input corresponds to an actual file
  doesExistAndIsReadable =
    stdair::BasFileMgr::doesExistAndIsReadable (lODInputFilename);
  BOOST_CHECK_MESSAGE (doesExistAndIsReadable == true,
                       "The '" << lODInputFilename
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
  DSIM::DSIM_Service dsimService (lLogParams, lDBParams,
                                  lScheduleInputFilename, lODInputFilename,
                                  lFareInputFilename, lDemandInputFilename);
  
  // Perform a simulation
  // TODO: add the missing fare rules, so that there is no longer an exception
  //BOOST_CHECK_NO_THROW (dsimService.simulate());
  BOOST_CHECK_THROW (dsimService.simulate(), DSIM::SimulationException);

  // Close the log file
  logOutputFile.close();
}

// End the test suite
BOOST_AUTO_TEST_SUITE_END()

/*!
 * \endcode
 */
