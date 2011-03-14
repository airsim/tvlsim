// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <exception>
// StdAir
#include <stdair/basic/BasConst_Inventory.hpp>
#include <stdair/bom/BomManager.hpp>
#include <stdair/bom/BomRoot.hpp>
#include <stdair/bom/Inventory.hpp>
#include <stdair/bom/FlightDate.hpp>
#include <stdair/bom/SegmentDate.hpp>
#include <stdair/bom/SegmentCabin.hpp>
#include <stdair/bom/LegDate.hpp>
#include <stdair/bom/LegCabin.hpp>
#include <stdair/bom/FareFamily.hpp>
#include <stdair/bom/BookingClass.hpp>
#include <stdair/factory/FacBomManager.hpp>
#include <stdair/bom/TravelSolutionStruct.hpp>
#include <stdair/service/Logger.hpp>
// AirInv
#include <airinv/AIRINV_Types.hpp>
#include <airinv/bom/BomRootHelper.hpp>
#include <airinv/bom/InventoryHelper.hpp>
#include <airinv/command/InventoryManager.hpp>

namespace AIRINV {

  // ////////////////////////////////////////////////////////////////////
  void InventoryManager::
  calculateAvailability (const stdair::BomRoot& iBomRoot,
                         stdair::TravelSolutionStruct& ioTravelSolution) {
    // Browse the list of segments and get the availability for the
    // children classes.
    const stdair::SegmentPath_T& lSegmentPath =
      ioTravelSolution.getSegmentPath();
    for (stdair::SegmentPath_T::const_iterator itSK = lSegmentPath.begin();
         itSK != lSegmentPath.end(); ++itSK) {
      const std::string& lSegmentKey = *itSK;
      std::string lInvKey;
      lInvKey.append (lSegmentKey, 0, 2);
      stdair::Inventory& lInventory =
        stdair::BomManager::getObject<stdair::Inventory>(iBomRoot, lInvKey);

      InventoryHelper::calculateAvailability (lInventory, lSegmentKey,
                                              ioTravelSolution);
    }
  }  

  // ////////////////////////////////////////////////////////////////////
  bool InventoryManager::sell (stdair::Inventory& ioInventory,
                               const std::string& iSegmentDateKey,
                               const stdair::ClassCode_T& iClassCode,
                               const stdair::PartySize_T& iPartySize) {

    // Make the sale within the inventory.
    return InventoryHelper::sell (ioInventory, iSegmentDateKey,
                                  iClassCode, iPartySize);
  }
    
  // ////////////////////////////////////////////////////////////////////
  void InventoryManager::
  createDirectAccesses (const stdair::BomRoot& iBomRoot) {

    // Browse the list of inventories and create direct accesses
    // within each inventory.
    const stdair::InventoryList_T& lInvList =
      stdair::BomManager::getList<stdair::Inventory> (iBomRoot);
    for (stdair::InventoryList_T::const_iterator itInv = lInvList.begin();
         itInv != lInvList.end(); ++itInv) {
      stdair::Inventory* lCurrentInv_ptr = *itInv;
      assert (lCurrentInv_ptr != NULL);
      createDirectAccesses (*lCurrentInv_ptr);
    }

    // Fill some attributes of segment-date with the routing legs.
    BomRootHelper::fillFromRouting (iBomRoot);
  }

  // ////////////////////////////////////////////////////////////////////
  void InventoryManager::
  createDirectAccesses (stdair::Inventory& ioInventory) {
    // Browse the list of flight-dates and create direct accesses
    // within each flight-date.
    const stdair::FlightDateList_T& lFlightDateList = 
      stdair::BomManager::getList<stdair::FlightDate> (ioInventory);
    for (stdair::FlightDateList_T::const_iterator itFlightDate = 
           lFlightDateList.begin();
         itFlightDate != lFlightDateList.end(); ++itFlightDate) {
      stdair::FlightDate* lCurrentFlightDate_ptr = *itFlightDate;
      assert (lCurrentFlightDate_ptr != NULL);
      createDirectAccesses (*lCurrentFlightDate_ptr);
    }
  }

  // ////////////////////////////////////////////////////////////////////
  void InventoryManager::
  createDirectAccesses (stdair::FlightDate& ioFlightDate) {
    // Browse the list of segment-dates and create direct accesses
    // within each segment-date.
    const stdair::SegmentDateList_T& lSegmentDateList = 
      stdair::BomManager::getList<stdair::SegmentDate> (ioFlightDate);
    for (stdair::SegmentDateList_T::const_iterator itSegmentDate = 
           lSegmentDateList.begin();
         itSegmentDate != lSegmentDateList.end(); ++itSegmentDate) {
      stdair::SegmentDate* lCurrentSegmentDate_ptr = *itSegmentDate;
      assert (lCurrentSegmentDate_ptr != NULL);
      const stdair::AirportCode_T& lBoardingPoint =
        lCurrentSegmentDate_ptr->getBoardingPoint();
      stdair::AirportCode_T currentBoardingPoint = lBoardingPoint;
      const stdair::AirportCode_T& lOffPoint =
        lCurrentSegmentDate_ptr->getOffPoint();
      
      // Add a sanity check so as to ensure that the loop stops. If
      // there are more than MAXIMAL_NUMBER_OF_LEGS legs, there is
      // an issue somewhere in the code (not in the parser, as the
      // segments are derived from the legs thanks to the
      // FlightPeriodStruct::buildSegments() method).
      unsigned short i = 1;
      while (currentBoardingPoint != lOffPoint
             && i <= stdair::MAXIMAL_NUMBER_OF_LEGS_IN_FLIGHT) {
        // Retrieve the (unique) LegDate getting that Boarding Point
        stdair::LegDate& lLegDate = stdair::BomManager::
          getObject<stdair::LegDate> (ioFlightDate, currentBoardingPoint);
        // Link the SegmentDate and LegDate together
        stdair::FacBomManager::
          instance().addToListAndMap (*lCurrentSegmentDate_ptr, lLegDate);
        stdair::FacBomManager::
          instance().addToListAndMap (lLegDate, *lCurrentSegmentDate_ptr);
        // Prepare the next iteration
        currentBoardingPoint = lLegDate.getOffPoint();
        ++i;
      }
      assert (i <= stdair::MAXIMAL_NUMBER_OF_LEGS_IN_FLIGHT);
          
      // Create the routing for the leg- and segment-cabins.
      // At the same time, set the SegmentDate attributes derived from
      // its routing legs (e.g., boarding and off dates).
      createDirectAccesses (*lCurrentSegmentDate_ptr);
    }
  }

  // ////////////////////////////////////////////////////////////////////
  void InventoryManager::
  createDirectAccesses (stdair::SegmentDate& ioSegmentDate) {
    // Browse the list of segment-cabins and create direct accesses
    // within each segment-cabin.
    const stdair::SegmentCabinList_T& lSegmentCabinList = 
      stdair::BomManager::getList<stdair::SegmentCabin> (ioSegmentDate);
    for (stdair::SegmentCabinList_T::const_iterator itSegmentCabin = 
           lSegmentCabinList.begin();
         itSegmentCabin != lSegmentCabinList.end(); ++itSegmentCabin) {
      stdair::SegmentCabin* lCurrentSegmentCabin_ptr = *itSegmentCabin;
      assert (lCurrentSegmentCabin_ptr != NULL);
        const stdair::CabinCode_T& lCabinCode =
          lCurrentSegmentCabin_ptr->getCabinCode();
      stdair::MapKey_T lSegmentCabinFullKey =
        ioSegmentDate.describeKey() + ", " + lCabinCode;
      
      // Iterate on the routing legs
      const stdair::LegDateList_T& lLegDateList =
        stdair::BomManager::getList<stdair::LegDate> (ioSegmentDate);
      for (stdair::LegDateList_T::const_iterator itLegDate =
             lLegDateList.begin();
           itLegDate != lLegDateList.end(); ++itLegDate) {
        const stdair::LegDate* lCurrentLegDate_ptr = *itLegDate;        
        assert (lCurrentLegDate_ptr != NULL);

        // Retrieve the LegCabin getting the same class of service
        // (cabin code) as the SegmentCabin.
        stdair::LegCabin& lLegCabin = stdair::BomManager::
          getObject<stdair::LegCabin> (*lCurrentLegDate_ptr, lCabinCode);
        stdair::MapKey_T lLegCabinFullKey =
          lCurrentLegDate_ptr->describeKey() + ", " + lCabinCode;

        // Link the SegmentCabin and LegCabin together
        stdair::FacBomManager::
          instance().addToListAndMap (*lCurrentSegmentCabin_ptr,
                                      lLegCabin, lLegCabinFullKey);
        stdair::FacBomManager::
          instance().addToListAndMap (lLegCabin, *lCurrentSegmentCabin_ptr,
                                      lSegmentCabinFullKey);
      }      
    }
  }


}
