// //////////////////////////////////////////////////////////////////////
#ifndef __AIRSCHED_BOM_REACHABLEUNIVERSETYPES_HPP
#define __AIRSCHED_BOM_REACHABLEUNIVERSETYPES_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <map>
#include <list>
// StdAir
#include <stdair/stdair_basic_types.hpp>
#include <stdair/bom/key_types.hpp>

namespace AIRSCHED {

  // Forward declarations.
  class ReachableUniverse;
  
  /** Define the reachable-universe list. */
  typedef std::list<ReachableUniverse*> ReachableUniverseList_T;

  /** Define the reachable-universe map. */
  typedef std::map<const stdair::MapKey_T,
                   ReachableUniverse*> ReachableUniverseMap_T;
  
}
#endif // __AIRSCHED_BOM_REACHABLEUNIVERSETYPES_HPP

