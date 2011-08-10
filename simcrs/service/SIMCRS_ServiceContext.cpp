// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// Standard Airline Object Model
#include <stdair/STDAIR_Service.hpp>
#include <stdair/service/Logger.hpp>
// Simcrs
#include <simcrs/basic/BasConst_SIMCRS_Service.hpp>
#include <simcrs/service/SIMCRS_ServiceContext.hpp>

namespace SIMCRS {

  // //////////////////////////////////////////////////////////////////////
  SIMCRS_ServiceContext::SIMCRS_ServiceContext ()
    : _ownStdairService (false), _CRSCode (DEFAULT_CRS_CODE) {
  }

  // //////////////////////////////////////////////////////////////////////
  SIMCRS_ServiceContext::SIMCRS_ServiceContext (const SIMCRS_ServiceContext&)
    : _ownStdairService (false) {
  }

  // //////////////////////////////////////////////////////////////////////
  SIMCRS_ServiceContext::SIMCRS_ServiceContext (const CRSCode_T& iCRSCode)
    : _CRSCode (iCRSCode) {
  }

  // //////////////////////////////////////////////////////////////////////
  SIMCRS_ServiceContext::~SIMCRS_ServiceContext() {
  }
  
  // //////////////////////////////////////////////////////////////////////
  const std::string SIMCRS_ServiceContext::shortDisplay() const {
    std::ostringstream oStr;
    oStr << "SIMCRS_ServiceContext [" << _CRSCode
         << "] - Owns StdAir service: " << _ownStdairService;
    return oStr.str();
  }

  // //////////////////////////////////////////////////////////////////////
  const std::string SIMCRS_ServiceContext::display() const {
    std::ostringstream oStr;
    oStr << shortDisplay();
    return oStr.str();
  }

  // //////////////////////////////////////////////////////////////////////
  const std::string SIMCRS_ServiceContext::describe() const {
    return shortDisplay();
  }

  // //////////////////////////////////////////////////////////////////////
  void SIMCRS_ServiceContext::reset() {
    if (_ownStdairService == true) {
      _stdairService.reset();
    }
  }

}
