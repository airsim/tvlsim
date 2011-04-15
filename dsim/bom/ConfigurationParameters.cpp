// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <sstream>
#include <cassert>
// DSIM 
#include <dsim/bom/ConfigurationParameters.hpp>

namespace DSIM {
  // //////////////////////////////////////////////////////////////////////
  ConfigurationParameters::ConfigurationParameters() {
    assert (false);
  }
    
  // //////////////////////////////////////////////////////////////////////
  ConfigurationParameters::
  ConfigurationParameters (const ConfigurationParameters& iSimulationContext)
    : _startDate (iSimulationContext._startDate),
      _endDate (iSimulationContext._endDate) {
  }
  
  // //////////////////////////////////////////////////////////////////////
  ConfigurationParameters::
  ConfigurationParameters (const stdair::Date_T& iStartDate,
                           const stdair::Date_T& iEndDate)
    : _startDate (iStartDate), _endDate (iEndDate) {
  }
  
  // //////////////////////////////////////////////////////////////////////
  ConfigurationParameters::~ConfigurationParameters() {
  }
  
  // //////////////////////////////////////////////////////////////////////
  void ConfigurationParameters::toStream (std::ostream& ioOut) const {
    ioOut << describe();
  }

  // //////////////////////////////////////////////////////////////////////
  void ConfigurationParameters::fromStream (std::istream& ioIn) {
  }
  
  // //////////////////////////////////////////////////////////////////////
  const std::string ConfigurationParameters::describe() const {
    std::ostringstream oStr;

    oStr << "Start date: " << _startDate
         << ", end date: " << _endDate;
    
    return oStr.str();
  }
}
