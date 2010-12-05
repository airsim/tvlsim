// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <sstream>
// StdAir
#include <stdair/STDAIR_Service.hpp>
#include <stdair/basic/BasConst_General.hpp>
// Trademgen
#include <trademgen/basic/BasConst_DemandGeneration.hpp>
#include <trademgen/service/TRADEMGEN_ServiceContext.hpp>

namespace TRADEMGEN {

  // //////////////////////////////////////////////////////////////////////
  TRADEMGEN_ServiceContext::TRADEMGEN_ServiceContext ()
    : _seed (stdair::DEFAULT_RANDOM_SEED), _generator (_seed),
      _uniformGenerator (_generator, boost::uniform_real<> (0, 1)),
      _posProbabilityMass (DEFAULT_POS_PROBALILITY_MASS) {
  }

  // //////////////////////////////////////////////////////////////////////
  TRADEMGEN_ServiceContext::
  TRADEMGEN_ServiceContext (const std::string& iServiceName) 
    : _seed (stdair::DEFAULT_RANDOM_SEED), _generator (_seed),
      _uniformGenerator (_generator, boost::uniform_real<> (0, 1)),
      _posProbabilityMass (DEFAULT_POS_PROBALILITY_MASS) {
  }

  // //////////////////////////////////////////////////////////////////////
  TRADEMGEN_ServiceContext::~TRADEMGEN_ServiceContext() {
  }
  
  // //////////////////////////////////////////////////////////////////////
  const std::string TRADEMGEN_ServiceContext::shortDisplay() const {
    std::ostringstream oStr;
    oStr << "TRADEMGEN_ServiceContext: " << std::endl
         << std::endl;
    return oStr.str();
  }

  // //////////////////////////////////////////////////////////////////////
  const std::string TRADEMGEN_ServiceContext::display() const {
    std::ostringstream oStr;
    oStr << shortDisplay();
    return oStr.str();
  }

}
