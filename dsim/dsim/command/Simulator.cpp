// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <fstream>
#include <string>
#include <vector>
#include <exception>
// STDAIR
#include <stdair/STDAIR_Types.hpp>
// Distribution
#include <simcrs/SIMCRS_Service.hpp>
// Dsim
#include <dsim/command/Simulator.hpp>
#include <dsim/service/Logger.hpp>

namespace DSIM {

  // //////////////////////////////////////////////////////////////////////
  void Simulator::simulate (SIMCRS::SIMCRS_Service& ioSIMCRS_Service) {

    try {

      // DEBUG
      DSIM_LOG_DEBUG ("The simulation is starting");

      // Airline code
      stdair::AirlineCode_T lAirlineCode ("BA");
    
      // Number of passengers in the travelling group
      stdair::PartySize_T lPartySize = 5;
    
      // Make a booking
      ioSIMCRS_Service.sell (lAirlineCode, lPartySize);
    
      // DEBUG
      DSIM_LOG_DEBUG ("The simulation has ended");

    } catch (const std::exception& lStdError) {
      DSIM_LOG_ERROR ("Error: " << lStdError.what());
      throw SimulationException();
    }
  }

}
