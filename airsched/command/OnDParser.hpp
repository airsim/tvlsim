#ifndef __AIRSCHED_CMD_ONDPARSER_HPP
#define __AIRSCHED_CMD_ONDPARSER_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
// StdAir
#include <stdair/stdair_basic_types.hpp>
#include <stdair/command/CmdAbstract.hpp>
  
/// Forward declarations
namespace stdair {
  class BomRoot;
}

namespace AIRSCHED {

  /**
   * @brief Class wrapping the parser entry point.
   */
  class OnDParser : public stdair::CmdAbstract {
  public:
    /**
     * Parse the CSV file describing the O&D.
     *
     * @param const std::string& The file-name of the CSV-formatted fare
     *        input file and the container.
     */
    static void generateOnDPeriods (const stdair::Filename_T&,
                                    stdair::BomRoot&);
  };
    
}
#endif // __AIRSCHED_CMD_ONDPARSER_HPP
