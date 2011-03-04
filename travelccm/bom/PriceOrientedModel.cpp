// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <fstream>
// StdAir
#include <stdair/stdair_types.hpp>
#include <stdair/bom/BookingRequestStruct.hpp>
#include <stdair/bom/TravelSolutionStruct.hpp>
#include <stdair/bom/FareOptionStruct.hpp>
#include <stdair/service/Logger.hpp>
// TRAVEL-CCM
#include <travelccm/bom/PriceOrientedModel.hpp>

namespace TRAVELCCM {
  // ////////////////////////////////////////////////////////////////////
  const stdair::TravelSolutionStruct* PriceOrientedModel::
  chooseTravelSolution (stdair::TravelSolutionList_T& ioTravelSolutionList,
                        const stdair::BookingRequestStruct& iBookingRequest) {
    stdair::TravelSolutionStruct* oChosenTS_ptr = NULL;

    // Get the willingness-to-pay of the customer
    const stdair::WTP_T& lWTP = iBookingRequest.getWTP();

    stdair::Fare_T lLowestFare = std::numeric_limits<stdair::Fare_T>::max();
    // Browse the travel solution list and choose the cheapest one.
    for (stdair::TravelSolutionList_T::iterator itTS =
           ioTravelSolutionList.begin(); itTS != ioTravelSolutionList.end();
         ++itTS) {
      stdair::TravelSolutionStruct& lCurrentTS = *itTS;

      const stdair::FareOptionList_T& lFareOptionList =
        lCurrentTS.getFareOptionList();
      for (stdair::FareOptionList_T::const_iterator itFO =
             lFareOptionList.begin(); itFO != lFareOptionList.end(); ++itFO) {
        const stdair::FareOptionStruct& lCurrentFO = *itFO;

        // Choose the current fare option and the current solution
        // if the current fare is lower than the current lowest fare.
        const stdair::Fare_T& lCurrentFare = lCurrentFO.getFare();
        if (lCurrentFare < lLowestFare) {
          lLowestFare = lCurrentFare;
          oChosenTS_ptr = &lCurrentTS;
          oChosenTS_ptr->setChosenFareOption (lCurrentFO);
        }
      }
    }
    
    return oChosenTS_ptr;
  }

}
