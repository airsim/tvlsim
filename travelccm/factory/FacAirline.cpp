// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// C
#include <assert.h>
// TRAVELCCM 
#include <travelccm/bom/Airline.hpp>
#include <travelccm/factory/FacAirline.hpp>
#include <travelccm/factory/FacSupervisor.hpp>
#include <travelccm/service/Logger.hpp>

namespace TRAVELCCM {

  FacAirline* FacAirline::_instance = NULL;

  // ////////////////////////////////////////////////////////////////////
  FacAirline::~FacAirline () {
    _instance = NULL;
  }

  // ////////////////////////////////////////////////////////////////////
  FacAirline& FacAirline::instance () {

    if (_instance == NULL) {
      _instance = new FacAirline();
      assert (_instance != NULL);

      FacSupervisor::instance().registerBomFactory (_instance);
    }
    return *_instance;
  }

  // ////////////////////////////////////////////////////////////////////
  Airline& FacAirline::create () {
    Airline* aAirline_ptr = NULL;

    aAirline_ptr = new Airline ();
    assert (aAirline_ptr != NULL);

    // The new object is added to the Bom pool
    _pool.push_back (aAirline_ptr);

    return *aAirline_ptr;
  }

}

