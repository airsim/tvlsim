// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <vector>

// StdAir
#include <stdair/bom/Inventory.hpp>
#include <stdair/bom/BomManager.hpp>
#include <stdair/bom/BomRetriever.hpp>
#include <stdair/bom/BomRoot.hpp>

#include <stdair/bom/BomKeyManager.hpp>
#include <stdair/bom/InventoryKey.hpp>
#include <stdair/bom/FlightDateKey.hpp>
#include <stdair/bom/SegmentDateKey.hpp>
#include <stdair/bom/BomDisplay.hpp>

// AirInv
#include <airinv/command/OnDGenerator.hpp>

//#include <airsched/bom/BomDisplay.hpp>



namespace AIRINV {

  // ////////////////////////////////////////////////////////////////////
  void OnDGenerator::
  createOnD (const stdair::BomRoot& iBomRoot) {

    // std::cout << "OnD creation" << std::endl;

    // //std::cout << AIRSCHED::BomDisplay::csvDisplay (iBomRoot) << std::endl;
    // std::ostringstream oStr;
    // stdair::BomDisplay::csvDisplay (oStr, iBomRoot);
    // std::cout << oStr.str() << std::endl;
    

    stdair::InventoryKey lInvKey ("SQ");
    stdair::Date_T lDate (2010, 2, 8);
    // std::cout << lDate << std::endl;
    stdair::FlightNumber_T lFlightNumber (11);
    stdair::AirportCode_T lOrigin ("SIN");
    stdair::AirportCode_T lDestination ("BKK");
    std::string lFullKeyStr = "SQ;11,2010-Feb-08;SIN,BKK";

    // std::cout << stdair::BomKeyManager::extractInventoryKey (lFullKeyStr).toString() << " ";
    // std::cout << stdair::BomKeyManager::extractFlightDateKey (lFullKeyStr).toString() << " ";
    // std::cout << stdair::BomKeyManager::extractSegmentDateKey (lFullKeyStr).toString() << std::endl;
    
    
    std::string lStr = "Y:M";
    DemandStruct lDemandStruct (500.0, 20.0, 1.0);    
    stdair::SegmentDate* lSegmentDate_ptr =
      stdair::BomRetriever::retrieveSegmentDateFromLongKey (iBomRoot, lFullKeyStr);
    assert (lSegmentDate_ptr != NULL);

    //OnDDate lOnDDate (lOrigin, lDestination, lDate);

    //OnDDateKey lOnDDateKey (lOrigin, lDestination, lDate);
    
    std::vector<std::string> lFullKeyStrList;
    lFullKeyStrList.push_back (lFullKeyStr);
    OnDDateKey lOnDDateKey (lFullKeyStrList);
    OnDDate lOnDDate (lOnDDateKey);
    
    lOnDDate.addSegmentDatePtr (lSegmentDate_ptr);
    lOnDDate.addClassPathDemand (lStr, lDemandStruct);

    lOnDDate.display();
  }
  
}
