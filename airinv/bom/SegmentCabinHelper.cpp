// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// STDAIR
#include <stdair/bom/BomManager.hpp>
#include <stdair/bom/FlightDate.hpp>
#include <stdair/bom/SegmentCabin.hpp>
#include <stdair/bom/LegCabin.hpp>
#include <stdair/bom/BookingClass.hpp>
// AIRINV
#include <airinv/bom/SegmentCabinHelper.hpp>
#include <airinv/bom/FlightDateHelper.hpp>

namespace AIRINV {
  // ////////////////////////////////////////////////////////////////////
  void SegmentCabinHelper::
  initialiseAU (stdair::SegmentCabin& iSegmentCabin) {
    // Initialise the capacity and availability pool.
    const stdair::LegCabinList_T& lLCList =
      stdair::BomManager::getList<stdair::LegCabin> (iSegmentCabin);
    stdair::CabinCapacity_T lCapacity =
      std::numeric_limits<stdair::CabinCapacity_T>::max();
    for (stdair::LegCabinList_T::const_iterator itLC = lLCList.begin();
         itLC != lLCList.end(); ++itLC) {
      const stdair::LegCabin* lLC_ptr = *itLC;
      assert (lLC_ptr != NULL);
      const stdair::CabinCapacity_T& lCabinCap = lLC_ptr->getOfferedCapacity ();
      if (lCapacity > lCabinCap) {
        lCapacity = lCabinCap;
      }
    }
    iSegmentCabin.setCapacity (lCapacity);
    iSegmentCabin.setAvailabilityPool (lCapacity);
    
    // Browse the list of booking classes and set the AU of each booking
    // class to the availability pool of the cabin.
    const stdair::BookingClassList_T& lBCList =
      stdair::BomManager::getList<stdair::BookingClass> (iSegmentCabin);
    for (stdair::BookingClassList_T::const_iterator itBC = lBCList.begin();
         itBC != lBCList.end(); ++itBC) {
      stdair::BookingClass* lBC_ptr = *itBC;
      assert (lBC_ptr != NULL);
      lBC_ptr->setAuthorizationLevel (lCapacity);
    }
  }
  
  // ////////////////////////////////////////////////////////////////////
  void SegmentCabinHelper::
  updateFromReservation (const stdair::FlightDate& iFlightDate,
                         stdair::SegmentCabin& ioSegmentCabin,
                         const stdair::PartySize_T& iNbOfBookings){
    // Update the commited space of the segment-cabin.
    ioSegmentCabin.updateFromReservation (iNbOfBookings);

    // Update the commited space of the member leg-cabins.
    const stdair::LegCabinList_T& lLegCabinList =
      stdair::BomManager::getList<stdair::LegCabin> (ioSegmentCabin);
    for (stdair::LegCabinList_T::const_iterator itLegCabin =
           lLegCabinList.begin();
         itLegCabin != lLegCabinList.end(); ++itLegCabin) {
      stdair::LegCabin* lLegCabin_ptr = *itLegCabin;
      assert (lLegCabin_ptr != NULL);
      lLegCabin_ptr->updateFromReservation (iNbOfBookings);
    }

    // Update the availability pool of all the segment-cabin which belong to the
    // same flight-date.
    const stdair::CabinCode_T& lCabinCode = ioSegmentCabin.getCabinCode();
    FlightDateHelper::updateAvailablityPool (iFlightDate, lCabinCode);
  }

 }
