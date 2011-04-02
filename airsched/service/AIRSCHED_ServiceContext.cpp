// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <sstream>
// StdAir
#include <stdair/STDAIR_Service.hpp>
// AirSched
#include <airsched/basic/BasConst_AIRSCHED_Service.hpp>
#include <airsched/service/AIRSCHED_ServiceContext.hpp>
#include <airsched/command/FileMgr.hpp>

namespace AIRSCHED {
  
  // //////////////////////////////////////////////////////////////////////
  AIRSCHED_ServiceContext::AIRSCHED_ServiceContext()
    : _ownStdairService (false) {
  }
  
  // //////////////////////////////////////////////////////////////////////
  AIRSCHED_ServiceContext::
  AIRSCHED_ServiceContext (const AIRSCHED_ServiceContext&) {
    assert (false);
  }

  // //////////////////////////////////////////////////////////////////////
  AIRSCHED_ServiceContext::~AIRSCHED_ServiceContext() {
  }

  // ////////////////////////////////////////////////////////////////////
  stdair::STDAIR_Service& AIRSCHED_ServiceContext::getSTDAIR_Service() const {
    assert (_stdairService != NULL);
    return *_stdairService;
  }

  // //////////////////////////////////////////////////////////////////////
  const std::string AIRSCHED_ServiceContext::shortDisplay() const {
    std::ostringstream oStr;
    oStr << "AIRSCHED_ServiceContext -- Owns StdAir service: "
         << _ownStdairService;
    return oStr.str();
  }

  // //////////////////////////////////////////////////////////////////////
  const std::string AIRSCHED_ServiceContext::display() const {
    std::ostringstream oStr;
    oStr << shortDisplay();
    return oStr.str();
  }

  // //////////////////////////////////////////////////////////////////////
  const std::string AIRSCHED_ServiceContext::describe() const {
    return shortDisplay();
  }

  // ////////////////////////////////////////////////////////////////////
  void AIRSCHED_ServiceContext::reset() {
    if (_ownStdairService == true) {
      _stdairService.reset();
    }
  }

}
