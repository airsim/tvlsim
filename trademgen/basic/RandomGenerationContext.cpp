// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <sstream>
// TraDemGen
#include <trademgen/basic/RandomGenerationContext.hpp>

namespace TRADEMGEN {

  // //////////////////////////////////////////////////////////////////////
  RandomGenerationContext::RandomGenerationContext ()
    : _numberOfRequestsGeneratedSoFar (0),
      _cumulativeProbabilitySoFar (0.0) {
  }
  
  // //////////////////////////////////////////////////////////////////////
  RandomGenerationContext::
  RandomGenerationContext (const RandomGenerationContext& iRGC)
    : _numberOfRequestsGeneratedSoFar (iRGC._numberOfRequestsGeneratedSoFar),
      _cumulativeProbabilitySoFar (iRGC._cumulativeProbabilitySoFar) {
  }
  
  // //////////////////////////////////////////////////////////////////////
  RandomGenerationContext::~RandomGenerationContext() {
  }
    
  // //////////////////////////////////////////////////////////////////////
  const std::string RandomGenerationContext::describe() const {
    std::ostringstream oStr;
    oStr << _numberOfRequestsGeneratedSoFar
         << " => " << _cumulativeProbabilitySoFar;
    return oStr.str();
  }

  // //////////////////////////////////////////////////////////////////////
  void RandomGenerationContext::incrementGeneratedRequestsCounter() {
    ++_numberOfRequestsGeneratedSoFar;
  }

  // //////////////////////////////////////////////////////////////////////
  void RandomGenerationContext::reset() {
    _cumulativeProbabilitySoFar = 0.0;
    _numberOfRequestsGeneratedSoFar = 0;
  }

}
