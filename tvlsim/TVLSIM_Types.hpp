#ifndef __TVLSIM_TVLSIM_TYPES_HPP
#define __TVLSIM_TVLSIM_TYPES_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <exception>
#include <string>
#include <map>
// Boost
#include <boost/shared_ptr.hpp>
// StdAir
#include <stdair/stdair_exceptions.hpp>
#include <stdair/basic/EventType.hpp>

namespace TVLSIM {

  // Forward declarations
  class TVLSIM_Service;


  // ///////// Exceptions ///////////
  /**
   * Specific exception related to the simulation.
   */
  class SimulationException : public stdair::RootException {
  };


  // //////// Type definitions specific to TvlSim /////////
  /**
   * Simulator ID (identifier of the simulator; not actually used for now).
   */
  typedef std::string SimulatorID_T;

  /** Number of simulation runs. */
  typedef unsigned int NbOfRuns_T;
  
  /**
   * (Smart) Pointer on the SimCRS service handler.
   */
  typedef boost::shared_ptr<TVLSIM_Service> TVLSIM_ServicePtr_T;  

  /**
   * Definition of the (STL) map of chronometer structures, one
   * for each event type (e.g., booking request, optimisation
   * notification).
   */
  typedef std::map<stdair::EventType::EN_EventType,
		   double> ChronometerMap_T;
  
  
}
#endif // __TVLSIM_TVLSIM_TYPES_HPP

