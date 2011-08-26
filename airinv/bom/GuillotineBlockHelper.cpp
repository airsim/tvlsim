// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <cmath>
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
#include <airinv/basic/BasConst_Curves.hpp>
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
      
      // Compare the date of the snapshot time and the departure date of
      // the segment-cabin in order to verify the necescity of taking snapshots.
      const stdair::SegmentDate& lSegmentDate =
        stdair::BomManager::getParent<stdair::SegmentDate> (*lSC_ptr);
      const stdair::Date_T& lDepartureDate = lSegmentDate.getBoardingDate();
      const stdair::DateOffset_T lDateOffset = lDepartureDate - lSnapshotDate;
      const stdair::DTD_T lDTD = lDateOffset.days() + 1;
      
      if (lDTD >= 0 && lDTD <= stdair::DEFAULT_MAX_DTD) {
        SegmentCabinHelper::updateAvailabilities (*lSC_ptr);
        takeSnapshots (ioGuillotineBlock, lDTD, *lSC_ptr, lSCIdx);
        registerProductAndPriceOrientedBookings (ioGuillotineBlock,
                                                 lDTD, *lSC_ptr, lSCIdx);
      }
    }
  }

  // ////////////////////////////////////////////////////////////////////
  void GuillotineBlockHelper::
  takeSnapshots (stdair::GuillotineBlock& ioGuillotineBlock,
                 const stdair::DTD_T& iDTD,
                 const stdair::SegmentCabin& iSegmentCabin,
                 const stdair::BlockNumber_T iSegmentCabinIdx) {

    // Extract the views for the corresponding DTD and segment-cabin.
    stdair::SegmentCabinDTDSnapshotView_T lBookingView = ioGuillotineBlock.
      getSegmentCabinDTDBookingSnapshotView (iSegmentCabinIdx,
                                             iSegmentCabinIdx, iDTD);
    stdair::SegmentCabinDTDSnapshotView_T lCancellationView = ioGuillotineBlock.
      getSegmentCabinDTDCancellationSnapshotView (iSegmentCabinIdx,
                                                  iSegmentCabinIdx, iDTD);
    stdair::SegmentCabinDTDSnapshotView_T lAvailabilityView = ioGuillotineBlock.
      getSegmentCabinDTDAvailabilitySnapshotView (iSegmentCabinIdx,
                                                  iSegmentCabinIdx, iDTD);
    
    // Retrieve the block index of the segment-cabin.
    std::ostringstream lSCMapKey;
    lSCMapKey << stdair::DEFAULT_SEGMENT_CABIN_VALUE_TYPE
              << iSegmentCabin.describeKey();    
    const stdair::BlockIndex_T& lCabinIdx =
      ioGuillotineBlock.getBlockIndex (lSCMapKey.str());
    lAvailabilityView[lCabinIdx] = iSegmentCabin.getAvailabilityPool();
    

    // Browse the booking class list
    const stdair::BookingClassList_T& lBCList =
      stdair::BomManager::getList<stdair::BookingClass> (iSegmentCabin);
    for (stdair::BookingClassList_T::const_iterator itBC = lBCList.begin();
         itBC != lBCList.end(); ++itBC) {
      const stdair::BookingClass* lBookingClass_ptr = *itBC;
      assert (lBookingClass_ptr != NULL);

      // Retrieve the block index of the booking class.
      const stdair::BlockIndex_T& lIdx =
        ioGuillotineBlock.getBlockIndex (lBookingClass_ptr->describeKey());

      // DEBUG
      // STDAIR_LOG_DEBUG ("Taking snapshot for "
      //                   << iSegmentCabin.describeKey() << ", "
      //                   << lBookingClass_ptr->describeKey()
      //                   << ", DTD: " << iDTD << ", nb of bookings: "
      //                   << lBookingClass_ptr->getNbOfBookings());

      // Write the snapshot.
      lBookingView[lIdx]=lBookingClass_ptr->getNbOfBookings();
      lCancellationView[lIdx] =
        lBookingClass_ptr->getNbOfCancellations();
      lAvailabilityView[lIdx] =
        lBookingClass_ptr->getSegmentAvailability();
    }
  }

  // ////////////////////////////////////////////////////////////////////
  void GuillotineBlockHelper::registerProductAndPriceOrientedBookings
  (stdair::GuillotineBlock& ioGuillotineBlock, const stdair::DTD_T& iDTD,
   const stdair::SegmentCabin& iSegmentCabin,
   const stdair::BlockNumber_T iSegmentCabinIdx) {

    // Extract the views for the corresponding DTD and segment-cabin.
    stdair::SegmentCabinDTDRangeSnapshotView_T lRangeBookingView =
      ioGuillotineBlock.getSegmentCabinDTDRangeBookingSnapshotView (iSegmentCabinIdx, iSegmentCabinIdx, iDTD, iDTD + 1);
    stdair::SegmentCabinDTDRangeSnapshotView_T lRangeCancellationView =
      ioGuillotineBlock.getSegmentCabinDTDRangeCancellationSnapshotView (iSegmentCabinIdx, iSegmentCabinIdx, iDTD, iDTD + 1);
    stdair::SegmentCabinDTDSnapshotView_T lProductAndPriceOrientedBookingView =
      ioGuillotineBlock.getSegmentCabinDTDProductAndPriceOrientedBookingSnapshotView (iSegmentCabinIdx, iSegmentCabinIdx, iDTD);
    
    // Retrieve the block index of the segment-cabin.
    std::ostringstream lSCMapKey;
    lSCMapKey << stdair::DEFAULT_SEGMENT_CABIN_VALUE_TYPE
              << iSegmentCabin.describeKey();    
    const stdair::BlockIndex_T& lCabinIdx =
      ioGuillotineBlock.getBlockIndex (lSCMapKey.str());

    // Retrieve the lowest class and treat the number of gross
    // bookings of this class the price oriented bookings.
    const stdair::BookingClassList_T& lBCList =
      stdair::BomManager::getList<stdair::BookingClass> (iSegmentCabin);
    stdair::BookingClassList_T::const_reverse_iterator itBC = lBCList.rbegin();
    assert (itBC != lBCList.rend());
    stdair::BookingClass* lLowestClass_ptr = *itBC; ++itBC;
    assert (lLowestClass_ptr != NULL);

    // Retrieve the block index of the booking class.
    const stdair::BlockIndex_T& lClassIdx =
      ioGuillotineBlock.getBlockIndex (lLowestClass_ptr->describeKey());

    // Compute the number of gross bookings for this class.
    const stdair::NbOfBookings_T lNbOfNetBkgs = 
      lRangeBookingView[lClassIdx][0] - lRangeBookingView[lClassIdx][1];
    const stdair::NbOfCancellations_T lNbOfCx =
      lRangeCancellationView[lClassIdx][0]-lRangeCancellationView[lClassIdx][1];
    const stdair::NbOfBookings_T lNbOfGrossBkgs = lNbOfNetBkgs + lNbOfCx;

    // Write this number of bookings to the price-oriented value.
    lProductAndPriceOrientedBookingView[lCabinIdx] = lNbOfGrossBkgs;

    // Retrieve the lowest yield.
    const stdair::Yield_T& lLowestYield = lLowestClass_ptr->getYield();
    
    // Boolean for "no lower class available" verification.
    bool noLowerClassAvl = true;
    if (lLowestClass_ptr->getSegmentAvailability() >= 1.0) {
      noLowerClassAvl = false;
    }

    // Retrieve the FRAT5 coefficient and compute the sell-up coef.
    const double lFRAT5Coef = getFRAT5Coefficient (iDTD);
    const double lSellUpCoef = -log(0.5) / (lFRAT5Coef - 1); 
    
    // Browse the booking class list
    for (; itBC != lBCList.rend(); ++itBC) {
      const stdair::BookingClass* lBookingClass_ptr = *itBC;
      assert (lBookingClass_ptr != NULL);

      // Retrieve the yield of the this class.
      const stdair::Yield_T& lYield = lBookingClass_ptr->getYield();
      assert (lYield > lLowestYield);
      
      // Retrieve the block index of the booking class.
      const stdair::BlockIndex_T& lIdx =
        ioGuillotineBlock.getBlockIndex (lBookingClass_ptr->describeKey());

      // Compute the number of gross bookings for this class.
      const stdair::NbOfBookings_T lNetBkgs = 
        lRangeBookingView[lIdx][0] - lRangeBookingView[lIdx][1];
      const stdair::NbOfCancellations_T lCx =
        lRangeCancellationView[lIdx][0] - lRangeCancellationView[lIdx][1];
      const stdair::NbOfBookings_T lGrossBkgs = lNetBkgs + lCx;
      
      // If there is a lower class available, these gross bookings
      // will be considered product-oriented. Otherwise, they will be
      // considered price-oriented
      if (noLowerClassAvl == false) {
        lProductAndPriceOrientedBookingView[lIdx] = lGrossBkgs;
      } else {
        // Convert the bookings to Q-equivalent bookings.
        const stdair::NbOfBookings_T lQEquiBkgs =
          lGrossBkgs / exp ((1.0 - lYield/lLowestYield) * lSellUpCoef);
        lProductAndPriceOrientedBookingView[lCabinIdx] += lQEquiBkgs;
        
        if (lBookingClass_ptr->getSegmentAvailability() >= 1.0) {
          noLowerClassAvl = false;
        }
      }
    }
  }

  // ////////////////////////////////////////////////////////////////////
  double GuillotineBlockHelper::getFRAT5Coefficient (const stdair::DTD_T& iDTD){
    FRAT5Curve_T::const_iterator itFRAT5 =
      DEFAULT_PICKUP_FRAT5_CURVE.lower_bound (iDTD);
    assert (itFRAT5 != DEFAULT_PICKUP_FRAT5_CURVE.end());

    if (itFRAT5 == DEFAULT_PICKUP_FRAT5_CURVE.begin()) {
      return itFRAT5->second;
    }
    
    assert (itFRAT5 != DEFAULT_PICKUP_FRAT5_CURVE.begin());
    FRAT5Curve_T::const_iterator itNextFRAT5 = itFRAT5; --itNextFRAT5;

    const stdair::DTD_T& lPrevDTD = itFRAT5->first;
    const stdair::DTD_T& lNextDTD = itNextFRAT5->first;
    const double& lPrevFRAT5 = itFRAT5->second;
    const double& lNextFRAT5 = itNextFRAT5->second;
    assert (lPrevDTD > lNextDTD);

    double oFRAT5 = lPrevFRAT5
      + (iDTD - lNextDTD) * (lNextFRAT5 - lPrevFRAT5) / (lPrevDTD - lNextDTD);

    return oFRAT5;
  }
}
