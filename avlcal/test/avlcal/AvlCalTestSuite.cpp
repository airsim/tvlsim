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
// Avlcal
#include <avlcal/AVLCAL_Types.hpp>
#include <avlcal/AVLCAL_Service.hpp>
// Avlcal Test Suite
#include <test/avlcal/AvlCalTestSuite.hpp>

// //////////////////////////////////////////////////////////////////////
// Test is based on ...
// //////////////////////////////////////////////////////////////////////

// //////////////////////////////////////////////////////////////////////
void AvlCalTestSuite::simpleAvlCalHelper() {

  try {
    
    // Airline code
    std::string lAirlineCode ("SV");
    
    // Number of passengers in the travelling group
    AVLCAL::PartySize_T lPartySize = 5;
    
    // Output log File
    std::string lLogFilename ("AvlCalTestSuite.log");

    // Set the log parameters
    std::ofstream logOutputFile;
    // Open and clean the log outputfile
    logOutputFile.open (lLogFilename.c_str());
    logOutputFile.clear();
    
    // Initialise the list of classes/buckets
    const stdair::BasLogParams lLogParams (stdair::LOG::DEBUG, logOutputFile);
    AVLCAL::AVLCAL_Service avlcalService (lLogParams, lAirlineCode);

    // Perform an availability calculation
    avlcalService.avlCalculate (lPartySize);
    
  } catch (const AVLCAL::RootException& otexp) {
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
void AvlCalTestSuite::simpleAvlCal () {
  // TODO: Check that the availability calculation goes as expected
  CPPUNIT_ASSERT_NO_THROW ( simpleAvlCalHelper(););
}

// //////////////////////////////////////////////////////////////////////
// void AvlCalTestSuite::errorCase () {
//  CPPUNIT_ASSERT (false);
// }

// //////////////////////////////////////////////////////////////////////
AvlCalTestSuite::AvlCalTestSuite () {
  _describeKey << "Running test on availability calculation";  
}

// /////////////// M A I N /////////////////
CPPUNIT_MAIN()

