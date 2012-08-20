#ifndef __DSIM_BAS_BASCONST_DSIM_SERVICE_HPP
#define __DSIM_BAS_BASCONST_DSIM_SERVICE_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
// StdAir
#include <stdair/stdair_file.hpp>
#include <stdair/stdair_date_time_types.hpp>

namespace DSIM {

  /**
   * Default ID for the DSIM_Service.
   */
  extern const std::string DEFAULT_DSIM_ID;
  
  /**
   * Default starting date for a simulation (e.g., 1-JAN-2010).
   */
  extern const stdair::Date_T DEFAULT_SIMULATION_START_DATE;

  /**
   * Default end date for a simulation (e.g., 31-DEC-2010).
   */
  extern const stdair::Date_T DEFAULT_SIMULATION_END_DATE;

  /**
   * Default config INI file (e.g., "dsim.cfg").
   */
  extern const stdair::ConfigINIFile DEFAULT_CONFIG_INI_FILE;

}
#endif // __DSIM_BAS_BASCONST_DSIM_SERVICE_HPP
