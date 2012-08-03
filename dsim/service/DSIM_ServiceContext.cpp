// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <sstream>
// StdAir
#include <stdair/factory/FacBom.hpp>
// Dsim
#include <dsim/bom/SimulationStatus.hpp>
#include <dsim/basic/BasConst_DSIM_Service.hpp>
#include <dsim/service/DSIM_ServiceContext.hpp>

namespace DSIM {

  // //////////////////////////////////////////////////////////////////////
  DSIM_ServiceContext::DSIM_ServiceContext (const DSIM_ServiceContext&)
    : _ownStdairService (false),
      _simulationStatus (NULL) {
    assert (false);
  }

  // //////////////////////////////////////////////////////////////////////
  DSIM_ServiceContext::DSIM_ServiceContext ()
    : _simulatorID (DEFAULT_DSIM_ID),
      _simulationStatus (NULL) {
    initSimulationStatus (DEFAULT_DSIM_ID);
  }
  

  // //////////////////////////////////////////////////////////////////////
  DSIM_ServiceContext::DSIM_ServiceContext (const SimulatorID_T& iSimulatorID)
    : _simulatorID (iSimulatorID),
      _simulationStatus (NULL) {
    initSimulationStatus (iSimulatorID);
  }

  // //////////////////////////////////////////////////////////////////////
  DSIM_ServiceContext::~DSIM_ServiceContext() {
  }


  // //////////////////////////////////////////////////////////////////////
  void DSIM_ServiceContext::initSimulationStatus (const SimulatorID_T& iSimulatorID) {

    // Create a Simulation Status key
    const SimulationStatusKey lKey (iSimulatorID);

    // Create a Simulation Status object instance
    SimulationStatus& lSimulationStatus =
      stdair::FacBom<SimulationStatus>::instance().create (lKey);

    // Store the event queue object
    _simulationStatus = &lSimulationStatus;
  }

  // //////////////////////////////////////////////////////////////////////
  const std::string DSIM_ServiceContext::shortDisplay() const {
    std::ostringstream oStr;
    oStr << "DSIM_ServiceContext [" << _simulatorID
         << "] -- Owns StdAir service: " << _ownStdairService
         << ". Configuration parameters: "
         << _simulationStatus->describe();
    return oStr.str();
  }

  // //////////////////////////////////////////////////////////////////////
  const std::string DSIM_ServiceContext::display() const {
    std::ostringstream oStr;
    oStr << shortDisplay();
    return oStr.str();
  }

  // //////////////////////////////////////////////////////////////////////
  const std::string DSIM_ServiceContext::describe() const {
    return shortDisplay();
  }

  // //////////////////////////////////////////////////////////////////////
  void DSIM_ServiceContext::reset() {
    if (_ownStdairService == true) {
      _stdairService.reset();
    }
  }

}
