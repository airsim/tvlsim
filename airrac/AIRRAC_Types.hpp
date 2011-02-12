#ifndef __AIRRAC_AIRRAC_TYPES_HPP
#define __AIRRAC_AIRRAC_TYPES_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <map>
// Boost
#include <boost/shared_ptr.hpp>
// StdAir
#include <stdair/stdair_exceptions.hpp>
#include <stdair/stdair_basic_types.hpp>

namespace AIRRAC {

  // Forward declarations
  class AIRRAC_Service;

  // ///////// Exceptions ///////////
  class YielCalculationException : public stdair::RootException {
  };

  // //////// Type definitions /////////
  /** Pointer on the AIRRAC Service handler. */
  typedef boost::shared_ptr<AIRRAC::AIRRAC_Service> AIRRAC_ServicePtr_T;
  
  /** Typedef which defines a map of airline codes and the corresponding
      airline inventories. */
  typedef std::map<const stdair::AirlineCode_T,
                   AIRRAC_ServicePtr_T> AIRRAC_ServicePtr_Map_T;
}
#endif // __AIRRAC_AIRRAC_TYPES_HPP

