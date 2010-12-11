#ifndef __TRADEMGEN_CMD_DEMANDGENERATOR_HPP
#define __TRADEMGEN_CMD_DEMANDGENERATOR_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// StdAir
#include <stdair/command/CmdAbstract.hpp>
// Trademgen
#include <trademgen/TRADEMGEN_Types.hpp>

// Forward declarations
namespace stdair {
  class BomRoot;
  class Demand;
}

namespace TRADEMGEN {

  // Forward declarations
  struct DemandStruct;
  namespace DemandParserHelper {
    struct doEndDemand;
  }
    
  /** Class handling the generation / instantiation of the Demand BOM. */
  class DemandGenerator : public stdair::CmdAbstract {
    // Only the following class may use methods of DemandGenerator.
    // Indeed, as those methods build the BOM, it is not good to expose
    // them publicly.
    friend class DemandFileParser;
    friend struct DemandParserHelper::doEndDemand;
    friend class DemandParser;
  private:
    /** Generate the Demand objects corresponding to the given
        Flight-Period, and add them to the given BomRoot. */
    static void createDemandCharacteristics (stdair::BomRoot&,
                                             stdair::UniformGenerator_T&,
                                             const POSProbabilityMass_T&,
                                             const DemandStruct&);

    /** Generate the random seed for the demand characteristic distributions. */
    static stdair::RandomSeed_T generateSeed (stdair::UniformGenerator_T&);
  };

}
#endif // __TRADEMGEN_CMD_DEMANDGENERATOR_HPP
