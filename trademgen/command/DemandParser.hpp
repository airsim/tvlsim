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
#include <trademgen/basic/DemandCharacteristicsTypes.hpp>

// Forward declarations
namespace stdair {
  class EventQueue;
}

namespace TRADEMGEN {
  
  /**
   * Class wrapping the parser entry point.
   */
  class DemandParser : public stdair::CmdAbstract {
  public:
    /**
     * Parses the CSV file describing travel demand, for instance for
     * generating simulated booking request in a simulator.
     * @param const stdair::Filename_T& The file-name of the
              CSV-formatted demand input file.
     * @param const stdair::Date_T&
     * @param stdair::EventQueue& Root of the BOM tree.
     */
    static void generateDemand (const stdair::Filename_T&, stdair::EventQueue&,
                                stdair::UniformGenerator_T&,
                                const POSProbabilityMass_T&);
  };
}
#endif // __TRADEMGEN_CMD_DEMANDPARSER_HPP
