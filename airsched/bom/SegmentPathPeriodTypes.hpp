// //////////////////////////////////////////////////////////////////////
#ifndef __AIRSCHED_BOM_SEGMENTPATHPERIODTYPES_HPP
#define __AIRSCHED_BOM_SEGMENTPATHPERIODTYPES_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <map>
#include <vector>
#include <list>
// StdAir
#include <stdair/stdair_basic_types.hpp>
#include <stdair/bom/key_types.hpp>

namespace AIRSCHED {

  // Forward declarations.
  class SegmentPathPeriod;

  /** Define the segment path list. */
  typedef std::list<SegmentPathPeriod*> SegmentPathPeriodList_T;

  /** Define the segment path map. */
  typedef std::multimap<const stdair::MapKey_T,
                        SegmentPathPeriod*> SegmentPathPeriodMultimap_T;

  /** Define the (temporary) containers of segment path period. */
  typedef std::vector<const SegmentPathPeriod*> SegmentPathPeriodLightList_T;
  typedef std::vector<SegmentPathPeriodLightList_T>SegmentPathPeriodListList_T;

  /** Define the vector of boarding date offsets of the member segments of
      a segment path compare to the boarding date of the first segment. */
  typedef std::vector<stdair::DateOffset_T> DateOffsetList_T;
  
}
#endif // __AIRSCHED_BOM_SEGMENTPATHPERIODTYPES_HPP

