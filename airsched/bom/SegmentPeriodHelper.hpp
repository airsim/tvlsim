#ifndef __AIRSCHED_BOM_SEGMENTPERIODHELPER_HPP
#define __AIRSCHED_BOM_SEGMENTPERIODHELPER_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// AIRSCHED
#include <airsched/bom/LegStruct.hpp>
#include <airsched/bom/SegmentStruct.hpp>

// Forward declarations
namespace stdair {
  class SegmentPeriod;
}

namespace AIRSCHED {
    /** Class representing the actual business functions for
      an airline segment-period. */
  class SegmentPeriodHelper {
  public:
    // ////////// Business Methods /////////
    /** Fill the attributes of the given segment-period with the cabins
        and classes. */
    static void fill (stdair::SegmentPeriod&, const SegmentStruct&);
    
    /** Fill the attributes of the given segment-period with the list of used
        legs. */
    static void fill (stdair::SegmentPeriod&, const LegStructList_T&);
  };

}
#endif // __AIRSCHED_BOM_SEGMENTPERIODHELPER_HPP
