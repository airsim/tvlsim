// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// TRAVELCCM 
#include <travelccm/factory/FacRestriction.hpp>
#include <travelccm/bom/Restriction.hpp>
#include <travelccm/factory/FacSupervisor.hpp>

namespace TRAVELCCM {

  FacRestriction* FacRestriction::_instance = NULL;

  // ////////////////////////////////////////////////////////////////////
  FacRestriction::~FacRestriction () {
    _instance = NULL;
  }

  // ////////////////////////////////////////////////////////////////////
  FacRestriction& FacRestriction::instance () {

    if (_instance == NULL) {
      _instance = new FacRestriction();
      assert (_instance != NULL);

      FacSupervisor::instance().registerBomFactory (_instance);
    }
    return *_instance;
  }

  // ////////////////////////////////////////////////////////////////////
  Restriction& FacRestriction::create () {
    Restriction* aRestriction_ptr = NULL;

    aRestriction_ptr = new Restriction ();
    assert (aRestriction_ptr != NULL);

    // The new object is added to the Bom pool
    _pool.push_back (aRestriction_ptr);

    return *aRestriction_ptr;
  }

   // ////////////////////////////////////////////////////////////////////
  Restriction& FacRestriction::create (std::string restrictionType) {
    Restriction* aRestriction_ptr = NULL;

    aRestriction_ptr = new Restriction (restrictionType);
    assert (aRestriction_ptr != NULL);

    // The new object is added to the Bom pool
    _pool.push_back (aRestriction_ptr);

    return *aRestriction_ptr;
  }

   // ////////////////////////////////////////////////////////////////////
  Restriction& FacRestriction::create (std::string restrictionType,
                                       std::string namePreference) {
    Restriction* aRestriction_ptr = NULL;

    aRestriction_ptr = new Restriction (restrictionType, namePreference);
    assert (aRestriction_ptr != NULL);

    // The new object is added to the Bom pool
    _pool.push_back (aRestriction_ptr);

    return *aRestriction_ptr;
  }

   // ////////////////////////////////////////////////////////////////////
  Restriction& FacRestriction::create (std::string restrictionType,
                                       stdair::DateTime_T departureTime) {
    Restriction* aRestriction_ptr = NULL;

    aRestriction_ptr = new Restriction (restrictionType, departureTime);
    assert (aRestriction_ptr != NULL);

    // The new object is added to the Bom pool
    _pool.push_back (aRestriction_ptr);

    return *aRestriction_ptr;
  }

  // /////////////////////////////////////////////////////////////////////
  void FacRestriction::
  addRestriction (RestrictionHolder& ioRestrictionHolder,
                  Restriction& ioRestriction) {
    ioRestrictionHolder.addRestriction(ioRestriction);
  }
  

}

