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
     * Initialise the AU for the booking classes.
     */
    static void initialiseAU (stdair::SegmentCabin&);
    
    /**
     * Update the segment-cabin with the reservation.
     */
    static void updateFromReservation (const stdair::FlightDate&,
                                       stdair::SegmentCabin&,
                                       const stdair::PartySize_T&);

  private:
    /**
     * Initialise the AU for the booking classes.
     */
    static void initialiseAU (stdair::FareFamily&,
                              const stdair::CabinCapacity_T&);
  };

}
#endif // __AIRINV_BOM_SEGMENTCABINHELPER_HPP
