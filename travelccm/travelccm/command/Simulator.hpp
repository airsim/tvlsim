#ifndef __TRAVELCCM_COM_CMD_SIMULATOR_HPP
#define __TRAVELCCM_COM_CMD_SIMULATOR_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// TRAVELCCM Common
#include <travelccm/bom/TravelSolutionHolder.hpp>
#include <travelccm/bom/Passenger.hpp>
#include <travelccm/command/CmdAbstract.hpp>
#include <travelccm/bom/CCM.hpp>


namespace TRAVELCCM {

  /** Class implementing a small simulation, which uses the Customer-Choice
      Model (CCM). */
  class Simulator : public CmdAbstract {
    
  public:

    // ////////// Business Methods /////////
    /** Perform a small simulation, which uses the Customer Choice Model.*/
    static bool simulate (Passenger&, TravelSolutionHolder&);

    
  private:
    
  };

}
#endif // __TRAVELCCM_COM_CMD_SIMULATOR_HPP
