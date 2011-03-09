// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// STDAIR
#include <stdair/basic/BasConst_General.hpp>
#include <stdair/bom/BomManager.hpp>
#include <stdair/bom/SegmentDate.hpp>
#include <stdair/bom/SegmentCabin.hpp>
#include <stdair/bom/LegDate.hpp>
// AIRINV
#include <airinv/bom/SegmentDateHelper.hpp>
#include <airinv/bom/SegmentCabinHelper.hpp>

namespace AIRINV {
  // ////////////////////////////////////////////////////////////////////
  void SegmentDateHelper::fillFromRouting (stdair::SegmentDate& ioSegmentDate) {
    // Retrieve the first and the last legs of the routing.
    // Note that in the majority of the cases, as flights are mono-legs,
    // the first and last legs are thus the same.
    const stdair::LegDateList_T& lLegDateList =
      stdair::BomManager::getList<stdair::LegDate> (ioSegmentDate);
    stdair::LegDateList_T::const_iterator itFirstLeg = lLegDateList.begin();
    const stdair::LegDate* lFirstLeg_ptr = *itFirstLeg;
    assert (lFirstLeg_ptr != NULL);
    stdair::LegDateList_T::const_reverse_iterator itLastLeg =
      lLegDateList.rbegin();
    const stdair::LegDate* lLastLeg_ptr = *itLastLeg;
    assert (lLastLeg_ptr != NULL);
    
    // Set the Boarding Date
    const stdair::Date_T& lBoardingDate = lFirstLeg_ptr->getBoardingDate();
    ioSegmentDate.setBoardingDate (lBoardingDate);
    // Set the Boarding Time
    const stdair::Duration_T& lBoardingTime = lFirstLeg_ptr->getBoardingTime();
    ioSegmentDate.setBoardingTime (lBoardingTime);
    // Set the Off Date
    const stdair::Date_T& lOffDate = lLastLeg_ptr->getOffDate();
    ioSegmentDate.setOffDate (lOffDate);
    // Set the Off Time
    const stdair::Duration_T& lOffTime = lLastLeg_ptr->getOffTime();
    ioSegmentDate.setOffTime (lOffTime);
    // Set the Elapsed Time for the whole path
    updateElapsedTimeFromRouting (ioSegmentDate);

    // Initialise the AU for all classes.
    const stdair::SegmentCabinList_T& lSegmentCabinList =
      stdair::BomManager::getList<stdair::SegmentCabin> (ioSegmentDate);
    for (stdair::SegmentCabinList_T::const_iterator itSC =
           lSegmentCabinList.begin(); itSC != lSegmentCabinList.end(); ++itSC) {
      stdair::SegmentCabin* lSC_ptr = *itSC;
      assert (lSC_ptr != NULL);
      
      // Initialise the AU for children booking classes.
      SegmentCabinHelper::initialiseAU (*lSC_ptr);
    }
  }

  // //////////////////////////////////////////////////////////////////////
  void SegmentDateHelper::
  updateElapsedTimeFromRouting (stdair::SegmentDate& ioSegmentDate) {

    const stdair::LegDateList_T& lLegDateList =
      stdair::BomManager::getList<stdair::LegDate> (ioSegmentDate);
      
    stdair::LegDateList_T::const_iterator itLegDate = lLegDateList.begin();
    const stdair::LegDate* lCurrentLegDate_ptr = *itLegDate;
    assert (lCurrentLegDate_ptr != NULL);

    // Retrieve the elapsed time of the first leg
    stdair::Duration_T lElapsedTime = lCurrentLegDate_ptr->getElapsedTime();

    // Go to the next leg, if existing. If not existing, the following
    // loop will not be entered (as it means: currentLeg == _legDateList.end()).
    ++itLegDate;

    for (const stdair::LegDate* lPreviousLegDate_ptr = lCurrentLegDate_ptr;
         itLegDate != lLegDateList.end();
         ++itLegDate, lPreviousLegDate_ptr = lCurrentLegDate_ptr) {
      lCurrentLegDate_ptr = *itLegDate;
      
      // As the boarding point of the current leg is the same as the off point
      // of the previous leg (by construction), there is no time difference.
      assert (lCurrentLegDate_ptr->getBoardingPoint()
              == lPreviousLegDate_ptr->getOffPoint());
      const stdair::Duration_T& lStopOverTime =
        lCurrentLegDate_ptr->getBoardingTime() - lPreviousLegDate_ptr->getOffTime();
      lElapsedTime += lStopOverTime;

      // Add the elapsed time of the current leg
      const stdair::Duration_T& currentElapsedTime =
        lCurrentLegDate_ptr->getElapsedTime();
      lElapsedTime += currentElapsedTime;
    }
      
    // Store the result
    ioSegmentDate.setElapsedTime (lElapsedTime);
    // From the elapsed time, update the distance
    updateDistanceFromElapsedTime (ioSegmentDate);
  }

  // //////////////////////////////////////////////////////////////////////
  void SegmentDateHelper::
  updateDistanceFromElapsedTime (stdair::SegmentDate& ioSegmentDate) {
    const stdair::Duration_T& lElapsedTime = ioSegmentDate.getElapsedTime();
    const double lElapseInHours=static_cast<const double>(lElapsedTime.hours());
    const long int lDistance =
      static_cast<const long int>(stdair::DEFAULT_FLIGHT_SPEED*lElapseInHours);
    ioSegmentDate.setDistance (lDistance);
  }

}
