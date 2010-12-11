#ifndef __SIMFQT_CMD_FAREQUOTER_HPP
#define __SIMFQT_CMD_FAREQUOTER_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// StdAir
#include <stdair/stdair_basic_types.hpp>
// Simfqt
#include <simfqt/SIMFQT_Types.hpp>

namespace SIMFQT {

  /** Command wrapping the pricing request process. */
  class FareQuoter {
    friend class SIMFQT_Service;
  private:

    /** Quote (with a price) a travel request. */
    static Price_T priceQuote (const FareQuoteID_T&,
                               const stdair::AirlineCode_T&, const PartySize_T&);

  private:
    /** Constructors. */
    FareQuoter() {}
    FareQuoter(const FareQuoter&) {}
    /** Destructor. */
    ~FareQuoter() {}
  };

}
#endif // __SIMFQT_CMD_FAREQUOTER_HPP
