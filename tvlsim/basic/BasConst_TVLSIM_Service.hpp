#ifndef __TVLSIM_BAS_BASCONST_TVLSIM_SERVICE_HPP
#define __TVLSIM_BAS_BASCONST_TVLSIM_SERVICE_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
// StdAir
#include <stdair/stdair_file.hpp>
#include <stdair/stdair_date_time_types.hpp>

namespace TVLSIM {

  /**
   * Default ID for the TVLSIM_Service.
   */
  extern const std::string DEFAULT_TVLSIM_ID;
  
  /**
   * Default starting date for a simulation (e.g., 1-JAN-2010).
   */
  extern const stdair::Date_T DEFAULT_SIMULATION_START_DATE;

  /**
   * Default end date for a simulation (e.g., 31-DEC-2010).
   */
  extern const stdair::Date_T DEFAULT_SIMULATION_END_DATE;

  /**
   * Default config INI file (e.g., "tvlsim.cfg").
   */
  extern const stdair::ConfigINIFile DEFAULT_CONFIG_INI_FILE;

}
#endif // __TVLSIM_BAS_BASCONST_TVLSIM_SERVICE_HPP
