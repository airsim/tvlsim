// STL
#include <iostream>
#include <fstream>
#include <string>
// CppUnit
#include <extracppunit/CppUnitCore.hpp>
// StdAir
#include <stdair/STDAIR_Service.hpp>
#include <stdair/bom/BomManager.hpp>
#include <stdair/bom/BomRoot.hpp>
#include <stdair/bom/Inventory.hpp>
#include <stdair/bom/InventoryTypes.hpp>
#include <stdair/bom/FlightDate.hpp>
#include <stdair/bom/FlightDateTypes.hpp>
#include <stdair/bom/LegDate.hpp>
#include <stdair/bom/LegDateTypes.hpp>
#include <stdair/bom/LegCabin.hpp>
#include <stdair/bom/LegCabinTypes.hpp>
#include <stdair/bom/SegmentDate.hpp>
#include <stdair/bom/SegmentDateTypes.hpp>
#include <stdair/bom/SegmentCabin.hpp>
#include <stdair/bom/SegmentCabinTypes.hpp>
#include <stdair/bom/BookingClass.hpp>
#include <stdair/bom/BookingClassTypes.hpp>
#include <stdair/factory/FacBomManager.hpp>
#include <stdair/service/Logger.hpp>
// AirSched
#include <airsched/command/Simulator.hpp>
#include <airsched/AIRSCHED_Service.hpp>
// AirSched Test Suite
#include <test/airsched/AirlineScheduleTestSuite.hpp>

// //////////////////////////////////////////////////////////////////////
void externalMemoryManagementHelper() {

  /**
     The standard initialisation requires an (CSV) input file to be given.
     The initialisation then parses that file and builds the corresponding
     inventories.
     <br><br>
     So, though inventories are already built by the initialisation process,
     another one is built from scratch, in order to test the stdair object
     construction with a fine granularity.
  */
  
  try {
    
    // Input file name
    const std::string lInputFilename ("../samples/schedule02.csv");
    
    // Output log File
    const std::string lLogFilename ("AirlineScheduleTestSuite.log");
    
    // Set the log parameters
    std::ofstream logOutputFile;
    // open and clean the log outputfile
    logOutputFile.open (lLogFilename.c_str());
    logOutputFile.clear();

    const stdair::BasLogParams lLogParams (stdair::LOG::DEBUG, logOutputFile);
    AIRSCHED::AIRSCHED_Service airschedService (lLogParams, lInputFilename);

    // DEBUG
    STDAIR_LOG_DEBUG ("Welcome to Air-Schedule");
    
    // Close the Log outputFile
    logOutputFile.close();

    // Clean the memory.
//     stdair::FacSupervisor::instance().cleanBomContentLayer();
//     stdair::FacSupervisor::instance().cleanBomStructureLayer();
//     AIRSCHED::FacSupervisor::instance().cleanServiceLayer();
//     AIRSCHED::FacSupervisor::instance().cleanLoggerService();

  } catch (const std::exception& stde) {
    std::cerr << "Standard exception: " << stde.what() << std::endl;
    
  } catch (...) {
    std::cerr << "Unknown exception" << std::endl;
  }
}

// //////////////////////////////////////////////////////////////////////
void AirlineScheduleTestSuite::externalMemoryManagement() {
  CPPUNIT_ASSERT_NO_THROW (externalMemoryManagementHelper(););
}

// //////////////////////////////////////////////////////////////////////
void scheduleParsingHelper() {

  try {
    
    // Input file name
    stdair::Filename_T lInputFilename ("../samples/schedule02.csv");

    // Output log File
    std::string lLogFilename ("AirlineScheduleTestSuite.log");

    // Set the log parameters
    std::ofstream logOutputFile;
    // open and clean the log outputfile
    logOutputFile.open (lLogFilename.c_str());
    logOutputFile.clear();

    const stdair::BasLogParams lLogParams (stdair::LOG::DEBUG, logOutputFile);
    AIRSCHED::AIRSCHED_Service airschedService (lLogParams, lInputFilename);

    // DEBUG
    STDAIR_LOG_DEBUG ("Schedule Parsing Test");
    
    // Start a mini-simulation
    airschedService.simulate();

    // Close the Log outputFile
    logOutputFile.close();

  } catch (const std::exception& stde) {
    std::cerr << "Standard exception: " << stde.what()  << std::endl;
    
  } catch (...) {
    std::cerr << "Unknown exception" << std::endl;
  }
}

// //////////////////////////////////////////////////////////////////////
void AirlineScheduleTestSuite::scheduleParsing() {
  CPPUNIT_ASSERT_NO_THROW (scheduleParsingHelper(););
}

// //////////////////////////////////////////////////////////////////////
AirlineScheduleTestSuite::AirlineScheduleTestSuite () {
  _describeKey << "Running test on AIRSCHED Optimisation function";  
}

// /////////////// M A I N /////////////////
CPPUNIT_MAIN()
