#ifndef __DSIM_CMD_SIMULATOR_HPP
#define __DSIM_CMD_SIMULATOR_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// StdAir
#include <stdair/basic/DemandGenerationMethod.hpp>
#include <stdair/bom/EventStruct.hpp>
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

  class SimulationStatus;

  /** Class wrapping the simulation methods. */
  class Simulator : public CmdAbstract {
    friend class DSIM_Service;
  private:

    /** Perform a simulation. */
    static void simulate(SIMCRS::SIMCRS_Service&, TRADEMGEN::TRADEMGEN_Service&,
                         TRAVELCCM::TRAVELCCM_Service&,stdair::STDAIR_Service&,
                         SimulationStatus&,
                         const stdair::DemandGenerationMethod&);  

    /** Update the simulation status. */
    static void updateStatus(TRADEMGEN::TRADEMGEN_Service&,
			     const stdair::EventType::EN_EventType&,
			     SimulationStatus& ioSimulationStatus);

    /** Play a booking request event. */
    static void playBookingRequest (SIMCRS::SIMCRS_Service&,
                                    TRADEMGEN::TRADEMGEN_Service&,
                                    TRAVELCCM::TRAVELCCM_Service&,
                                    const stdair::EventStruct&,
                                    stdair::ProgressStatusSet&,
                                    SimulationStatus&,
                                    const stdair::DemandGenerationMethod&);

    /** Play a cancellation event. */
    static void playCancellation (SIMCRS::SIMCRS_Service&,
                                  const stdair::EventStruct&,
                                  SimulationStatus&);
                                  
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
