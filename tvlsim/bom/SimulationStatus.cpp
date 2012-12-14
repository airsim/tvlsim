// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <sstream>
// TvlSim
#include <tvlsim/basic/BasConst_General.hpp>
#include <tvlsim/basic/BasConst_TVLSIM_Service.hpp>
#include <tvlsim/bom/SimulationStatus.hpp>

namespace TVLSIM {
  
  // //////////////////////////////////////////////////////////////////////
  SimulationStatus::SimulationStatus (const Key_T& iKey)
    : _key (iKey),   
      _startDate (DEFAULT_SIMULATION_START_DATE),    
      _currentDate (DEFAULT_SIMULATION_START_DATE),     
      _endDate (DEFAULT_SIMULATION_END_DATE),   
      _totalNumberOfRuns (DEFAULT_NUMBER_OF_RUNS),
      _currentRun (DEFAULT_NUMBER_OF_RUNS),
      _currentNbOfBookings (0),
      _totalNbOfBookings (0),
      _totalElapsedTime (0),    
      _estimatedRemainingTime (0),     
      _currentElapsedTime (0),
      _currentEstimatedRemainingTime (0),     
      _simulationMode('s') {
  }
  
  // //////////////////////////////////////////////////////////////////////
  SimulationStatus::SimulationStatus()
    : _key (DEFAULT_TVLSIM_ID),
      _startDate (DEFAULT_SIMULATION_START_DATE),   
      _currentDate (DEFAULT_SIMULATION_START_DATE),     
      _endDate (DEFAULT_SIMULATION_END_DATE),   
      _totalNumberOfRuns (DEFAULT_NUMBER_OF_RUNS),
      _currentRun (0),
      _currentNbOfBookings (0), 
      _totalNbOfBookings (0),  
      _totalElapsedTime (0),    
      _estimatedRemainingTime (0),  
      _currentElapsedTime  (0),      
      _currentEstimatedRemainingTime (0),  
      _simulationMode ('s') {
  }

  // //////////////////////////////////////////////////////////////////////
  SimulationStatus::SimulationStatus (const SimulationStatus& iSimulationStatus)
    : _key (iSimulationStatus._key),   
      _startDate (iSimulationStatus._startDate),  
      _currentDate (iSimulationStatus._currentDate),     
      _endDate (iSimulationStatus._endDate),     
      _totalNumberOfRuns (iSimulationStatus._totalNumberOfRuns),
      _currentRun (iSimulationStatus._currentRun),
      _currentNbOfBookings (iSimulationStatus._currentNbOfBookings), 
      _totalNbOfBookings (iSimulationStatus._totalNbOfBookings),
      _totalElapsedTime (iSimulationStatus._totalElapsedTime),    
      _estimatedRemainingTime (iSimulationStatus._estimatedRemainingTime), 
      _currentElapsedTime (iSimulationStatus._currentElapsedTime),
      _currentEstimatedRemainingTime (iSimulationStatus._currentEstimatedRemainingTime),
      _simulationMode(iSimulationStatus.getSimulationMode()) {
    assert (false);
  }

  // //////////////////////////////////////////////////////////////////////
  SimulationStatus::~SimulationStatus() {
   
  }   

  // //////////////////////////////////////////////////////////////////////
  void SimulationStatus::
  setCurrentProgressStatus (const stdair::ProgressStatus& iProgressStatus) { 
 
    // Update the current progress status
    _currentProgressStatus = iProgressStatus;
  }
 
  // //////////////////////////////////////////////////////////////////////
  bool SimulationStatus::isTheSimulationDone() const {

    bool isTheSimulationDone = false;

    if (_simulationMode == SimulationMode::DONE) {
      if (_currentRun >= _totalNumberOfRuns) {
	isTheSimulationDone = true;
	return isTheSimulationDone;
      } else { 
	return isTheSimulationDone;
      }
    } else {  
      return isTheSimulationDone;
    }      

    return isTheSimulationDone;
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

    // Update the cuurent elapsed time chronometer
    _currentElapsedTime += iEventMeasure;  

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
      _currentEstimatedRemainingTime += 
	lNbOfRemainingEventsOfSuchType*iEventMeasure; 
      return;
    } else {
      double& lChronometer = itChronometer->second; 
      const stdair::Count_T lPreviousNbOfRemainingEventsOfSuchType = 
	lPreviousNbOfActualEventsOfSuchType - 
	lPreviousNbOfCurrentEventsOfSuchType; 
      if (lPreviousNbOfCurrentEventsOfSuchType > 0) { 
	_currentEstimatedRemainingTime -= 
	  lPreviousNbOfRemainingEventsOfSuchType*
	  lChronometer/lPreviousNbOfCurrentEventsOfSuchType;  
      }
      if (lNbOfCurrentEventsOfSuchType > 0) {
	lChronometer += iEventMeasure; 	
	_currentEstimatedRemainingTime  += lNbOfRemainingEventsOfSuchType*
	  lChronometer/lNbOfCurrentEventsOfSuchType;
      } 
      _currentEstimatedRemainingTime = 
	std::max(_currentEstimatedRemainingTime, 0.0);
    }   

    // Update the global chronometers
    _totalElapsedTime = _currentElapsedTime 
      + (_currentRun - 1)
      *(_currentElapsedTime + _currentEstimatedRemainingTime);
    _estimatedRemainingTime = _currentEstimatedRemainingTime
      + (_totalNumberOfRuns - _currentRun)
      *(_currentElapsedTime + _currentEstimatedRemainingTime);
    
  }  

  // //////////////////////////////////////////////////////////////////////
  void SimulationStatus::
  increaseGlobalNumberOfBookings (const stdair::PartySize_T& iPartySize) {
    _currentNbOfBookings += iPartySize;
    _totalNbOfBookings += iPartySize;
  }

  // //////////////////////////////////////////////////////////////////////
  void SimulationStatus::reset () {    

    // Update the total number of bookings
    _totalNbOfBookings -= _currentNbOfBookings;  

    // Update the current and total time counters  
    _estimatedRemainingTime = 
      (_currentElapsedTime + _currentEstimatedRemainingTime)
      *(_totalNumberOfRuns - _currentRun + 1);
    _totalElapsedTime -= _currentElapsedTime;

    // Prepare the new run
    prepareNewRun ();
    
    // Reset the simulation Mode
    _simulationMode.setMode (SimulationMode::START);
   
  }

  // ////////////////////////////////////////////////////////////////////// 
  void SimulationStatus::prepareNewRun () { 
   
    // Reset the start date
    _currentDate = _startDate;

    // Update the current number of bookings
    _currentNbOfBookings = 0;
  
    // Update the current and total time counters
    _currentElapsedTime = 0;
    _currentEstimatedRemainingTime = 0;

    // Reset the current progress status and maps
    _currentProgressStatus.reset();
    _progressStatusMap.clear();
    _chronometerMap.clear();

    // Increase the current run number.
    _currentRun++;
   
  }  

  // //////////////////////////////////////////////////////////////////////
  void SimulationStatus::displayStartStatusMessage () const {  

    std::ostringstream oStr;
	
    switch (_simulationMode.getMode()) {
    case SimulationMode::START:   
    case SimulationMode::DONE:	
      // DEBUG
      oStr << "Simulation";
      if (_totalNumberOfRuns > 1) {
	oStr << " [" <<_currentRun << "/" << _totalNumberOfRuns << "]";
      }
      oStr << " is starting..." << std::endl;  
      std::cout << oStr.str() << std::endl;    
      STDAIR_LOG_DEBUG (oStr.str());
      break;

    case SimulationMode::BREAK:	 

      // DEBUG
      oStr << "Resuming the Simulation ";
      if (_totalNumberOfRuns > 1) {
	oStr << "[" <<_currentRun << "/" << _totalNumberOfRuns << "]"
	     << std::endl;  
      } 
      std::cout << oStr.str() << std::endl;  
      STDAIR_LOG_DEBUG (oStr.str());
      break; 
 
    case SimulationMode::RUNNING:
    default :
      break;
    } 
  } 

  // //////////////////////////////////////////////////////////////////////
  void SimulationStatus::displayEndStatusMessage () const {  

    std::ostringstream oStr;

    switch (_simulationMode.getMode()) {  
    case SimulationMode::DONE:	
      // DEBUG
      oStr << "Simulation";
      if (_totalNumberOfRuns > 1) {
	oStr << " [" <<_currentRun << "/" << _totalNumberOfRuns << "]";
      }
      oStr << " has ended." << std::endl;  
      std::cout << oStr.str() << std::endl;    
      STDAIR_LOG_DEBUG (oStr.str());
      break;

    case SimulationMode::BREAK:		

      // DEBUG
      std::cout << "The simulation has stopped on '" << _currentDate
		<< "': break point encountered.\n" << describe() << std::endl; 
      STDAIR_LOG_DEBUG ("Break point encountered\n" << describe());
      break; 
  
    case SimulationMode::START:  
    case SimulationMode::RUNNING:
    default :
      break;
    } 
  }
   
  // //////////////////////////////////////////////////////////////////////
  const std::string SimulationStatus::describe() const {
    std::ostringstream oStr;

    //
    // Display information on all the runs (if many)
    //  
    if (_totalNumberOfRuns > 1) {  
      //
      // Add the number of the run
      // 
      oStr << "Simulation composed of " << _totalNumberOfRuns << " runs."
	   << std::endl;
      oStr << "\nTotal number of bookings:      " << _totalNbOfBookings
	 << "\n\nElapsed time:             " 
	 << std::setprecision (2) << std::fixed << _totalElapsedTime << " s"
	 << "\nEstimated remaining time: " 
	 << _estimatedRemainingTime << " s"
         << std::endl;

    }   

    //
    // Add the number of the run
    //
    NbOfRuns_T lRunNumberToDisplay = _currentRun;
    if (lRunNumberToDisplay > _totalNumberOfRuns) {
      lRunNumberToDisplay--;
    } 
    oStr << "Current Run (" << lRunNumberToDisplay 
	 << "/"  << _totalNumberOfRuns << ")"
	 << std::endl;   
    
    //
    // Add the display of the start, current and end date
    // 
    oStr << "\nStart Date ---- Current Date ---- End Date\n"
         << _startDate << "     " << _currentDate
         << "       " << _endDate << std::endl;

    //
    // Add the display of the overall progress status
    //
     std::string lEventTypeStr = "All";
    describeHelper(lEventTypeStr);  
    oStr << "\n\n----------------- Progress statuses ----------------"
	 << "\n " << lEventTypeStr << _currentProgressStatus.toString()
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
    oStr << lOptionalStr << std::endl;

    //
    // Add other information such as the total number of bookings, ...
    //
    oStr << "\nTotal number of bookings:      " 
	 << _currentNbOfBookings
	 << "\n\nElapsed time:             " 
	 << std::setprecision (2) << std::fixed << _currentElapsedTime << " s"
	 << "\nEstimated remaining time: " 
	 << _currentEstimatedRemainingTime << " s"
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
