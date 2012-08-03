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
  SimulationStatusKey::SimulationStatusKey (const SimulatorID_T& iSimulatorID)
    : _simulatorID (iSimulatorID) {
  }

  // ////////////////////////////////////////////////////////////////////
  SimulationStatusKey::SimulationStatusKey()
    : _simulatorID (DEFAULT_DSIM_ID) {
    assert (false);
  }
  
  // ////////////////////////////////////////////////////////////////////
  SimulationStatusKey::SimulationStatusKey (const SimulationStatusKey& iKey)
    : _simulatorID (iKey._simulatorID) {
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
    oStr << "(" <<_simulatorID << ")";
    return oStr.str();
  }

}
