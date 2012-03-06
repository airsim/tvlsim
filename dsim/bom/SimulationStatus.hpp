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
#include <stdair/basic/EventType.hpp>
// DSim
#include <dsim/bom/SimulationStatusKey.hpp>

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
      return _key.getStartDate();
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
      return _key.getEndDate();
    }
    
  public:
    // //////////////// Setters /////////////////
    /**
     * Set the current date of the simulation.
     */
    void setCurrentDate (const stdair::Date_T& iCurrentDate) {
      _currentDate = iCurrentDate;
    } 
 
    /**
     * Update the progress status of the given event type
     */
    void updateProgress(const stdair::EventType::EN_EventType&,
			const stdair::ProgressPercentage_T);  

    /**
     * Update the total progress status
     */
    void updateProgress(const stdair::ProgressPercentage_T);

    /**
     * Reset all the parameters
     */
    void reset ();

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
     * Primary key: Simulator ID.
     */
    Key_T _key;
  
    /**
     * Currrent date of the simulation.
     */
    stdair::Date_T _currentDate;

    /**
     * Current number of bookings
     */
     stdair::NbOfBookings_T _nbOfBookings;
  
    /**
     * Progress statuses.
     *
     * <br>The progress is status is the ratio of:
     * <ul>
     *   <li>the current number of events, summed over all the demand
     *       streams,</li>
     *   <li>over the total number of events, also summed over all the demand
     *       streams.</li>
     * </ul>
     */
    stdair::ProgressPercentage_T _bookingRequestProgressPercentage;
    stdair::ProgressPercentage_T _snapShotProgressPercentage;
    stdair::ProgressPercentage_T _rmEventProgressPercentage;
    stdair::ProgressPercentage_T _optimisationNotificationProgressPercentage;
    stdair::ProgressPercentage_T _cancellationProgressPercentage;
    stdair::ProgressPercentage_T _breakPointProgressPercentage;
    stdair::ProgressPercentage_T _allEventsProgressPercentage;
    
  };

}
#endif // __DSIM_BOM_SIMULATIONSTATUS_HPP
