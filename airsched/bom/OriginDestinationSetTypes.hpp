// //////////////////////////////////////////////////////////////////////
#ifndef __AIRSCHED_BOM_ORIGINDESTINATIONSETTYPES_HPP
#define __AIRSCHED_BOM_ORIGINDESTINATIONSETTYPES_HPP

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
  class OriginDestinationSet;

  /** Define the OriginDestinationSet list. */
  typedef std::list<OriginDestinationSet*> OriginDestinationSetList_T;

  /** Define the OriginDestinationSet map. */
  typedef std::map<const stdair::MapKey_T,
                   OriginDestinationSet*> OriginDestinationSetMap_T;
  
}
#endif // __AIRSCHED_BOM_ORIGINDESTINATIONSETTYPES_HPP

