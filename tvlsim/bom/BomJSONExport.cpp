// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <ostream>
#if BOOST_VERSION >= 104100
// Boost Property Tree
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/regex.hpp>
#endif // BOOST_VERSION >= 104100
// TVLSIM
#include <tvlsim/bom/SimulationStatus.hpp>
#include <tvlsim/bom/BomJSONExport.hpp>

#if BOOST_VERSION >= 104100
namespace bpt = boost::property_tree;
#else // BOOST_VERSION >= 104100
namespace bpt {
  typedef char ptree;
}
#endif // BOOST_VERSION >= 104100

namespace TVLSIM { 

  // ////////////////////////////////////////////////////////////////////
  void BomJSONExport::
  jsonExportSimulationStatus (std::ostream& oStream,
			      const SimulationStatus& iSimulationStatus) {

#if BOOST_VERSION >= 104100   
    // Create empty property tree objects
    bpt::ptree ptSimulationStatus;   
    bpt::ptree ptEventStatusList;   
    bpt::ptree pt;    
  
    // Get the current mode of the simulation.
    const SimulationMode& lSimulationMode = 
      iSimulationStatus.getSimulationMode();
    ptSimulationStatus.put ("state", lSimulationMode.describe());
    // Put the start date in the simulation status tree
    const stdair::Date_T& lStartDate = 
      iSimulationStatus.getStartDate();
    ptSimulationStatus.put ("start_date", lStartDate);
    // Put the end date in the simulation status tree
    const stdair::Date_T& lEndDate = 
      iSimulationStatus.getEndDate();
    ptSimulationStatus.put ("end_date", lEndDate); 
    // Put the current date in the simulation status tree
    const stdair::Date_T& lCurrentDate = 
      iSimulationStatus.getCurrentDate();
    ptSimulationStatus.put ("current_date", lCurrentDate);   
    // Put the current number of bookings in the simulation status tree
    const stdair::NbOfBookings_T& lCurrentNumberOfBookings = 
      iSimulationStatus.getCurrentNumberOfBookings();
    ptSimulationStatus.put ("bks", lCurrentNumberOfBookings);     
    // Put the total elapsed time in the simulation status tree
    const double& lCurrentElapsedTime = 
      iSimulationStatus.getCurrentElapsedTime(); 
    ptSimulationStatus.put ("elapsed_time", lCurrentElapsedTime);   
    // Put the estimate remaining time in the simulation status tree
    const double& lCurrentEstimatedRemainingTime = 
      iSimulationStatus.getCurrentEstimatedRemainingTime(); 
    ptSimulationStatus.put ("remaining_time", lCurrentEstimatedRemainingTime);
    // Get the current progress status
    const stdair::ProgressStatus& lCurrentProgressStatus =
      iSimulationStatus.getCurrentProgressStatus();  
    // Put the current number in the simulation status tree
    const stdair::Count_T& lCurrentNumber = 
      lCurrentProgressStatus.getCurrentNb(); 
    ptSimulationStatus.put ("current_number", lCurrentNumber);
    // Put the actual number in the simulation status tree
    const stdair::Count_T& lActualNumber = 
      lCurrentProgressStatus.getActualNb(); 
    ptSimulationStatus.put ("actual_number", lActualNumber);

    // Retrieve the progress status map
    const SEVMGR::ProgressStatusMap_T& lProgressStatusMap = 
      iSimulationStatus.getProgressStatusMap();  
    // Put data for each event type in the simulation status tree 
    for (SEVMGR::ProgressStatusMap_T::const_iterator itPS = 
      lProgressStatusMap.begin(); itPS != lProgressStatusMap.end(); itPS++) {   
      bpt::ptree ptEventStatus;
      const stdair::EventType::EN_EventType& lEventType = itPS->first;
      const stdair::ProgressStatus& lProgressStatus = itPS->second;
      // Put the event type
      const std::string& lEventTypeStr = 
	stdair::EventType::getLabel(lEventType);
      ptEventStatus.put ("type", lEventTypeStr);  
      // Put the current number of events for this event type
      const stdair::Count_T& lCurrentNumber = 
	lProgressStatus.getCurrentNb();
      ptEventStatus.put ("current_number", lCurrentNumber);
      // Put the actual number of events for this event type
      const stdair::Count_T& lActualNumber = 
	lProgressStatus.getActualNb(); 
      ptEventStatus.put ("actual_number", lActualNumber);  

      // Put the current status tree in the status array
      ptEventStatusList.push_back(std::make_pair("", ptEventStatus));
    }

    // Add the list of status to the status tree
    ptSimulationStatus.add_child ("specific_status", ptEventStatusList); 

    // Store the status tree into the global tree
    pt.add_child ("status", ptSimulationStatus); 	

    // Write the property tree into the JSON stream.
    write_json (oStream,  pt);

#endif // BOOST_VERSION >= 104100
  }

}
