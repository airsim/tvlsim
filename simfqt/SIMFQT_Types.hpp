#ifndef __SIMFQT_SIMFQT_TYPES_HPP
#define __SIMFQT_SIMFQT_TYPES_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <vector>
#include <string>
// StdAir
#include <stdair/stdair_exceptions.hpp>

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

  class BOMObjectNotFoundException : public RootException {
  public:
    /** Constructor. */
    BOMObjectNotFoundException (const std::string& iWhat)
      : RootException  (iWhat) {}
  };

  class AirportPairNotFoundException : public BOMObjectNotFoundException {
  public:
    /** Constructor. */
    AirportPairNotFoundException (const std::string& iWhat)
      : BOMObjectNotFoundException (iWhat) {}
  };

  class PositionNotFoundException : public BOMObjectNotFoundException {
  public:
    /** Constructor. */
    PositionNotFoundException (const std::string& iWhat)
      : BOMObjectNotFoundException (iWhat) {}
  };

  class FareInputFileNotFoundException : public stdair::FileNotFoundException {
  public:
    /** Constructor. */
    FareInputFileNotFoundException (const std::string& iWhat)
      : stdair::FileNotFoundException (iWhat) {}
  };

  class QuotingException : public stdair::RootException {
  };

  // //////// Type definitions specific to SimFQT /////////
  /** ID for the Fare Quote system. */
  typedef unsigned int FareQuoteID_T;

}
#endif // __SIMFQT_SIMFQT_TYPES_HPP

