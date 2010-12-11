#ifndef __AIRSCHED_BOM_LEGCABINSTRUCT_HPP
#define __AIRSCHED_BOM_LEGCABINSTRUCT_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
#include <vector>
// StdAir
#include <stdair/stdair_inventory_types.hpp>
#include <stdair/basic/StructAbstract.hpp>

// Forward declarations
namespace stdair {
  class LegCabin;
}

namespace AIRSCHED {

  /** Utility Structure for the parsing of LegCabin details. */
  struct LegCabinStruct : public stdair::StructAbstract {
    // Attributes
    stdair::CabinCode_T _cabinCode;
    stdair::CabinCapacity_T _capacity;

    /** Fill the LegCabin objects with the attributes of the
        LegCabinStruct. */
    void fill (stdair::LegCabin&) const;
      
    /** Give a description of the structure (for display purposes). */
    const std::string describe() const;
  };

  /** List of LegCabin-Detail strucutres. */
  typedef std::vector<LegCabinStruct> LegCabinStructList_T;

}
#endif // __AIRSCHED_BOM_LEGCABINSTRUCT_HPP
