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

        // Choose the current fare option and the current solution
        // if the current fare is lower than the current lowest fare.
        const stdair::Fare_T& lCurrentFare = lFO.getFare();
        const stdair::Availability_T& lCurrentAvl = lFO.getAvailability();
        if (lCurrentFare < lLowestFare && lCurrentAvl >= lPartySize
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
