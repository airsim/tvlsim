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
      _nbOfCancellations (0),
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
      _nbOfCancellations (0),
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
      _nbOfCancellations (iSimulationStatus._nbOfCancellations),
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
    _nbOfCancellations = 0;
    _currentDate = getStartDate();
    _simulationMode.setMode (SimulationMode::RUNNING);
   
  } 
  
  // //////////////////////////////////////////////////////////////////////
  const std::string SimulationStatus::describe() const {
    std::ostringstream oStr;
    oStr << "\nStart Date ---- Current Date ---- End Date\n"
         << getStartDate() << "     " << _currentDate
         << "       " << getEndDate()
         << "\n\n----------- Progress statuses ----------"
         << "\n All events: \t\t\t"
         << _allEventsProgressPercentage << "% "
         << "\n----------------------------------------"
         << "\n Booking requests: \t\t"
         << _bookingRequestProgressPercentage << "% "
         << "\n Snap shots: \t\t\t"
         << _snapShotProgressPercentage << "% "
         << "\n RM events: \t\t\t"
         << _rmEventProgressPercentage << "% "
      /**<< "\n Optimisation notifications: "
         << _optimisationNotificationProgressPercentage << "%"*/
         << "\n Cancellations: \t\t"
         << _cancellationProgressPercentage << "% "
	 << "\n Break points: \t\t\t"
         << _breakPointProgressPercentage << "% "
         << "\n----------------------------------------"
         << "\n\nTotal number of bookings:       " << _nbOfBookings
         << "\nTotal number of cancellations:  " << _nbOfCancellations
         << std::endl;
    return oStr.str();
  }
  
}
