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
// AIRINV
#include <airinv/bom/SegmentCabinHelper.hpp>
#include <airinv/bom/FlightDateHelper.hpp>

namespace AIRINV {
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
