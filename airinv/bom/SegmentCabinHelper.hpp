#ifndef __AIRINV_BOM_SEGMENTCABINHELPER_HPP
#define __AIRINV_BOM_SEGMENTCABINHELPER_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// StdAir
#include <stdair/stdair_basic_types.hpp>

// Forward declarations
namespace stdair {
  class FlightDate;
  class SegmentCabin;
  class FareFamily;
}

namespace AIRINV {

  /**
   * @brief Class representing the actual business functions for
   * an airline segment-cabin.
   */
  class SegmentCabinHelper {
  public:
    // ////////// Business Methods /////////
    
    /**
     * Update the segment-cabin with the reservation.
     */
    static void updateFromReservation (const stdair::FlightDate&,
                                       stdair::SegmentCabin&,
                                       const stdair::PartySize_T&);

    /**
     * Build the pseudo bid price vector from the vectors of the leg-cabins.
     */
    static void buildPseudoBidPriceVector (stdair::SegmentCabin&);

    /**
     * Update the booking controls using the pseudo bid price vector.
     */
    static void updateBookingControlsUsingPseudoBidPriceVector (const stdair::SegmentCabin&);

    /**
     * Update the authorisation levels using the booking limits. */
    static void updateAUs (const stdair::SegmentCabin&);

    /**
     * Update the availability of the booking classes. */
    static void updateAvailabilities (const stdair::SegmentCabin&);
    
    /**
     * Initialise the AU for the booking classes.
     */
    static void initialiseAU (stdair::SegmentCabin&);
  };

}
#endif // __AIRINV_BOM_SEGMENTCABINHELPER_HPP
