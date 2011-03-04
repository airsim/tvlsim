// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <fstream>
// StdAir
#include <stdair/service/Logger.hpp>
// TRAVEL-CCM
#include <travelccm/bom/PriceOrientedModel.hpp>
#include <travelccm/command/ChoiceManager.hpp>

namespace TRAVELCCM {
  // ////////////////////////////////////////////////////////////////////
  const stdair::TravelSolutionStruct* ChoiceManager::
  chooseTravelSolution (stdair::TravelSolutionList_T& ioTravelSolutionList,
                        const stdair::BookingRequestStruct& iBookingRequest) {
    // Return the cheapest solution.
    return PriceOrientedModel::chooseTravelSolution (ioTravelSolutionList,
                                                     iBookingRequest);
  }

}
