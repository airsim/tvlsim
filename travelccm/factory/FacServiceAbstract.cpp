// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// C
#include <assert.h>
// TRAVEL-CCM
#include <travelccm/service/ServiceAbstract.hpp>
#include <travelccm/factory/FacServiceAbstract.hpp>

namespace TRAVELCCM {
  
  // //////////////////////////////////////////////////////////////////////
  FacServiceAbstract::~FacServiceAbstract() {
    clean ();
  }

  // //////////////////////////////////////////////////////////////////////
  void FacServiceAbstract::clean() {
    for (ServicePool_T::iterator itService = _pool.begin();
         itService != _pool.end(); itService++) {
      ServiceAbstract* currentService_ptr = *itService;
      assert (currentService_ptr != NULL);

      delete (currentService_ptr); currentService_ptr = NULL;
    }

    // Empty the pool of Service Factories
    _pool.clear();
  }

}
