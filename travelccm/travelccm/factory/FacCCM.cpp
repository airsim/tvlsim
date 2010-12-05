// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// C
#include <assert.h>
// TRAVELCCM 
#include <travelccm/factory/FacCCM.hpp>
#include <travelccm/bom/CCM.hpp>
#include <travelccm/factory/FacSupervisor.hpp>
#include <travelccm/service/Logger.hpp>

namespace TRAVELCCM {

  FacCCM* FacCCM::_instance = NULL;

  // ////////////////////////////////////////////////////////////////////
  FacCCM::~FacCCM () {
    _instance = NULL;
  }

  // ////////////////////////////////////////////////////////////////////
  FacCCM& FacCCM::instance () {

    if (_instance == NULL) {
      _instance = new FacCCM();
      assert (_instance != NULL);

      FacSupervisor::instance().registerBomFactory (_instance);
    }
    return *_instance;
  }

  // ////////////////////////////////////////////////////////////////////
  CCM& FacCCM::create () {
    CCM* aCCM_ptr = NULL;

    aCCM_ptr = new CCM ();
    assert (aCCM_ptr != NULL);

    // The new object is added to the Bom pool
    _pool.push_back (aCCM_ptr);

    return *aCCM_ptr;
  }

}

