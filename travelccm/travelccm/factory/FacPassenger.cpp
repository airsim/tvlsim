// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// TRAVELCCM 
#include <travelccm/factory/FacPassenger.hpp>
#include <travelccm/bom/Passenger.hpp>
#include <travelccm/factory/FacSupervisor.hpp>

namespace TRAVELCCM {

  FacPassenger* FacPassenger::_instance = NULL;

  // ////////////////////////////////////////////////////////////////////
  FacPassenger::~FacPassenger () {
    _instance = NULL;
  }

  // ////////////////////////////////////////////////////////////////////
  FacPassenger& FacPassenger::instance () {

    if (_instance == NULL) {
      _instance = new FacPassenger();
      assert (_instance != NULL);

      FacSupervisor::instance().registerBomFactory (_instance);
    }
    return *_instance;
  }

  // ////////////////////////////////////////////////////////////////////
  Passenger& FacPassenger::create () {
    Passenger* aPassenger_ptr = NULL;

    aPassenger_ptr = new Passenger ();
    assert (aPassenger_ptr != NULL);

    // The new object is added to the Bom pool
    _pool.push_back (aPassenger_ptr);

    return *aPassenger_ptr;
  }

  // ////////////////////////////////////////////////////////////////////
  Passenger& FacPassenger::create (const stdair::PassengerType& iPaxType) {
    Passenger* aPassenger_ptr = NULL;

    aPassenger_ptr = new Passenger (iPaxType);
    assert (aPassenger_ptr != NULL);

    // The new object is added to the Bom pool
    _pool.push_back (aPassenger_ptr);

    return *aPassenger_ptr;
  }

  // ////////////////////////////////////////////////////////////////////
  void FacPassenger::
  linkPassengerWithRestrictionHolder (Passenger& ioPassenger,
                                      RestrictionHolder& ioRestrictionHolder) {
    ioPassenger._passengerRestrictions = &ioRestrictionHolder;
  }

  // ////////////////////////////////////////////////////////////////////
  void FacPassenger::linkPassengerWithRequest (Passenger& ioPassenger,
                                               Request& ioRequest) {
    ioPassenger._request = &ioRequest;
  }
  
  // ////////////////////////////////////////////////////////////////////
  void FacPassenger::
  linkPassengerWithDepartureTimePreferencePattern (Passenger& ioPassenger,
                                      DepartureTimePreferencePattern& ioDepartureTimePreferencePattern) {
    ioPassenger._departureTimePreferencePattern = &ioDepartureTimePreferencePattern;
  }
}

