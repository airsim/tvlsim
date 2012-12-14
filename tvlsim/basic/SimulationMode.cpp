// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <sstream>
// StdAir
#include <stdair/stdair_exceptions.hpp>
// TvlSim
#include <tvlsim/basic/SimulationMode.hpp>

namespace TVLSIM {
  
  // //////////////////////////////////////////////////////////////////////
  const std::string SimulationMode::_labels[LAST_VALUE] =
    { "start", "running", "break", "done" };

  // //////////////////////////////////////////////////////////////////////
  const char SimulationMode::_modeLabels[LAST_VALUE] = { 's', 'r', 'b', 'd'};

  // //////////////////////////////////////////////////////////////////////
  SimulationMode::SimulationMode()
    : _mode (LAST_VALUE) {
    assert (false);
  }

  // //////////////////////////////////////////////////////////////////////
  SimulationMode::
  SimulationMode (const SimulationMode& iSimulationMode)
    : _mode (iSimulationMode._mode) {
  }

  // //////////////////////////////////////////////////////////////////////
  SimulationMode::EN_SimulationMode
  SimulationMode::getMode (const char iModeChar) {
 
    EN_SimulationMode oSimulationMode;
    switch (iModeChar) { 
    case 's': oSimulationMode = START; break;
    case 'r': oSimulationMode = RUNNING; break;
    case 'b': oSimulationMode = BREAK; break;
    case 'd': oSimulationMode = DONE; break;
    default: oSimulationMode = LAST_VALUE; break;
    }

    if (oSimulationMode == LAST_VALUE) {
      const std::string& lLabels = describeLabels();
      std::ostringstream oMessage;
      oMessage << "The simulation mode '" << iModeChar
               << "' is not known. Known simulation modes: " << lLabels;
      throw stdair::CodeConversionException (oMessage.str());
    }

    return oSimulationMode;
  }

  
  // //////////////////////////////////////////////////////////////////////
  SimulationMode::SimulationMode (const char iModeChar) 
    : _mode (getMode (iModeChar)) {
  }
  
  // //////////////////////////////////////////////////////////////////////
  SimulationMode::SimulationMode (const std::string& iModeStr) {
    // 
    const size_t lSize = iModeStr.size();
    assert (lSize == 1);
    const char lModeChar = iModeStr[0];
    _mode = getMode (lModeChar);
  }

  // //////////////////////////////////////////////////////////////////////
  const std::string& SimulationMode::
  getLabel (const EN_SimulationMode& iMode) {
    return _labels[iMode];
  }
  
  // //////////////////////////////////////////////////////////////////////
  char SimulationMode::getModeLabel (const EN_SimulationMode& iMode) {
    return _modeLabels[iMode];
  }

  // //////////////////////////////////////////////////////////////////////
  std::string SimulationMode::
  getModeLabelAsString (const EN_SimulationMode& iMode) {
    std::ostringstream oStr;
    oStr << _modeLabels[iMode];
    return oStr.str();
  }

  // //////////////////////////////////////////////////////////////////////
  std::string SimulationMode::describeLabels() {
    std::ostringstream ostr;
    for (unsigned short idx = 0; idx != LAST_VALUE; ++idx) {
      if (idx != 0) {
        ostr << ", ";
      }
      ostr << _labels[idx] << " '" << _modeLabels[idx] << "'";
    }
    return ostr.str();
  }

  // //////////////////////////////////////////////////////////////////////
  SimulationMode::EN_SimulationMode SimulationMode::getMode() const {
    return _mode;
  }
  
  // //////////////////////////////////////////////////////////////////////
  std::string SimulationMode::getModeAsString() const {
    std::ostringstream oStr;
    oStr << _modeLabels[_mode];
    return oStr.str();
  }

  // //////////////////////////////////////////////////////////////////////
  char SimulationMode::getModeAsChar() const {
    const char oModeChar = _modeLabels[_mode];
    return oModeChar;
  }
  
  // //////////////////////////////////////////////////////////////////////
  const std::string SimulationMode::describe() const {
    std::ostringstream ostr;
    ostr << _labels[_mode];
    return ostr.str();
  }

  // //////////////////////////////////////////////////////////////////////
  bool SimulationMode::
  operator== (const EN_SimulationMode& iMode) const {
    return (_mode == iMode);
  }
  
}
