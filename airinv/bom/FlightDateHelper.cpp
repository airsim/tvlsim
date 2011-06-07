// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// STDAIR
#include <stdair/basic/BasConst_Inventory.hpp>
#include <stdair/bom/BomManager.hpp>
#include <stdair/bom/FlightDate.hpp>
#include <stdair/bom/SegmentDate.hpp>
#include <stdair/bom/SegmentCabin.hpp>
#include <stdair/bom/LegCabin.hpp>
// AIRINV
#include <airinv/bom/FlightDateHelper.hpp>
#include <airinv/bom/SegmentDateHelper.hpp>
#include <airinv/bom/SegmentCabinHelper.hpp>

namespace AIRINV {

  // ////////////////////////////////////////////////////////////////////
  void FlightDateHelper::
  updateBookingControls (stdair::FlightDate& ioFlightDate) {

    // Parse the segment-cabin list and build the pseudo bid price vector.
    const stdair::SegmentDateList_T& lSDList =
      stdair::BomManager::getList<stdair::SegmentDate> (ioFlightDate);
    for (stdair::SegmentDateList_T::const_iterator itSD = lSDList.begin();
         itSD != lSDList.end(); ++itSD) {
      const stdair::SegmentDate* lSD_ptr = *itSD;
      assert (lSD_ptr != NULL);

      //
      const stdair::SegmentCabinList_T& lSCList =
        stdair::BomManager::getList<stdair::SegmentCabin> (*lSD_ptr);
      for (stdair::SegmentCabinList_T::const_iterator itSC = lSCList.begin();
           itSC != lSCList.end(); ++itSC) {
        stdair::SegmentCabin* lSC_ptr = *itSC;
        assert (lSC_ptr != NULL);

        // Build the pseudo bid price vector for the segment-cabin.
        SegmentCabinHelper::buildPseudoBidPriceVector (*lSC_ptr);

        // Update the booking controls using the pseudo bid price vector.
        SegmentCabinHelper::
          updateBookingControlsUsingPseudoBidPriceVector (*lSC_ptr);
      }
    }
  }
  
  // ////////////////////////////////////////////////////////////////////
  void FlightDateHelper::fillFromRouting(const stdair::FlightDate& iFlightDate){
    const stdair::SegmentDateList_T& lSegmentDateList =
      stdair::BomManager::getList<stdair::SegmentDate> (iFlightDate);

    // Browse the list of segment-dates and update each segment-date.
    for (stdair::SegmentDateList_T::const_iterator itSegmentDate =
           lSegmentDateList.begin();
         itSegmentDate != lSegmentDateList.end(); ++itSegmentDate) {
      stdair::SegmentDate* lCurrentSegmentDate_ptr = *itSegmentDate;
      assert (lCurrentSegmentDate_ptr != NULL);
      SegmentDateHelper::fillFromRouting (*lCurrentSegmentDate_ptr);
    }
  }

  // ////////////////////////////////////////////////////////////////////
  void FlightDateHelper::
  updateAvailablityPool (const stdair::FlightDate& iFlightDate,
                         const stdair::CabinCode_T& iCabinCode){
    const stdair::SegmentDateList_T& lSegmentDateList =
      stdair::BomManager::getList<stdair::SegmentDate> (iFlightDate);
    for (stdair::SegmentDateList_T::const_iterator itSegmentDate =
           lSegmentDateList.begin(); itSegmentDate != lSegmentDateList.end();
         ++itSegmentDate) {
      const stdair::SegmentDate* lSegmentDate_ptr = *itSegmentDate;
      assert (lSegmentDate_ptr != NULL);
      stdair::SegmentCabin& lSegmentCabin =
        stdair::BomManager::getObject<stdair::SegmentCabin> (*lSegmentDate_ptr,
                                                             iCabinCode);

      // Update the availability pool of the segment-cabin to the minimal
      // availability pool of the member leg-cabins.
      const stdair::LegCabinList_T& lLegCabinList =
        stdair::BomManager::getList<stdair::LegCabin> (lSegmentCabin);
      stdair::Availability_T lAvailabilityPool = stdair::MAXIMAL_AVAILABILITY;
      for (stdair::LegCabinList_T::const_iterator itLegCabin =
             lLegCabinList.begin();
           itLegCabin != lLegCabinList.end(); ++itLegCabin) {
        const stdair::LegCabin* lLegCabin_ptr = *itLegCabin;
        assert (lLegCabin_ptr != NULL);
        const stdair::Availability_T& lLegCabinAvailabilityPool =
          lLegCabin_ptr->getAvailabilityPool();
        if (lAvailabilityPool > lLegCabinAvailabilityPool) {
          lAvailabilityPool = lLegCabinAvailabilityPool;
        }
      }
      lSegmentCabin.setAvailabilityPool (lAvailabilityPool);
    }
  }

}
