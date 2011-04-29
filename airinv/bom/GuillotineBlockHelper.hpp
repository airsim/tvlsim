#ifndef __AIRINV_BOM_GUILLOTINEBLOCKHELPER_HPP
#define __AIRINV_BOM_GUILLOTINEBLOCKHELPER_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
// StdAir
#include <stdair/stdair_basic_types.hpp>

// Forward declarations
namespace stdair {
  class GuillotineBlock;
  class SegmentCabin;
}

namespace AIRINV {
  
  /** Class representing the actual business functions for
      an airline inventory. */
  class GuillotineBlockHelper {
  public:
    // ////////// Business Methods /////////
    /** Take inventory snapshots. */
    static void takeSnapshots (stdair::GuillotineBlock&,
                               const stdair::DateTime_T&);

  private:
    // ////////// Helpers for business methods. //////////
    /** Take the snapshots for the given segment-cabin. */
    static void takeSnapshots (stdair::GuillotineBlock&, const stdair::Date_T&,
                               const stdair::SegmentCabin&,
                               const stdair::BlockNumber_T);
  };

}
#endif // __AIRINV_BOM_GUILLOTINEBLOCKHELPER_HPP
