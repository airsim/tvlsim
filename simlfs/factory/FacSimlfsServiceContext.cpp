// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// SIMLFS Common
#include <simlfs/factory/FacSupervisor.hpp>
#include <simlfs/factory/FacSimlfsServiceContext.hpp>
#include <simlfs/service/SIMLFS_ServiceContext.hpp>

namespace SIMLFS {

  FacSimlfsServiceContext* FacSimlfsServiceContext::_instance = NULL;

  // //////////////////////////////////////////////////////////////////////
  FacSimlfsServiceContext::~FacSimlfsServiceContext () {
    _instance = NULL;
  }

  // //////////////////////////////////////////////////////////////////////
  FacSimlfsServiceContext& FacSimlfsServiceContext::instance () {

    if (_instance == NULL) {
      _instance = new FacSimlfsServiceContext();
      assert (_instance != NULL);
      
      FacSupervisor::instance().registerServiceFactory (_instance);
    }
    return *_instance;
  }

  // //////////////////////////////////////////////////////////////////////
  SIMLFS_ServiceContext& FacSimlfsServiceContext::create () {
    SIMLFS_ServiceContext* aSIMLFS_ServiceContext_ptr = NULL;

    aSIMLFS_ServiceContext_ptr =
      new SIMLFS_ServiceContext ();
    assert (aSIMLFS_ServiceContext_ptr != NULL);

    // The new object is added to the Bom pool
    _pool.push_back (aSIMLFS_ServiceContext_ptr);

    return *aSIMLFS_ServiceContext_ptr;
  }

}
