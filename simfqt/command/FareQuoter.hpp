#ifndef __SIMFQT_CMD_FAREQUOTER_HPP
#define __SIMFQT_CMD_FAREQUOTER_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// StdAir
#include <stdair/stdair_basic_types.hpp>
#include <stdair/bom/TravelSolutionStruct.hpp>
#include <stdair/bom/TravelSolutionTypes.hpp>
#include <stdair/bom/AirportPair.hpp>
#include <stdair/bom/PosChannel.hpp>
#include <stdair/bom/DatePeriod.hpp>
#include <stdair/bom/TimePeriod.hpp>
#include <stdair/bom/AirlineClassList.hpp>
// SIMFQT
#include <simfqt/bom/FareRuleFeatures.hpp>

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
    static void priceQuote (const stdair::BookingRequestStruct&,
                            stdair::TravelSolutionStruct&,
                            const stdair::BomRoot&);
    static void priceQuote (const stdair::BookingRequestStruct&,
                            stdair::TravelSolutionStruct&,
                            const stdair::BomRoot&,
                            const std::vector<std::string>&,
                            const std::vector<std::string>&);
    static void priceQuote (const stdair::BookingRequestStruct&,
                            stdair::TravelSolutionStruct&,
                            const stdair::AirportPair&,
                            const std::vector<std::string>&);
    static void priceQuote (const stdair::BookingRequestStruct&,
                            stdair::TravelSolutionStruct&,
                            const stdair::PosChannel&,
                            const std::vector<std::string>&);
    static void priceQuote (const stdair::BookingRequestStruct&,
                            stdair::TravelSolutionStruct&,
                            const stdair::DatePeriod&,
                            const stdair::PosChannel&,
                            const std::vector<std::string>&);
    static void priceQuote (const stdair::BookingRequestStruct&,
                            stdair::TravelSolutionStruct&,
                            const stdair::TimePeriod&,
                            const stdair::PosChannel&,
                            const std::vector<std::string>&);
    static void priceQuote (const stdair::BookingRequestStruct&,
                            stdair::TravelSolutionStruct&,
                            const FareRuleFeatures&,
                            const stdair::PosChannel&,
                            stdair::FareOptionStruct&,
                            const std::vector<std::string>&);
    

  private:
    /** Constructors. */
    FareQuoter() {}
    FareQuoter(const FareQuoter&) {}
    /** Destructor. */
    ~FareQuoter() {}
  };

}
#endif // __SIMFQT_CMD_FAREQUOTER_HPP

