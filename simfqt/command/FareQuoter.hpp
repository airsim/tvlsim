#ifndef __SIMFQT_CMD_FAREQUOTER_HPP
#define __SIMFQT_CMD_FAREQUOTER_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// StdAir
#include <stdair/stdair_basic_types.hpp>
#include <stdair/bom/TravelSolutionStruct.hpp>
#include <stdair/bom/TravelSolutionTypes.hpp>

// Simfqt
#include <simfqt/SIMFQT_Types.hpp>

// Forward declarations
namespace stdair {
  class BomRoot;
  struct BookingRequestStruct;
}

namespace SIMFQT {

  /** Command wrapping the pricing request process. */
  class FareQuoter {
    friend class SIMFQT_Service;
  private:

    /** Quote (with a price) a travel request. */
    static void priceQuote (const stdair::SegmentPath_T&,
                            stdair::TravelSolutionList_T&,
                            const stdair::BookingRequestStruct&,
                            const stdair::BomRoot&);


  private:
    /** Constructors. */
    FareQuoter() {}
    FareQuoter(const FareQuoter&) {}
    /** Destructor. */
    ~FareQuoter() {}
  };

}
#endif // __SIMFQT_CMD_FAREQUOTER_HPP

