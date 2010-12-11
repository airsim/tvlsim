// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// SIMCRS Common
#include <simcrs/factory/FacSupervisor.hpp>
#include <simcrs/factory/FacSimcrsServiceContext.hpp>
#include <simcrs/service/SIMCRS_ServiceContext.hpp>

namespace SIMCRS {

  FacSimcrsServiceContext* FacSimcrsServiceContext::_instance = NULL;

  // //////////////////////////////////////////////////////////////////////
  FacSimcrsServiceContext::~FacSimcrsServiceContext () {
    _instance = NULL;
  }

  // //////////////////////////////////////////////////////////////////////
  FacSimcrsServiceContext& FacSimcrsServiceContext::instance () {

    if (_instance == NULL) {
      _instance = new FacSimcrsServiceContext();
      assert (_instance != NULL);
      
      FacSupervisor::instance().registerServiceFactory (_instance);
    }
    return *_instance;
  }

  // //////////////////////////////////////////////////////////////////////
  SIMCRS_ServiceContext& FacSimcrsServiceContext::
  create (const std::string& iTravelDatabaseName) {
    SIMCRS_ServiceContext* aSIMCRS_ServiceContext_ptr = NULL;

    aSIMCRS_ServiceContext_ptr =
      new SIMCRS_ServiceContext (iTravelDatabaseName);
    assert (aSIMCRS_ServiceContext_ptr != NULL);

    // The new object is added to the Bom pool
    _pool.push_back (aSIMCRS_ServiceContext_ptr);

    return *aSIMCRS_ServiceContext_ptr;
  }

}
