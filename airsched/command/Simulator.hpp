#ifndef __AIRSCHED_COM_CMD_SIMULATOR_HPP
#define __AIRSCHED_COM_CMD_SIMULATOR_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// StdAir
#include <stdair/command/CmdAbstract.hpp>

// Forward declarations
namespace stdair {
  class BomRoot;
}

namespace AIRSCHED {
  
  /** Class implementing a small simulation, which uses the Airline Schedule. */
  class Simulator : public stdair::CmdAbstract {
  public:

    // ////////// Business Methods /////////
    /** Perform a small simulation, which uses the Airline Schedule.
        @param stdair::BomRoot& Root of the BOM tree. */
    static void simulate (stdair::BomRoot&);
  };

}
#endif // __AIRSCHED_COM_CMD_SIMULATOR_HPP
