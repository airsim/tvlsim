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
    : _ownStdairService (false),
      _configurationParameters (DEFAULT_SIMULATION_START_DATE,
                                DEFAULT_SIMULATION_END_DATE) {
  }

  // //////////////////////////////////////////////////////////////////////
  DSIM_ServiceContext::DSIM_ServiceContext (const DSIM_ServiceContext&)
    : _ownStdairService (false),
      _configurationParameters (DEFAULT_SIMULATION_START_DATE,
                                DEFAULT_SIMULATION_END_DATE) {
  }

  // //////////////////////////////////////////////////////////////////////
  DSIM_ServiceContext::DSIM_ServiceContext (const stdair::Date_T& iStartDate,
                                            const stdair::Date_T& iEndDate)
    : _simulatorID (DEFAULT_DSIM_ID),
      _configurationParameters (iStartDate, iEndDate) {
  }

  // //////////////////////////////////////////////////////////////////////
  DSIM_ServiceContext::DSIM_ServiceContext (const SimulatorID_T& iSimulatorID,
                                            const stdair::Date_T& iStartDate,
                                            const stdair::Date_T& iEndDate)
    : _simulatorID (iSimulatorID),
      _configurationParameters (iStartDate, iEndDate) {
  }

  // //////////////////////////////////////////////////////////////////////
  DSIM_ServiceContext::~DSIM_ServiceContext() {
  }
  
  // //////////////////////////////////////////////////////////////////////
  const std::string DSIM_ServiceContext::shortDisplay() const {
    std::ostringstream oStr;
    oStr << "DSIM_ServiceContext [" << _simulatorID
         << "] -- Owns StdAir service: " << _ownStdairService
         << ". Configuration parameters: "
         << _configurationParameters.describe();
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
