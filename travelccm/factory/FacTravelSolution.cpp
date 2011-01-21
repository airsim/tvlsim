// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// TRAVELCCM 
#include <travelccm/factory/FacTravelSolution.hpp>
#include <travelccm/bom/TravelSolution.hpp>
#include <travelccm/factory/FacSupervisor.hpp>

namespace TRAVELCCM {

  FacTravelSolution* FacTravelSolution::_instance = NULL;

  // ////////////////////////////////////////////////////////////////////
  FacTravelSolution::~FacTravelSolution () {
    _instance = NULL;
  }

  // ////////////////////////////////////////////////////////////////////
  FacTravelSolution& FacTravelSolution::instance () {

    if (_instance == NULL) {
      _instance = new FacTravelSolution();
      assert (_instance != NULL);

      FacSupervisor::instance().registerBomFactory (_instance);
    }
    return *_instance;
  }

  // ////////////////////////////////////////////////////////////////////
  TravelSolution& FacTravelSolution::create () {
    TravelSolution* aTravelSolution_ptr = NULL;

    aTravelSolution_ptr = new TravelSolution ();
    assert (aTravelSolution_ptr != NULL);

    // The new object is added to the Bom pool
    _pool.push_back (aTravelSolution_ptr);

    return *aTravelSolution_ptr;
  }

  // ////////////////////////////////////////////////////////////////////
  TravelSolution& FacTravelSolution::
  create (std::string dAirport, std::string aAirport,
          stdair::Date_T depDate,
          stdair::Duration_T depTime, stdair::Duration_T arTime,
          stdair::Duration_T dur,
          bool Ref, std::string airline, std::string cabin,
          int flightNum, double fare, int lagsNum, bool SNS,
          bool change, std::string id) {
    TravelSolution* aTravelSolution_ptr = NULL;

    aTravelSolution_ptr = new TravelSolution ( dAirport, aAirport, depDate,
                                               depTime, arTime, dur, Ref,
                                               airline, cabin, flightNum,
                                               fare, lagsNum, SNS, change, id);
    assert (aTravelSolution_ptr != NULL);

    // The new object is added to the Bom pool
    _pool.push_back (aTravelSolution_ptr);

    return *aTravelSolution_ptr;
  }

  // /////////////////////////////////////////////////////////////////////
  void FacTravelSolution::
  addTravelSolution (TravelSolutionHolder& ioTravelSolutionHolder,
                     TravelSolution& ioTravelSolution) {
    ioTravelSolutionHolder.addTravelSolution(ioTravelSolution);
  }
  

}

