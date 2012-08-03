#ifndef __DSIM_BOM_SIMULATIONSTATUS_HPP
#define __DSIM_BOM_SIMULATIONSTATUS_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
// Boost Simulation
#include <boost/progress.hpp>
// StdAir
#include <stdair/stdair_date_time_types.hpp>
#include <stdair/basic/StructAbstract.hpp>
// SEvMgr
#include <sevmgr/SEVMGR_Types.hpp>
// DSim
#include <dsim/basic/SimulationMode.hpp>
#include <dsim/bom/SimulationStatusKey.hpp>
#include <dsim/DSIM_Types.hpp>

namespace DSIM {

  /**
   * Structure holding the details of the simulation status.
   *
   * The progress status is given by the ratio between the "current"
   * and the "expected" (or "actual") numbers. For instance, when the
   * expected/actual number is 1000 and the current number is 200,
   * then the progress status is 20% (= 200 / 1000).
   */
  struct SimulationStatus : public stdair::StructAbstract {
    
  public:
    // ////////// Type definitions ////////////
    /**
     * Definition allowing to retrieve the associated BOM key type.
     */
    typedef SimulationStatusKey Key_T;
    
  public:
    // //////////////// Getters /////////////////
    /**
     * Get the start date of the simulation.
     */
    const stdair::Date_T& getStartDate() const {
      return _startDate;
    }

    /**
     * Get the current date of the simulation.
     */
    const stdair::Date_T& getCurrentDate() const {
      return _currentDate;
    }
      
    /**
     * Get the end date of the simulation.
     */
    const stdair::Date_T& getEndDate() const {
      return _endDate;
    }    

    /**
     * Get the current run of the simulation.
     */
    const NbOfRuns_T& getCurrentRun() const {
      return _currentRun;
    } 
    
    /**
     * Get the current mode of the simulation.
     */
    const SimulationMode& getSimulationMode() const {
      return _simulationMode;
    } 

    /**
     * Get the current mode of the simulation.
     */
    SimulationMode::EN_SimulationMode getMode() const {
      return _simulationMode.getMode();
    } 

    /**
     * Get the total number of bookings
     */
    const stdair::NbOfBookings_T& getCurrentNumberOfBookings() const {
      return _currentNbOfBookings;
    }  

    /**
     * Get the elapsed time
     */
    const double& getCurrentElapsedTime() const {
      return _currentElapsedTime;
    }    

    /**
     * Get the estimated remaining time
     */
    const double& getCurrentEstimatedRemainingTime() const {
      return _currentEstimatedRemainingTime;
    }   

    /**
     * Get the overall progress status.
     */   
    const stdair::ProgressStatus& getCurrentProgressStatus() const {
      return _currentProgressStatus;
    }  

    /**
     * Get the current progress status.
     */   
    const SEVMGR::ProgressStatusMap_T& getProgressStatusMap() const {
      return _progressStatusMap;
    }

  public:
    // //////////////// Setters ///////////////// 
    /**
     * Set the start and end date of the simulation.
     */
    void setSimulationPeriod (const stdair::Date_T& iStartDate,
			      const stdair::Date_T& iEndDate) {
      _startDate = iStartDate; 
      _endDate = iEndDate;
      setCurrentDate (iStartDate);
    } 

    /**
     * Set the current date of the simulation.
     */
    void setCurrentDate (const stdair::Date_T& iCurrentDate) {
      _currentDate = iCurrentDate;
    } 
 
    /**
     * Update the progress status for a specific type
     */
    void updateProgress(const stdair::EventType::EN_EventType&,
			const stdair::ProgressStatus&,
			const double& iEventMeasure = 0);

    /**
     * Reset all the parameters
     */
    void reset ();

    /**
     * Prepare a new run (i.e. reset the values of the current run)
     */
    void prepareNewRun ();

    /**
     * Change the current mode of the simulation.
     */
    void setMode (const SimulationMode::EN_SimulationMode& iEN_SimulationMode) {
      return _simulationMode.setMode(iEN_SimulationMode);
    }
    
    /**
     * Update the total number of bookings
     */
    void increaseGlobalNumberOfBookings (const stdair::PartySize_T&);

    /**
     * Set the current and overall progress statuses
     */
    void setCurrentProgressStatus (const stdair::ProgressStatus&);
    
    /**
     * Update the total number of runs
     */
    void setTotalNumberOfRuns (const NbOfRuns_T& iNbOfRuns) {
      _totalNumberOfRuns = iNbOfRuns;
    } 

    /**
     * Increase the number of the current run
     */
    void increaseCurrentNumberOfRun () {
      if (_currentRun < _totalNumberOfRuns) {
	_currentRun++;
      }
    }

  public:
    // /////////// Display support method /////////////
    /**
     * Dump a Business Object into an output stream.
     *
     * @param ostream& the output stream.
     */
    void toStream (std::ostream& ioOut) const {
      ioOut <<  describe();
    }

    /**
     * Read a Business Object from an input stream.
     *
     * @param istream& the input stream.
     */
    void fromStream (std::istream& ioIn) {
    }

    /**
     * Display of the structure.
     */
    const std::string describe() const;

    /**
     * Get a string describing the key.
     */
    const std::string describeKey() const {
      return _key.toString();
    }

  private:
    void describeHelper(std::string&) const;
    
  public:
    /**
     * Main Constructor.
     *
     */
    SimulationStatus (const Key_T&);

    /**
     * Default Constructor.
     */
    SimulationStatus ();

    /**
     * Destructor.
     */
    ~SimulationStatus ();

  private:
    /**
     * Copy Constructor (not to be used).   
     */
    SimulationStatus (const SimulationStatus&);
  
  private:
    // //////////////// Attributes /////////////////
    /**
     * Primary key: start date, end date
     */
    Key_T _key;

    /**
     * Start date of the simulation.
     */ 
    stdair::Date_T _startDate;   

    /**
     * Current date of the simulation.
     */
    stdair::Date_T _currentDate;   
  
    /**
     * End date of the simulation.
     */
    stdair::Date_T _endDate;    

    /**
     * Total number of runs
     */
    NbOfRuns_T _totalNumberOfRuns;    

    /**
     * Number of the current run
     */
    NbOfRuns_T _currentRun;

    /**
     * Total number of bookings for the current run
     */
    stdair::NbOfBookings_T _currentNbOfBookings;  

    /**
     * Total number of bookings taking into account all the runs
     */
    stdair::NbOfBookings_T _totalNbOfBookings;

    /**
     * Counters holding the overall progress status
     */  
    stdair::ProgressStatus _overallProgressStatus;
  
    /**
     * Counters holding the progress status for the current run
     */
    stdair::ProgressStatus _currentProgressStatus;

    /**
     * Counters holding the overall progress status, for each event
     * type (e.g., booking request, optimisation notification,
     * schedule change, break point) for the current run
     */
    SEVMGR::ProgressStatusMap_T _progressStatusMap;

    /**
     * Global counters holding the elapsed time and the estimated 
     * remaining one.
     */
    double _totalElapsedTime;  
    double _estimatedRemainingTime; 

    /**
     * Counters holding the elapsed time and the estimated 
     * remaining one for the current run
     */  
    double _currentElapsedTime;  
    double _currentEstimatedRemainingTime; 

    /**
     * Counters holding the total chronometers, for each event
     * type (e.g., booking request, optimisation notification,
     * schedule change, break point).
     */
    ChronometerMap_T _chronometerMap;
    
    /** 
     * Simulation Mode (i.e. running, on a break point or done)
     */
    SimulationMode _simulationMode;
  };

}
#endif // __DSIM_BOM_SIMULATIONSTATUS_HPP
