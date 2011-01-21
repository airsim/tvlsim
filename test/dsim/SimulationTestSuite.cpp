// STL
#include <sstream>
#include <fstream>
#include <string>
// CPPUNIT
#include <extracppunit/CppUnitCore.hpp>
// StdAir
#include <stdair/basic/BasLogParams.hpp>
#include <stdair/basic/BasDBParams.hpp>
// Dsim
#include <dsim/DSIM_Types.hpp>
#include <dsim/DSIM_Service.hpp>
// Dsim Test Suite
#include <test/dsim/SimulationTestSuite.hpp>
#include <dsim/config/dsim-paths.hpp>

// //////////////////////////////////////////////////////////////////////
// Test is based on ...
// //////////////////////////////////////////////////////////////////////

// //////////////////////////////////////////////////////////////////////
void SimulationTestSuite::simpleSimulationHelper() {

  try {
    
    // Schedule input file name
    const std::string lScheduleInputFilename (STDAIR_SAMPLE_DIR "/schedule01.csv");
    
    // O&D input file name
    const std::string lODInputFilename (STDAIR_SAMPLE_DIR "/ond01.csv");

    // Demand input file name
    const stdair::Filename_T lDemandInputFilename (STDAIR_SAMPLE_DIR "/demand01.csv");

    // Fare input file name
    const stdair::Filename_T lFareInputFilename (STDAIR_SAMPLE_DIR "/fare01.csv");
    
    // Output log File
    const std::string lLogFilename ("SimulationTestSuite.log");

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
    dsimService.simulate();

  } catch (const DSIM::RootException& otexp) {
    std::cerr << "DSim exception: " << otexp.what() << std::endl;
    
  } catch (const std::exception& stde) {
    std::cerr << "Standard exception: " << stde.what() << std::endl;
  }
}

// //////////////////////////////////////////////////////////////////////
void SimulationTestSuite::simpleSimulation () {
  // TODO: Check that the simulation goes as expected
  CPPUNIT_ASSERT_NO_THROW ( simpleSimulationHelper(););
}

// //////////////////////////////////////////////////////////////////////
// void SimulationTestSuite::errorCase () {
//  CPPUNIT_ASSERT (false);
// }

// //////////////////////////////////////////////////////////////////////
SimulationTestSuite::SimulationTestSuite () {
  _describeKey << "Running test on simulation";  
}

// /////////////// M A I N /////////////////
CPPUNIT_MAIN()

