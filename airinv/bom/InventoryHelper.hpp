#ifndef __AIRINV_BOM_INVENTORYHELPER_HPP
#define __AIRINV_BOM_INVENTORYHELPER_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
// StdAir
#include <stdair/stdair_basic_types.hpp>

// Forward declarations
namespace stdair {
  struct TravelSolutionStruct;
  class Inventory;
}

namespace AIRINV {
  
  /** Class representing the actual business functions for
      an airline inventory. */
  class InventoryHelper {
  public:
    // ////////// Business Methods /////////
    /** Fill the attributes derived from the routing legs (e.g., board
        and off dates). */
    static void fillFromRouting (const stdair::Inventory&);

    /** Compute the availability for the given travel solution. */
    static void calculateAvailability (const stdair::Inventory&,
                                       const std::string&,
                                       stdair::TravelSolutionStruct&);

    /** Get yield and bid price information for the given travel solution. */
    static void getYieldAndBidPrice (const stdair::Inventory&,
                                       const std::string&,
                                       stdair::TravelSolutionStruct&);
    
    /** Make a sale with the given travel solution. */
    static bool sell (stdair::Inventory&, const std::string& iSegmentDateKey,
                      const stdair::ClassCode_T&, const stdair::PartySize_T&);
    
    /** Make a cancellation. */
    static bool cancel (stdair::Inventory&, const std::string& iSegmentDateKey,
                        const stdair::ClassCode_T&, const stdair::PartySize_T&);

    /** Take inventory snapshots. */
    static void takeSnapshots (const stdair::Inventory&,
                               const stdair::DateTime_T&);
  };

}
#endif // __AIRINV_BOM_INVENTORYHELPER_HPP
