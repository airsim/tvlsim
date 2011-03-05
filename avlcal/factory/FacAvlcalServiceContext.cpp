// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// StdAir
#include <stdair/service/FacSupervisor.hpp>
// AvlCal
#include <avlcal/service/AVLCAL_ServiceContext.hpp>
#include <avlcal/factory/FacAvlcalServiceContext.hpp>

namespace AVLCAL {

  FacAvlcalServiceContext* FacAvlcalServiceContext::_instance = NULL;

  // //////////////////////////////////////////////////////////////////////
  FacAvlcalServiceContext::~FacAvlcalServiceContext() {
    _instance = NULL;
  }

  // //////////////////////////////////////////////////////////////////////
  FacAvlcalServiceContext& FacAvlcalServiceContext::instance() {

    if (_instance == NULL) {
      _instance = new FacAvlcalServiceContext();
      assert (_instance != NULL);
      
      stdair::FacSupervisor::instance().registerServiceFactory (_instance);
    }
    return *_instance;
  }

  // //////////////////////////////////////////////////////////////////////
  AVLCAL_ServiceContext& FacAvlcalServiceContext::
  create (const std::string& iTravelDatabaseName) {
    AVLCAL_ServiceContext* aAVLCAL_ServiceContext_ptr = NULL;

    aAVLCAL_ServiceContext_ptr =
      new AVLCAL_ServiceContext (iTravelDatabaseName);
    assert (aAVLCAL_ServiceContext_ptr != NULL);

    // The new object is added to the Bom pool
    _pool.push_back (aAVLCAL_ServiceContext_ptr);

    return *aAVLCAL_ServiceContext_ptr;
  }

}
