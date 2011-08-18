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
  
  /**
   * @brief Class representing the actual business functions for
   *        an airline inventory.
   */
  class InventoryHelper {
  public:
    // //////////////// Business Methods ////////////////
    /**
     * Fill the attributes derived from the routing legs (e.g., board
     * and off dates).
     *
     * @param const stdair::Inventory& Reference on the Inventory-related
     *        BOM tree.
     */
    static void fillFromRouting (const stdair::Inventory&);

    /**
     * Re-calculate the availabilities for all the flight-dates of the
     * inventory.
     *
     * @param stdair::Inventory& Reference on the Inventory-related BOM tree.
     */
    static void recalculateAvailabilities (stdair::Inventory&);

    /**
     * Empty the inventory from its bookings (and re-calculate the
     * availabilities).
     *
     * @param stdair::Inventory& Reference on the Inventory-related BOM tree.
     */
    static void reset (stdair::Inventory&);

    /**
     * Compute the availability for the given travel solution.
     *
     * @param const stdair::Inventory& Reference on the Inventory-related
     *        BOM tree.
     * @param const std::string& Key of the segment-date on which the
     *        availability is calculated.
     * @param stdair::TravelSolutionStruct& The travel-solution (list of
     *        segment-dates) for which the availability is calculated.
     */
    static void calculateAvailability (const stdair::Inventory&,
                                       const std::string& iFullSegmentDateKey,
                                       stdair::TravelSolutionStruct&);
    
    /**
     * Make a sale with the given travel solution.
     *
     * @param const stdair::Inventory& Reference on the Inventory-related
     *        BOM tree.
     * @param const std::string& Key of the segment-date on which the sell
     *        is made.
     * @param const stdair::ClassCode_T& Booking-class on which the sell
     *        is made.
     * @param const stdair::PartySize_T& Number in party.
     */
    static bool sell (stdair::Inventory&, const std::string& iSegmentDateKey,
                      const stdair::ClassCode_T&, const stdair::PartySize_T&);

    /**
     * Take inventory snapshots.
     *
     * @param const stdair::Inventory& Reference on the Inventory-related
     *        BOM tree.
     * @param const stdair::DateTime_T& The date/time at which the snapshot
     *        is to be taken.
     */
    static void takeSnapshots (const stdair::Inventory&,
                               const stdair::DateTime_T& iSnapshotDateTime);
  };

}
#endif // __AIRINV_BOM_INVENTORYHELPER_HPP
