#ifndef __AVLCAL_CMD_INDEXBUILDER_HPP
#define __AVLCAL_CMD_INDEXBUILDER_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// Avlcal
#include <avlcal/AVLCAL_Types.hpp>

namespace AVLCAL {

  /** Command wrapping the availability calculation request process. */
  class AvailabilityCalculator {
    friend class AVLCAL_Service;
  private:

    /** Perform a seat availability calculation. */
    static void avlCalculate (const AirlineCode_T&, const PartySize_T&);

  private:
    /** Constructors. */
    AvailabilityCalculator() {}
    AvailabilityCalculator(const AvailabilityCalculator&) {}
    /** Destructor. */
    ~AvailabilityCalculator() {}
  };

}
#endif // __AVLCAL_CMD_INDEXBUILDER_HPP
