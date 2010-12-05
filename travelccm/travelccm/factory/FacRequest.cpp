// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// TRAVELCCM 
#include <travelccm/factory/FacRequest.hpp>
#include <travelccm/bom/Request.hpp>
#include <travelccm/factory/FacSupervisor.hpp>

namespace TRAVELCCM {

  FacRequest* FacRequest::_instance = NULL;

  // ////////////////////////////////////////////////////////////////////
  FacRequest::~FacRequest () {
    _instance = NULL;
  }

  // ////////////////////////////////////////////////////////////////////
  FacRequest& FacRequest::instance () {

    if (_instance == NULL) {
      _instance = new FacRequest();
      assert (_instance != NULL);

      FacSupervisor::instance().registerBomFactory (_instance);
    }
    return *_instance;
  }

  // ////////////////////////////////////////////////////////////////////
  Request& FacRequest::create () {
    Request* aRequest_ptr = NULL;

    aRequest_ptr = new Request ();
    assert (aRequest_ptr != NULL);

    // The new object is added to the Bom pool
    _pool.push_back (aRequest_ptr);

    return *aRequest_ptr;
  }

  // ///////////////////////////////////////////////////////////////////
  Request& FacRequest::create (bool refundability, bool changeability,
                               bool saturdayNightStay,
                               std::string preferredAirline,
                               std::string preferredCabin,
                               DateTime_T departureTime) {
    Request* aRequest_ptr = NULL;
    
    aRequest_ptr = new Request (refundability, changeability, saturdayNightStay,
                                preferredAirline, preferredCabin, departureTime);
    assert (aRequest_ptr != NULL);

    // The new object is added to the Bom pool
    _pool.push_back (aRequest_ptr);

    return *aRequest_ptr;
  }

}

