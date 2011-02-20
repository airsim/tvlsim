// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// AirSched
#include <airsched/factory/FacServiceAbstract.hpp>
#include <airsched/factory/FacSupervisor.hpp>

namespace AIRSCHED {

  FacSupervisor* FacSupervisor::_instance = NULL;

  // //////////////////////////////////////////////////////////////////////
  FacSupervisor& FacSupervisor::instance() {
    if (_instance == NULL) {
      _instance = new FacSupervisor();
    }

    return *_instance;
  }

  // //////////////////////////////////////////////////////////////////////
  void FacSupervisor::
  registerServiceFactory (FacServiceAbstract* ioFacServiceAbstract_ptr) {
    _svcPool.push_back (ioFacServiceAbstract_ptr);
  }

  // //////////////////////////////////////////////////////////////////////
  FacSupervisor::~FacSupervisor() {
    cleanServiceLayer();
 }

  // //////////////////////////////////////////////////////////////////////
  void FacSupervisor::cleanServiceLayer() {
    for (ServiceFactoryPool_T::const_iterator itFactory = _svcPool.begin();
         itFactory != _svcPool.end(); itFactory++) {
      const FacServiceAbstract* currentFactory_ptr = *itFactory;
      assert (currentFactory_ptr != NULL);

      delete (currentFactory_ptr); currentFactory_ptr = NULL;
    }

    // Empty the pool of Service Factories
    _svcPool.clear();
  }

  // //////////////////////////////////////////////////////////////////////
  void FacSupervisor::cleanFactory () {
	if (_instance != NULL) {
      _instance->cleanServiceLayer();
 	}
    delete (_instance); _instance = NULL;
  }

}
