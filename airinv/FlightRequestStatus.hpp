#ifndef __AIRINV_BAS_FLIGHTREQUESTSTATUS_HPP
#define __AIRINV_BAS_FLIGHTREQUESTSTATUS_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
// StdAir
#include <stdair/basic/StructAbstract.hpp>

namespace AIRINV {

  /** Enumeration of flight type codes. */
  struct FlightRequestStatus : public stdair::StructAbstract {
  public:
    typedef enum { 
      OK = 0,
      NOT_FOUND,
      INTERNAL_ERROR,
      LAST_VALUE
    } EN_FlightRequestStatus;

    /** Get the label as a string. */
    static const std::string& getLabel (const EN_FlightRequestStatus&);

    /** Get the label as a single char. */
    static const std::string& getCodeLabel (const EN_FlightRequestStatus&);

    /** List the labels. */
    static std::string describeLabels();

    /** Get the enumerated value. */
    EN_FlightRequestStatus getCode() const;

    /** Give a description of the structure (for display purposes). */
    const std::string describe() const;

    
  public:
    /** Constructor. */
    FlightRequestStatus (const EN_FlightRequestStatus&);
    /** Constructor. */
    FlightRequestStatus (const std::string& iCode);

    
  private:
    /** String version of the enumeration. */    
    static const std::string _labels[LAST_VALUE];
    /** Code version of the enumeration. */    
    static const std::string _codeLabels[LAST_VALUE];


  private:
    // //////// Attributes /////////
    /** FlightType code. */
    EN_FlightRequestStatus _code;
  };

}
#endif // __AIRINV_BAS_FLIGHTREQUESTSTATUS_HPP
