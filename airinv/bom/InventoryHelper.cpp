// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// STDAIR
#include <stdair/bom/BomManager.hpp>
#include <stdair/bom/Inventory.hpp>
#include <stdair/bom/FlightDate.hpp>
#include <stdair/bom/SegmentDate.hpp>
#include <stdair/bom/SegmentCabin.hpp>
#include <stdair/bom/BookingClass.hpp>
#include <stdair/bom/TravelSolutionStruct.hpp>
#include <stdair/service/Logger.hpp>
// AIRINV
#include <airinv/bom/InventoryHelper.hpp>
#include <airinv/bom/FlightDateHelper.hpp>
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
                         const std::string& iSegmentDateKey,
                         stdair::TravelSolutionStruct& ioTravelSolution) {
    // Create the map of class/availability for the given segment date.
    stdair::ClassAvailabilityMap_T lClassAvailabilityMap;

    const int lSize = iSegmentDateKey.size();
    // Retrieve the corresponding flight-date.
    std::string lFlightDateKey;
    STDAIR_LOG_DEBUG (iSegmentDateKey);
    lFlightDateKey.append (iSegmentDateKey, 4, lSize - 23);
    const stdair::FlightDate& lFlightDate =
      stdair::BomManager::getObject<stdair::FlightDate> (iInventory,
                                                         lFlightDateKey);

    // Retrieve the corresponding segment-date.
    std::string lSegmentDateKey;
    lSegmentDateKey.append (iSegmentDateKey, lSize - 7, 7);
    const stdair::SegmentDate& lSegmentDate =
      stdair::BomManager::getObject<stdair::SegmentDate> (lFlightDate,
                                                          lSegmentDateKey);
    
    // Browse the segment-cabins and fill the map with the availability of
    // each booking class.
    const stdair::SegmentCabinList_T& lSegmentCabinList =
      stdair::BomManager::getList<stdair::SegmentCabin> (lSegmentDate);
    for (stdair::SegmentCabinList_T::const_iterator itCabin =
           lSegmentCabinList.begin();
         itCabin != lSegmentCabinList.end(); ++itCabin) {
      stdair::SegmentCabin* lSegmentCabin_ptr = *itCabin;
      assert (lSegmentCabin_ptr != NULL);

      // TODO: re-implemented this part.
      const stdair::CommittedSpace_T& lCommittedSpace =
        lSegmentCabin_ptr->getCommittedSpace ();
      const stdair::BookingClassList_T& lBCList =
        stdair::BomManager::getList<stdair::BookingClass> (*lSegmentCabin_ptr);

      for (stdair::BookingClassList_T::const_iterator itBC = lBCList.begin();
           itBC != lBCList.end(); ++itBC) {
        const stdair::BookingClass* lBC_ptr = *itBC;
        assert (lBC_ptr != NULL);

        const stdair::ClassCode_T& lClassCode = lBC_ptr->getClassCode();
        const stdair::AuthorizationLevel_T& lAU=lBC_ptr->getAuthorizationLevel();
        const stdair::Availability_T lAvl = lAU - lCommittedSpace;

       bool insertSuccessful = lClassAvailabilityMap.
          insert (stdair::ClassAvailabilityMap_T::
                  value_type (lClassCode, lAvl)).second;
       assert (insertSuccessful == true);
        // DEBUG
        STDAIR_LOG_DEBUG ("Class: " << lClassCode << ", "
                          << "AU: " << lAU << ", "
                          << "Committed space: " << lCommittedSpace << ", "
                          << "Avl: " << lAvl);
        
      }
    }

    ioTravelSolution.addClassAvailabilityMap (lClassAvailabilityMap);
  }

  // ////////////////////////////////////////////////////////////////////
  bool InventoryHelper::sell (stdair::Inventory& ioInventory, 
                              const std::string& iSegmentDateKey,
                              const stdair::ClassCode_T& iClassCode,
                              const stdair::PartySize_T& iPartySize) {
    const int lSize = iSegmentDateKey.size();
    // Retrieve the corresponding flight-date.
    std::string lFlightDateKey;
    STDAIR_LOG_DEBUG (iSegmentDateKey);
    lFlightDateKey.append (iSegmentDateKey, 4, lSize - 23);
    const stdair::FlightDate& lFlightDate =
      stdair::BomManager::getObject<stdair::FlightDate> (ioInventory,
                                                         lFlightDateKey);

    // Retrieve the corresponding segment-date.
    std::string lSegmentDateKey;
    lSegmentDateKey.append (iSegmentDateKey, lSize - 7, 7);
    const stdair::SegmentDate& lSegmentDate =
      stdair::BomManager::getObject<stdair::SegmentDate> (lFlightDate,
                                                          lSegmentDateKey);

    // Browse the segment-cabins and make the sale with the
    // corresponding booking class.
    const stdair::SegmentCabinList_T& lSegmentCabinList =
      stdair::BomManager::getList<stdair::SegmentCabin> (lSegmentDate);
    for (stdair::SegmentCabinList_T::const_iterator itCabin =
           lSegmentCabinList.begin();
         itCabin != lSegmentCabinList.end(); ++itCabin) {
      stdair::SegmentCabin* lSegmentCabin_ptr = *itCabin;
      assert (lSegmentCabin_ptr != NULL);
      stdair::BookingClass* lBookingClass_ptr = stdair::BomManager::
        getObjectPtr<stdair::BookingClass> (*lSegmentCabin_ptr, iClassCode);
      if (lBookingClass_ptr) {
        // Register the sale in the class.
        lBookingClass_ptr->sell (iPartySize);
        // Update the commited space of the segment-cabins and the leg-cabins.
        SegmentCabinHelper::updateFromReservation (lFlightDate,
                                                   *lSegmentCabin_ptr,
                                                   iPartySize);
        return true;
      }
    }
    return false;
  }
}
