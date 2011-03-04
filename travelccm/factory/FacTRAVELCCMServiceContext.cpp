// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// TRAVELCCM Common
#include <travelccm/factory/FacSupervisor.hpp>
#include <travelccm/factory/FacTRAVELCCMServiceContext.hpp>
#include <travelccm/service/TRAVELCCM_ServiceContext.hpp>

namespace TRAVELCCM {

  FacTRAVELCCMServiceContext* FacTRAVELCCMServiceContext::_instance = NULL;

  // ////////////////////////////////////////////////////////////////////
  FacTRAVELCCMServiceContext::~FacTRAVELCCMServiceContext () {
    _instance = NULL;
  }

  // ////////////////////////////////////////////////////////////////////
  FacTRAVELCCMServiceContext& FacTRAVELCCMServiceContext::instance () {

    if (_instance == NULL) {
      _instance = new FacTRAVELCCMServiceContext();
      assert (_instance != NULL);
      
      FacSupervisor::instance().registerServiceFactory (_instance);
    }
    return *_instance;
  }

  // ////////////////////////////////////////////////////////////////////
  TRAVELCCM_ServiceContext& FacTRAVELCCMServiceContext::create () {
    TRAVELCCM_ServiceContext* aServiceContext_ptr = NULL;

    aServiceContext_ptr = new TRAVELCCM_ServiceContext ();
    assert (aServiceContext_ptr != NULL);

    // The new object is added to the Bom pool
    _pool.push_back (aServiceContext_ptr);

    return *aServiceContext_ptr;
  }

}
