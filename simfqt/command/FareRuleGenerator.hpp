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
  class DatePeriod;
  class PosChannel;
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
<<<<<<< HEAD
<<<<<<< HEAD
    /**
     * Generate the airport pair objects corresponding to the
     * given fareRule and add them to the given BomRoot.
=======
=======
>>>>>>> trunk
    
    /**
     * Generate the airport pair object corresponding to the given
     * fareRule and add them to the given BomRoot.
     *
     * @param stdair::BomRoot&      Boom root node to attach an airport pair
     * object.
     * @param const FareRuleStruct& Parsed fare rule to add to the BomRoot.
<<<<<<< HEAD
>>>>>>> trunk
=======
>>>>>>> trunk
     */
    static void createAirportPair (stdair::BomRoot&,
                                   const FareRuleStruct&);

    /**
<<<<<<< HEAD
<<<<<<< HEAD
     * Generate the date-period objects corresponding to
     * the given fareRule.
=======
=======
>>>>>>> trunk
     * Generate the date-period object corresponding to the given
     * fareRule.  
     *
     * @param stdair::AirportPair&  Airport Pair node to attach a date period
     * object.
     * @param const FareRuleStruct& Parsed fare rule to add to the BomRoot.
<<<<<<< HEAD
>>>>>>> trunk
=======
>>>>>>> trunk
     */
    static void createDateRange (stdair::AirportPair&,
                                 const FareRuleStruct&);
    
    /**
<<<<<<< HEAD
<<<<<<< HEAD
     * Generate the point_of_sale-channel objects corresponding to
     * the given fareRule.
=======
=======
>>>>>>> trunk
     * Generate the point_of_sale-channel object corresponding to
     * the given fareRule.
     *
     * @param stdair::DatePeriod&   Date Period node to attach a pos-channel
     * object.
     * @param const FareRuleStruct& Parsed fare rule to add to the BomRoot.
<<<<<<< HEAD
>>>>>>> trunk
=======
>>>>>>> trunk
     */
    static void createPOSChannel (stdair::DatePeriod&,
                                  const FareRuleStruct&);
    
    /**
<<<<<<< HEAD
<<<<<<< HEAD
     * Generate the time-period objects corresponding to
     * the given fareRule.
=======
=======
>>>>>>> trunk
     * Generate the time-period object corresponding to the given
     * fareRule.
     *
     * @param stdair::PosChannel&   Pos-Channel node to attach a time period
     * object.
     * @param const FareRuleStruct& Parsed fare rule to add to the BomRoot.
<<<<<<< HEAD
>>>>>>> trunk
=======
>>>>>>> trunk
     */
    static void createTimeRange (stdair::PosChannel&,
                                 const FareRuleStruct&);

    /**
<<<<<<< HEAD
<<<<<<< HEAD
     * Generate the fare-features objects corresponding to
     * the given fareRule.
=======
=======
>>>>>>> trunk
     * Generate the fare-features object corresponding to the given
     * fareRule.
     *
     * @param stdair::TimePeriod&   Time Period node to attach a fare features
     * object.
     * @param const FareRuleStruct& Parsed fare rule to add to the BomRoot.
<<<<<<< HEAD
>>>>>>> trunk
=======
>>>>>>> trunk
     */
    static void createFareFeatures (stdair::TimePeriod&,
                                    const FareRuleStruct&);

    /**
<<<<<<< HEAD
<<<<<<< HEAD
     * Generate the airline-class list objects corresponding to
     * the given fareRule.
=======
=======
>>>>>>> trunk
     * Generate the airline-class list object corresponding to the
     * given fareRule.
     *
     * @param stdair::FareFeatures& Fare Features node to attach an
     * airline-class list object.
     * @param const FareRuleStruct& Parsed fare rule to add to the BomRoot.
<<<<<<< HEAD
>>>>>>> trunk
=======
>>>>>>> trunk
     */
    static void createAirlineClassList (stdair::FareFeatures&,
                                        const FareRuleStruct&);


    
  };

}
#endif // __SIMFQT_CMD_FARERULEGENERATOR_HPP
