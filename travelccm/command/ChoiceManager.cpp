// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// StdAir
#include <stdair/bom/TravelSolutionStruct.hpp>
#include <stdair/bom/BookingRequestStruct.hpp>
// TravelCCM
#include <travelccm/bom/PriceOrientedModel.hpp>
#include <travelccm/command/ChoiceManager.hpp>

namespace TRAVELCCM {

  // ////////////////////////////////////////////////////////////////////
  const stdair::TravelSolutionStruct* ChoiceManager::
  chooseTravelSolution (stdair::TravelSolutionList_T& ioTravelSolutionList,
                        const stdair::BookingRequestStruct& iBookingRequest) {
    // Return the cheapest solution.
    const stdair::TravelSolutionStruct* oTravelSolution_ptr = 
      PriceOrientedModel::chooseTravelSolution (ioTravelSolutionList,
                                                iBookingRequest);
    return oTravelSolution_ptr;
  }

}
