// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <sstream>
#include <cassert>
// DSIM 
#include <dsim/bom/RDSParameters.hpp>
#include <dsim/service/Logger.hpp>

namespace DSIM {

  // ////////////////////////////////////////////////////////////////////
  RDSParameters::RDSParameters () {
  }
  
  // ////////////////////////////////////////////////////////////////////
  RDSParameters::RDSParameters (const RDSParameters& iRDSParameters)
    : _scheduleInputFilename (iRDSParameters._scheduleInputFilename) {
  }

  // ////////////////////////////////////////////////////////////////////
  RDSParameters::~RDSParameters () {
  }

  // ////////////////////////////////////////////////////////////////////
  const std::string RDSParameters::describe() const {
    // Store current formatting flags of std::cout
    std::ios::fmtflags oldFlags = std::cout.flags();
      
    std::ostringstream ostr;

    ostr << "RDS Parameters: " << std::endl;

    ostr << " Schedule file: " << _scheduleInputFilename;
    ostr << std::endl;
    
    // Reset formatting flags of std::cout
    std::cout.flags (oldFlags);

    return ostr.str();
  }

}
