#ifndef __AIRINV_CMD_DCPPARSER_HPP
#define __AIRINV_CMD_DCPPARSER_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// StdAir
#include <stdair/stdair_basic_types.hpp>
#include <stdair/command/CmdAbstract.hpp>

// Forward declarations.
namespace stdair {
  class BomRoot;
}

namespace AIRINV {
  
  /** Class wrapping the parser entry point. */
  class DCPParser : public stdair::CmdAbstract {
  public:
    /**
     * Parses the CSV file describing the DCPs for the
     * simulator, and generates the event structures accordingly.
     * @param const stdair::Filename_T& The file-name of the CSV-formatted 
     *   DCP input file.
     * @param stdair::BomRoot& Root of the BOM tree.
     */
    static void DCPRuleGeneration (const stdair::Filename_T&,
                                   stdair::BomRoot&);
  };
}
#endif // __AIRINV_CMD_DCPPARSER_HPP
