// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// StdAir
#include <stdair/service/Logger.hpp>
// TravelCCM
#include <travelccm/command/Simulator.hpp>

namespace TRAVELCCM {

  // //////////////////////////////////////////////////////////////////////
  bool Simulator::simulate(Passenger& ioPassenger,
                           TravelSolutionHolder& ioTravelSolutionHolder) {
    
    int numTravelSolIn = ioTravelSolutionHolder.numberOfTravelSolutions();

    // print the number of travel solutions before the customer choice
    STDAIR_LOG_DEBUG ("Initially there are " << numTravelSolIn
                         << " travel solutions");

    // run the algorithm of passenger choice
    CCM::orderedPreferences (ioTravelSolutionHolder, ioPassenger);
    
    // print the travel solutions among which the customer will choose
    int numTravelSolOut = ioTravelSolutionHolder.numberOfTravelSolutions();

    STDAIR_LOG_DEBUG ("Finally there are " << numTravelSolOut
                         << " travel solutions");
    
    STDAIR_LOG_DEBUG ("Simulation ended properly!");

    return (numTravelSolOut == 0);
  }
  
}
