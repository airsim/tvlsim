#ifndef __DSIM_BOM_CONFIGURATIONPARAMETERS_HPP
#define __DSIM_BOM_CONFIGURATIONPARAMETERS_HPP

// //////////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////////
// StdAir
#include <stdair/stdair_date_time_types.hpp>
#include <stdair/basic/StructAbstract.hpp>

namespace DSIM {

  /**
   * @brief Structure wrapping the configuration parameters.
   */
  struct ConfigurationParameters : public stdair::StructAbstract {
  public:
    // //////// Getters /////////     
    /**
     * Get the start date of the simulation.
     */
    const stdair::Date_T& getStartDate() const {
      return _startDate;
    }
      
    /**
     * Get the end date of the simulation.
     */
    const stdair::Date_T& getEndDate() const {
      return _endDate;
    }
    

  public:
    // //////// Setters /////////     
    /**
     * Set the start date of the simulation.
     */
    void setStartDate (const stdair::Date_T& iStartDate) {
      _startDate = iStartDate;
    }
      
    /**
     * Set the end date of the simulation.
     */
    void setEndDate (const stdair::Date_T& iEndDate) {
      _endDate = iEndDate;
    }
    

  public:
    // /////////// Display support method /////////////
    /**
     * Dump a Business Object into an output stream.
     *
     * @param ostream& the output stream.
     */
    void toStream (std::ostream& ioOut) const;

    /**
     * Read a Business Object from an input stream.
     *
     * @param istream& the input stream.
     */
    void fromStream (std::istream& ioIn);

    /**
     * Display of the structure.
     */
    const std::string describe() const;
    

  public:
    // /////////////// Constructors and Destructors /////////////////
    /**
     * Constructor.
     */
    ConfigurationParameters (const stdair::Date_T& iStartDate,
                             const stdair::Date_T& iEndDate);

    /**
     * Copy constructor.
     */
    ConfigurationParameters (const ConfigurationParameters&);
    
  public:
    /**
     * Destructor.
     */
    ~ConfigurationParameters();


  private:
    /**
     * Default constructor (not to be used)
     */
    ConfigurationParameters();


  public:
    // ///////// Attributes //////////
    /**
     * Start date of the simulation.
     */
    stdair::Date_T _startDate;

    /**
     * End date of the simulation.
     */
    stdair::Date_T _endDate;
  };
  
}
#endif //__DSIM_BOM_CONFIGURATIONPARAMETERS_HPP
