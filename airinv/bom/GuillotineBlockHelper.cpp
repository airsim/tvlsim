// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// StdAir
#include <stdair/basic/BasConst_Inventory.hpp>
#include <stdair/bom/BomRetriever.hpp>
#include <stdair/bom/BomManager.hpp>
#include <stdair/bom/SegmentDate.hpp>
#include <stdair/bom/SegmentCabin.hpp>
#include <stdair/bom/FareFamily.hpp>
#include <stdair/bom/BookingClass.hpp>
#include <stdair/bom/GuillotineBlock.hpp>
#include <stdair/service/Logger.hpp>
// AirInv
#include <airinv/bom/GuillotineBlockHelper.hpp>
#include <airinv/bom/FlightDateHelper.hpp>
#include <airinv/bom/SegmentCabinHelper.hpp>

namespace AIRINV {

  // ////////////////////////////////////////////////////////////////////
  void GuillotineBlockHelper::
  takeSnapshots (stdair::GuillotineBlock& ioGuillotineBlock,
                 const stdair::DateTime_T& iSnapshotTime) {
    // Retrieve the segment-cabin index and take the snapshots for
    // each segment-cabin.
    const stdair::SegmentCabinIndexMap_T& lSegmentCabinIndexMap =
      ioGuillotineBlock.getSegmentCabinIndexMap();
    for (stdair::SegmentCabinIndexMap_T::const_iterator itSCIdx =
           lSegmentCabinIndexMap.begin();
         itSCIdx != lSegmentCabinIndexMap.end(); ++itSCIdx) {
      const stdair::SegmentCabin* lSC_ptr = itSCIdx->first;
      assert (lSC_ptr != NULL);
      const stdair::BlockNumber_T& lSCIdx = itSCIdx->second;

      const stdair::Date_T& lSnapshotDate = iSnapshotTime.date();
      takeSnapshots (ioGuillotineBlock, lSnapshotDate, *lSC_ptr, lSCIdx);
    }
  }

  // ////////////////////////////////////////////////////////////////////
  void GuillotineBlockHelper::
  takeSnapshots (stdair::GuillotineBlock& ioGuillotineBlock,
                 const stdair::Date_T& iSnapshotDate,
                 const stdair::SegmentCabin& iSegmentCabin,
                 const stdair::BlockNumber_T iSegmentCabinIdx) {
    // Compare the date of the snapshot time and the departure date of
    // the segment-cabin in order to verify the necescity of taking snapshots.
    const stdair::SegmentDate& lSegmentDate =
      stdair::BomManager::getParent<stdair::SegmentDate> (iSegmentCabin);
    const stdair::Date_T& lDepartureDate = lSegmentDate.getBoardingDate();
    const stdair::DateOffset_T lDateOffset = lDepartureDate - iSnapshotDate;
    const stdair::DTD_T lDTD = lDateOffset.days();

    if (lDTD >= 0 && lDTD <= stdair::DEFAULT_MAX_DTD) {
      // Retrieve the value type index map.
      const stdair::ValueTypeIndexMap_T& lValueTypeIndexMap =
        ioGuillotineBlock.getValueTypeIndexMap();

      // Extract the view for the corresponding DTD and segment-cabin.
      stdair::SegmentCabinDTDSnapshotView_T lSegmentCabinDTDBookingView =
        ioGuillotineBlock.getSegmentCabinDTDBookingSnapshotView (iSegmentCabinIdx, iSegmentCabinIdx, lDTD);

      // Browse the fare family list
      const stdair::FareFamilyList_T& lFFList =
        stdair::BomManager::getList<stdair::FareFamily> (iSegmentCabin);
      for (stdair::FareFamilyList_T::const_iterator itFF = lFFList.begin();
           itFF != lFFList.end(); ++itFF) {
        const stdair::FareFamily* lFareFamily_ptr = *itFF;
        assert (lFareFamily_ptr != NULL);

        // Browse the booking class list
        const stdair::BookingClassList_T& lBCList =
          stdair::BomManager::getList<stdair::BookingClass>(*lFareFamily_ptr);
        for (stdair::BookingClassList_T::const_iterator itBC= lBCList.begin();
             itBC != lBCList.end(); ++itBC) {
          const stdair::BookingClass* lBookingClass_ptr = *itBC;
          assert (lBookingClass_ptr != NULL);

          // Retrieve the block index of the booking class.
          stdair::ValueTypeIndexMap_T::const_iterator itVTIdx =
            lValueTypeIndexMap.find (lBookingClass_ptr->describeKey());
          assert (itVTIdx != lValueTypeIndexMap.end());

          const stdair::BlockIndex_T lIdx = itVTIdx->second;

          // DEBUG
          STDAIR_LOG_DEBUG ("Taking snapshot for "
                            << iSegmentCabin.describeKey() << ", "
                            << lBookingClass_ptr->describeKey()
                            << ", DTD: " << lDTD << ", nb of bookings: "
                            << lBookingClass_ptr->getNbOfBookings());

          // Write the snapshot.
          lSegmentCabinDTDBookingView[lIdx] =
            lBookingClass_ptr->getNbOfBookings();
        }
        // TODO: equivalent bookings for FF.
      }
    }
  }
}
