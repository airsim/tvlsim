// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <sstream>
// StdAir
#include <stdair/service/Logger.hpp>
// Airinv
#include <airinv/AIRINV_Types.hpp>
#include <airinv/basic/FlightVisibilityCode.hpp>

namespace AIRINV {
  
  // //////////////////////////////////////////////////////////////////////
  const std::string FlightVisibilityCode::_labels[LAST_VALUE] =
    { "Normal", "Hidden", "Pseudo"};

  const std::string FlightVisibilityCode::_codeLabels[LAST_VALUE] =
    { "NOR", "HID", "PSD" };

  
  // //////////////////////////////////////////////////////////////////////
  FlightVisibilityCode::
  FlightVisibilityCode (const EN_FlightVisibilityCode& iFlightVisibilityCode)
    : _code (iFlightVisibilityCode) {
  }

  // //////////////////////////////////////////////////////////////////////
  FlightVisibilityCode::FlightVisibilityCode (const std::string& iCode) {
    _code = LAST_VALUE;
    
    if (iCode == "NOR") {
      _code = NORMAL;

    } else if (iCode == "HID") {
      _code = HIDDEN;

    } else if (iCode == "PSD") {
      _code = PSEUDO;
    }

    if (_code == LAST_VALUE) {
      const std::string& lLabels = describeLabels();
      STDAIR_LOG_ERROR ("The flight visibility code '" << iCode
                        << "' is not known. Known flight visibility codes: "
                        << lLabels);
      throw stdair::CodeConversionException ("The flight visibility code '"
                                             + iCode
                                             + "' is not known. Known flight visibility codes: "
                                             + lLabels);
    }
  }
  
  // //////////////////////////////////////////////////////////////////////
  const std::string& FlightVisibilityCode::
  getLabel (const EN_FlightVisibilityCode& iCode) {
    return _labels[iCode];
  }
  
  // //////////////////////////////////////////////////////////////////////
  const std::string& FlightVisibilityCode::
  getCodeLabel (const EN_FlightVisibilityCode& iCode) {
    return _codeLabels[iCode];
  }

  // //////////////////////////////////////////////////////////////////////
  std::string FlightVisibilityCode::describeLabels() {
    std::ostringstream ostr;
    for (unsigned short idx = 0; idx != LAST_VALUE; ++idx) {
      if (idx != 0) {
        ostr << ", ";
      }
      ostr << _labels[idx];
    }
    return ostr.str();
  }

  // //////////////////////////////////////////////////////////////////////
  FlightVisibilityCode::EN_FlightVisibilityCode FlightVisibilityCode::
  getCode() const {
    return _code;
  }
  
  // //////////////////////////////////////////////////////////////////////
  const std::string FlightVisibilityCode::describe() const {
    std::ostringstream ostr;
    ostr << _labels[_code];
    return ostr.str();
  }

}
