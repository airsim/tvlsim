#ifndef __SIMFQT_SIMFQT_TYPES_HPP
#define __SIMFQT_SIMFQT_TYPES_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <vector>
#include <string>
// StdAir

namespace SIMFQT {

  // ///////// Exceptions ///////////
  class RootException : public std::exception {
  public:
     /** Constructors. */
    RootException (const std::string& iWhat) : _what (iWhat) {}
    RootException () : _what ("No more details") {}
    /** Destructor. */
    virtual ~RootException() throw() {}
    /** Give the details of the exception. */
    const char* what() const throw() { return _what.c_str(); } 
  protected:
    /** Details for the exception. */
    std::string _what;
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

  class FileException : public RootException {
  public:
    /** Constructor. */
    FileException (const std::string& iWhat) : RootException (iWhat) {}
  };

  class FareInputFileNotFoundException : public FileException {
  public:
    /** Constructor. */
    FareInputFileNotFoundException (const std::string& iWhat)
      : FileException (iWhat) {}
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
  typedef unsigned int FareQuoteID_T;

  /** Price in euros. */
  typedef double Price_T;

  /** Number of passengers (in a group) for a booking. */
  typedef unsigned short PartySize_T;
  
}
#endif // __SIMFQT_SIMFQT_TYPES_HPP

