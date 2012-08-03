// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// StdAir
#include <stdair/service/FacSupervisor.hpp>
// DSim
#include <dsim/factory/FacDsimServiceContext.hpp>
#include <dsim/service/DSIM_ServiceContext.hpp>

namespace DSIM {

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
  DSIM_ServiceContext& FacDsimServiceContext::
  create () {
    DSIM_ServiceContext* aDSIM_ServiceContext_ptr = NULL;

    aDSIM_ServiceContext_ptr = new DSIM_ServiceContext ();
    assert (aDSIM_ServiceContext_ptr != NULL);

    // The new object is added to the Bom pool
    _pool.push_back (aDSIM_ServiceContext_ptr);

    return *aDSIM_ServiceContext_ptr;
  }

}
