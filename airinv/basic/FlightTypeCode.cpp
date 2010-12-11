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
#include <airinv/basic/FlightTypeCode.hpp>

namespace AIRINV {
  
  // //////////////////////////////////////////////////////////////////////
  const std::string FlightTypeCode::_labels[LAST_VALUE] =
    { "Domestic", "International", "Ground Handling"};

  const std::string FlightTypeCode::_codeLabels[LAST_VALUE] =
    { "DOM", "INT", "GRD" };

  
  // //////////////////////////////////////////////////////////////////////
  FlightTypeCode::FlightTypeCode (const EN_FlightTypeCode& iFlightTypeCode)
    : _code (iFlightTypeCode) {
  }

  // //////////////////////////////////////////////////////////////////////
  FlightTypeCode::FlightTypeCode (const std::string& iCode) {
    _code = LAST_VALUE;
    
    if (iCode == "DOM") {
      _code = DOMESTIC;

    } else if (iCode == "INT") {
      _code = INTERNATIONAL;

    } else if (iCode == "GRD") {
      _code = GROUND_HANDLING;
    }

    if (_code == LAST_VALUE) {
      const std::string& lLabels = describeLabels();
      STDAIR_LOG_ERROR ("The flight type code '" << iCode
                        << "' is not known. Known flight type codes: "
                        << lLabels);
      throw CodeConversionException();
    }
  }
  
  // //////////////////////////////////////////////////////////////////////
  const std::string& FlightTypeCode::getLabel (const EN_FlightTypeCode& iCode) {
    return _labels[iCode];
  }
  
  // //////////////////////////////////////////////////////////////////////
  const std::string& FlightTypeCode::
  getCodeLabel (const EN_FlightTypeCode& iCode) {
    return _codeLabels[iCode];
  }

  // //////////////////////////////////////////////////////////////////////
  std::string FlightTypeCode::describeLabels() {
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
  FlightTypeCode::EN_FlightTypeCode FlightTypeCode::getCode() const {
    return _code;
  }
  
  // //////////////////////////////////////////////////////////////////////
  const std::string FlightTypeCode::describe() const {
    std::ostringstream ostr;
    ostr << _labels[_code];
    return ostr.str();
  }

}
