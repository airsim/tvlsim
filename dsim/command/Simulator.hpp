#ifndef __DSIM_CMD_SIMULATOR_HPP
#define __DSIM_CMD_SIMULATOR_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// DSIM
#include <dsim/command/CmdAbstract.hpp>

// Forward declarations
namespace stdair {
  struct BookingRequestStruct;
}

namespace SIMCRS {
  class SIMCRS_Service;
}

namespace TRADEMGEN {
  class TRADEMGEN_Service;
}

namespace DSIM {

  /** Class wrapping the simulation methods. */
  class Simulator : public CmdAbstract {
    friend class DSIM_Service;
  private:

    /** Perform a simulation. */
    static void simulate (SIMCRS::SIMCRS_Service&,TRADEMGEN::TRADEMGEN_Service&);

    /** Play a booking request event. */
    static void playBookingRequest (SIMCRS::SIMCRS_Service&,
                                    const stdair::BookingRequestStruct&);

  private:
    /** Constructors. */
    Simulator() {}
    Simulator(const Simulator&) {}
    /** Destructor. */
    ~Simulator() {}
  };

}
#endif // __DSIM_CMD_SIMULATOR_HPP