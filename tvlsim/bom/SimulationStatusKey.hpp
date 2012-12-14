#ifndef __TVLSIM_BOM_EVENTQUEUEKEY_HPP
#define __TVLSIM_BOM_EVENTQUEUEKEY_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// StdAir
#include <stdair/stdair_basic_types.hpp>
#include <stdair/stdair_date_time_types.hpp>
#include <stdair/bom/KeyAbstract.hpp>
// TvlSim
#include <tvlsim/TVLSIM_Types.hpp>

namespace TVLSIM {
  
  /**
   * Key of eventqueue.
   */
  struct SimulationStatusKey : public stdair::KeyAbstract {

  public:
    // /////////// Getters //////////
    /**
     * Get the ID of the SimulationStatus object.
     */
    const SimulatorID_T& getSimulationStatusID() const {
      return _simulatorID;
    }

    // /////////// Display support methods /////////
    /**
     * Dump a Business Object Key into an output stream.
     * @param ostream& the output stream.
     */
    void toStream (std::ostream& ioOut) const;
    
    /**
     * Read a Business Object Key from an input stream.
     * @param istream& the input stream.
     */
    void fromStream (std::istream& ioIn);
      
   /**
    * Get the serialised version of the Business Object Key.
    * <br>That string is unique, at the level of a given Business Object,
    * when among children of a given parent Business Object.
    * <br>For instance, "H" and "K" allow to differentiate among two
    * marketing classes for the same segment-date.
    */
    const std::string toString() const;
    

  public:
    // /////////// Construction ///////////
    /**
     * Main constructor.
     */
    SimulationStatusKey (const SimulatorID_T& iSimulatorID);

    /**
     * Copy Constructor.
     */
    SimulationStatusKey (const SimulationStatusKey&);
    
    /**
     * Destructor.
     */
    ~SimulationStatusKey ();
    

  private:
    /**
     * Default Constructor (not to be used).
     */
    SimulationStatusKey ();

    
  private:
    // //////////// Attributes ////////////////
    /**
     * ID for the SimulationStatus object.
     */
    SimulatorID_T _simulatorID;

  };

}
#endif // __TVLSIM_BOM_EVENTQUEUEKEY_HPP
