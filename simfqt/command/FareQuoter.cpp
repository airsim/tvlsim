// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// StdAir
#include <stdair/service/Logger.hpp>
// Simfqt
#include <simfqt/command/FareQuoter.hpp>

namespace SIMFQT {

  // //////////////////////////////////////////////////////////////////////
  Price_T FareQuoter::
  priceQuote (const FareQuoteID_T& iFareQuoteID,
              const stdair::AirlineCode_T& iAirlineCode, const PartySize_T& iPartySize) {
    Price_T oPrice = 0.0;

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
