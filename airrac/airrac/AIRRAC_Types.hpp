#ifndef __AIRRAC_AIRRAC_TYPES_HPP
#define __AIRRAC_AIRRAC_TYPES_HPP

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
#include <stdair/stdair_basic_types.hpp>

namespace AIRRAC {

  // Forward declarations
  class AIRRAC_Service;

  // ///////// Exceptions ///////////
  class RootException : public std::exception {
  };

  class NonInitialisedServiceException : public RootException {
  };

  class MemoryAllocationException : public RootException {
  };

  class ObjectNotFoundException : public RootException {
  };

  class ParserException : public RootException {
  };

  class CodeConversionException : public ParserException {
  };

  class CodeDuplicationException : public ParserException {
  };

  class SegmentDateNotFoundException : public ParserException {
  };

  class SQLDatabaseException : public RootException {
  };

  class SQLDatabaseConnectionImpossibleException : public SQLDatabaseException {
  };

  class BookingException : public RootException {
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

