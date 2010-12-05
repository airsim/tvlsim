#ifndef __TRAVELCCM_TRAVELSOLUTIONLIST_HPP
#define __TRAVELCCM_TRAVELSOLUTIONLIST_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <list>

namespace TRAVELCCM {

  /** Forward declarations. */
  class TravelSolution;

  /** Define a vector (ordered map) of N TravelSlutions/classes. */
  typedef std::list<const TravelSolution*> TravelSolutionList_T;
  
}
#endif // __TRAVELCCM_TRAVELSOLUTIONLIST_HPP
