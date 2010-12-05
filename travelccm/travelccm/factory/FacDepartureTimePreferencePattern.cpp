// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// StdAir
#include <stdair/service/Logger.hpp>
// TravelCCM 
#include <travelccm/bom/DepartureTimePreferencePattern.hpp>
#include <travelccm/factory/FacDepartureTimePreferencePattern.hpp>
#include <travelccm/factory/FacSupervisor.hpp>

namespace TRAVELCCM {

  FacDepartureTimePreferencePattern* FacDepartureTimePreferencePattern::_instance = NULL;

  // ////////////////////////////////////////////////////////////////////
  FacDepartureTimePreferencePattern::~FacDepartureTimePreferencePattern () {
    _instance = NULL;
  }

  // ////////////////////////////////////////////////////////////////////
  FacDepartureTimePreferencePattern& FacDepartureTimePreferencePattern::
  instance () {

    if (_instance == NULL) {
      _instance = new FacDepartureTimePreferencePattern();
      assert (_instance != NULL);

      FacSupervisor::instance().registerBomFactory (_instance);
    }
    return *_instance;
  }

  // ////////////////////////////////////////////////////////////////////
  DepartureTimePreferencePattern& FacDepartureTimePreferencePattern::
  create (const PatternId_T& iPatternId) {
    DepartureTimePreferencePattern* aDepartureTimePreferencePattern_ptr = NULL;

    aDepartureTimePreferencePattern_ptr =
      new DepartureTimePreferencePattern (iPatternId);
    assert (aDepartureTimePreferencePattern_ptr != NULL);

    // The new object is added to the Bom pool
    _pool.push_back (aDepartureTimePreferencePattern_ptr);

    return *aDepartureTimePreferencePattern_ptr;
  }

}
