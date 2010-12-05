// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// StdAir
#include <stdair/STDAIR_Service.hpp>
// AirSched
#include <airsched/basic/BasConst_AIRSCHED_Service.hpp>
#include <airsched/factory/FacSupervisor.hpp>
#include <airsched/service/AIRSCHED_ServiceContext.hpp>
#include <airsched/command/FileMgr.hpp>

namespace AIRSCHED {
  
  // //////////////////////////////////////////////////////////////////////
  AIRSCHED_ServiceContext::
  AIRSCHED_ServiceContext (const AIRSCHED_ServiceContext&) {
    assert (false);
  }

  // //////////////////////////////////////////////////////////////////////
  AIRSCHED_ServiceContext::AIRSCHED_ServiceContext () {
    init ();
  }
  
  // //////////////////////////////////////////////////////////////////////
  AIRSCHED_ServiceContext::~AIRSCHED_ServiceContext() {
  }

  // //////////////////////////////////////////////////////////////////////
  void AIRSCHED_ServiceContext::init () {
  }

  // //////////////////////////////////////////////////////////////////////
  const std::string AIRSCHED_ServiceContext::shortDisplay() const {
    std::ostringstream oStr;
    oStr << "AIRSCHED_ServiceContext: " << std::endl;
    return oStr.str();
  }

  // //////////////////////////////////////////////////////////////////////
  const std::string AIRSCHED_ServiceContext::display() const {
    std::ostringstream oStr;
    oStr << shortDisplay();
    return oStr.str();
  }

  // //////////////////////////////////////////////////////////////////////
  void AIRSCHED_ServiceContext::
  readFromInputFile (const std::string& iInputFileName) {

    FileMgr::readAndProcessTravelSolutionInputFile (iInputFileName);
  }

}
