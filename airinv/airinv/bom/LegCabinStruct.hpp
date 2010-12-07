#ifndef __AIRINV_BOM_LEGCABINSTRUCT_HPP
#define __AIRINV_BOM_LEGCABINSTRUCT_HPP

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
#include <airinv/bom/BucketStruct.hpp>

// Forward declarations
namespace stdair {
  class LegCabin;
}

namespace AIRINV {

  /** Utility Structure for the parsing of LegCabin details. */
  struct LegCabinStruct : public stdair::StructAbstract {
    // Attributes
    stdair::CabinCode_T _cabinCode;
    stdair::CabinCapacity_T _saleableCapacity;
    stdair::CapacityAdjustment_T _adjustment;
    stdair::CapacityAdjustment_T _dcsRegrade;
    stdair::AuthorizationLevel_T _au;
    stdair::Availability_T _avPool;
    stdair::UPR_T _upr;
    stdair::NbOfBookings_T _nbOfBookings;
    stdair::Availability_T _nav;
    stdair::Availability_T _gav;
    stdair::OverbookingRate_T _acp;
    stdair::NbOfBookings_T _etb;
    stdair::NbOfBookings_T _staffNbOfBookings;
    stdair::NbOfBookings_T _wlNbOfBookings;
    stdair::NbOfBookings_T _groupNbOfBookings;
    BucketStructList_T _bucketList;

    /** Fill the LegCabin objects with the attributes of the
        LegCabinStruct. */
    void fill (stdair::LegCabin&) const;
      
    /** Give a description of the structure (for display purposes). */
    const std::string describe() const;
  };

  /** List of LegCabin-Detail strucutres. */
  typedef std::vector<LegCabinStruct> LegCabinStructList_T;

}
#endif // __AIRINV_BOM_LEGCABINSTRUCT_HPP
