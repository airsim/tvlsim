// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// TRAVELCCM 
#include <travelccm/bom/RestrictionHolder.hpp>
#include <travelccm/factory/FacRestrictionHolder.hpp>
#include <travelccm/factory/FacSupervisor.hpp>

namespace TRAVELCCM {

  FacRestrictionHolder* FacRestrictionHolder::_instance = NULL;

  // ////////////////////////////////////////////////////////////////////
  FacRestrictionHolder::~FacRestrictionHolder () {
    _instance = NULL;
  }

  // ////////////////////////////////////////////////////////////////////
  FacRestrictionHolder& FacRestrictionHolder::instance () {

    if (_instance == NULL) {
      _instance = new FacRestrictionHolder();
      assert (_instance != NULL);

      FacSupervisor::instance().registerBomFactory (_instance);
    }
    return *_instance;
  }

  // ////////////////////////////////////////////////////////////////////
  RestrictionHolder& FacRestrictionHolder::create () {
    RestrictionHolder* aRestrictionHolder_ptr = NULL;

    aRestrictionHolder_ptr = new RestrictionHolder ();
    assert (aRestrictionHolder_ptr != NULL);

    // The new object is added to the Bom pool
    _pool.push_back (aRestrictionHolder_ptr);

    return *aRestrictionHolder_ptr;
  }

}

