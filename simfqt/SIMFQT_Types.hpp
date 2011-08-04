#ifndef __SIMFQT_SIMFQT_TYPES_HPP
#define __SIMFQT_SIMFQT_TYPES_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <vector>
#include <string>
// Boost
#include <boost/shared_ptr.hpp>
// StdAir
#include <stdair/stdair_exceptions.hpp>

namespace SIMFQT {

  // Forward declarations
  class SIMFQT_Service;


  // ///////// Exceptions ///////////
  /**
   * The fare input file can not be parsed.
   */
  class FareFileParsingFailedException
    : public stdair::ParsingFileFailedException {
  public:
    /**
     * Constructor.
     */
    FareFileParsingFailedException (const std::string& iWhat)
      : stdair::ParsingFileFailedException (iWhat) {}
  };
  
  /**
   * The given airport pair can not be found.
   */
  class AirportPairNotFoundException : public stdair::ObjectNotFoundException {
  public:
    /**
     * Constructor.
     */
    AirportPairNotFoundException (const std::string& iWhat)
      : stdair::ObjectNotFoundException (iWhat) {}
  };

  /**
   * The given POS/channel can not be found.
   */
  class PosOrChannelNotFoundException : public stdair::ObjectNotFoundException {
  public:
    /**
     * Constructor.
     */
    PosOrChannelNotFoundException (const std::string& iWhat)
      : stdair::ObjectNotFoundException (iWhat) {}
  };

  /**
   * The departure date of the flight can not be found.
   */
  class FlightDateNotFoundException : public stdair::ObjectNotFoundException {
  public:
    /**
     * Constructor.
     */
    FlightDateNotFoundException (const std::string& iWhat)
      : stdair::ObjectNotFoundException (iWhat) {}
  };

  /**
   * The departure time of the flight can not be found.
   */
  class FlightTimeNotFoundException : public stdair::ObjectNotFoundException {
  public:
    /**
     * Constructor.
     */
    FlightTimeNotFoundException (const std::string& iWhat)
      : stdair::ObjectNotFoundException (iWhat) {}
  };

  /**
   * The fare features can not be found.
   */
  class FeaturesNotFoundException : public stdair::ObjectNotFoundException {
  public:
    /**
     * Constructor.
     */
    FeaturesNotFoundException (const std::string& iWhat)
      : stdair::ObjectNotFoundException (iWhat) {}
  };
  
  /**
   * The airline can not be found.
   */
  class AirlineNotFoundException : public stdair::ObjectNotFoundException {
  public:
    /**
     * Constructor.
     */
    AirlineNotFoundException (const std::string& iWhat)
      : stdair::ObjectNotFoundException (iWhat) {}
  };

  /**
   * The fare input file can not be found.
   */
  class FareInputFileNotFoundException : public stdair::FileNotFoundException {
  public:
    /**
     * Constructor.
     */
    FareInputFileNotFoundException (const std::string& iWhat)
      : stdair::FileNotFoundException (iWhat) {}
  };

  /**
   * The pricing operation fails.
   */
  class QuotingException : public stdair::RootException {
  };


  // //////// Type definitions specific to SimFQT /////////
  /**
   * ID for the Fare Quote system.
   */
  typedef unsigned int FareQuoteID_T;

  /**
   * (Smart) Pointer on the SimFQT service handler.
   */
  typedef boost::shared_ptr<SIMFQT_Service> SIMFQT_ServicePtr_T;
}
#endif // __SIMFQT_SIMFQT_TYPES_HPP
