// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <sstream>
// Dsim
#include <dsim/basic/BasConst_DSIM_Service.hpp>
#include <dsim/service/DSIM_ServiceContext.hpp>

namespace DSIM {

  // //////////////////////////////////////////////////////////////////////
  DSIM_ServiceContext::DSIM_ServiceContext ()
    : _simulatorID (DEFAULT_DSIM_ID) {
  }

  // //////////////////////////////////////////////////////////////////////
  DSIM_ServiceContext::
  DSIM_ServiceContext (const SimulatorID_T& iSimulatorID)
    : _simulatorID (iSimulatorID) {
  }

  // //////////////////////////////////////////////////////////////////////
  DSIM_ServiceContext::~DSIM_ServiceContext() {
  }
  
  // //////////////////////////////////////////////////////////////////////
  const std::string DSIM_ServiceContext::shortDisplay() const {
    std::ostringstream oStr;
    oStr << "DSIM_ServiceContext: Simulator ID: " << _simulatorID;
    return oStr.str();
  }

  // //////////////////////////////////////////////////////////////////////
  const std::string DSIM_ServiceContext::display() const {
    std::ostringstream oStr;
    oStr << shortDisplay();
    return oStr.str();
  }

}
