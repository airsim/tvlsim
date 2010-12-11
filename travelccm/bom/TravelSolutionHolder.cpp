// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <istream>
#include <ostream>
#include <iomanip>
// StdAir
#include <stdair/service/Logger.hpp>
// TRAVELCCM
#include <travelccm/bom/TravelSolutionHolder.hpp>

namespace TRAVELCCM {

  // ////////////////////////////////////////////////////////////////////
  TravelSolutionHolder::TravelSolutionHolder () {
    
  }
  
  // ////////////////////////////////////////////////////////////////////
  TravelSolutionHolder::~TravelSolutionHolder () {
    _travelSolutionList.clear ();
  }

  // //////////////////////////////////////////////////////////////////////
  void TravelSolutionHolder::toStream (std::ostream& ioOut) const {
  }

  // //////////////////////////////////////////////////////////////////////
  void TravelSolutionHolder::fromStream (std::istream& ioIn) {
  }

  // //////////////////////////////////////////////////////////////////////
  std::string TravelSolutionHolder::toString() const {
    std::string oString;
    TravelSolutionList_T::const_iterator it = _travelSolutionList.begin();
    while (it != _travelSolutionList.end() ){
      const TravelSolution* TS_ptr = *it;
      assert(TS_ptr != NULL);
      oString += TS_ptr->toString();
      it++;
    }
    return oString;
  }

  // ////////////////////////////////////////////////////////////////////////
  int TravelSolutionHolder::numberOfTravelSolutions() {
    return _travelSolutionList.size();
  }

  // ////////////////////////////////////////////////////////////////////////
  bool TravelSolutionHolder::isVoid() {
    return (numberOfTravelSolutions() == 0);
  }
    
  // //////////////////////////////////////////////////////////////////////
  const std::string TravelSolutionHolder::describeKey() const {
    std::string oKey;

    return oKey;
  }

  // //////////////////////////////////////////////////////////////////////
  const std::string TravelSolutionHolder::describeShortKey() const {
    std::string oKey;

    return oKey;
  }

  // //////////////////////////////////////////////////////////////////////
  const TravelSolution& TravelSolutionHolder::getCurrentTravelSolution () const {
    const TravelSolution* resultTravelSolution_ptr = *_itCurrentTravelSolution;
    assert (resultTravelSolution_ptr != NULL);
    
    return (*resultTravelSolution_ptr);
  }

  // //////////////////////////////////////////////////////////////////////
  void TravelSolutionHolder::begin () {
    _itCurrentTravelSolution = _travelSolutionList.begin();
  }

  // //////////////////////////////////////////////////////////////////////
  bool TravelSolutionHolder::hasNotReachedEnd () const {
    bool result = (_itCurrentTravelSolution != _travelSolutionList.end());
    return result;
  }

  // //////////////////////////////////////////////////////////////////////
  void TravelSolutionHolder::iterate () {
    if (_itCurrentTravelSolution != _travelSolutionList.end()) {
      _itCurrentTravelSolution++;
    }
  }

  // //////////////////////////////////////////////////////////////////////
  void TravelSolutionHolder::addTravelSolution (const TravelSolution& TS) {
    _travelSolutionList.push_back(&TS);
  }

  // //////////////////////////////////////////////////////////////////////
  void TravelSolutionHolder::
  addTravelSolutionList (TravelSolutionList_T addList) {
    _travelSolutionList.merge(addList);
  }

  // //////////////////////////////////////////////////////////////////////
  void TravelSolutionHolder::eraseCurrentTravelSolution () {
    /* ok even if the list is at the end */
    assert (_itCurrentTravelSolution != _travelSolutionList.end());
    _itCurrentTravelSolution =
      _travelSolutionList.erase (_itCurrentTravelSolution);
  }
  
  // //////////////////////////////////////////////////////////////////////
  void TravelSolutionHolder::
  restrictionMeetsTSList (const Restriction& iRestriction,
                          TravelSolutionList_T& ioRemovedElements,
                          const Passenger& iPassenger) {
    /* if we are at the end of the list, we return the removedElements list
       because all the travel solutions have been matched */
    if (hasNotReachedEnd() == false) {
      return;
    }
    
    /** Call a function in the TravelSolution class which returns if a
        restriction meets a single travel solution */
    const TravelSolution& currentTravelSolution = getCurrentTravelSolution();
    const bool curTSOK =
      currentTravelSolution.restrictionMeetsTravelSolution (iRestriction,
                                                            iPassenger);
    if (curTSOK == false) {
      // we add this travel solution to the temp list
      ioRemovedElements.push_back(&currentTravelSolution);
      // STDAIR_LOG_DEBUG (currentTravelSolution.toString());
      // we erase this travel solution from the current holder
      eraseCurrentTravelSolution();
      
    } else {
      /* if the travel solution matches the restriction, we do not alter the
         temporary list, and test the next elements */
      iterate();
    }

    restrictionMeetsTSList (iRestriction, ioRemovedElements, iPassenger);
  }

  // ///////////////////////////////////////////////////////////////////////
  const TravelSolution* TravelSolutionHolder::getCheapestTravelSolution() {

    if (isVoid()) {
      return NULL;
    };

    begin();
    const TravelSolution* cheapestTravelSolution_ptr =
      &getCurrentTravelSolution();
    iterate();
      
    while (hasNotReachedEnd()) {
      const TravelSolution& lCurrentTravelSolution =
        getCurrentTravelSolution();
      
      assert (cheapestTravelSolution_ptr != NULL);
      const bool isCheaper =
        lCurrentTravelSolution.isCheaper (*cheapestTravelSolution_ptr);
      
      if (isCheaper == true) {
        cheapestTravelSolution_ptr = &lCurrentTravelSolution;
      }
      assert (cheapestTravelSolution_ptr != NULL);
      
      const bool hasTheSamePrice =
        lCurrentTravelSolution.hasTheSamePrice (*cheapestTravelSolution_ptr);
      if (hasTheSamePrice == true) {
        // then we cast a random number to determine which of the two
        // "identical" travel solutions - from the customer's point of
        // view - we will choose.
        int randomIndicator = rand () % 2;
        
        // We change only when we cast a 0, if more than two travel
        // solutions have the same price, they do not have the
        // same probability!!
        if (randomIndicator == 0) {
          cheapestTravelSolution_ptr = &lCurrentTravelSolution;
        }
      }
      
      iterate();
    }
    
    return cheapestTravelSolution_ptr;
  }

}

