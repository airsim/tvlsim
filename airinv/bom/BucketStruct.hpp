#ifndef __AIRINV_BOM_BUCKETSTRUCT_HPP
#define __AIRINV_BOM_BUCKETSTRUCT_HPP

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
#include <airinv/AIRINV_Types.hpp>

/// Forward declarations
namespace stdair {
  class Bucket;
}

namespace AIRINV {

  /**
   * @brief Utility Structure for the parsing of Bucket structures.
   */
  struct BucketStruct : public stdair::StructAbstract {
    // Attributes
    stdair::Yield_T _yieldRangeUpperValue;
    stdair::CabinCapacity_T _availability;
    stdair::NbOfSeats_T _nbOfSeats;
    stdair::SeatIndex_T _seatIndex;

    /** Fill the Bucket objects with the attributes of the BucketStruct. */
    void fill (stdair::Bucket&) const;
      
    /** Give a description of the structure (for display purposes). */
    const std::string describe() const;

    /** Default Constructor. */
    BucketStruct();
  };

  /** List of Bucket structures. */
  typedef std::vector<BucketStruct> BucketStructList_T;

}
#endif // __AIRINV_BOM_BUCKETSTRUCT_HPP
