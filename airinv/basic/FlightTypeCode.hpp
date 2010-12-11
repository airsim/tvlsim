#ifndef __AIRINV_BAS_FLIGHTTYPECODE_HPP
#define __AIRINV_BAS_FLIGHTTYPECODE_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
// StdAir
#include <stdair/basic/StructAbstract.hpp>

namespace AIRINV {

  /** Enumeration of flight type codes. */
  struct FlightTypeCode : public stdair::StructAbstract {
  public:
    typedef enum { 
      DOMESTIC = 0,
      INTERNATIONAL,
      GROUND_HANDLING,
      LAST_VALUE
    } EN_FlightTypeCode;

    /** Get the label as a string. */
    static const std::string& getLabel (const EN_FlightTypeCode&);

    /** Get the label as a single char. */
    static const std::string& getCodeLabel (const EN_FlightTypeCode&);

    /** List the labels. */
    static std::string describeLabels();

    /** Get the enumerated value. */
    EN_FlightTypeCode getCode() const;

    /** Give a description of the structure (for display purposes). */
    const std::string describe() const;

    
  public:
    /** Constructor. */
    FlightTypeCode (const EN_FlightTypeCode&);
    /** Constructor. */
    FlightTypeCode (const std::string& iCode);

    
  private:
    /** String version of the enumeration. */    
    static const std::string _labels[LAST_VALUE];
    /** Code version of the enumeration. */    
    static const std::string _codeLabels[LAST_VALUE];


  private:
    // //////// Attributes /////////
    /** FlightType code. */
    EN_FlightTypeCode _code;
  };

}
#endif // __AIRINV_BAS_FLIGHTTYPECODE_HPP
