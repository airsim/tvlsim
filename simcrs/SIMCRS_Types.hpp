#ifndef __SIMCRS_SIMCRS_TYPES_HPP
#define __SIMCRS_SIMCRS_TYPES_HPP

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

namespace SIMCRS {

  // Forward declarations
  class SIMCRS_Service;


  // ///////// Exceptions ///////////
  /**
   * Specific exception related to bookings made against the CRS.
   */
  class BookingException : public stdair::RootException {
  };

  /**
   * Specific exception related to availability calculation.
   */
  class AvailabilityRetrievalException : public stdair::RootException {
  };


  // //////// Type definitions specific to SimCRS /////////
  /**
   * CRS code (identifier of the CRS; not actually used for now).
   */
  typedef std::string CRSCode_T;
  
  /**
   * (Smart) Pointer on the SimCRS service handler.
   */
  typedef boost::shared_ptr<SIMCRS_Service> SIMCRS_ServicePtr_T;
  
}
#endif // __SIMCRS_SIMCRS_TYPES_HPP

