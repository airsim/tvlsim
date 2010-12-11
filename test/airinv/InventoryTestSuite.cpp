// STL
#include <sstream>
#include <fstream>
#include <string>
// CPPUNIT
#include <extracppunit/CppUnitCore.hpp>
// StdAir
#include <stdair/basic/BasLogParams.hpp>
#include <stdair/basic/BasDBParams.hpp>
#include <stdair/bom/TravelSolutionStruct.hpp>
// Airinv
#include <airinv/AIRINV_Types.hpp>
#include <airinv/AIRINV_Master_Service.hpp>
// Airinv Test Suite
#include <test/airinv/InventoryTestSuite.hpp>

// //////////////////////////////////////////////////////////////////////
// Test is based on ...
// //////////////////////////////////////////////////////////////////////

// //////////////////////////////////////////////////////////////////////
void InventoryTestSuite::simpleInventoryHelper() {

  try {
    
    // Number of passengers in the travelling group
    // const stdair::PartySize_T lPartySize = 5;

    // Travel solution
    // stdair::TravelSolutionStruct lTravelSolution;
    
    // Input file name
    stdair::Filename_T lInputFilename ("../../test/samples/invdump01.csv");

    // Output log File
    stdair::Filename_T lLogFilename ("InventoryTestSuite.log");

    // Set the log parameters
    std::ofstream logOutputFile;
    // Open and clean the log outputfile
    logOutputFile.open (lLogFilename.c_str());
    logOutputFile.clear();
    
    // Initialise the list of classes/buckets
    const stdair::BasLogParams lLogParams (stdair::LOG::DEBUG, logOutputFile);
    AIRINV::AIRINV_Master_Service airinvService (lLogParams, lInputFilename);

    // Make a booking
    // airinvService.sell (lTravelSolution, lPartySize);
    
  } catch (const AIRINV::RootException& otexp) {
    std::cerr << "Standard exception: " << otexp.what() << std::endl;
    return;
    
  } catch (const std::exception& stde) {
    std::cerr << "Standard exception: " << stde.what() << std::endl;
    return;
    
  } catch (...) {
    return;
  }
  
}

// //////////////////////////////////////////////////////////////////////
void InventoryTestSuite::simpleInventory () {
  // TODO: Check that the inventory goes as expected
  CPPUNIT_ASSERT_NO_THROW ( simpleInventoryHelper(););
}

// //////////////////////////////////////////////////////////////////////
// void InventoryTestSuite::errorCase () {
//  CPPUNIT_ASSERT (false);
// }

// //////////////////////////////////////////////////////////////////////
InventoryTestSuite::InventoryTestSuite () {
  _describeKey << "Running test on inventory";  
}

// /////////////// M A I N /////////////////
CPPUNIT_MAIN()

