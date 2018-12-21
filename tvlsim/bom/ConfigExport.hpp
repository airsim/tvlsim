#ifndef __TVLSIM_BOM_CONFIGEXPORT_HPP
#define __TVLSIM_BOM_CONFIGEXPORT_HPP

// //////////////////////////////////////////////////////////////////////
// Export section
// //////////////////////////////////////////////////////////////////////
// StdAir
#include <stdair/stdair_date_time_types.hpp>
// TvlSim
#include <tvlsim/TVLSIM_Types.hpp>


// Forward declarations
namespace stdair {
  class STDAIR_Service;
}

namespace TVLSIM {

  // Forward declaration
  struct SimulationStatus;

  /**
   * @brief Utility class to export object from the confguration holder.
   */
  class ConfigExport {
  public:
    // //////////////// Export support methods /////////////////
    /**
     * Look for the start date, the end date and the number of runs into the
     * configuration holder and update the simulation status with the retrieved
     * values.
     *
     * @param stdair::STDAIR_ServicePtr_T Reference on the StdAir service.
     * @param const SimulationStatus& Simulation status to be updated.
     * @param const stdair::Date_T& Start date of the simulation.
     * @param const stdair::Date_T& End date of the simulation. 
     * @param const NbOfRuns_T& Number of simulation runs to be performed.
     *
     */
    static void updateSimulationStatus (stdair::STDAIR_Service&,
                                        SimulationStatus&,
                                        const stdair::Date_T& iStartDate,
                                        const stdair::Date_T& iEndDate,
                                        const NbOfRuns_T&);
    
  };
}
#endif // __TVLSIM_BOM_CONFIGEXPORT_HPP
