#ifndef __SIMLFS_CMD_LOWFARESEARCHMANAGER_HPP
#define __SIMLFS_CMD_LOWFARESEARCHMANAGER_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// StdAir
#include <stdair/stdair_basic_types.hpp>
// SimLFS
#include <simlfs/SIMLFS_Types.hpp>

namespace SIMLFS {

  /** Command wrapping the pricing request process. */
  class LowFareSearchManager {
    friend class SIMLFS_Service;
  private:

    /** Quote (with a price) a travel request. */
    static stdair::PriceValue_T priceQuote (const stdair::AirlineCode_T&,
                                            const stdair::PartySize_T&);

  private:
    /** Constructors. */
    LowFareSearchManager() {}
    LowFareSearchManager(const LowFareSearchManager&) {}
    /** Destructor. */
    ~LowFareSearchManager() {}
  };

}
#endif // __SIMLFS_CMD_LOWFARESEARCHMANAGER_HPP
