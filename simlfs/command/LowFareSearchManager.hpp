#ifndef __SIMLFS_CMD_LOWFARESEARCHMANAGER_HPP
#define __SIMLFS_CMD_LOWFARESEARCHMANAGER_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// Simlfs
#include <simlfs/SIMLFS_Types.hpp>

namespace SIMLFS {

  /** Command wrapping the pricing request process. */
  class LowFareSearchManager {
    friend class SIMLFS_Service;
  private:

    /** Quote (with a price) a travel request. */
    static Price_T priceQuote (const FareQuoteID_T&,
                               const AirlineCode_T&, const PartySize_T&);

  private:
    /** Constructors. */
    LowFareSearchManager() {}
    LowFareSearchManager(const LowFareSearchManager&) {}
    /** Destructor. */
    ~LowFareSearchManager() {}
  };

}
#endif // __SIMLFS_CMD_LOWFARESEARCHMANAGER_HPP
