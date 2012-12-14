// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
#include <tvlsim/basic/BasConst_General.hpp>
#include <tvlsim/basic/BasConst_TVLSIM_Service.hpp>

namespace TVLSIM {

  /**
   * Default ID for the TVLSIM_Service.
   */
  const std::string DEFAULT_TVLSIM_ID = "01150";

  /**
   * Default starting date for a simulation.
   */
  const stdair::Date_T DEFAULT_SIMULATION_START_DATE (2010,
                                                      boost::gregorian::Jan, 1);

  /**
   * Default end date for a simulation.
   */
  const stdair::Date_T DEFAULT_SIMULATION_END_DATE (2010,
                                                    boost::gregorian::Dec, 31);

  /**
   * Default number of runs for the simulation (= one run).
   */
  const NbOfRuns_T DEFAULT_NUMBER_OF_RUNS (1);

  /**
   * Default config INI file (e.g., "tvlsim.cfg").
   */
  extern const stdair::ConfigINIFile DEFAULT_CONFIG_INI_FILE ("tvlsim.cfg");

}
