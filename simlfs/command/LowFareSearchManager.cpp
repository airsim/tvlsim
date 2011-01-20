// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// StdAir
#include <stdair/service/Logger.hpp>
// Simlfs
#include <simlfs/command/LowFareSearchManager.hpp>

namespace SIMLFS {

  // //////////////////////////////////////////////////////////////////////
  stdair::PriceValue_T LowFareSearchManager::
  priceQuote (const stdair::AirlineCode_T& iAirlineCode,
              const stdair::PartySize_T& iPartySize) {
    stdair::PriceValue_T oPrice = 0.0;

    try {

      // DEBUG
      STDAIR_LOG_DEBUG ("The price is " << oPrice
                        << " EUR for the travel request for the airline "
                        << iAirlineCode << " for " << iPartySize
                        << " passengers.");
    
    } catch (const std::exception& lStdError) {
      STDAIR_LOG_ERROR ("Error: " << lStdError.what());
      throw QuotingException();
    }

    return oPrice;
  }

}
