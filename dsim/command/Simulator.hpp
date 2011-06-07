#ifndef __DSIM_CMD_SIMULATOR_HPP
#define __DSIM_CMD_SIMULATOR_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// DSIM
#include <dsim/command/CmdAbstract.hpp>

// Forward declarations
namespace stdair {
  class STDAIR_Service;
  struct EventStruct;
  struct ProgressStatusSet;
}
namespace SIMCRS {
  class SIMCRS_Service;
}
namespace TRADEMGEN {
  class TRADEMGEN_Service;
}
namespace TRAVELCCM {
  class TRAVELCCM_Service;
}

namespace DSIM {

  /** Class wrapping the simulation methods. */
  class Simulator : public CmdAbstract {
    friend class DSIM_Service;
  private:

    /** Perform a simulation. */
    static void simulate (SIMCRS::SIMCRS_Service&, TRADEMGEN::TRADEMGEN_Service&,
                          TRAVELCCM::TRAVELCCM_Service&,stdair::STDAIR_Service&,
                          const bool);

    /** Play a booking request event. */
    static void playBookingRequest (SIMCRS::SIMCRS_Service&,
                                    TRADEMGEN::TRADEMGEN_Service&,
                                    TRAVELCCM::TRAVELCCM_Service&,
                                    const stdair::EventStruct&,
                                    stdair::ProgressStatusSet&,
                                    const bool);

    /** Play a snapshot event. */
    static void playSnapshotEvent (SIMCRS::SIMCRS_Service&,
                                   const stdair::EventStruct&);


    /** Play a RM event. */
    static void playRMEvent (SIMCRS::SIMCRS_Service&,
                             const stdair::EventStruct&);

  private:
    /** Constructors. */
    Simulator() {}
    Simulator(const Simulator&) {}
    /** Destructor. */
    ~Simulator() {}
  };

}
#endif // __DSIM_CMD_SIMULATOR_HPP
