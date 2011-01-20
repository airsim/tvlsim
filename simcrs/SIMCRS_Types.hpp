#ifndef __SIMCRS_SIMCRS_TYPES_HPP
#define __SIMCRS_SIMCRS_TYPES_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <exception>
#include <string>
// StdAir
#include <stdair/stdair_exceptions.hpp>

namespace SIMCRS {

  // ///////// Exceptions ///////////
  class BookingException : public stdair::RootException {
  };

  class AvailabilityRetrievalException : public stdair::RootException {
  };

  // //////// Type definitions specific to SimCRS /////////
  /** CRS code. */
  typedef std::string CRSCode_T;
  
}
#endif // __SIMCRS_SIMCRS_TYPES_HPP

