// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <sstream>
// DSim
#include <dsim/basic/BasConst_DSIM_Service.hpp>
#include <dsim/bom/SimulationStatusKey.hpp>

namespace DSIM {

  // ////////////////////////////////////////////////////////////////////
  SimulationStatusKey::SimulationStatusKey (const SimulatorID_T& iSimulatorID,
                                            const stdair::Date_T& iStartDate,
                                            const stdair::Date_T& iEndDate)
    : _simulatorID (iSimulatorID),
      _startDate (iStartDate),
      _endDate (iEndDate) {
  }

  // ////////////////////////////////////////////////////////////////////
  SimulationStatusKey::SimulationStatusKey()
    : _simulatorID (DEFAULT_DSIM_ID),
      _startDate (DEFAULT_SIMULATION_START_DATE),
      _endDate (DEFAULT_SIMULATION_END_DATE) {
    assert (false);
  }
  
  // ////////////////////////////////////////////////////////////////////
  SimulationStatusKey::SimulationStatusKey (const SimulationStatusKey& iKey)
    : _simulatorID (iKey._simulatorID),
      _startDate (iKey._startDate),
      _endDate (iKey._endDate) {
  }

  // ////////////////////////////////////////////////////////////////////
  SimulationStatusKey::~SimulationStatusKey () {
  }

  // ////////////////////////////////////////////////////////////////////
  void SimulationStatusKey::toStream (std::ostream& ioOut) const {
    ioOut << "SimulationStatusKey: " << toString() << std::endl;
  }

  // ////////////////////////////////////////////////////////////////////
  void SimulationStatusKey::fromStream (std::istream& ioIn) {
  }

  // ////////////////////////////////////////////////////////////////////
  const std::string SimulationStatusKey::toString () const {
    std::ostringstream oStr;
    oStr << "(" <<_simulatorID << ")"
         << "[" << _startDate
         << ", " <<_endDate
         << "]";
    return oStr.str();
  }

}
