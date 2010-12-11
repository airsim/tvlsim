#ifndef __AIRSCHED_BOM_LEGSTRUCT_HPP
#define __AIRSCHED_BOM_LEGSTRUCT_HPP

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
#include <airsched/bom/LegCabinStruct.hpp>

// Forward declarations
namespace stdair {
  class LegDate;
}

namespace AIRSCHED {

  /** Utility Structure for the parsing of Leg structures. */
  struct LegStruct : public stdair::StructAbstract {
    // Attributes
    stdair::AirportCode_T _boardingPoint;
    stdair::DateOffset_T _boardingDateOffset;
    stdair::Duration_T _boardingTime;
    stdair::AirportCode_T _offPoint;
    stdair::DateOffset_T _offDateOffset;
    stdair::Duration_T _offTime;
    stdair::Duration_T _elapsed;
    LegCabinStructList_T _cabinList;

    /** Fill the LegDate objects with the attributes of the LegStruct.
        <br>The given reference date corresponds to the date of the
        FlightDate. Indeed, each Leg gets date off-sets, when compared
        to that (reference) flight-date, both for the boarding date
        and for the off date. */
    void fill (const stdair::Date_T& iRefDate, stdair::LegDate&) const;
      
    /** Give a description of the structure (for display purposes). */
    const std::string describe() const;

    /** Default Constructor. */
    LegStruct();
  };

  /** List of Leg strucutres. */
  typedef std::vector<LegStruct> LegStructList_T;

}
#endif // __AIRSCHED_BOM_LEGSTRUCT_HPP
