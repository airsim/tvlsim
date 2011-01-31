#ifndef __SIMFQT_CMD_FAREQUOTER_HPP
#define __SIMFQT_CMD_FAREQUOTER_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// StdAir
#include <stdair/stdair_basic_types.hpp>
#include <stdair/bom/TravelSolutionStruct.hpp>
#include <stdair/bom/TravelSolutionTypes.hpp>
// SimFQT
#include <simfqt/bom/AirportPair.hpp>
#include <simfqt/bom/FarePosChannel.hpp>
#include <simfqt/bom/FareDatePeriod.hpp>
#include <simfqt/bom/FareTimePeriod.hpp>
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
    static void priceQuote (const stdair::SegmentPath_T&,
                            stdair::TravelSolutionList_T&,
                            const stdair::BookingRequestStruct&,
                            const stdair::BomRoot&);
    static void priceQuote (const stdair::SegmentPath_T&,
                            stdair::TravelSolutionList_T&,
                            const stdair::BookingRequestStruct&,
                            const stdair::BomRoot&,
                            stdair::TravelSolutionStruct&,
                            const std::vector<std::string>&);
    static void priceQuote (const stdair::SegmentPath_T&,
                            stdair::TravelSolutionList_T&,
                            const stdair::BookingRequestStruct&,
                            const AirportPair&,
                            stdair::TravelSolutionStruct&,
                            const std::vector<std::string>&);
    static void priceQuote (const stdair::SegmentPath_T&,
                            stdair::TravelSolutionList_T&,
                            const stdair::BookingRequestStruct&,
                            const FarePosChannel&,
                            stdair::TravelSolutionStruct&,
                            const std::vector<std::string>&);
    static void priceQuote (const stdair::SegmentPath_T&,
                            stdair::TravelSolutionList_T&,
                            const stdair::BookingRequestStruct&,
                            const FareDatePeriod&,
                            const FarePosChannel&,
                            stdair::TravelSolutionStruct&,
                            const std::vector<std::string>&);
    static void priceQuote (const stdair::SegmentPath_T&,
                            stdair::TravelSolutionList_T&,
                            const stdair::BookingRequestStruct&,
                            const FareTimePeriod&,
                            const FarePosChannel&,
                            stdair::TravelSolutionStruct&,
                            const std::vector<std::string>&);
    static void priceQuote (const stdair::SegmentPath_T&,
                            stdair::TravelSolutionList_T&,
                            const stdair::BookingRequestStruct&,
                            const FareRuleFeatures&,
                            const FarePosChannel&,
                            stdair::TravelSolutionStruct&,
                            const std::vector<std::string>&,
                            const stdair::PriceValue_T&,
                            const stdair::SaturdayStay_T&,
                            const stdair::ChangeFees_T&,
                            const stdair::NonRefundable_T&);
    

  private:
    /** Constructors. */
    FareQuoter() {}
    FareQuoter(const FareQuoter&) {}
    /** Destructor. */
    ~FareQuoter() {}
  };

}
#endif // __SIMFQT_CMD_FAREQUOTER_HPP

