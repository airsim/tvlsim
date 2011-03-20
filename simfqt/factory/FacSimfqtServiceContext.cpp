// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// StdAir
#include <stdair/service/FacSupervisor.hpp>
// SimFQT
#include <simfqt/factory/FacSimfqtServiceContext.hpp>
#include <simfqt/service/SIMFQT_ServiceContext.hpp>

namespace SIMFQT {

  FacSimfqtServiceContext* FacSimfqtServiceContext::_instance = NULL;

  // //////////////////////////////////////////////////////////////////////
  FacSimfqtServiceContext::~FacSimfqtServiceContext() {
    _instance = NULL;
  }

  // //////////////////////////////////////////////////////////////////////
  FacSimfqtServiceContext& FacSimfqtServiceContext::instance() {

    if (_instance == NULL) {
      _instance = new FacSimfqtServiceContext();
      assert (_instance != NULL);
      
      stdair::FacSupervisor::instance().registerServiceFactory (_instance);
    }
    return *_instance;
  }

  // //////////////////////////////////////////////////////////////////////
  SIMFQT_ServiceContext& FacSimfqtServiceContext::create() {
    SIMFQT_ServiceContext* aServiceContext_ptr = NULL;

    aServiceContext_ptr = new SIMFQT_ServiceContext();
    assert (aServiceContext_ptr != NULL);

    // The new object is added to the Bom pool
    _pool.push_back (aServiceContext_ptr);

    return *aServiceContext_ptr;
  }

}
