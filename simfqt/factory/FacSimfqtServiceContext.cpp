// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// SIMFQT Common
#include <simfqt/factory/FacSupervisor.hpp>
#include <simfqt/factory/FacSimfqtServiceContext.hpp>
#include <simfqt/service/SIMFQT_ServiceContext.hpp>

namespace SIMFQT {

  FacSimfqtServiceContext* FacSimfqtServiceContext::_instance = NULL;

  // //////////////////////////////////////////////////////////////////////
  FacSimfqtServiceContext::~FacSimfqtServiceContext () {
    _instance = NULL;
  }

  // //////////////////////////////////////////////////////////////////////
  FacSimfqtServiceContext& FacSimfqtServiceContext::instance () {

    if (_instance == NULL) {
      _instance = new FacSimfqtServiceContext();
      assert (_instance != NULL);
      
      FacSupervisor::instance().registerServiceFactory (_instance);
    }
    return *_instance;
  }

  // //////////////////////////////////////////////////////////////////////
  SIMFQT_ServiceContext& FacSimfqtServiceContext::create () {
    SIMFQT_ServiceContext* aSIMFQT_ServiceContext_ptr = NULL;

    aSIMFQT_ServiceContext_ptr =
      new SIMFQT_ServiceContext ();
    assert (aSIMFQT_ServiceContext_ptr != NULL);

    // The new object is added to the Bom pool
    _pool.push_back (aSIMFQT_ServiceContext_ptr);

    return *aSIMFQT_ServiceContext_ptr;
  }

}
