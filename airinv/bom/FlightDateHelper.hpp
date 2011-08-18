#ifndef __AIRINV_BOM_FLIGHTDATEHELPER_HPP
#define __AIRINV_BOM_FLIGHTDATEHELPER_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// StdAir
#include <stdair/stdair_basic_types.hpp>

/// Forward declarations
namespace stdair {
  class FlightDate;
}

namespace AIRINV {

  /**
   * @brief Class representing the actual business functions for
   *        an airline flight-date.
   */
  class FlightDateHelper {
  public:
    // ////////////////////// Business Methods ////////////////////////
    /**
     * Fill the attributes derived from the routing legs (e.g., board
     * and off dates).
     *
     * @param const stdair::FlightDate& Reference on the BOM tree.
     */
    static void fillFromRouting (const stdair::FlightDate&);

    /**
     * Update the committed spaces of all the segment- and leg-cabins.
     *
     * That method has be called after events altering the inventory, e.g.:
     * <ul>
     *   <li>Flight dressing (at the initialisation, after the flight
     *       creation or reset)</li>
     *   <li>Sell, cancellations (and re-accommodations)</li>
     *   <li>Flight optimisations (by the revenue management component, e.g.,
     *       RMOL)</li>
     * </ul>
     *
     * @param const stdair::FlightDate& Reference on the BOM tree.
     */
    static void updateCommittedSpaces (const stdair::FlightDate&);

    /**
     * Update the availability pools of all the segment- and leg-cabins.
     *
     * @param const stdair::FlightDate& Reference on the BOM tree.
     */
    static void updateAvailablityPools (const stdair::FlightDate&);

    /**
     * Update the availability pool of a segment-cabin.
     *
     * @param const stdair::FlightDate& Reference on the BOM tree.
     * @param const stdair::CabinCode_T& Cabin for which the availability pool
     *        should be re-calculated.
     */
    static void updateAvailablityPool (const stdair::FlightDate&,
                                       const stdair::CabinCode_T&);

    /**
     * Re-calculate the availabilities for all the flight-dates of the
     * inventory.
     *
     * @param const stdair::FlightDate& Reference on the BOM tree.
     */
    static void recalculateAvailabilities (stdair::FlightDate&);

    /**
     * Empty the inventory from its bookings (and re-calculate the
     * availabilities).
     *
     * @param const stdair::FlightDate& Reference on the BOM tree.
     */
    static void reset (stdair::FlightDate&);

    /**
     * Update the booking controls after an optimisation.
     *
     * @param const stdair::FlightDate& Reference on the BOM tree.
     */
    static void updateBookingControls (stdair::FlightDate&);
  };

}
#endif // __AIRINV_BOM_FLIGHTDATEHELPER_HPP
