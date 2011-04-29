// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// StdAir
#include <stdair/bom/BomRetriever.hpp>
#include <stdair/bom/BomManager.hpp>
#include <stdair/bom/Inventory.hpp>
#include <stdair/bom/FlightDate.hpp>
#include <stdair/bom/SegmentDate.hpp>
#include <stdair/bom/SegmentCabin.hpp>
#include <stdair/bom/FareFamily.hpp>
#include <stdair/bom/BookingClass.hpp>
#include <stdair/bom/GuillotineBlock.hpp>
#include <stdair/bom/TravelSolutionStruct.hpp>
#include <stdair/service/Logger.hpp>
// AirInv
#include <airinv/bom/InventoryHelper.hpp>
#include <airinv/bom/FlightDateHelper.hpp>
#include <airinv/bom/GuillotineBlockHelper.hpp>
#include <airinv/bom/SegmentCabinHelper.hpp>

namespace AIRINV {

  // ////////////////////////////////////////////////////////////////////
  void InventoryHelper::fillFromRouting (const stdair::Inventory& iInventory) {
    const stdair::FlightDateList_T& lFlightDateList =
      stdair::BomManager::getList<stdair::FlightDate> (iInventory);

    // Browse the list of flight-dates and update each flight-date.
    for (stdair::FlightDateList_T::const_iterator itFlightDate =
           lFlightDateList.begin();
         itFlightDate != lFlightDateList.end(); ++itFlightDate) {
      const stdair::FlightDate* lCurrentFlightDate_ptr = *itFlightDate;
      assert (lCurrentFlightDate_ptr != NULL);
      FlightDateHelper::fillFromRouting (*lCurrentFlightDate_ptr);
    }
  }

  // ////////////////////////////////////////////////////////////////////
  void InventoryHelper::
  calculateAvailability (const stdair::Inventory& iInventory, 
                         const std::string& iFullSegmentDateKey,
                         stdair::TravelSolutionStruct& ioTravelSolution) {

    // Create the map of class/availability for the given segment date.
    stdair::ClassAvailabilityMap_T lClassAvailabilityMap;

    // DEBUG
    STDAIR_LOG_DEBUG (iFullSegmentDateKey);
    //
    stdair::SegmentDate* lSegmentDate_ptr =
      stdair::BomRetriever::retrieveSegmentDateFromLongKey(iInventory,
                                                           iFullSegmentDateKey);
    assert (lSegmentDate_ptr != NULL);
    
    // Browse the segment-cabins and fill the map with the availability of
    // each booking class.
    const stdair::SegmentCabinList_T& lSegmentCabinList =
      stdair::BomManager::getList<stdair::SegmentCabin> (*lSegmentDate_ptr);
    for (stdair::SegmentCabinList_T::const_iterator itCabin =
           lSegmentCabinList.begin();
         itCabin != lSegmentCabinList.end(); ++itCabin) {
      stdair::SegmentCabin* lSegmentCabin_ptr = *itCabin;
      assert (lSegmentCabin_ptr != NULL);

      // TODO: re-implemented this part.
      const stdair::CommittedSpace_T& lCommittedSpace =
        lSegmentCabin_ptr->getCommittedSpace();

      const stdair::FareFamilyList_T& lFFList =
        stdair::BomManager::getList<stdair::FareFamily> (*lSegmentCabin_ptr);
      for (stdair::FareFamilyList_T::const_iterator itFF = lFFList.begin();
           itFF != lFFList.end(); ++itFF) {
        const stdair::FareFamily* lFareFamily_ptr = *itFF;
        assert (lFareFamily_ptr != NULL);

        const stdair::BookingClassList_T& lBCList =
          stdair::BomManager::getList<stdair::BookingClass> (*lFareFamily_ptr);
        for (stdair::BookingClassList_T::const_iterator itBC = lBCList.begin();
             itBC != lBCList.end(); ++itBC) {
          const stdair::BookingClass* lBC_ptr = *itBC;
          assert (lBC_ptr != NULL);
          
          const stdair::ClassCode_T& lClassCode = lBC_ptr->getClassCode();
          const stdair::AuthorizationLevel_T& lAU =
            lBC_ptr->getAuthorizationLevel();
          const stdair::Availability_T lAvl = lAU - lCommittedSpace;
          
          const bool insertSuccessful = lClassAvailabilityMap.
            insert (stdair::ClassAvailabilityMap_T::value_type (lClassCode,
                                                                lAvl)).second;
          assert (insertSuccessful == true);
          
          // DEBUG
          STDAIR_LOG_DEBUG ("Class: " << lClassCode
                            << ", " << "AU: " << lAU << ", "
                            << "Committed space: " << lCommittedSpace << ", "
                            << "Avl: " << lAvl);
        }
      }
    }

    //
    ioTravelSolution.addClassAvailabilityMap (lClassAvailabilityMap);
  }

  // ////////////////////////////////////////////////////////////////////
  bool InventoryHelper::sell (stdair::Inventory& ioInventory, 
                              const std::string& iFullSegmentDateKey,
                              const stdair::ClassCode_T& iClassCode,
                              const stdair::PartySize_T& iPartySize) {
    bool hasSaleBeenSuccessful = false;

    // DEBUG
    STDAIR_LOG_DEBUG ("Full key: '" << iFullSegmentDateKey
                      << "', " << iClassCode);

    //
    stdair::BookingClass* lBookingClass_ptr =
      stdair::BomRetriever::retrieveBookingClassFromLongKey(ioInventory,
                                                            iFullSegmentDateKey,
                                                            iClassCode);

    // DEBUG
    const std::string hasFoundBookingClassStr =
      (lBookingClass_ptr != NULL)?"Yes":"No";
    STDAIR_LOG_DEBUG ("Found booking class? " << hasFoundBookingClassStr);

    if (lBookingClass_ptr != NULL) {
      // Register the sale in the class.
      lBookingClass_ptr->sell (iPartySize);

      //
      stdair::FareFamily& lFareFamily =
        stdair::BomManager::getParent<stdair::FareFamily> (*lBookingClass_ptr);

      //
      stdair::SegmentCabin& lSegmentCabin =
        stdair::BomManager::getParent<stdair::SegmentCabin> (lFareFamily);

      //
      stdair::SegmentDate& lSegmentDate =
        stdair::BomManager::getParent<stdair::SegmentDate,
                                      stdair::SegmentCabin> (lSegmentCabin);

      //
      stdair::FlightDate& lFlightDate =
        stdair::BomManager::getParent<stdair::FlightDate,
                                      stdair::SegmentDate> (lSegmentDate);
      
      // Update the commited space of the segment-cabins and the leg-cabins.
      SegmentCabinHelper::updateFromReservation (lFlightDate, lSegmentCabin,
                                                 iPartySize);
      hasSaleBeenSuccessful = true;
    }

    return hasSaleBeenSuccessful;
  }

  // ////////////////////////////////////////////////////////////////////
  void InventoryHelper::takeSnapshots(const stdair::Inventory& iInventory,
                                      const stdair::DateTime_T& iSnapshotTime) {
    // Browse the guillotine block list and take the snapshots for
    // each guillotine.
    const stdair::GuillotineBlockList_T& lGuillotineBlockList =
      stdair::BomManager::getList<stdair::GuillotineBlock> (iInventory);
    for (stdair::GuillotineBlockList_T::const_iterator itGB =
           lGuillotineBlockList.begin();
         itGB != lGuillotineBlockList.end(); ++itGB) {
      stdair::GuillotineBlock* lGuillotineBlock_ptr = *itGB;

      GuillotineBlockHelper::takeSnapshots(*lGuillotineBlock_ptr,iSnapshotTime);
    }    
  }
}
