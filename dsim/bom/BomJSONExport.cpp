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
// DSIM
#include <dsim/bom/SimulationStatus.hpp>
#include <dsim/bom/BomJSONExport.hpp>

#if BOOST_VERSION >= 104100
namespace bpt = boost::property_tree;
#else // BOOST_VERSION >= 104100
namespace bpt {
  typedef char ptree;
}
#endif // BOOST_VERSION >= 104100

namespace DSIM { 

  // ////////////////////////////////////////////////////////////////////
  void BomJSONExport::
  jsonExportSimulationStatus (std::ostream& oStream,
			      const SimulationStatus& iSimulationStatus) {

#if BOOST_VERSION >= 104100   
    // Create an empty property tree object
    bpt::ptree ptSimulationStatus;  
    bpt::ptree pt;    

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
    // Put the total number of bookings in the simulation status tree
    const stdair::NbOfBookings_T& lNumberOfBookings = 
      iSimulationStatus.getNumberOfBookings();
    ptSimulationStatus.put ("bks", lNumberOfBookings);     
    // Put the total elapsed time in the simulation status tree
    const double& lElapsedTime = 
      iSimulationStatus.getElapsedTime(); 
    ptSimulationStatus.put ("elapsed_time", lElapsedTime);   
    // Put the estimate remaining time in the simulation status tree
    const double& lEstimatedRemainingTime = 
      iSimulationStatus.getEstimatedRemainingTime(); 
    ptSimulationStatus.put ("remaining_time", lEstimatedRemainingTime);
    // Get the overall progress status
    const stdair::ProgressStatus& lOverallProgressStatus =
      iSimulationStatus.getOverallProgressStatus();  
    // Put the overall current number in the simulation status tree
    const stdair::Count_T& lOverallCurrentNumber = 
      lOverallProgressStatus.getCurrentNb(); 
    ptSimulationStatus.put ("current_number", lOverallCurrentNumber);
    // Put the overall current number in the simulation status tree
    const stdair::Count_T& lOverallActualNumber = 
      lOverallProgressStatus.getActualNb(); 
    ptSimulationStatus.put ("actual_number", lOverallActualNumber);

    // Store the status tree into the global tree
    pt.add_child ("status", ptSimulationStatus); 	

    // Write the property tree into the JSON stream.
    write_json (oStream,  pt);

#endif // BOOST_VERSION >= 104100
  }

}
