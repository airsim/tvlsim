// //////////////////////////////////////////////////////////////////////
#ifndef __SIMFQT_BOM_FAREPOSITIONTYPES_HPP
#define __SIMFQT_BOM_FAREPOSITIONTYPES_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <map>
#include <list>
// STDAIR
#include <stdair/bom/key_types.hpp>

namespace SIMFQT {

  // Forward declarations.
  class FarePosition;
  
  /** Define the fare-position list. */
  typedef std::list<FarePosition*> FarePositionList_T;

  /** Define the fare-position map. */
  typedef std::map<const stdair::MapKey_T, FarePosition*> FarePositionMap_T;

  /** Define the list of pair<MapKey_T, FarePosition>. */
  typedef std::pair<stdair::MapKey_T, FarePosition*> FarePositionWithKey_T;
  typedef std::list<FarePositionWithKey_T> FarePositionDetailedList_T;
}
#endif // __SIMFQT_BOM_FAREPOSITIONTYPES_HPP
