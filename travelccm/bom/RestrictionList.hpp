#ifndef __TRAVELCCM_RESTRICTIONLIST_HPP
#define __TRAVELCCM_RESTRICTIONLIST_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <list>

namespace TRAVELCCM {

  /** Forward declarations. */
  class Restriction;

  /** Define a vector (ordered list) of N Restrictions/classes. */
  typedef std::list<const Restriction*> RestrictionList_T;
  
}
#endif // __TRAVELCCM_RESTRICTIONLIST_HPP
