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
#include <airinv/FlightRequestStatus.hpp>

namespace AIRINV {
  
  // //////////////////////////////////////////////////////////////////////
  const std::string FlightRequestStatus::_labels[LAST_VALUE] =
    { "OK", "Not Found", "Internal Error"};

  const std::string FlightRequestStatus::_codeLabels[LAST_VALUE] =
    { "OK", "NF", "IE" };

  
  // //////////////////////////////////////////////////////////////////////
  FlightRequestStatus::
  FlightRequestStatus (const EN_FlightRequestStatus& iFlightRequestStatus)
    : _code (iFlightRequestStatus) {
  }

  // //////////////////////////////////////////////////////////////////////
  FlightRequestStatus::FlightRequestStatus (const std::string& iCode) {
    _code = LAST_VALUE;
    
    if (iCode == "OK") {
      _code = OK;

    } else if (iCode == "NF") {
      _code = NOT_FOUND;

    } else if (iCode == "IE") {
      _code = INTERNAL_ERROR;

    }

    if (_code == LAST_VALUE) {
      const std::string& lLabels = describeLabels();
      STDAIR_LOG_ERROR ("The flight request status '" << iCode
                        << "' is not known. Known flight request status: "
                        << lLabels);
      throw stdair::CodeConversionException ("The flight request status '"
                                             + iCode
                                             + "' is not known. Known flight request status: "
                                             + lLabels);
    }
  }
  
  // //////////////////////////////////////////////////////////////////////
  const std::string& FlightRequestStatus::
  getLabel (const EN_FlightRequestStatus& iCode) {
    return _labels[iCode];
  }
  
  // //////////////////////////////////////////////////////////////////////
  const std::string& FlightRequestStatus::
  getCodeLabel (const EN_FlightRequestStatus& iCode) {
    return _codeLabels[iCode];
  }

  // //////////////////////////////////////////////////////////////////////
  std::string FlightRequestStatus::describeLabels() {
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
  FlightRequestStatus::EN_FlightRequestStatus FlightRequestStatus::
  getCode() const {
    return _code;
  }
  
  // //////////////////////////////////////////////////////////////////////
  const std::string FlightRequestStatus::describe() const {
    std::ostringstream ostr;
    ostr << _labels[_code];
    return ostr.str();
  }

}
