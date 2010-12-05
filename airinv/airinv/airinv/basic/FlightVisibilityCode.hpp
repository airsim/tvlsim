#ifndef __AIRINV_BAS_FLIGHTVISIBILITYCODE_HPP
#define __AIRINV_BAS_FLIGHTVISIBILITYCODE_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
// StdAir
#include <stdair/basic/StructAbstract.hpp>

namespace AIRINV {

  /** Enumeration of flight visibility codes. */
  struct FlightVisibilityCode : public stdair::StructAbstract {
  public:
    typedef enum { 
      NORMAL = 0,
      HIDDEN,
      PSEUDO,
      LAST_VALUE
    } EN_FlightVisibilityCode;

    /** Get the label as a string. */
    static const std::string& getLabel (const EN_FlightVisibilityCode&);

    /** Get the label as a single char. */
    static const std::string& getCodeLabel (const EN_FlightVisibilityCode&);

    /** List the labels. */
    static std::string describeLabels();

    /** Get the enumerated value. */
    EN_FlightVisibilityCode getCode() const;

    /** Give a description of the structure (for display purposes). */
    const std::string describe() const;

    
  public:
    /** Constructor. */
    FlightVisibilityCode (const EN_FlightVisibilityCode&);
    /** Constructor. */
    FlightVisibilityCode (const std::string& iCode);

    
  private:
    /** String version of the enumeration. */    
    static const std::string _labels[LAST_VALUE];
    /** Code version of the enumeration. */    
    static const std::string _codeLabels[LAST_VALUE];


  private:
    // //////// Attributes /////////
    /** FlightVisibility code. */
    EN_FlightVisibilityCode _code;
  };

}
#endif // __AIRINV_BAS_FLIGHTVISIBILITYCODE_HPP
