#ifndef __AIRSCHED_CMD_SCHEDULEPARSER_HPP
#define __AIRSCHED_CMD_SCHEDULEPARSER_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
// StdAir
#include <stdair/stdair_basic_types.hpp>
#include <stdair/command/CmdAbstract.hpp>

// Forward declarations.
namespace stdair {
  class BomRoot;
}

namespace AIRSCHED {
  
  /** Class wrapping the parser entry point. */
  class ScheduleParser : public stdair::CmdAbstract {
  public:
    /** Parse the CSV file describing the airline schedules for the
        simulator, and generates the inventories accordingly.
        @param const stdair::Filename_T& The file-name of the CSV-formatted 
        schedule input file.
        @param stdair::BomRoot& Root of the BOM tree. */
    static void generateInventories (const stdair::Filename_T&,
                                     stdair::BomRoot&);
  };
}
#endif // __AIRSCHED_CMD_SCHEDULEPARSER_HPP
