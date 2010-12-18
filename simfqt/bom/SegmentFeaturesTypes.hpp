// //////////////////////////////////////////////////////////////////////
#ifndef __SIMFQT_BOM_SEGMENTFEATURESTYPES_HPP
#define __SIMFQT_BOM_SEGMENTFEATURESTYPES_HPP

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
  class SegmentFeatures;
  
  /** Define the segment-features list. */
  typedef std::list<SegmentFeatures*> SegmentFeaturesList_T;

  /** Define the segment-features map. */
  typedef std::map<const stdair::MapKey_T, SegmentFeatures*> SegmentFeaturesMap_T;

  /** Define the list of pair<MapKey_T, SegmentFeatures>. */
  typedef std::pair<stdair::MapKey_T, SegmentFeatures*> SegmentFeaturesWithKey_T;
  typedef std::list<SegmentFeaturesWithKey_T> SegmentFeaturesDetailedList_T;
}
#endif // __SIMFQT_BOM_SEGMENTFEATURESTYPES_HPP
