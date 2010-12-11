#ifndef __AIRSCHED_CMD_SEGMENTPATHGENERATOR_HPP
#define __AIRSCHED_CMD_SEGMENTPATHGENERATOR_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <vector>
// StdAir
#include <stdair/stdair_basic_types.hpp>
#include <stdair/command/CmdAbstract.hpp>
// AirSched
#include <airsched/AIRSCHED_Types.hpp>

// Forward declarations
namespace stdair {
  class BomRoot;
  class Inventory;
  class FlightPeriod;
  class SegmentPeriod;
}

namespace AIRSCHED {
  //Forward declarations
  class ReachableUniverse;
  class OriginDestinationSet;
  class SegmentPathPeriod;
  
  /** Class handling the generation / instantiation of the network BOM. */
  class SegmentPathGenerator : public stdair::CmdAbstract {
  public:
    /** Generate the segment path network. */
    static void createSegmentPathNetwork (const stdair::BomRoot&);

    /** Generate the single-segment segment path periods while browsing the
        inventory and flight-period lists. */
    static void createSinglePaths (const stdair::Inventory&);
    static void createSinglePaths (const stdair::FlightPeriod&);

    /** Generate the single-segment segment path period with the
        given segment-period. */
    static void createSinglePath (stdair::SegmentPeriod&);
    static void createSinglePath (ReachableUniverse&, stdair::SegmentPeriod&);

    /** Generate the i-fixed-length list of segment path period objects. */
    static void buildSegmentPathNetwork (const stdair::BomRoot&,
                                         const stdair::NbOfSegments_T&);
    static void buildSegmentPathNetwork (ReachableUniverse&,
                                         const stdair::NbOfSegments_T&);

    
  };
 
}
#endif // __AIRSCHED_CMD_SEGMENTPATHGENERATOR_HPP
