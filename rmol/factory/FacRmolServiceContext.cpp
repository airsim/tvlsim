// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// RMOL
#include <rmol/service/RMOL_ServiceContext.hpp>
#include <rmol/factory/FacSupervisor.hpp>
#include <rmol/factory/FacRmolServiceContext.hpp>

namespace RMOL {

  FacRmolServiceContext* FacRmolServiceContext::_instance = NULL;

  // //////////////////////////////////////////////////////////////////////
  FacRmolServiceContext::~FacRmolServiceContext () {
    _instance = NULL;
  }

  // //////////////////////////////////////////////////////////////////////
  FacRmolServiceContext& FacRmolServiceContext::instance () {

    if (_instance == NULL) {
      _instance = new FacRmolServiceContext();
      assert (_instance != NULL);
      
      FacSupervisor::instance().registerServiceFactory (_instance);
    }
    return *_instance;
  }

  // //////////////////////////////////////////////////////////////////////
  RMOL_ServiceContext& FacRmolServiceContext::
  create (const stdair::AirlineCode_T& iAirlineCode) {
    RMOL_ServiceContext* aServiceContext_ptr = NULL;

    aServiceContext_ptr = new RMOL_ServiceContext (iAirlineCode);
    assert (aServiceContext_ptr != NULL);

    // The new object is added to the Bom pool
    _pool.push_back (aServiceContext_ptr);

    return *aServiceContext_ptr;
  }

  // //////////////////////////////////////////////////////////////////////
  RMOL_ServiceContext& FacRmolServiceContext::
  create (const stdair::AirlineCode_T& iAirlineCode,
          const ResourceCapacity_T iResourceCapacity) {
    RMOL_ServiceContext* aServiceContext_ptr = NULL;

    aServiceContext_ptr = new RMOL_ServiceContext (iAirlineCode,
                                                   iResourceCapacity);
    assert (aServiceContext_ptr != NULL);

    // The new object is added to the Bom pool
    _pool.push_back (aServiceContext_ptr);

    return *aServiceContext_ptr;
  }

}