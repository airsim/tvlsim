#ifndef __TRAVELCCM_FAC_TRAVELSOLUTIONHOLDER_HPP
#define __TRAVELCCM_FAC_TRAVELSOLUTIONHOLDER_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// TRAVELCCM 
#include <travelccm/factory/FacBomAbstract.hpp>

namespace TRAVELCCM {

  // Forward declarations.
  class TravelSolutionHolder;
  class TravelSolution;
  
  /** Factory for TravelSolutionHolder objects. */
  class FacTravelSolutionHolder : public FacBomAbstract {
  public:
    /** Provide the unique instance.
        <br>The singleton is instantiated when first used
        @return FacTravelSolutionHolder& */
    static FacTravelSolutionHolder& instance();
    
    /** Destructor.
        <br>The Destruction put the _instance to NULL
        in order to be clean for the next FacTravelSolutionHolder::instance() */
    virtual ~FacTravelSolutionHolder();
    
    /** Create a new TravelSolutionHolder object.
        <br>This new object is added to the list of instantiated objects.
        @return TravelSolutionHolder& The newly created object. */
    TravelSolutionHolder& create ();

  protected:
    /** Default Constructor.
        <br>This constructor is protected in order to ensure the singleton
        pattern.*/
    FacTravelSolutionHolder () {}

  private:
    /** The unique instance.*/
    static FacTravelSolutionHolder* _instance;
  };

}
#endif // __TRAVELCCM_FAC_TRAVELSOLUTIONHOLDER_HPP

