// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
#include <dsim/basic/BasConst_General.hpp>
#include <dsim/basic/BasConst_DSIM_Service.hpp>

namespace DSIM {

  /**
   * Default ID for the DSIM_Service.
   */
  const std::string DEFAULT_DSIM_ID = "01150";

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

}
