#ifndef __AVLCAL_AVLCAL_TYPES_HPP
#define __AVLCAL_AVLCAL_TYPES_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <exception>
#include <string>

namespace AVLCAL {

  // ///////// Exceptions ///////////
  class RootException : public std::exception {
  };

  class NonInitialisedServiceException : public RootException {
  };

  class MemoryAllocationException : public RootException {
  };

  class ObjectNotFoundException : public RootException {
  };

  class SQLDatabaseException : public RootException {
  };

  class SQLDatabaseConnectionImpossibleException : public SQLDatabaseException {
  };

  class AvlCalcultationException : public RootException {
  };


  // /////////////// Log /////////////
  /** Level of logs. */
  namespace LOG {
    typedef enum {
      CRITICAL = 0,
      ERROR,
      NOTIFICATION,
      WARNING,
      DEBUG,
      VERBOSE,
      LAST_VALUE
    } EN_LogLevel;
  }


  // //////// Type definitions /////////
  /** IATA code of the airline owner of the inventory system. */
  typedef std::string AirlineCode_T;

  /** Number of passengers (in a group) for an availability request. */
  typedef unsigned short PartySize_T;
  
}
#endif // __AVLCAL_AVLCAL_TYPES_HPP
