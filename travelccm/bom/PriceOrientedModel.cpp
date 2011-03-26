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
        const stdair::Fare_T& lFOFare = lFO.getFare();
        const stdair::Availability_T& lFOAvl = lFO.getAvailability();

        if (lFOFare < lLowestFare && lFOFare <= lWTP && lFOAvl >= lPartySize) {

          // DEBUG
          /*
          STDAIR_LOG_DEBUG ("The travel solution (TS) '" << lTS
                            << "' is chosen because its fare (" << lFOFare
                            << ") is lower than the lowest fare (" << lLowestFare
                            << ") and than the WTP (" << lWTP
                            << "), and because the party size (" << lPartySize
                            << ") is lower than the availability (" << lFOAvl
                            << ")");
          */

          lLowestFare = lFOFare;
          oChosenTS_ptr = &lTS;
          oChosenTS_ptr->setChosenFareOption (lFO);

        } else {
          // DEBUG
          /*
          STDAIR_LOG_DEBUG ("The travel solution (TS) '" << lTS
                            << "' is not chosen because either its fare ("
                            << lFOFare << ") is greater than the lowest fare ("
                            << lLowestFare << ") or than the WTP (" << lWTP
                            << "), or because the party size (" << lPartySize
                            << ") is greater than the availability (" << lFOAvl
                            << ")");
          */
        }
      }
    }
    
    return oChosenTS_ptr;
  }

}
