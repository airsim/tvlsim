#ifndef __AIRINV_BOM_SEGMENTCABINSTRUCT_HPP
#define __AIRINV_BOM_SEGMENTCABINSTRUCT_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
#include <vector>
// StdAir
#include <stdair/stdair_inventory_types.hpp>
#include <stdair/basic/StructAbstract.hpp>
// AirInv
#include <airinv/bom/FareFamilyStruct.hpp>
#include <airinv/bom/BookingClassStruct.hpp>

// Forward declarations
namespace stdair {
  class SegmentCabin;
}

namespace AIRINV {
  
  /** Utility Structure for the parsing of SegmentCabin details. */
  struct SegmentCabinStruct : public stdair::StructAbstract {
    // Attributes
    stdair::CabinCode_T _cabinCode;
    stdair::ClassList_String_T _classes;
    stdair::FamilyCode_T _itFamilyCode;
    stdair::NbOfBookings_T _nbOfBookings;
    FareFamilyStructList_T _fareFamilies;
    BookingClassStructList_T _classList;
    
    /** Fill the SegmentCabin objects with the attributes of the
        SegmentCabinStruct. */
    void fill (stdair::SegmentCabin&) const;
      
    /** Give a description of the structure (for display purposes). */
    const std::string describe() const;

  };

  /** List of SegmentCabin-Detail strucutres. */
  typedef std::vector<SegmentCabinStruct> SegmentCabinStructList_T;

}
#endif // __AIRINV_BOM_SEGMENTCABINSTRUCT_HPP
