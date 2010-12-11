// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// StdAir
#include <stdair/service/Logger.hpp>
// TRAVELCCM 
#include <travelccm/bom/RestrictionHolder.hpp>
#include <travelccm/bom/Passenger.hpp>
#include <travelccm/bom/Request.hpp>
#include <travelccm/bom/TravelSolutionHolder.hpp>
#include <travelccm/bom/CCM.hpp>

namespace TRAVELCCM {
  
  // ////////////////////////////////////////////////////////////////////
  CCM::~CCM () {
  }

  // //////////////////////////////////////////////////////////////////////
  void CCM::toStream (std::ostream& ioOut) const {
  }

  // //////////////////////////////////////////////////////////////////////
  void CCM::fromStream (std::istream& ioIn) {
  }
 
  // //////////////////////////////////////////////////////////////////////
  std::string CCM::toString() const {
    std::string oString;
    return oString;
  }
    
  // //////////////////////////////////////////////////////////////////////
  const std::string CCM::describeKey() const {
    std::string oKey;
    return oKey;
  }

  // //////////////////////////////////////////////////////////////////////
  const std::string CCM::describeShortKey() const {
    std::string oKey;
    return oKey;
  }

  // //////////////////////////////////////////////////////////////////////
  void CCM::
  getBestTravelSolutionByMatchingIndicator (const stdair::PassengerType& iPaxType,
                                            const Request& iRequest,
                                            TravelSolutionHolder* travelSolutionHolder_ptr) {
    assert (travelSolutionHolder_ptr != NULL);
    
    // We define the base number in which the matching indicator will be computed
    int baseNumber = 2;

    travelSolutionHolder_ptr->begin();
    const TravelSolution* oBestTravelSolution_ptr =
      &travelSolutionHolder_ptr->getCurrentTravelSolution();

    const int matchingIndicator =
      oBestTravelSolution_ptr->CalculateMatchingNumber (iPaxType, iRequest,
                                                        baseNumber);

    while (travelSolutionHolder_ptr->hasNotReachedEnd()) {
      const TravelSolution& lCurrentTravelSolution =
        travelSolutionHolder_ptr->getCurrentTravelSolution();
      
      const int lCurrentMatchingIndicator =
        lCurrentTravelSolution.CalculateMatchingNumber (iPaxType, iRequest,
                                                        baseNumber);
      
      // If the current travel solution has a better matching indicator then
      // than the stored one, then we change and erase the previous ones which
      // have a matching indicator strictly inferior.
      if (lCurrentMatchingIndicator > matchingIndicator) {
        oBestTravelSolution_ptr = &lCurrentTravelSolution;
        travelSolutionHolder_ptr->begin();
      }
      
      if (lCurrentMatchingIndicator < matchingIndicator) {
        // Then we erase the current travel solution
        travelSolutionHolder_ptr->eraseCurrentTravelSolution();
        
      } else {
        // The 2 indicators are equal, in that case we iterate
        travelSolutionHolder_ptr->iterate();
      }
    }
  }
   
  // //////////////////////////////////////////////////////////////////////
  void CCM::orderedPreferences (TravelSolutionHolder& ioTSHolder,
                                Passenger& iPassenger) {
    /** For each travel solution, test if it meets the different restrictions;
        if it does, we keep it in the list, else we delete it
    */
    /** The temporary travel solution list allows us to keep in memory the
        travel solutions we erase from the ioTSHolder, in order to readd them
        in case of the list become void
    */

    RestrictionHolder& passengerRestrictions =
      iPassenger.getPassengerRestrictions();
    
    // We check if each restriction meets the list of travel solutions
    passengerRestrictions.begin();
    while (passengerRestrictions.hasNotReachedEnd()) {
      
      const Restriction& lCurrentRestriction =
        passengerRestrictions.getCurrentRestriction ();
      
      // We put the pointer at the beginning of the travel solutions list
      ioTSHolder.begin();

      // We retrive the erased travel solutions due to the restriction
      TravelSolutionList_T erasedElements;
      ioTSHolder.restrictionMeetsTSList (lCurrentRestriction, erasedElements,
                                         iPassenger);
      
      // If no travel solution remains
      if (ioTSHolder.isVoid() == true) {
        // We print the restriction which will not be respected
        STDAIR_LOG_DEBUG ("Restriction not respected: "
                          << lCurrentRestriction.toString() << ";");
        
        /* We put the just erased travel solutions in the travel solution
        holder again */
        ioTSHolder.addTravelSolutionList (erasedElements);
      }
      
      // Then we do not need those saved travel solutions anymore
      erasedElements.clear();
      
      /* The pointer of the list is put at the beginning for the
         next restriction(s) */
      passengerRestrictions.iterate();
    }
  }
  
}

