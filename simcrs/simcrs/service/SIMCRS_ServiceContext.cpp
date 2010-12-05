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
  SIMCRS_ServiceContext::SIMCRS_ServiceContext () : _CRSCode (DEFAULT_CRS_CODE) {
    assert (false);
  }

  // //////////////////////////////////////////////////////////////////////
  SIMCRS_ServiceContext::SIMCRS_ServiceContext (const SIMCRS_ServiceContext&) {
    assert (false);
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
    oStr << "SIMCRS_ServiceContext: " << std::endl
         << "CRS code: " << _CRSCode
         << std::endl;
    return oStr.str();
  }

  // //////////////////////////////////////////////////////////////////////
  const std::string SIMCRS_ServiceContext::display() const {
    std::ostringstream oStr;
    oStr << shortDisplay();
    return oStr.str();
  }
}
