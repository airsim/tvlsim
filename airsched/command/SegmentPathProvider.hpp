#ifndef __AIRSCHED_COM_CMD_SEGMENTPATHPROVIDER_HPP
#define __AIRSCHED_COM_CMD_SEGMENTPATHPROVIDER_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STDAIR
#include <stdair/bom/TravelSolutionTypes.hpp>
#include <stdair/command/CmdAbstract.hpp>

// Forward declarations
namespace stdair {
  class BomRoot;
  class ReachableUniverse;
  class OriginDestinationSet;
  class SegmentPathPeriod;
  struct BookingRequestStruct;
}

namespace AIRSCHED {
  
  /** Class handling the generation / instantiation of the travel
      solution block and travel solutions. */
  class SegmentPathProvider : public stdair::CmdAbstract {
    friend class AIRSCHED_Service;
    
  private:
    // ////////// Business Methods /////////
    /** Calculate and return a list of travel solutions corresponding to a given
        product demand. */
    static void buildSegmentPathList (stdair::SegmentPathList_T&,
                                      const stdair::BomRoot&,
                                      const stdair::BookingRequestStruct&);
    static void buildSegmentPathList (stdair::SegmentPathList_T&,
                                      const ReachableUniverse&,
                                      const stdair::BookingRequestStruct&);
    static void buildSegmentPathList (stdair::SegmentPathList_T&,
                                      const OriginDestinationSet&,
                                      const stdair::BookingRequestStruct&);
    static void buildSegmentPathList (stdair::SegmentPathList_T&,
                                      const SegmentPathPeriod&,
                                      const stdair::BookingRequestStruct&);
  };

}
#endif // __AIRSCHED_COM_CMD_SEGMENTPATHPROVIDER_HPP
