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
  class AirportPairNotFoundException : public stdair::ObjectNotFoundException {
  public:
    /** Constructor. */
    AirportPairNotFoundException (const std::string& iWhat)
      : stdair::ObjectNotFoundException (iWhat) {}
  };

  class PosOrChannelNotFoundException : public stdair::ObjectNotFoundException {
  public:
    /** Constructor. */
    PosOrChannelNotFoundException (const std::string& iWhat)
      : stdair::ObjectNotFoundException (iWhat) {}
  };

  class FlightDateNotFoundException : public stdair::ObjectNotFoundException {
  public:
    /** Constructor. */
    FlightDateNotFoundException (const std::string& iWhat)
      : stdair::ObjectNotFoundException (iWhat) {}
  };

  class FlightTimeNotFoundException : public stdair::ObjectNotFoundException {
  public:
    /** Constructor. */
    FlightTimeNotFoundException (const std::string& iWhat)
      : stdair::ObjectNotFoundException (iWhat) {}
  };

  class FeaturesNotFoundException : public stdair::ObjectNotFoundException {
  public:
    /** Constructor. */
    FeaturesNotFoundException (const std::string& iWhat)
      : stdair::ObjectNotFoundException (iWhat) {}
  };
  
  class AirlineNotFoundException : public stdair::ObjectNotFoundException {
  public:
    /** Constructor. */
    AirlineNotFoundException (const std::string& iWhat)
      : stdair::ObjectNotFoundException (iWhat) {}
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
