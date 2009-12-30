// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <sstream>
#include <cassert>
// DSIM 
#include <dsim/bom/ConfigurationParameters.hpp>
#include <dsim/service/Logger.hpp>

namespace DSIM {

  // ////////////////////////////////////////////////////////////////////
  ConfigurationParameters::ConfigurationParameters () {
  }

  // ////////////////////////////////////////////////////////////////////
  ConfigurationParameters::
  ConfigurationParameters (const ConfigurationParameters& iConfigurationParameters) {
  }

  // ////////////////////////////////////////////////////////////////////
  ConfigurationParameters::~ConfigurationParameters () {
  }

  // ////////////////////////////////////////////////////////////////////
  const std::string ConfigurationParameters::describe() const {
    // Store current formatting flags of std::cout
    std::ios::fmtflags oldFlags = std::cout.flags();
      
    std::ostringstream ostr;

    // Reset formatting flags of std::cout
    std::cout.flags (oldFlags);

    return ostr.str();
  }

}
