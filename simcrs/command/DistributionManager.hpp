#ifndef __SIMCRS_CMD_DISTRIBUTIONMANAGER_HPP
#define __SIMCRS_CMD_DISTRIBUTIONMANAGER_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// StdAir
#include <stdair/stdair_basic_types.hpp>
#include <stdair/bom/TravelSolutionTypes.hpp>
#include <stdair/basic/PartnershipTechnique.hpp>
// Airinv
#include <airinv/AIRINV_Types.hpp>
// Simcrs
#include <simcrs/SIMCRS_Types.hpp>

// Forward declarations
namespace stdair {
  struct TravelSolutionStruct;
  struct CancellationStruct;
}

namespace AIRINV {
  class AIRINV_Master_Service;
}

namespace SIMCRS {

  /**
   * @brief Command wrapping the travel distribution (CRS/GDS) process.
   */
  class DistributionManager {
    friend class SIMCRS_Service;
  private:
    /** Get the availability for each travel solution in the list.. */
    static void calculateAvailability (AIRINV::AIRINV_Master_Service&,
                                       stdair::TravelSolutionList_T&,
                                       const stdair::PartnershipTechnique&);
    
    /** Register a booking (segment sell). */
    static bool sell (AIRINV::AIRINV_Master_Service&,
                      const stdair::TravelSolutionStruct&,
                      const stdair::NbOfSeats_T&);

    /** Register a cancellation. */
    static bool playCancellation (AIRINV::AIRINV_Master_Service&,
                                  const stdair::CancellationStruct&);

  private:
    /** Constructors. */
    DistributionManager() {}
    DistributionManager(const DistributionManager&) {}
    /** Destructor. */
    ~DistributionManager() {}
  };

}
#endif // __SIMCRS_CMD_DISTRIBUTIONMANAGER_HPP
