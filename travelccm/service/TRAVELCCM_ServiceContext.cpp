 // //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// StdAir
#include <stdair/service/Logger.hpp>
// TravelCCM Basic
#include <travelccm/service/TRAVELCCM_ServiceContext.hpp>

namespace TRAVELCCM {
  
  // //////////////////////////////////////////////////////////////////////
  TRAVELCCM_ServiceContext::TRAVELCCM_ServiceContext () {
  }
  
  // //////////////////////////////////////////////////////////////////////
  TRAVELCCM_ServiceContext::
  TRAVELCCM_ServiceContext (const TRAVELCCM_ServiceContext&) {
    assert (false);
  }
  
  // //////////////////////////////////////////////////////////////////////
  TRAVELCCM_ServiceContext::~TRAVELCCM_ServiceContext() {
  }
  
  // //////////////////////////////////////////////////////////////////////
  const std::string TRAVELCCM_ServiceContext::shortDisplay() const {
    std::ostringstream oStr;
    oStr << "TRAVELCCM_ServiceContext: " << std::endl;
    return oStr.str();
  }

  // //////////////////////////////////////////////////////////////////////
  const std::string TRAVELCCM_ServiceContext::display() const {
    std::ostringstream oStr;
    oStr << shortDisplay();
    return oStr.str();
  }
}
