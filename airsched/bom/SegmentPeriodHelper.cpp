// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// STDAIR
#include <stdair/basic/BasConst_General.hpp>
#include <stdair/bom/SegmentPeriod.hpp>
// AIRSCHED
#include <airsched/bom/SegmentPeriodHelper.hpp>

namespace AIRSCHED {
  // ////////////////////////////////////////////////////////////////////
  void SegmentPeriodHelper::fill (stdair::SegmentPeriod& ioSegmentPeriod,
                                  const SegmentStruct& iSegmentStruct) {
    // Browse the list of segment cabins and fill the cabin booking
    // class map of the BOM segment period.
    for (SegmentCabinStructList_T::const_iterator itCabin =
           iSegmentStruct._cabinList.begin();
         itCabin != iSegmentStruct._cabinList.end(); ++itCabin) {
      const SegmentCabinStruct& lSegmentCabinStruct = *itCabin;
      ioSegmentPeriod.
        addCabinBookingClassList (lSegmentCabinStruct._cabinCode,
                                  lSegmentCabinStruct._classes);
    }
  }

  // ////////////////////////////////////////////////////////////////////
  void SegmentPeriodHelper::fill (stdair::SegmentPeriod& ioSegmentPeriod,
                                  const LegStructList_T& iLegList) {

    const stdair::AirportCode_T& lBoardingPoint =
      ioSegmentPeriod.getBoardingPoint ();
    const stdair::AirportCode_T& lOffPoint = ioSegmentPeriod.getOffPoint();
    stdair::Duration_T lElapsedTime;

    // Find the leg which has the same boarding point.
    LegStructList_T::const_iterator itLeg = iLegList.begin ();
    while (itLeg != iLegList.end()) {
      const LegStruct& lLeg = *itLeg;
      if (lLeg._boardingPoint == lBoardingPoint) {
        break;
      } else {
        ++itLeg;
      }
    }
    assert (itLeg != iLegList.end());
    const LegStruct& lFirstLeg = *itLeg;
    stdair::AirportCode_T lCurrentOffPoint = lFirstLeg._offPoint;
    stdair::Duration_T lCurrentOffTime = lFirstLeg._offTime;
    
    // Update the elapsed time.
    lElapsedTime += lFirstLeg._elapsed;

    // Find the last used leg.
    while (lCurrentOffPoint != lOffPoint) {
      ++itLeg;
      assert (itLeg != iLegList.end());
      
      const LegStruct& lCurrentLeg = *itLeg;
      assert (lCurrentOffPoint == lCurrentLeg._boardingPoint);
      // As the boarding point of the current leg is the same as the off point
      // of the previous leg (by construction), there is no time difference.
      const stdair::Duration_T lStopOverTime =
        lCurrentLeg._boardingTime - lCurrentOffTime;
      lElapsedTime += lStopOverTime;

      // Add the elapsed time of the current leg
      lElapsedTime += lCurrentLeg._elapsed;

      lCurrentOffTime = lCurrentLeg._offTime;
      lCurrentOffPoint = lCurrentLeg._offPoint;
    }
    const LegStruct& lLastLeg = *itLeg;

    // Update the attributes of the segment-period.
    ioSegmentPeriod.setBoardingTime (lFirstLeg._boardingTime);
    ioSegmentPeriod.setOffTime (lLastLeg._offTime);
    ioSegmentPeriod.setBoardingDateOffset (lFirstLeg._boardingDateOffset);
    ioSegmentPeriod.setOffDateOffset (lLastLeg._offDateOffset);
    ioSegmentPeriod.setElapsedTime (lElapsedTime);
  }

}
