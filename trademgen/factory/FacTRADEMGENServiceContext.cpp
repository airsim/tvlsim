// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// TRADEMGEN Common
#include <trademgen/factory/FacSupervisor.hpp>
#include <trademgen/factory/FacTRADEMGENServiceContext.hpp>
#include <trademgen/service/TRADEMGEN_ServiceContext.hpp>

namespace TRADEMGEN {

  FacTRADEMGENServiceContext* FacTRADEMGENServiceContext::_instance = NULL;

  // ////////////////////////////////////////////////////////////////////
  FacTRADEMGENServiceContext::~FacTRADEMGENServiceContext () {
    _instance = NULL;
  }

  // ////////////////////////////////////////////////////////////////////
  FacTRADEMGENServiceContext& FacTRADEMGENServiceContext::instance () {

    if (_instance == NULL) {
      _instance = new FacTRADEMGENServiceContext();
      assert (_instance != NULL);
      
      FacSupervisor::instance().registerServiceFactory (_instance);
    }
    return *_instance;
  }

  // ////////////////////////////////////////////////////////////////////
  TRADEMGEN_ServiceContext& FacTRADEMGENServiceContext::create () {
    TRADEMGEN_ServiceContext* aServiceContext_ptr = NULL;

    aServiceContext_ptr = new TRADEMGEN_ServiceContext ();
    assert (aServiceContext_ptr != NULL);

    // The new object is added to the Bom pool
    _pool.push_back (aServiceContext_ptr);

    return *aServiceContext_ptr;
  }

}
