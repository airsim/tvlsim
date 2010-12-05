// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// TRAVELCCM
#include <travelccm/service/TRAVELCCM_ServiceContext.hpp>
#include <travelccm/factory/FacSupervisor.hpp>
#include <travelccm/factory/FacTRAVELCCMServiceContext.hpp>

namespace TRAVELCCM {

  FacTRAVELCCMServiceContext* FacTRAVELCCMServiceContext::_instance = NULL;

  // //////////////////////////////////////////////////////////////////////
  FacTRAVELCCMServiceContext::~FacTRAVELCCMServiceContext () {
    _instance = NULL;
  }

  // //////////////////////////////////////////////////////////////////////
  FacTRAVELCCMServiceContext& FacTRAVELCCMServiceContext::instance () {

    if (_instance == NULL) {
      _instance = new FacTRAVELCCMServiceContext();
      assert (_instance != NULL);
      
      FacSupervisor::instance().registerServiceFactory (_instance);
    }
    return *_instance;
  }

  // //////////////////////////////////////////////////////////////////////
  TRAVELCCM_ServiceContext& FacTRAVELCCMServiceContext::
  create (const TravelCCMType& iCCMType) {
    TRAVELCCM_ServiceContext* aServiceContext_ptr = NULL;

    aServiceContext_ptr = new TRAVELCCM_ServiceContext (iCCMType);
    assert (aServiceContext_ptr != NULL);

    // The new object is added to the Bom pool
    _pool.push_back (aServiceContext_ptr);

    return *aServiceContext_ptr;
  }

}
