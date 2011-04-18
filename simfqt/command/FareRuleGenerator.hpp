#ifndef __SIMFQT_CMD_FARERULEGENERATOR_HPP
#define __SIMFQT_CMD_FARERULEGENERATOR_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// StdAir
#include <stdair/command/CmdAbstract.hpp>
// Simfqt
#include <simfqt/SIMFQT_Types.hpp>

// Forward declarations
namespace stdair {
  class BomRoot;
  class FareRule;
  class AirportPair;
  class PosChannel;
  class DatePeriod;
  class TimePeriod;
  class FareFeatures;
  class AirlineClassList;
}

namespace SIMFQT {

  // Forward declarations
  struct FareRuleStruct;
  namespace FareParserHelper {
    struct doEndFare;
  }
    
  /** Class handling the generation / instantiation of the Fare BOM. */
  class FareRuleGenerator : public stdair::CmdAbstract {
    // Only the following class may use methods of FareGenerator.
    // Indeed, as those methods build the BOM, it is not good to expose
    // them public.
    friend class FareFileParser;
    friend struct FareParserHelper::doEndFare;
    friend class FareParser;
  private:
    /**
     * Generate the airport pair objects corresponding to the
     * given fareRule and add them to the given BomRoot.
     */
    static void createAirportPair (stdair::BomRoot&,
                                   const FareRuleStruct&);

    /**
     * Generate the point_of_sale-channel objects corresponding to
     * the given fareRule.
     */
    static void createPOSChannel (stdair::AirportPair&,
                                  const FareRuleStruct&);
    
    /**
     * Generate the date-period objects corresponding to
     * the given fareRule.
     */
    static void createDateRange (stdair::PosChannel&,
                                 const FareRuleStruct&);

    /**
     * Generate the time-period objects corresponding to
     * the given fareRule.
     */
    static void createTimeRange (stdair::DatePeriod&,
                                 const FareRuleStruct&);

    /**
     * Generate the fare-features objects corresponding to
     * the given fareRule.
     */
    static void createFareFeatures (stdair::TimePeriod&,
                                    const FareRuleStruct&);

    /**
     * Generate the airline-class list objects corresponding to
     * the given fareRule.
     */
    static void createAirlineClassList (stdair::FareFeatures&,
                                        const FareRuleStruct&);


    
  };

}
#endif // __SIMFQT_CMD_FARERULEGENERATOR_HPP
