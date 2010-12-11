// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// STDAIR
#include <stdair/basic/BasConst_Inventory.hpp>
// AIRSCHED
#include <airsched/bom/ReachableUniverse.hpp>
#include <airsched/bom/SegmentPathPeriod.hpp>

namespace AIRSCHED {

  // ////////////////////////////////////////////////////////////////////
  ReachableUniverse::ReachableUniverse (const Key_T& iKey)
    : _key (iKey), _parent (NULL) {
  }

  // ////////////////////////////////////////////////////////////////////
  ReachableUniverse::~ReachableUniverse () {
  }

  // ////////////////////////////////////////////////////////////////////
  std::string ReachableUniverse::toString() const {
    std::ostringstream oStr;
    oStr << _key.toString();
    return oStr.str();
  }
  
  // ////////////////////////////////////////////////////////////////////
  void ReachableUniverse::
  addSegmentPathPeriod (const SegmentPathPeriod& iSegmentPathPeriod) {
    const stdair::NbOfSegments_T& lNbOfSegments =
      iSegmentPathPeriod.getNbOfSegments ();
    assert (lNbOfSegments > 0
            && lNbOfSegments <= stdair::MAXIMAL_NUMBER_OF_SEGMENTS_IN_OND);

    // If needed, initialise the list of lists with empty fixed-length
    // segment path period lists.
    while (_segmentPathPeriodListList.size() < lNbOfSegments) {
      SegmentPathPeriodLightList_T lSegmentPathPeriodLightList;
      _segmentPathPeriodListList.push_back (lSegmentPathPeriodLightList);
    }

    // Retrieve the i-fixed-length segment path period list (i = number of
    // segments).
    SegmentPathPeriodLightList_T& lSegmentPathPeriodLightList =
      _segmentPathPeriodListList.at (lNbOfSegments-1);

    // Add the SegmentPathPeriod to that fixed-length-path list.
    lSegmentPathPeriodLightList.push_back (&iSegmentPathPeriod);
  }
  
}

