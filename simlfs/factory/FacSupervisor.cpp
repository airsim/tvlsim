// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// SIMLFS
#include <simlfs/factory/FacBomAbstract.hpp>
#include <simlfs/factory/FacServiceAbstract.hpp>
#include <simlfs/factory/FacSupervisor.hpp>

namespace SIMLFS {

  FacSupervisor* FacSupervisor::_instance = NULL;

  // //////////////////////////////////////////////////////////////////////
  FacSupervisor::FacSupervisor () {
  }
    
  // //////////////////////////////////////////////////////////////////////
  FacSupervisor& FacSupervisor::instance() {
    if (_instance == NULL) {
      _instance = new FacSupervisor();
    }

    return *_instance;
  }

  // //////////////////////////////////////////////////////////////////////
  void FacSupervisor::
  registerBomFactory (FacBomAbstract* ioFacBomAbstract_ptr) {
    _bomPool.push_back (ioFacBomAbstract_ptr);
  }

  // //////////////////////////////////////////////////////////////////////
  void FacSupervisor::
  registerServiceFactory (FacServiceAbstract* ioFacServiceAbstract_ptr) {
    _svcPool.push_back (ioFacServiceAbstract_ptr);
  }

  // //////////////////////////////////////////////////////////////////////
  FacSupervisor::~FacSupervisor() {
    cleanBomLayer();
    cleanServiceLayer();
  }

  // //////////////////////////////////////////////////////////////////////
  void FacSupervisor::cleanBomLayer() {
    for (BomFactoryPool_T::const_iterator itFactory = _bomPool.begin();
         itFactory != _bomPool.end(); itFactory++) {
      const FacBomAbstract* currentFactory_ptr = *itFactory;
      assert (currentFactory_ptr != NULL);

      delete (currentFactory_ptr); currentFactory_ptr = NULL;
    }

    // Empty the pool of Bom Factories
    _bomPool.clear();
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
		_instance->cleanBomLayer();
		_instance->cleanServiceLayer();
	}
    delete (_instance); _instance = NULL;
  }

}
