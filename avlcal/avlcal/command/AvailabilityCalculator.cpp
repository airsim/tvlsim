// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// StdAir
#include <stdair/service/Logger.hpp>
// Avlcal
#include <avlcal/command/AvailabilityCalculator.hpp>

namespace AVLCAL {

  // //////////////////////////////////////////////////////////////////////
  void AvailabilityCalculator::avlCalculate (const AirlineCode_T& iAirlineCode,
                                             const PartySize_T& iPartySize) {

    try {

      // DEBUG
      STDAIR_LOG_DEBUG ("An availability calculation has been performed "
                        << "for the airline " << iAirlineCode
                        << " for " << iPartySize << " passengers.");
    
    } catch (const std::exception& lStdError) {
      STDAIR_LOG_ERROR ("Error: " << lStdError.what());
      throw AvlCalcultationException();
    }
  }

}
