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
    /** Generate the fare objects corresponding to the given
        fareRule and add them to the given BomRoot. */
    static void createFareRule (stdair::BomRoot&,
                                FareRuleStruct&);
  };

}
#endif // __SIMFQT_CMD_FARERULEGENERATOR_HPP
