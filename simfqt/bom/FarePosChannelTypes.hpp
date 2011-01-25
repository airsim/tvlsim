// //////////////////////////////////////////////////////////////////////
#ifndef __SIMFQT_BOM_FAREPOSCHANNELTYPES_HPP
#define __SIMFQT_BOM_FAREPOSCHANNELTYPES_HPP

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
  class FarePosChannel;
  
  /** Define the fare-position list. */
  typedef std::list<FarePosChannel*> FarePosChannelList_T;

  /** Define the fare-position map. */
  typedef std::map<const stdair::MapKey_T, FarePosChannel*> FarePosChannelMap_T;

  /** Define the list of pair<MapKey_T, FarePosition>. */
  typedef std::pair<stdair::MapKey_T, FarePosChannel*> FarePosChannelWithKey_T;
  typedef std::list<FarePosChannelWithKey_T> FarePosChannelDetailedList_T;
}
#endif // __SIMFQT_BOM_FAREPOSCHANNELTYPES_HPP
