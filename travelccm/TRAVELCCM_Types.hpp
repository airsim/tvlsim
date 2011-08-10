#ifndef __TRAVELCCM_TRAVELCCM_TYPES_HPP
#define __TRAVELCCM_TRAVELCCM_TYPES_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
// Boost
#include <boost/shared_ptr.hpp>
// StdAir
#include <stdair/stdair_exceptions.hpp>

namespace TRAVELCCM {

  /// Forward declarations
  class TRAVELCCM_Service;


  // ///////// Exceptions ///////////
  /**
   * TravelCCM specific root exception.
   */
  class CustomerChoiceException : public stdair::RootException {
  public:
    /**
     * Constructor.
     */
    CustomerChoiceException (const std::string& iWhat)
      : stdair::RootException (iWhat) {}
  };


  // //////// Type definitions /////////
  /**
   * Pointer on the TRAVELCCM Service handler.
   */
  typedef boost::shared_ptr<TRAVELCCM_Service> TRAVELCCM_ServicePtr_T;

}
#endif // __TRAVELCCM_TRAVELCCM_TYPES_HPP
