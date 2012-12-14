// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <sstream>
// TvlSim
#include <tvlsim/basic/BasConst_TVLSIM_Service.hpp>
#include <tvlsim/bom/SimulationStatusKey.hpp>

namespace TVLSIM {

  // ////////////////////////////////////////////////////////////////////
  SimulationStatusKey::SimulationStatusKey (const SimulatorID_T& iSimulatorID)
    : _simulatorID (iSimulatorID) {
  }

  // ////////////////////////////////////////////////////////////////////
  SimulationStatusKey::SimulationStatusKey()
    : _simulatorID (DEFAULT_TVLSIM_ID) {
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
