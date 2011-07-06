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
     * Quote a list of travel solutions (with a list of fare-options for each
     * travel solution).
     *
     * @param const stdair::BookingRequestStruct&  Booking request correponding
     * to the travel solution list.
     * @param stdair::TravelSolutionList_T&        List of travel solutions.
     * @param const stdair::BomRoot&               Root of the BOM tree.
     */
    static void priceQuote (const stdair::BookingRequestStruct&,
                            stdair::TravelSolutionList_T&,
                            const stdair::BomRoot&);

    /**
     * First step of fare quoting a single travel solution (with a list of
     * fare-options):
     * selecting the fare rules with the correct origin/destination.
     *
     * @param const stdair::BookingRequestStruct&  Booking request correponding
     * to the travel solution list.
     * @param stdair::TravelSolutionStruct&        Travel solution to fare
     * quote.
     * @param const stdair::BomRoot&               Root of the BOM tree.
     */
    static void priceQuote (const stdair::BookingRequestStruct&,
                            stdair::TravelSolutionStruct&,
                            const stdair::BomRoot&);

    /**
     * Second step of fare quoting a single travel solution (with a list of
     * fare-options):
     * selecting the fare rules with the correct date range.
     *
     * @param const stdair::BookingRequestStruct& Booking request correponding
     * to the travel solution list.
     * @param stdair::TravelSolutionStruct&       Travel solution to fare quote.
     * @param const stdair::AirportPair&          Root of the BOM tree.
     */
    static void priceQuote (const stdair::BookingRequestStruct&,
                            stdair::TravelSolutionStruct&,
                            const stdair::AirportPair&);

    /**
     * Third step of fare quoting a single travel solution (with a list of
     * fare-options):
     * selecting the fare rules with the correct point of sale and the correct
     * channel.
     *
     * @param const stdair::BookingRequestStruct&  Booking request correponding
     * to the travel solution list.
     * @param stdair::TravelSolutionStruct&        Travel solution to fare
     * quote.
     * @param const stdair::DatePeriod&            Root of the BOM tree.
     * @param const stdair::AirportPair&           Information needed in case
     * of error message to display.
     */
    static void priceQuote (const stdair::BookingRequestStruct&,
                            stdair::TravelSolutionStruct&,
                            const stdair::DatePeriod&,
                            const stdair::AirportPair&);

    /**
     * Fourth step of fare quoting a single travel solution (with a list of
     * fare-options):
     * selecting the fare rules with the correct time range.
     *
     * @param const stdair::BookingRequestStruct&  Booking request correponding
     * to the travel solution list.
     * @param stdair::TravelSolutionStruct&        Travel solution to fare
     * quote.
     * @param const stdair::PosChannel&            Root of the BOM tree.
     */
    static void priceQuote (const stdair::BookingRequestStruct&,
                            stdair::TravelSolutionStruct&,
                            const stdair::PosChannel&);

    /**
     * Fifth step of fare quoting a single travel solution (with a list of
     * fare-options):
     * selecting the fare rules with the correct trip type, the correct
     * advanced purchase and the correct stay duration.
     *
     * @param const stdair::BookingRequestStruct&  Booking request correponding
     * to the travel solution list.
     * @param stdair::TravelSolutionStruct&        Travel solution to fare
     * quote.
     * @param const stdair::TimePeriod&            Root of the BOM tree.
     * @param const stdair::PosChannel&            Information needed in case
     * of error message to display.
     */
    static void priceQuote (const stdair::BookingRequestStruct&,
                            stdair::TravelSolutionStruct&,
                            const stdair::TimePeriod&,
                            const stdair::PosChannel&);

    /**
     * Last step of fare quoting a single travel solution (with a list of
     * fare-options):
     * fulfilling the fare otpion structure (fare value, options) and adding it
     * to the travel solution.
     *
     * @param const stdair::BookingRequestStruct&  Booking request correponding
     * to the travel solution list.
     * @param stdair::TravelSolutionStruct&        Travel solution to fare
     * quote.
     * @param const FareRuleFeatures&              Root of the BOM tree.
     * @param const stdair::PosChannel&            Information needed in case
     * of error message to display.
     * @param stdair::FareOptionStruct&            Fare quote structure
     * including a fare and options such as saturday night stay, change fees,
     * refundable option. 
     */
    static void priceQuote (const stdair::BookingRequestStruct&,
                            stdair::TravelSolutionStruct&,
                            const stdair::FareFeatures&,
                            const stdair::PosChannel&,
                            stdair::FareOptionStruct&);
    
    /**
     * Return the parsed key of the first segment of a travel solution.
     *
     * @param stdair::TravelSolutionStruct& Travel solution.
     * @return stdair::ParsedKey            Parsed key of the first segment of
     * the travel solution.
     */
    static stdair::ParsedKey getFirstSPParsedKey (stdair::TravelSolutionStruct&);

    /**
     * Return the parsed key of the last segment of a travel solution.
     *
     * @param stdair::TravelSolutionStruct& Travel solution.
     * @return stdair::ParsedKey            Parsed key of the last segment of
     * the travel solution.
     */
    static stdair::ParsedKey getLastSPParsedKey (stdair::TravelSolutionStruct&);



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

