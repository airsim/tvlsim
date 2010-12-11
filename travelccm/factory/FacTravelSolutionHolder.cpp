// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// TRAVELCCM 
#include <travelccm/bom/TravelSolutionHolder.hpp>
#include <travelccm/factory/FacTravelSolutionHolder.hpp>
#include <travelccm/factory/FacSupervisor.hpp>

namespace TRAVELCCM {

  FacTravelSolutionHolder* FacTravelSolutionHolder::_instance = NULL;

  // ////////////////////////////////////////////////////////////////////
  FacTravelSolutionHolder::~FacTravelSolutionHolder () {
    _instance = NULL;
  }

  // ////////////////////////////////////////////////////////////////////
  FacTravelSolutionHolder& FacTravelSolutionHolder::instance () {

    if (_instance == NULL) {
      _instance = new FacTravelSolutionHolder();
      assert (_instance != NULL);

      FacSupervisor::instance().registerBomFactory (_instance);
    }
    return *_instance;
  }

  // ////////////////////////////////////////////////////////////////////
  TravelSolutionHolder& FacTravelSolutionHolder::create () {
    TravelSolutionHolder* aTravelSolutionHolder_ptr = NULL;

    aTravelSolutionHolder_ptr = new TravelSolutionHolder ();
    assert (aTravelSolutionHolder_ptr != NULL);

    // The new object is added to the Bom pool
    _pool.push_back (aTravelSolutionHolder_ptr);

    return *aTravelSolutionHolder_ptr;
  }

}

