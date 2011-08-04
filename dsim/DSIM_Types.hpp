#ifndef __DSIM_DSIM_TYPES_HPP
#define __DSIM_DSIM_TYPES_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <exception>
#include <string>
// Boost
#include <boost/shared_ptr.hpp>
// StdAir
#include <stdair/stdair_exceptions.hpp>

namespace DSIM {

  // Forward declarations
  class DSIM_Service;


  // ///////// Exceptions ///////////
  /**
   * Specific exception related to the simulation.
   */
  class SimulationException : public stdair::RootException {
  };


  // //////// Type definitions specific to DSim /////////
  /**
   * Simulator ID (identifier of the simulator; not actually used for now).
   */
  typedef std::string SimulatorID_T;
  
  /**
   * (Smart) Pointer on the SimCRS service handler.
   */
  typedef boost::shared_ptr<DSIM_Service> DSIM_ServicePtr_T;
  
}
#endif // __DSIM_DSIM_TYPES_HPP

