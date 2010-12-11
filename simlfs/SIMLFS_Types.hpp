#ifndef __SIMLFS_SIMLFS_TYPES_HPP
#define __SIMLFS_SIMLFS_TYPES_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <exception>
#include <string>

namespace SIMLFS {

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

  class QuotingException : public RootException {
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
  /** ID for the Fare Quote system. */
  typedef std::string FareQuoteID_T;

  /** Price, in Euros. */
  typedef double Price_T;

  /** IATA code of an airline. */
  typedef std::string AirlineCode_T;

  /** Number of passengers (in a group) for a booking. */
  typedef unsigned short PartySize_T;
  
}
#endif // __SIMLFS_SIMLFS_TYPES_HPP

