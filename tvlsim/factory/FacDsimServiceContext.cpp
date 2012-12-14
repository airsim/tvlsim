// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// StdAir
#include <stdair/service/FacSupervisor.hpp>
// TvlSim
#include <tvlsim/factory/FacDsimServiceContext.hpp>
#include <tvlsim/service/TVLSIM_ServiceContext.hpp>

namespace TVLSIM {

  FacDsimServiceContext* FacDsimServiceContext::_instance = NULL;

  // //////////////////////////////////////////////////////////////////////
  FacDsimServiceContext::~FacDsimServiceContext () {
    _instance = NULL;
  }

  // //////////////////////////////////////////////////////////////////////
  FacDsimServiceContext& FacDsimServiceContext::instance () {

    if (_instance == NULL) {
      _instance = new FacDsimServiceContext();
      assert (_instance != NULL);
      
      stdair::FacSupervisor::instance().registerServiceFactory (_instance);
    }
    return *_instance;
  }

  // //////////////////////////////////////////////////////////////////////
  TVLSIM_ServiceContext& FacDsimServiceContext::
  create () {
    TVLSIM_ServiceContext* aTVLSIM_ServiceContext_ptr = NULL;

    aTVLSIM_ServiceContext_ptr = new TVLSIM_ServiceContext ();
    assert (aTVLSIM_ServiceContext_ptr != NULL);

    // The new object is added to the Bom pool
    _pool.push_back (aTVLSIM_ServiceContext_ptr);

    return *aTVLSIM_ServiceContext_ptr;
  }

}
