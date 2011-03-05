// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// StdAir
#include <stdair/service/FacSupervisor.hpp>
// AirInv
#include <airinv/factory/FacAirinvMasterServiceContext.hpp>
#include <airinv/service/AIRINV_Master_ServiceContext.hpp>

namespace AIRINV {

  FacAirinvMasterServiceContext* FacAirinvMasterServiceContext::_instance = NULL;

  // //////////////////////////////////////////////////////////////////////
  FacAirinvMasterServiceContext::~FacAirinvMasterServiceContext() {
    _instance = NULL;
  }

  // //////////////////////////////////////////////////////////////////////
  FacAirinvMasterServiceContext& FacAirinvMasterServiceContext::instance() {

    if (_instance == NULL) {
      _instance = new FacAirinvMasterServiceContext();
      assert (_instance != NULL);
      
      stdair::FacSupervisor::instance().registerServiceFactory (_instance);
    }
    return *_instance;
  }

  // //////////////////////////////////////////////////////////////////////
  AIRINV_Master_ServiceContext& FacAirinvMasterServiceContext::create() {
    AIRINV_Master_ServiceContext* aAIRINV_Master_ServiceContext_ptr = NULL;

    aAIRINV_Master_ServiceContext_ptr = new AIRINV_Master_ServiceContext();
    assert (aAIRINV_Master_ServiceContext_ptr != NULL);

    // The new object is added to the Bom pool
    _pool.push_back (aAIRINV_Master_ServiceContext_ptr);

    return *aAIRINV_Master_ServiceContext_ptr;
  }

}
