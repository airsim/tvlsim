// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// StdAir
#include <stdair/service/FacSupervisor.hpp>
// AirSched
#include <airsched/factory/FacAIRSCHEDServiceContext.hpp>
#include <airsched/service/AIRSCHED_ServiceContext.hpp>

namespace AIRSCHED {

  FacAIRSCHEDServiceContext* FacAIRSCHEDServiceContext::_instance = NULL;

  // //////////////////////////////////////////////////////////////////////
  FacAIRSCHEDServiceContext::~FacAIRSCHEDServiceContext() {
    _instance = NULL;
  }

  // //////////////////////////////////////////////////////////////////////
  FacAIRSCHEDServiceContext& FacAIRSCHEDServiceContext::instance () {

    if (_instance == NULL) {
      _instance = new FacAIRSCHEDServiceContext();
      assert (_instance != NULL);
      
      stdair::FacSupervisor::instance().registerServiceFactory (_instance);
    }
    return *_instance;
  }

  // //////////////////////////////////////////////////////////////////////
  AIRSCHED_ServiceContext& FacAIRSCHEDServiceContext::create () {
    AIRSCHED_ServiceContext* aServiceContext_ptr = NULL;

    aServiceContext_ptr = new AIRSCHED_ServiceContext();
    assert (aServiceContext_ptr != NULL);

    // The new object is added to the Bom pool
    _pool.push_back (aServiceContext_ptr);

    return *aServiceContext_ptr;
  }

}
