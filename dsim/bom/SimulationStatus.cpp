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
    : _key (iKey), 
      _currentDate (iKey.getStartDate()),  
      _nbOfBookings (0),
      _totalElapsedTime (0),
      _estimatedRemainingTime (0),
      _simulationMode('r') {
  }
  
  // //////////////////////////////////////////////////////////////////////
  SimulationStatus::SimulationStatus()
    : _key (DEFAULT_DSIM_ID,
            DEFAULT_SIMULATION_START_DATE,
            DEFAULT_SIMULATION_END_DATE),
      _currentDate (DEFAULT_SIMULATION_START_DATE),
      _nbOfBookings (0),
      _totalElapsedTime (0),
      _estimatedRemainingTime (0),
      _simulationMode ('r') {
  }

  // //////////////////////////////////////////////////////////////////////
  SimulationStatus::SimulationStatus (const SimulationStatus& iSimulationStatus)
    : _key (iSimulationStatus._key),
      _currentDate (iSimulationStatus.getStartDate()),
      _nbOfBookings (iSimulationStatus._nbOfBookings),
      _totalElapsedTime (iSimulationStatus._totalElapsedTime),
      _estimatedRemainingTime (iSimulationStatus._estimatedRemainingTime),
      _simulationMode(iSimulationStatus.getSimulationMode()) {
    assert (false);
  }

  // //////////////////////////////////////////////////////////////////////
  SimulationStatus::~SimulationStatus() {
   
  } 

  // //////////////////////////////////////////////////////////////////////
  void SimulationStatus::
  updateProgress (const stdair::EventType::EN_EventType& iType,
		  const stdair::ProgressStatus& iProgressStatus,
                  const double& iEventMeasure) {    

    const stdair::Count_T lNbOfActualEventsOfSuchType = 
      iProgressStatus.getActualNb();
    const stdair::Count_T lNbOfCurrentEventsOfSuchType = 
      iProgressStatus.getCurrentNb();    
    const stdair::Count_T lNbOfRemainingEventsOfSuchType = 
      lNbOfActualEventsOfSuchType - lNbOfCurrentEventsOfSuchType;
    stdair::Count_T lPreviousNbOfActualEventsOfSuchType = 0;
    stdair::Count_T lPreviousNbOfCurrentEventsOfSuchType = 0;

    // Retrieve, if existing, the ProgressStatus structure
    // corresponding to the given event type and update it
    SEVMGR::ProgressStatusMap_T::iterator itProgressStatus =
      _progressStatusMap.find (iType);
    if (itProgressStatus == _progressStatusMap.end()) { 
      const bool hasInsertBeenSuccessful =
        _progressStatusMap.insert (SEVMGR::ProgressStatusMap_T::
                                   value_type (iType, iProgressStatus)).second;
      
      if (hasInsertBeenSuccessful == false) {
        STDAIR_LOG_ERROR ("No progress_status can be inserted "
                          << "for the following event type: "
                          << stdair::EventType::getLabel(iType) << ".");
	throw stdair::EventException ("No progress_status can be inserted "
				      "for the following event type: "
				      + stdair::EventType::getLabel(iType));
      }
    } else {
      stdair::ProgressStatus& lProgressStatus = itProgressStatus->second;
      lPreviousNbOfActualEventsOfSuchType = lProgressStatus.getActualNb();
      lPreviousNbOfCurrentEventsOfSuchType = lProgressStatus.getCurrentNb();
      lProgressStatus = iProgressStatus;
    }

    // Update the global elapsed time chronometer
    _totalElapsedTime += iEventMeasure;  

    // Retrieve, if existing, the Chronometer structure
    // corresponding to the given event type and update it
    ChronometerMap_T::iterator itChronometer =
      _chronometerMap.find (iType);
    if (itChronometer == _chronometerMap.end()) { 
      const bool hasInsertBeenSuccessful =
        _chronometerMap.insert (ChronometerMap_T::
				value_type (iType, iEventMeasure)).second;
      
      if (hasInsertBeenSuccessful == false) {
        STDAIR_LOG_ERROR ("No chronometer can be inserted "
                          << "for the following event type: "
                          << stdair::EventType::getLabel(iType) << ".");
	throw stdair::EventException ("No chronometer can be inserted for the "
				      "following event type: "
				      + stdair::EventType::getLabel(iType));
      }
      _estimatedRemainingTime += lNbOfRemainingEventsOfSuchType*iEventMeasure; 
      return;
    } else {
      double& lChronometer = itChronometer->second; 
      const stdair::Count_T lPreviousNbOfRemainingEventsOfSuchType = 
	lPreviousNbOfActualEventsOfSuchType - lPreviousNbOfCurrentEventsOfSuchType; 
      if (lPreviousNbOfCurrentEventsOfSuchType > 0) { 
	_estimatedRemainingTime -= lPreviousNbOfRemainingEventsOfSuchType*
	  lChronometer/lPreviousNbOfCurrentEventsOfSuchType;  
      }
      if (lNbOfCurrentEventsOfSuchType > 0) {
	lChronometer += iEventMeasure; 	
	_estimatedRemainingTime += lNbOfRemainingEventsOfSuchType*
	  lChronometer/lNbOfCurrentEventsOfSuchType;
      } 
      _estimatedRemainingTime = std::max(_estimatedRemainingTime, 0.0);
    }
    
  }

  // //////////////////////////////////////////////////////////////////////
  void SimulationStatus::reset () { 

    _progressStatusMap.clear();
    _nbOfBookings = 0;
    _currentDate = getStartDate();
    _totalElapsedTime = 0;
    _estimatedRemainingTime = 0;
    _overallProgressStatus.reset();
    _simulationMode.setMode (SimulationMode::RUNNING);
    _progressStatusMap.clear();
    _chronometerMap.clear();
   
  } 
  
  // //////////////////////////////////////////////////////////////////////
  const std::string SimulationStatus::describe() const {
    std::ostringstream oStr;

    //
    // Add the display of the start, current and end date
    //
    oStr << "\nStart Date ---- Current Date ---- End Date\n"
         << getStartDate() << "     " << _currentDate
         << "       " << getEndDate();

    //
    // Add the display of the overall progress status
    //
    std::string lEventTypeStr = "All";
    describeHelper(lEventTypeStr);  
    oStr << "\n\n----------------- Progress statuses ----------------"
	 << "\n " << lEventTypeStr << _overallProgressStatus.toString()
	 << "\n----------------------------------------------------";

    //
    // Add the display of the specific progress statuses
    //
    std::string lOptionalStr;
    SEVMGR::ProgressStatusMap_T::const_iterator itPS = 
      _progressStatusMap.begin(); 
    while (itPS != _progressStatusMap.end()) {
      const stdair::EventType::EN_EventType& lType = itPS->first;
      const stdair::ProgressStatus& lProgressStatus = itPS->second;
      lEventTypeStr = stdair::EventType::getLabel(lType);
      describeHelper(lEventTypeStr);
      oStr << "\n " << lEventTypeStr << lProgressStatus.toString(); 
      itPS++;
      lOptionalStr = "\n----------------------------------------------------";
    }
    oStr << lOptionalStr << "\n";  

    //
    // Add other information such as the total number of bookings, ...
    //
    oStr << "\nTotal number of bookings:      " << _nbOfBookings
	 << "\n\nElapsed time:             " 
	 << std::setprecision (2) << std::fixed << _totalElapsedTime << " s"
	 << "\nEstimated remaining time: " 
	 << _estimatedRemainingTime << " s"
         << std::endl;
    return oStr.str();
  }

  // //////////////////////////////////////////////////////////////////////
  void SimulationStatus::describeHelper(std::string& ioEventTypeName) const {  
    ioEventTypeName += " Events:"; 
    const std::size_t lSizeBeforePercent (30);  
    ioEventTypeName.resize (lSizeBeforePercent,' ');  
  }
  
}
