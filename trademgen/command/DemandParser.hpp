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

/// Forward declarations
namespace stdair {
  class EventQueue;
  struct RandomGeneration;
}

namespace TRADEMGEN {
  
  /**
   * @brief Class wrapping the parser entry point.
   */
  class DemandParser : public stdair::CmdAbstract {
  public:
    /**
     * Parse the CSV file describing travel demand, for instance for
     * generating simulated booking request in a simulator.
     *
     * The state of the random generator, given as parameter, evolves
     * each time a demand request is generated.
     *
     * @param const stdair::Filename_T& The file-name of the
              CSV-formatted demand input file.
     * @param stdair::EventQueue& Event queue.
     * @param stdair::RandomGeneration& Random generator.
     */
    static void generateDemand (const stdair::Filename_T&, stdair::EventQueue&,
                                stdair::RandomGeneration&,
                                const POSProbabilityMass_T&);
  };
}
#endif // __TRADEMGEN_CMD_DEMANDPARSER_HPP
