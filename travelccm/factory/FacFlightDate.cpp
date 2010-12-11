// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// TRAVELCCM 
#include <travelccm/factory/FacFlightDate.hpp>
#include <travelccm/bom/FlightDate.hpp>
#include <travelccm/factory/FacSupervisor.hpp>

namespace TRAVELCCM {

  FacFlightDate* FacFlightDate::_instance = NULL;

  // ////////////////////////////////////////////////////////////////////
  FacFlightDate::~FacFlightDate () {
    _instance = NULL;
  }

  // ////////////////////////////////////////////////////////////////////
  FacFlightDate& FacFlightDate::instance () {

    if (_instance == NULL) {
      _instance = new FacFlightDate();
      assert (_instance != NULL);

      FacSupervisor::instance().registerBomFactory (_instance);
    }
    return *_instance;
  }

  // ////////////////////////////////////////////////////////////////////
  FlightDate& FacFlightDate::create () {
    FlightDate* aFlightDate_ptr = NULL;

    aFlightDate_ptr = new FlightDate ();
    assert (aFlightDate_ptr != NULL);

    // The new object is added to the Bom pool
    _pool.push_back (aFlightDate_ptr);

    return *aFlightDate_ptr;
  }

}

