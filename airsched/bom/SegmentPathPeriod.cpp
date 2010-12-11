// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// STDAIR
#include <stdair/basic/BasConst_TravelSolution.hpp>
#include <stdair/bom/Inventory.hpp>
#include <stdair/bom/FlightPeriod.hpp>
#include <stdair/bom/SegmentPeriod.hpp>
#include <stdair/bom/BomManager.hpp>
// AIRSCHED
#include <airsched/bom/SegmentPathPeriod.hpp>

namespace AIRSCHED {

  // ////////////////////////////////////////////////////////////////////
  SegmentPathPeriod::SegmentPathPeriod (const Key_T& iKey)
    :  _key (iKey), _parent (NULL) {
  }
  
  // ////////////////////////////////////////////////////////////////////
  SegmentPathPeriod::~SegmentPathPeriod () {
  }

  // ////////////////////////////////////////////////////////////////////
  std::string SegmentPathPeriod::toString() const {
    std::ostringstream oStr;
    oStr << _key.toString();
    return oStr.str();
  }

  // ////////////////////////////////////////////////////////////////////
  stdair::SegmentPeriod* SegmentPathPeriod::getLastSegmentPeriod () const {
    // Retrieve the last segment of the list
    const stdair::SegmentPeriodList_T& lSegmentPeriodList =
      stdair::BomManager::getList<stdair::SegmentPeriod> (*this);
    stdair::SegmentPeriodList_T::const_reverse_iterator itLastSegment =
      lSegmentPeriodList.rbegin();

    if (itLastSegment == lSegmentPeriodList.rend()) {
      return NULL;
    }
    
    stdair::SegmentPeriod* oSegment_ptr = *itLastSegment;
    assert (oSegment_ptr != NULL);

    return oSegment_ptr;
  }

  // ////////////////////////////////////////////////////////////////////
  stdair::SegmentPeriod* SegmentPathPeriod::getFirstSegmentPeriod () const{
    // Retrieve the first segment of the list
    const stdair::SegmentPeriodList_T& lSegmentPeriodList =
      stdair::BomManager::getList<stdair::SegmentPeriod> (*this);
    stdair::SegmentPeriodList_T::const_iterator itFirstSegment = 
      lSegmentPeriodList.begin();

    if (itFirstSegment == lSegmentPeriodList.end()) {
      return NULL;
    }
    
    stdair::SegmentPeriod* oSegment_ptr = *itFirstSegment;
    assert (oSegment_ptr != NULL);

    return oSegment_ptr;
  }

  // ////////////////////////////////////////////////////////////////////
  const stdair::AirportCode_T& SegmentPathPeriod::getDestination () const {
    const stdair::SegmentPeriod* lLastSegment_ptr = getLastSegmentPeriod();
    assert (lLastSegment_ptr != NULL);
    return lLastSegment_ptr->getOffPoint();
  }  

  // ////////////////////////////////////////////////////////////////////
  bool SegmentPathPeriod::
  isAirlineFlown (const stdair::AirlineCode_T& iAirlineCode) const {
    bool oAirlineFlown = false;

    const stdair::SegmentPeriodList_T& lSegmentPeriodList = 
      stdair::BomManager::getList<stdair::SegmentPeriod> (*this);
    for (stdair::SegmentPeriodList_T::const_iterator itSegmentPeriod =
           lSegmentPeriodList.begin();
         itSegmentPeriod != lSegmentPeriodList.end(); ++itSegmentPeriod) {
      const stdair::SegmentPeriod* lSegmentPeriod_ptr = *itSegmentPeriod;
      assert (lSegmentPeriod_ptr != NULL);

      const stdair::FlightPeriod& lFlightPeriod =
        stdair::BomManager::getParent<stdair::FlightPeriod>(*lSegmentPeriod_ptr);
      const stdair::Inventory& lInventory =
        stdair::BomManager::getParent<stdair::Inventory> (lFlightPeriod);
      const stdair::AirlineCode_T& lSegmentAirlineCode =
        lInventory.getAirlineCode ();
      if (lSegmentAirlineCode == iAirlineCode) {
        oAirlineFlown = true;
        break;
      }
    }

    return oAirlineFlown;
  }

  // ////////////////////////////////////////////////////////////////////
  SegmentPathPeriodKey SegmentPathPeriod::
  connectWithAnotherSegment(const SegmentPathPeriod& iSingleSegmentPath) const {
    SegmentPathPeriodKey oSegmentPathPeriodKey;

    // Retrieve the (only) segment period of the single segment path.
    const stdair::SegmentPeriod* lNextSegmentPeriod_ptr =
      iSingleSegmentPath.getFirstSegmentPeriod ();
    assert (lNextSegmentPeriod_ptr != NULL);

    // Retrive the last segment period of the current segment path and check
    // if the combination of the last segment and the next segment that we
    // want to add to the current segment path will create a new segment
    // (i.e., the two segment period belongs to the same flight number).
    const stdair::SegmentPeriod* lLastSegmentPeriod_ptr = getLastSegmentPeriod ();
    assert (lLastSegmentPeriod_ptr != NULL);
    const stdair::FlightPeriod& lLastFlightPeriod = stdair::BomManager::
      getParent<stdair::FlightPeriod> (*lLastSegmentPeriod_ptr);
    const stdair::Inventory& lLastInventory =
      stdair::BomManager::getParent<stdair::Inventory> (lLastFlightPeriod);
    
    const stdair::FlightPeriod& lNextFlightPeriod = stdair::BomManager::
      getParent<stdair::FlightPeriod> (*lNextSegmentPeriod_ptr);
    const stdair::Inventory& lNextInventory =
      stdair::BomManager::getParent<stdair::Inventory> (lNextFlightPeriod);
    
    if (lLastFlightPeriod.getFlightNumber()==lNextFlightPeriod.getFlightNumber()
        && lLastInventory.getAirlineCode() == lNextInventory.getAirlineCode()) {
      return oSegmentPathPeriodKey;
    }
    
    // Check if the new segment period will create a circle.
    const stdair::AirportCode_T& lDestination =
      lNextSegmentPeriod_ptr->getOffPoint();
    if (checkCircle (lDestination) == true) {
      return oSegmentPathPeriodKey;
    }

    // Check if a passenger can connect from the last segment of the
    // current segment path to the first segment of the to-be-added
    // segment path. If yes, build a new departure period for the new
    // segment path.
    DateOffsetList_T lBoardingDateOffsetList = 
      getBoardingDateOffsetList();
    const stdair::PeriodStruct& lCurrentDeparturePeriod = getDeparturePeriod();
    const stdair::PeriodStruct& lNextDeparturePeriod =
      iSingleSegmentPath.getDeparturePeriod();
    const stdair::Duration_T& lLastOffTime =
      lLastSegmentPeriod_ptr->getOffTime();
    const stdair::Duration_T& lNextBoardingTime =
      lNextSegmentPeriod_ptr->getBoardingTime();
    // If the next boarding time is later than the last off time, check if
    // the passengers will have enough time for the transfer. If the next
    // boarding time is earlier than the last off time, check if the passengers
    // can connect to a flight in the next day.
    if (lNextBoardingTime >= lLastOffTime) {
      const stdair::Duration_T lStopTime = lNextBoardingTime - lLastOffTime;
      if (lStopTime < stdair::DEFAULT_MINIMAL_CONNECTION_TIME) {
        return oSegmentPathPeriodKey;
      } else {
        // Calulcate the date offset of the next segment compare to
        // the first one. In this case, this value is equal to the offset
        // of the off date of the last segment compare to the boarding date
        // of the first segment.
        const stdair::DateOffset_T& lLastBoardingDateOffset =
          lBoardingDateOffsetList.at (getNbOfSegments() - 1);
        const stdair::DateOffset_T lNextBoardingDateOffset =
          lLastBoardingDateOffset + lLastSegmentPeriod_ptr->getOffDateOffset()
          - lLastSegmentPeriod_ptr->getBoardingDateOffset();
        const stdair::DateOffset_T lNegativeNextBoardingDateOffset =
          stdair::DateOffset_T (0) - lNextBoardingDateOffset;

        // Compute the adjusted departure period of the next segment by
        // substracting the origin one with the boarding date offset.
        const stdair::PeriodStruct lAdjustedNextDeparturePeriod =
          lNextDeparturePeriod.addDateOffset (lNegativeNextBoardingDateOffset);

        // Build the intersection of the two periods.
        const stdair::PeriodStruct lNewDeparturePeriod =
          lCurrentDeparturePeriod.intersection (lAdjustedNextDeparturePeriod);
        stdair::Duration_T lNewElapsed = getElapsedTime() + lStopTime +
          lNextSegmentPeriod_ptr->getElapsedTime();
        lBoardingDateOffsetList.push_back (lNextBoardingDateOffset);
        oSegmentPathPeriodKey.setPeriod (lNewDeparturePeriod);
        oSegmentPathPeriodKey.setElapsedTime (lNewElapsed);
      }
    } else {
      const stdair::Duration_T lStopTime = 
        lNextBoardingTime - lLastOffTime + stdair::Duration_T (24, 0, 0);
      if (lStopTime < stdair::DEFAULT_MINIMAL_CONNECTION_TIME) {
        return oSegmentPathPeriodKey;
      } else {
        // Calulcate the date offset of the next segment compare to
        // the first one.
        const stdair::DateOffset_T& lLastBoardingDateOffset =
          lBoardingDateOffsetList.at (getNbOfSegments() - 1);
        const stdair::DateOffset_T lNextBoardingDateOffset =
          lLastBoardingDateOffset + lLastSegmentPeriod_ptr->getOffDateOffset()
          - lLastSegmentPeriod_ptr->getBoardingDateOffset() +
          stdair::DateOffset_T (1);
        const stdair::DateOffset_T lNegativeNextBoardingDateOffset =
          stdair::DateOffset_T (0) - lNextBoardingDateOffset;

        // Compute the adjusted departure period of the next segment by
        // substracting the origin one with the boarding date offset.
        const stdair::PeriodStruct lAdjustedNextDeparturePeriod =
          lNextDeparturePeriod.addDateOffset (lNegativeNextBoardingDateOffset);

        // Build the intersection of the two periods.
        const stdair::PeriodStruct lNewDeparturePeriod =
          lCurrentDeparturePeriod.intersection (lAdjustedNextDeparturePeriod);
        stdair::Duration_T lNewElapsed = getElapsedTime() + lStopTime +
          lNextSegmentPeriod_ptr->getElapsedTime();
        lBoardingDateOffsetList.push_back (lNextBoardingDateOffset);
        oSegmentPathPeriodKey.setPeriod (lNewDeparturePeriod);
        oSegmentPathPeriodKey.setElapsedTime (lNewElapsed);
      }
    }
    
    const stdair::Duration_T& lBoardingTime = getBoardingTime();
    oSegmentPathPeriodKey.setBoardingTime (lBoardingTime);
    oSegmentPathPeriodKey.setBoardingDateOffsetList (lBoardingDateOffsetList);
    
    return oSegmentPathPeriodKey;
  }

  // ////////////////////////////////////////////////////////////////////
  bool SegmentPathPeriod::
  checkCircle (const stdair::AirlineCode_T& iDestination) const {
    const stdair::SegmentPeriodList_T& lSegmentPeriodList =
      stdair::BomManager::getList<stdair::SegmentPeriod> (*this);
    for (stdair::SegmentPeriodList_T::const_iterator itSegment =
           lSegmentPeriodList.begin();
         itSegment != lSegmentPeriodList.end(); ++itSegment) {
      const stdair::SegmentPeriod* lCurrentSegment_ptr = *itSegment;
      assert (lCurrentSegment_ptr != NULL);
      const stdair::AirlineCode_T& lCurrentBoardingPoint =
        lCurrentSegment_ptr->getBoardingPoint();
      if (lCurrentBoardingPoint == iDestination) {
        return true;
      }
    }
    return false;
  }

  // ////////////////////////////////////////////////////////////////////
  bool SegmentPathPeriod::
  isDepartureDateValid (const stdair::Date_T& iDepartureDate) const {
    const stdair::PeriodStruct& lPeriod = getDeparturePeriod ();

    // Check if the departure date is within the date range.
    const stdair::DatePeriod_T& lDeparturePeriod = lPeriod.getDateRange ();
    if (lDeparturePeriod.contains (iDepartureDate) == false) {
      return false;
    }

    // Check if the departure date is valid within the DOW.
    // 0 = Sunday, 1 = Monday, etc.
    const short lDay = iDepartureDate.day_of_week ();
    const stdair::DoWStruct& lDoW = lPeriod.getDoW ();
    if (lDoW.getStandardDayOfWeek (lDay) == false) {
      return false;
    }

    return true;
  }

}
