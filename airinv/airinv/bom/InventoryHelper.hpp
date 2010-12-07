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
    
    /** Make a sale with the given travel solution. */
    static bool sell (stdair::Inventory&, const std::string& iSegmentDateKey,
                      const stdair::ClassCode_T&, const stdair::PartySize_T&);
  };

}
#endif // __AIRINV_BOM_INVENTORYHELPER_HPP
