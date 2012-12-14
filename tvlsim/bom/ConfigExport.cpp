// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
#include <stdlib.h>
// StdAir
#include <stdair/STDAIR_Service.hpp>
// TVLSIM
#include <tvlsim/bom/ConfigExport.hpp>
#include <tvlsim/bom/SimulationStatus.hpp>


namespace TVLSIM {

  // ////////////////////////////////////////////////////////////////////
  void ConfigExport::
  updateSimulationStatus (stdair::STDAIR_Service& ioSTDAIR_Service,
                          SimulationStatus& ioSimulationStatus,
                          const stdair::Date_T& iStartDate,
                          const stdair::Date_T& iEndDate,
                          const NbOfRuns_T& iNbOfRuns) {
    
    // Look for the start and end date in the configuration holder.
    stdair::Date_T lStartDate (iStartDate);
    const bool hasStartDateBeenRetrieved = 
      ioSTDAIR_Service.exportConfigValue<stdair::Date_T> (lStartDate, 
							 "date.start"); 
    assert (hasStartDateBeenRetrieved == true);
    stdair::Date_T lEndDate (iEndDate);
    const bool hasEndDateBeenRetrieved = 
      ioSTDAIR_Service.exportConfigValue<stdair::Date_T> (lEndDate, 
							 "date.end");
    assert (hasEndDateBeenRetrieved == true);
    // Look for the number of runs in the configuration holder.  
    NbOfRuns_T lTotalNumberOfRuns (iNbOfRuns);
    const bool hasNumberOfRunsBeenRetrieved = 
      ioSTDAIR_Service.exportConfigValue<NbOfRuns_T> (lTotalNumberOfRuns, 
						     "runs.number");
    assert (hasNumberOfRunsBeenRetrieved == true);
 
    // Update the Simulation Status with the start date, end date and number 
    // of runs.
    ioSimulationStatus.setSimulationPeriod (lStartDate, lEndDate); 
    ioSimulationStatus.setTotalNumberOfRuns(lTotalNumberOfRuns);
    
    

  }
}
