#ifndef __TRADEMGEN_CMD_DEMANDPARSER_HPP
#define __TRADEMGEN_CMD_DEMANDPARSER_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
// StdAir
#include <stdair/stdair_basic_types.hpp>
#include <stdair/command/CmdAbstract.hpp>
// TraDemGen
#include <trademgen/basic/DemandCharacteristicTypes.hpp>

// Forward declarations
namespace stdair {
  class BomRoot;
}

namespace TRADEMGEN {
  
  /** Class wrapping the parser entry point. */
  class DemandParser : public stdair::CmdAbstract {
  public:
    /** Parses the CSV file describing travel demand, for instance for
        generating simulated booking request in a simulator.
        @param const stdair::Filename_T& The file-name of the CSV-formatted
               demand input file.
        @param const stdair::Date_T&
        @param stdair::BomRoot& Root of the BOM tree. */
    static void generateDemand (const stdair::Filename_T&, stdair::BomRoot&,
                                stdair::UniformGenerator_T&,
                                const POSProbabilityMass_T&);
  };
}
#endif // __TRADEMGEN_CMD_DEMANDPARSER_HPP
