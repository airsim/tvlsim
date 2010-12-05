#ifndef __TRADEMGEN_TRADEMGEN_TYPES_HPP
#define __TRADEMGEN_TRADEMGEN_TYPES_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <exception>
#include <string>
#include <list>
#include <map>
// Boost Array
#include <boost/array.hpp>

namespace TRADEMGEN {

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

  class ParserException : public RootException {
  };

  class CodeConversionException : public ParserException {
  };

  class CodeDuplicationException : public ParserException {
  };

  class SQLDatabaseException : public RootException {
  };

  class SQLDatabaseConnectionImpossibleException : public SQLDatabaseException {
  };

  class TrademgenGenerationException : public RootException {
  };

  class FileException : public RootException {
  public:
    /** Constructor. */
    FileException (const std::string& iWhat) : RootException (iWhat) {}
  };

  class DemandInputFileNotFoundException : public FileException {
  public:
    /** Constructor. */
    DemandInputFileNotFoundException (const std::string& iWhat)
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

  
}
#endif // __TRADEMGEN_TRADEMGEN_TYPES_HPP

