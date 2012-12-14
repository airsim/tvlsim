// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
#include <stdlib.h>
// StdAir
#include <stdair/STDAIR_Service.hpp>
// AIRRAC
#include <airrac/AIRRAC_Types.hpp>
// SimFQT
#include <simfqt/SIMFQT_Types.hpp>
// TRADEMGEN
#include <trademgen/TRADEMGEN_Types.hpp>
// TVLSIM
#include <tvlsim/basic/BasConst_TVLSIM_Service.hpp>
#include <tvlsim/bom/ConfigImport.hpp>
#include <tvlsim/config/tvlsim-paths.hpp>


namespace TVLSIM {

  // ////////////////////////////////////////////////////////////////////
  void ConfigImport::importINI (stdair::STDAIR_Service& ioSTDAIR_Service,
                                const stdair::Date_T& iStartDate,
                                const stdair::Date_T& iEndDate,
                                const stdair::RandomSeed_T& iRandomSeed,
                                const stdair::DemandGenerationMethod& iDemandGenerationMethod,
                                const NbOfRuns_T& iNbOfRuns) {
    
    // Import the start date, the end date, the random seed,
    // the demand generation method and the number of runs in the 
    // configuration.
    const std::string& lStartDateStr =
      boost::gregorian::to_simple_string(iStartDate);
    ioSTDAIR_Service.importConfigValue (lStartDateStr, "date.start");    
    const std::string& lEndDateStr =
      boost::gregorian::to_simple_string(iEndDate); 
    ioSTDAIR_Service.importConfigValue (lEndDateStr, "date.end");   
    const std::string& lRandomSeedStr = 
      boost::lexical_cast<std::string> (iRandomSeed);
    ioSTDAIR_Service.importConfigValue (lRandomSeedStr, "random.seed"); 
    const std::string& lDemandGenerationMethodStr = 
      iDemandGenerationMethod.getMethodAsString();   
    ioSTDAIR_Service.importConfigValue (lDemandGenerationMethodStr, 
				       "demand generation.method");  
    const std::string& lNbOfRunsStr = 
      boost::lexical_cast<std::string> (iNbOfRuns); 
    ioSTDAIR_Service.importConfigValue (lNbOfRunsStr, 
				       "runs.number");

    // Look for a config INI file (which may be present in the etc
    // directory) and try to import the configuration
    std::string lETCDirPath ("/etc/");
    lETCDirPath += DEFAULT_CONFIG_INI_FILE.name();
    const stdair::ConfigINIFile lETCDirConfigFile (lETCDirPath);
    ioSTDAIR_Service.importINIConfig (lETCDirConfigFile);

    // Look for a config INI file (which may be present in the home
    // directory) and try to import the configuration
    const char *lHomeDIR = getenv("HOME");
    std::string lHomeDIRPath (lHomeDIR);
    lHomeDIRPath += "/";
    lHomeDIRPath += DEFAULT_CONFIG_INI_FILE.name();
    const stdair::ConfigINIFile lConfigFile (lHomeDIRPath);
    ioSTDAIR_Service.importINIConfig (lConfigFile);

    // Look for a config INI file (which may be present in the current
    // directory) and try to import the configuration
    ioSTDAIR_Service.importINIConfig (DEFAULT_CONFIG_INI_FILE);

  }

  // ////////////////////////////////////////////////////////////////////
  void ConfigImport::importFiles (stdair::STDAIR_Service& ioSTDAIR_Service,
                                  const stdair::ScheduleFilePath& iScheduleInputFilename,
                                  const stdair::ODFilePath& iODInputFilename,
                                  const stdair::FRAT5FilePath& iFRAT5InputFilename,
                                  const stdair::FFDisutilityFilePath& iFFDisutilityInputFilename,
                                  const AIRRAC::YieldFilePath& iYieldInputFilepath,
                                  const SIMFQT::FareFilePath& iFareInputFilepath,
                                  const TRADEMGEN::DemandFilePath& iDemandFilepath) {

    // Import the input files names into the configuration
    ioSTDAIR_Service.importConfigValue (iScheduleInputFilename.name(), 
                                        "input.schedule");    
    ioSTDAIR_Service.importConfigValue (iODInputFilename.name(), 
                                        "input.ond");   
    ioSTDAIR_Service.importConfigValue (iFRAT5InputFilename.name(), 
                                        "input.frat5"); 
    ioSTDAIR_Service.importConfigValue (iFFDisutilityInputFilename.name(), 
                                        "input.ffdisutility");    
    ioSTDAIR_Service.importConfigValue (iYieldInputFilepath.name(), 
                                        "input.yield");   
    ioSTDAIR_Service.importConfigValue (iFareInputFilepath.name(), 
                                        "input.fare");  
    ioSTDAIR_Service.importConfigValue (iDemandFilepath.name(), 
                                        "input.demand");
  }
  
}
