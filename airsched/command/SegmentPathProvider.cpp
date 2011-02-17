// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <string>
#include <sstream>
// StdAir
#include <stdair/bom/BomManager.hpp>
#include <stdair/bom/BomRoot.hpp>
#include <stdair/bom/Inventory.hpp>
#include <stdair/bom/FlightPeriod.hpp>
#include <stdair/bom/SegmentPeriod.hpp>
#include <stdair/bom/BookingRequestStruct.hpp>
#include <stdair/bom/TravelSolutionStruct.hpp>
#include <stdair/service/Logger.hpp>
// AIRSCHED
#include <airsched/bom/ReachableUniverse.hpp>
#include <airsched/bom/OriginDestinationSet.hpp>
#include <airsched/bom/SegmentPathPeriod.hpp>
#include <airsched/command/SegmentPathProvider.hpp>

namespace AIRSCHED {

  // ////////////////////////////////////////////////////////////////////
  void SegmentPathProvider::
  buildSegmentPathList (stdair::TravelSolutionList_T& ioTravelSolutionList,
                        const stdair::BomRoot& iBomRoot,
                        const stdair::BookingRequestStruct& iBookingRequest) {
    // Retrieve  the reachable  universe object  corresponding  to the
    // origin of the booking request.
    const stdair::AirportCode_T& lOrigin = iBookingRequest.getOrigin ();
    const ReachableUniverse* lReachableUniverse_ptr =
      stdair::BomManager::getObjectPtr<ReachableUniverse> (iBomRoot, lOrigin);
    if (lReachableUniverse_ptr != NULL) {
      buildSegmentPathList (ioTravelSolutionList, *lReachableUniverse_ptr,
                            iBookingRequest);
    }
  }

  // ////////////////////////////////////////////////////////////////////
  void SegmentPathProvider::
  buildSegmentPathList (stdair::TravelSolutionList_T& ioTravelSolutionList,
                        const ReachableUniverse& iReachableUniverse,
                        const stdair::BookingRequestStruct& iBookingRequest) {
    // Retrieve the origin-destination set objet correponding to the
    // destination of the booking request.
    const stdair::AirportCode_T& lDestination = iBookingRequest.getDestination();
    const OriginDestinationSet* lOriginDestinationSet_ptr =
      stdair::BomManager::getObjectPtr<OriginDestinationSet> (iReachableUniverse,
                                                              lDestination);
    if (lOriginDestinationSet_ptr != NULL) {
      buildSegmentPathList (ioTravelSolutionList, *lOriginDestinationSet_ptr,
                            iBookingRequest);
    }
  }

  // ////////////////////////////////////////////////////////////////////
  void SegmentPathProvider::
  buildSegmentPathList (stdair::TravelSolutionList_T& ioTravelSolutionList,
                        const OriginDestinationSet& iOriginDestinationSet,
                        const stdair::BookingRequestStruct& iBookingRequest) {
    // Retrieve the departure date of the booking request.
    const stdair::Date_T& lPreferedDepartureDate =
      iBookingRequest.getPreferedDepartureDate ();

    // Browse the list of segment path periods and find those which content
    // the prefered departure date.
    const SegmentPathPeriodList_T& lSegmentPathPeriodList =
      stdair::BomManager::getList<SegmentPathPeriod> (iOriginDestinationSet);
    for (SegmentPathPeriodList_T::const_iterator itSegmentPath =
           lSegmentPathPeriodList.begin ();
         itSegmentPath != lSegmentPathPeriodList.end (); ++itSegmentPath) {
      const SegmentPathPeriod* lCurrentSegmentPath_ptr = *itSegmentPath;
      assert (lCurrentSegmentPath_ptr != NULL);
      if (lCurrentSegmentPath_ptr->isDepartureDateValid(lPreferedDepartureDate)){
        buildSegmentPathList (ioTravelSolutionList, *lCurrentSegmentPath_ptr,
                              iBookingRequest);
      }
    }
  } 

  // ////////////////////////////////////////////////////////////////////
  void SegmentPathProvider::
  buildSegmentPathList (stdair::TravelSolutionList_T& ioTravelSolutionList,
                        const SegmentPathPeriod& iSegmentPathPeriod,
                        const stdair::BookingRequestStruct& iBookingRequest) {
    // Create a new travel solution.
    stdair::TravelSolutionStruct lTravelSolution;
    
    // Browse the list of segments and retrieve the necessary informations
    // for identifying the corresponding segment-date.
    const stdair::Date_T& lPreferedDepartureDate =
      iBookingRequest.getPreferedDepartureDate ();
    const stdair::SegmentPeriodList_T& lSegmentPeriodList =
      stdair::BomManager::getList<stdair::SegmentPeriod> (iSegmentPathPeriod);
    const DateOffsetList_T& lBoardingDateOffsetList =
      iSegmentPathPeriod.getBoardingDateOffsetList ();
    assert (lSegmentPeriodList.size() == lBoardingDateOffsetList.size());
    DateOffsetList_T::const_iterator itOffset = lBoardingDateOffsetList.begin();
    for (stdair::SegmentPeriodList_T::const_iterator itSegment =
           lSegmentPeriodList.begin();
         itSegment != lSegmentPeriodList.end(); ++itSegment) {
      const stdair::SegmentPeriod* lSegmentPeriod_ptr = *itSegment;
      assert (lSegmentPeriod_ptr != NULL);
      const stdair::DateOffset_T& lBoardingDateOffset = *itOffset;

      // Find the corresponding segment-date within the segment period.
      const stdair::DateOffset_T& lSegmentBoardingDateOffset =
        lSegmentPeriod_ptr->getBoardingDateOffset();
      const stdair::Date_T& lReferenceFlightDate = lPreferedDepartureDate 
        + lBoardingDateOffset - lSegmentBoardingDateOffset;

      // Build the whole segment-date key string.
      const stdair::FlightPeriod& lFlightPeriod =
        stdair::BomManager::getParent<stdair::FlightPeriod>(*lSegmentPeriod_ptr);
      const stdair::Inventory& lInventory =
        stdair::BomManager::getParent<stdair::Inventory> (lFlightPeriod);
      const stdair::Duration_T lBoardingTime = lSegmentPeriod_ptr->getBoardingTime();
      std::ostringstream oStr;
      oStr << lInventory.getAirlineCode()
           << ", " << lFlightPeriod.getFlightNumber()
           << ", " << lReferenceFlightDate
           << ", " << lBoardingTime
           << ", " << lSegmentPeriod_ptr->getBoardingPoint()
           << "-" << lSegmentPeriod_ptr->getOffPoint();

      lTravelSolution.addSegment (oStr.str());

      ++itOffset;
    }
    ioTravelSolutionList.push_back (lTravelSolution);
  }

}
