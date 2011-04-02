#ifndef __SIMFQT_CMD_FAREQUOTER_HPP
#define __SIMFQT_CMD_FAREQUOTER_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// StdAir
#include <stdair/stdair_basic_types.hpp>
#include <stdair/bom/TravelSolutionTypes.hpp>

/// Forward declarations
namespace stdair {
  class BomRoot;
  struct BookingRequestStruct;
  struct TravelSolutionStruct;
  struct ParsedKey;
  class AirportPair;
  class PosChannel;
  class DatePeriod;
  class TimePeriod;
  class FareFeatures;
}

namespace SIMFQT {

  /**
   * @brief Command wrapping the pricing request process.
   */
  class FareQuoter {
    /** Friend classes: only the SimFQT service may access to the
        methods of that command class. */
    friend class SIMFQT_Service;

  private:
    // /////////////////// Business support methods //////////////
    /**
     * Quote a list of travel solutions (with a price for each travel solution).
     *
     * @param const stdair::BookingRequestStruct&
     * @param stdair::TravelSolutionList_T&
     * @param const stdair::BomRoot&
     */
    static void priceQuote (const stdair::BookingRequestStruct&,
                            stdair::TravelSolutionList_T&,
                            const stdair::BomRoot&);

    /**
     * Quote a single travel solution (with a price).
     *
     * @param const stdair::BookingRequestStruct&
     * @param stdair::TravelSolutionStruct&
     * @param const stdair::BomRoot&
     */
    static void priceQuote (const stdair::BookingRequestStruct&,
                            stdair::TravelSolutionStruct&,
                            const stdair::BomRoot&);

    /**
     * Quote a single travel solution (with a price).
     *
     * @param const stdair::BookingRequestStruct&
     * @param stdair::TravelSolutionStruct&
     * @param const stdair::AirportPair&
     * @param const stdair::ParsedKey&
     */
    static void priceQuote (const stdair::BookingRequestStruct&,
                            stdair::TravelSolutionStruct&,
                            const stdair::AirportPair&,
                            const stdair::ParsedKey&);

    /**
     * Quote a single travel solution (with a price).
     *
     * @param const stdair::BookingRequestStruct&
     * @param stdair::TravelSolutionStruct&
     * @param const stdair::PosChannel&
     * @param const stdair::ParsedKey&
     */
    static void priceQuote (const stdair::BookingRequestStruct&,
                            stdair::TravelSolutionStruct&,
                            const stdair::PosChannel&, const stdair::ParsedKey&);

    /**
     * Quote a single travel solution (with a price).
     *
     * @param const stdair::BookingRequestStruct&
     * @param stdair::TravelSolutionStruct&
     * @param const stdair::DatePeriod&
     * @param const stdair::PosChannel&
     * @param const stdair::ParsedKey&
     */
    static void priceQuote (const stdair::BookingRequestStruct&,
                            stdair::TravelSolutionStruct&,
                            const stdair::DatePeriod&,
                            const stdair::PosChannel&, const stdair::ParsedKey&);

    /**
     * Quote a single travel solution (with a price).
     *
     * @param const stdair::BookingRequestStruct&
     * @param stdair::TravelSolutionStruct&
     * @param const stdair::TimePeriod&
     * @param const stdair::PosChannel&
     * @param const stdair::ParsedKey&
     */
    static void priceQuote (const stdair::BookingRequestStruct&,
                            stdair::TravelSolutionStruct&,
                            const stdair::TimePeriod&,
                            const stdair::PosChannel&, const stdair::ParsedKey&);

    /**
     * Quote a single travel solution (with a price).
     *
     * @param const stdair::BookingRequestStruct&
     * @param stdair::TravelSolutionStruct&
     * @param const FareRuleFeatures&
     * @param const stdair::PosChannel&
     * @param stdair::FareOptionStruct&
     * @param const stdair::ParsedKey&
     */
    static void priceQuote (const stdair::BookingRequestStruct&,
                            stdair::TravelSolutionStruct&,
                            const stdair::FareFeatures&, const stdair::PosChannel&,
                            stdair::FareOptionStruct&, const stdair::ParsedKey&);


  private:
    // //////////////////// Construction and destruction ////////////////
    /**
     * Constructor.
     */
    FareQuoter();

    /**
     * Copy constructor.
     */
    FareQuoter(const FareQuoter&);

    /**
     * Destructor.
     */
    ~FareQuoter();
  };

}
#endif // __SIMFQT_CMD_FAREQUOTER_HPP

