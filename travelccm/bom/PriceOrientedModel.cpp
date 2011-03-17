// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <sstream>
// StdAir
#include <stdair/bom/BomKeyManager.hpp>
#include <stdair/bom/BookingClassKey.hpp>
#include <stdair/bom/BookingRequestStruct.hpp>
#include <stdair/bom/TravelSolutionStruct.hpp>
#include <stdair/bom/FareOptionStruct.hpp>
#include <stdair/service/Logger.hpp>
// TravelCCM
#include <travelccm/bom/PriceOrientedModel.hpp>

namespace TRAVELCCM {

  // ////////////////////////////////////////////////////////////////////
  const stdair::TravelSolutionStruct* PriceOrientedModel::
  chooseTravelSolution (stdair::TravelSolutionList_T& ioTSList,
                        const stdair::BookingRequestStruct& iBookingRequest) {
    stdair::TravelSolutionStruct* oChosenTS_ptr = NULL;

    // Retrieve the number of passengers
    const stdair::NbOfSeats_T& lPartySize = iBookingRequest.getPartySize();

    // Retrieve the Willingness-to-Pay (WTP) of the customer
    const stdair::WTP_T& lWTP = iBookingRequest.getWTP();

    // Browse the travel solution list and choose the cheapest one
    stdair::Fare_T lLowestFare = std::numeric_limits<stdair::Fare_T>::max();
    for (stdair::TravelSolutionList_T::iterator itTS = ioTSList.begin();
         itTS != ioTSList.end(); ++itTS) {
      stdair::TravelSolutionStruct& lTS = *itTS;

      // Browse the fare options
      const stdair::FareOptionList_T& lFOList = lTS.getFareOptionList();
      for (stdair::FareOptionList_T::const_iterator itFO = lFOList.begin();
           itFO != lFOList.end(); ++itFO) {
        const stdair::FareOptionStruct& lFO = *itFO;

        // Check the availability
        const stdair::ClassList_StringList_T& lClassPath = lFO.getClassPath();

        const stdair::ClassAvailabilityMapHolder_T& lClassAvailabilityMapHolder =
          lTS.getClassAvailabilityMapHolder();

        // Initialise the flag stating whether the availability is enough
        bool isAvlEnough = true;

        // Sanity check: the travel solution must contain two lists,
        // one for the booking class availabilities, the other for the
        // fare options.
        assert (lClassAvailabilityMapHolder.empty() == false
                && lClassPath.empty() == false);

        // List of booking class availability maps (one map per segment)
        stdair::ClassAvailabilityMapHolder_T::const_iterator itCAMH =
          lClassAvailabilityMapHolder.begin();

        // List of fare options
        stdair::ClassList_StringList_T::const_iterator itClassList =
          lClassPath.begin();

        // Browse both lists at the same time, i.e., one element per segment
        for (; itCAMH != lClassAvailabilityMapHolder.end()
               && itClassList != lClassPath.end(); ++itCAMH, ++itClassList) {

          // Retrieve the booking class list for the current segment
          const stdair::ClassList_String_T& lCurrentClassList = *itClassList;
          assert (lCurrentClassList.size() > 0);
          
          // TODO: instead of just extracting the first booking class,
          //       perform a choice on the full list of classes.
          // Extract one booking class key (class code)
          stdair::ClassCode_T lFirstClass;
          lFirstClass.append (lCurrentClassList, 0, 1);

          // Retrieve the booking class map for the current segment
          const stdair::ClassAvailabilityMap_T& lClassAvlMap = *itCAMH;

          // Retrieve the availability of the chosen booking class
          const stdair::ClassAvailabilityMap_T::const_iterator itClassAvl =
            lClassAvlMap.find (lFirstClass);

          if (itClassAvl == lClassAvlMap.end()) {
            // DEBUG
            STDAIR_LOG_DEBUG ("No availability has been set up for the class '"
                              << lFirstClass << "'. Travel solution: "
                              << lTS.display());
          }
          assert (itClassAvl != lClassAvlMap.end());

          const stdair::Availability_T& lAvl = itClassAvl->second;
          if (lAvl < lPartySize) {
            isAvlEnough = false;
          }
        }

        // Choose the current fare option and the current solution
        // if the current fare is lower than the current lowest fare.
        const stdair::Fare_T& lCurrentFare = lFO.getFare();
        if (lCurrentFare < lLowestFare && isAvlEnough == true
            && lCurrentFare <= lWTP) {
          lLowestFare = lCurrentFare;
          oChosenTS_ptr = &lTS;
          oChosenTS_ptr->setChosenFareOption (lFO);
        }
      }
    }
    
    return oChosenTS_ptr;
  }

}
