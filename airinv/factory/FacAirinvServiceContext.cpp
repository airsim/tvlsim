// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// StdAir
#include <stdair/service/FacSupervisor.hpp>
// AirInv
#include <airinv/factory/FacAirinvServiceContext.hpp>
#include <airinv/service/AIRINV_ServiceContext.hpp>

namespace AIRINV {

  FacAirinvServiceContext* FacAirinvServiceContext::_instance = NULL;

  // //////////////////////////////////////////////////////////////////////
  FacAirinvServiceContext::~FacAirinvServiceContext() {
    _instance = NULL;
  }

  // //////////////////////////////////////////////////////////////////////
  FacAirinvServiceContext& FacAirinvServiceContext::instance() {

    if (_instance == NULL) {
      _instance = new FacAirinvServiceContext();
      assert (_instance != NULL);
      
      stdair::FacSupervisor::instance().registerServiceFactory (_instance);
    }
    return *_instance;
  }

  // //////////////////////////////////////////////////////////////////////
  AIRINV_ServiceContext& FacAirinvServiceContext::create() {
    AIRINV_ServiceContext* aAIRINV_ServiceContext_ptr = NULL;

    aAIRINV_ServiceContext_ptr = new AIRINV_ServiceContext();
    assert (aAIRINV_ServiceContext_ptr != NULL);

    // The new object is added to the Bom pool
    _pool.push_back (aAIRINV_ServiceContext_ptr);

    return *aAIRINV_ServiceContext_ptr;
  }

}
