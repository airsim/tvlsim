#ifndef __AIRINV_BOM_LEGSTRUCT_HPP
#define __AIRINV_BOM_LEGSTRUCT_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
#include <vector>
// STDAIR
#include <stdair/stdair_inventory_types.hpp>
#include <stdair/basic/StructAbstract.hpp>
// AIRINV
#include <airinv/bom/LegCabinStruct.hpp>

// Forward declarations
namespace stdair {
  class LegDate;
}

namespace AIRINV {

  /** Utility Structure for the parsing of Leg structures. */
  struct LegStruct : public stdair::StructAbstract {
    // Attributes
    stdair::AirportCode_T _boardingPoint;
    stdair::DateOffset_T _boardingDateOffset;
    stdair::Date_T _boardingDate;
    stdair::Duration_T _boardingTime;
    stdair::AirportCode_T _offPoint;
    stdair::DateOffset_T _offDateOffset;
    stdair::Date_T _offDate;
    stdair::Duration_T _offTime;
    stdair::Duration_T _elapsed;
    LegCabinStructList_T _cabinList;

    /** Fill the LegDate objects with the attributes of the LegStruct.
        <br>The given reference date corresponds to the date of the
        FlightDate. Indeed, each Leg gets date off-sets, when compared
        to that (reference) flight-date, both for the boarding date
        and for the off date. */
    void fill (const stdair::Date_T& iRefDate, stdair::LegDate&) const;

    /** Fill the LegDate objects with the attributes of the LegStruct. */
    void fill (stdair::LegDate&) const;
      
    /** Give a description of the structure (for display purposes). */
    const std::string describe() const;

    /** Default Constructor. */
    LegStruct();
  };

  /** List of Leg structures. */
  typedef std::vector<LegStruct> LegStructList_T;

}
#endif // __AIRINV_BOM_LEGSTRUCT_HPP
