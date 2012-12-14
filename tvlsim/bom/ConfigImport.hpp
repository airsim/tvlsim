#ifndef __TVLSIM_BOM_CONFIGIMPORT_HPP
#define __TVLSIM_BOM_CONFIGIMPORT_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// StdAir
#include <stdair/stdair_date_time_types.hpp>
#include <stdair/stdair_maths_types.hpp>
#include <stdair/basic/DemandGenerationMethod.hpp>
// TvlSim
#include <tvlsim/TVLSIM_Types.hpp>
#include <tvlsim/bom/SimulationStatus.hpp>

// Forward declarations
namespace stdair {
  class STDAIR_Service;
}

namespace TVLSIM {  

  /**
   * @brief Utility class to import object in the configuration holder.
   */
  class ConfigImport {
  public:
    // //////////////// Import support methods /////////////////
    /**
     * Search if INI config files exist in pre-defined directories.
     * If existing, try to load their contents into a boost property tree. 
     *
     * @param stdair::STDAIR_ServicePtr_T Reference on the StdAir service.
     * @param const stdair::Date_T& Start date of the simulation.
     * @param const stdair::Date_T& End date of the simulation. 
     * @param const stdair::RandomSeed_T& Seed for the random generation used
     *        by the demand generation component (TraDemGen).  
     * @param const stdair::DemandGenerationMethod&
     *        States whether the demand generation must be performed
     *        following the method based on statistic orders.
     *        The alternative method, while more "intuitive", is also a
     *        sequential algorithm.  
     * @param const NbOfRuns_T& Number of simulation runs to be performed.
     *
     */
    static void importINI (stdair::STDAIR_Service&,
                           const stdair::Date_T& iStartDate,
                           const stdair::Date_T& iEndDate,
                           const stdair::RandomSeed_T&,
                           const stdair::DemandGenerationMethod&,
                           const NbOfRuns_T&);

    /**
     * Load the schedule, O&D, FRAT5, FFDisutility, yield, fare and demand 
     * input file anmes into the configuration holder.
     *
     * @param stdair::STDAIR_ServicePtr_T Reference on the StdAir service.
     * @param const stdair::ScheduleFilePath& Filename of the input schedule file.
     * @param const stdair::ODFilePath& Filename of the input O&D file.
     * @param const stdair::FRAT5FilePath& Filename of the input FRAT5 file.
     * @param const stdair::FFDisutilityFilePath& Filename of the input FF disutility file.
     * @param const AIRRAC::YieldFilePath& Filename of the input yield file.
     * @param const SIMFQT::FareFilePath& Filename of the input fare file.
     * @param const TRADEMGEN::DemandFilePath& Filename of the input demand file.
     */
    static void importFiles (stdair::STDAIR_Service&,
                             const stdair::ScheduleFilePath&,
                             const stdair::ODFilePath&,
                             const stdair::FRAT5FilePath&,
                             const stdair::FFDisutilityFilePath&,
                             const AIRRAC::YieldFilePath&,
                             const SIMFQT::FareFilePath&,
                             const TRADEMGEN::DemandFilePath&);

                     
    
  };
}
#endif // __TVLSIM_BOM_CONFIGIMPORT_HPP
