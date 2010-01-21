// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <fstream>
#include <string>
#include <vector>
#include <exception>
// StdAir
#include <stdair/STDAIR_Types.hpp>
#include <stdair/service/Logger.hpp>
// Distribution
#include <simcrs/SIMCRS_Service.hpp>
// Dsim
#include <dsim/DSIM_Types.hpp>
#include <dsim/command/Simulator.hpp>

namespace DSIM {

  // //////////////////////////////////////////////////////////////////////
  void Simulator::simulate (SIMCRS::SIMCRS_Service& ioSIMCRS_Service) {

    try {

      // DEBUG
      STDAIR_LOG_DEBUG ("The simulation is starting");

      // Airline code
      stdair::AirlineCode_T lAirlineCode ("BA");
    
      // Number of passengers in the travelling group
      stdair::PartySize_T lPartySize = 5;
    
      // Make a booking
      ioSIMCRS_Service.sell (lAirlineCode, lPartySize);
    
      // DEBUG
      STDAIR_LOG_DEBUG ("The simulation has ended");

    } catch (const std::exception& lStdError) {
      STDAIR_LOG_ERROR ("Error: " << lStdError.what());
      throw SimulationException();
    }
  }

}
