// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <sstream>
// DSim
#include <dsim/basic/BasConst_DSIM_Service.hpp>
#include <dsim/bom/SimulationStatus.hpp>

namespace DSIM {
  
  // //////////////////////////////////////////////////////////////////////
  SimulationStatus::SimulationStatus (const Key_T& iKey)
    : _key (iKey), _currentDate (iKey.getStartDate()),  
      _nbOfBookings (0),
      _bookingRequestProgressPercentage (0),
      _snapShotProgressPercentage (0),
      _rmEventProgressPercentage (0),
      _optimisationNotificationProgressPercentage (0),
      _cancellationProgressPercentage (0),
      _breakPointProgressPercentage (0),
      _allEventsProgressPercentage (0),
      _simulationMode('r') {
  }
  
  // //////////////////////////////////////////////////////////////////////
  SimulationStatus::SimulationStatus()
    : _key (DEFAULT_DSIM_ID,
            DEFAULT_SIMULATION_START_DATE,
            DEFAULT_SIMULATION_END_DATE),
      _currentDate (DEFAULT_SIMULATION_START_DATE),
      _nbOfBookings (0),
      _bookingRequestProgressPercentage (0),
      _snapShotProgressPercentage (0),
      _rmEventProgressPercentage (0),
      _optimisationNotificationProgressPercentage (0),
      _cancellationProgressPercentage (0),     
      _breakPointProgressPercentage (0),
      _allEventsProgressPercentage (0),
      _simulationMode ('r') {
  }

  // //////////////////////////////////////////////////////////////////////
  SimulationStatus::SimulationStatus (const SimulationStatus& iSimulationStatus)
    : _key (iSimulationStatus._key),
      _currentDate (iSimulationStatus.getStartDate()),
      _nbOfBookings (iSimulationStatus._nbOfBookings),
      _bookingRequestProgressPercentage (iSimulationStatus._bookingRequestProgressPercentage),
      _snapShotProgressPercentage (iSimulationStatus._snapShotProgressPercentage),
      _rmEventProgressPercentage (iSimulationStatus._rmEventProgressPercentage),
      _optimisationNotificationProgressPercentage (iSimulationStatus._optimisationNotificationProgressPercentage),
      _cancellationProgressPercentage (iSimulationStatus._cancellationProgressPercentage),
      _breakPointProgressPercentage (iSimulationStatus._breakPointProgressPercentage),
      _allEventsProgressPercentage (iSimulationStatus._allEventsProgressPercentage),
      _simulationMode(iSimulationStatus.getSimulationMode()) {
    assert (false);
  }

  // //////////////////////////////////////////////////////////////////////
  SimulationStatus::~SimulationStatus() {
   
  } 

  // //////////////////////////////////////////////////////////////////////
  void SimulationStatus::
  updateProgress (const stdair::EventType::EN_EventType& iEventType,
		  const stdair::ProgressPercentage_T lProgressPercentage) { 

    switch (iEventType) {
    case stdair::EventType::BKG_REQ: {
      _bookingRequestProgressPercentage = lProgressPercentage;
      break;
    }
    case stdair::EventType::CX: {
      _cancellationProgressPercentage = lProgressPercentage;
      break;
    }
    case stdair::EventType::OPT_NOT_4_FD: {
      _optimisationNotificationProgressPercentage = lProgressPercentage;
      break;
    }
    case stdair::EventType::SNAPSHOT: {
      _snapShotProgressPercentage = lProgressPercentage;
      break;
    }
    case stdair::EventType::RM: {
      _rmEventProgressPercentage = lProgressPercentage;
      break;
    } 
    case stdair::EventType::BRK_PT: {
      _breakPointProgressPercentage = lProgressPercentage;
      break;
    }
    default: {
      assert (false);
      break;
    }
    }  
  }

  // //////////////////////////////////////////////////////////////////////
  void SimulationStatus::
  updateProgress (const stdair::ProgressPercentage_T lProgressPercentage) {
    _allEventsProgressPercentage = lProgressPercentage;
  }

  // //////////////////////////////////////////////////////////////////////
  void SimulationStatus::reset () { 

    _allEventsProgressPercentage =0;
    _bookingRequestProgressPercentage = 0;
    _cancellationProgressPercentage = 0;
    _optimisationNotificationProgressPercentage = 0;
    _snapShotProgressPercentage = 0;
    _rmEventProgressPercentage = 0;
    _breakPointProgressPercentage = 0;
    _nbOfBookings = 0; 
    _currentDate = getStartDate();
   
  } 
  
  // //////////////////////////////////////////////////////////////////////
  const std::string SimulationStatus::describe() const {
    std::ostringstream oStr;
    oStr << _key.toString()
         << "\nCurrent date: " << _currentDate
         << "\n------ Progress statuses ------"
         << "\nAll events:                 "
         << _allEventsProgressPercentage << "%"
         << "\nBooking requests:           "
         << _bookingRequestProgressPercentage << "%"
         << "\nSnap shots:                 "
         << _snapShotProgressPercentage << "%"
         << "\nRM events:                  "
         << _rmEventProgressPercentage << "%"
      /**<< "\nOptimisation notifications: "
         << _optimisationNotificationProgressPercentage << "%"*/
         << "\nCancellations:              "
         << _cancellationProgressPercentage << "%"
	 << "\nBreak points:               "
         << _breakPointProgressPercentage << "%";
    return oStr.str();
  }
  
}
