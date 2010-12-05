#ifndef __AIRSCHED_BOM_SEGMENTSTRUCT_HPP
#define __AIRSCHED_BOM_SEGMENTSTRUCT_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
#include <vector>
// StdAir
#include <stdair/stdair_basic_types.hpp>
#include <stdair/basic/StructAbstract.hpp>
// AirSched
#include <airsched/bom/SegmentCabinStruct.hpp>

// Forward declarations
namespace stdair {
  class SegmentDate;
}

namespace AIRSCHED {
  
  /** Utility Structure for the parsing of Segment structures. */
  struct SegmentStruct : public stdair::StructAbstract {
    // Attributes
    stdair::AirportCode_T _boardingPoint;
    stdair::Date_T _boardingDate;
    stdair::Duration_T _boardingTime;
    stdair::AirportCode_T _offPoint;
    stdair::Date_T _offDate;
    stdair::Duration_T _offTime;
    stdair::Duration_T _elapsed;
    SegmentCabinStructList_T _cabinList;

    /** Fill the SegmentDate objects with the attributes of the
        SegmentStruct. */
    void fill (stdair::SegmentDate&) const;
      
    /** Give a description of the structure (for display purposes). */
    const std::string describe() const;
  };

  /** List of Segment strucutres. */
  typedef std::vector<SegmentStruct> SegmentStructList_T;

}
#endif // __AIRSCHED_BOM_SEGMENTSTRUCT_HPP
