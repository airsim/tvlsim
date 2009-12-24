#ifndef __DSIM_CMD_SIMULATOR_HPP
#define __DSIM_CMD_SIMULATOR_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// DSIM
#include <dsim/command/CmdAbstract.hpp>

namespace DSIM {

  /** Class wrapping the simulation methods. */
  class Simulator : public CmdAbstract {
    friend class DSIM_Service;
  private:

    /** Perform a simulation. */
    static void simulate();

  private:
    /** Constructors. */
    Simulator() {}
    Simulator(const Simulator&) {}
    /** Destructor. */
    ~Simulator() {}
  };

}
#endif // __DSIM_CMD_SIMULATOR_HPP
