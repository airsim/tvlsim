// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
#include <vector>
#include <exception>
// Dsim
#include <dsim/command/Simulator.hpp>
#include <dsim/service/Logger.hpp>

namespace DSIM {

  // //////////////////////////////////////////////////////////////////////
  void Simulator::simulate() {

    try {

      // DEBUG
      DSIM_LOG_DEBUG ("A simulation is being performed");
    
    } catch (const std::exception& lStdError) {
      DSIM_LOG_ERROR ("Error: " << lStdError.what());
      throw SimulationException();
    }
  }

}
