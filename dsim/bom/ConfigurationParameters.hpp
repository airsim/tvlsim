#ifndef __DSIM_BOM_CONFIGURATIONPARAMETERS_HPP
#define __DSIM_BOM_CONFIGURATIONPARAMETERS_HPP

// //////////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////////
// StdAir
#include <stdair/stdair_date_time_types.hpp>
#include <stdair/basic/StructAbstract.hpp>

namespace DSIM {

  /** Structure wrapping the configuration parameters. */
  struct ConfigurationParameters : public stdair::StructAbstract {
  public:
    // //////// GETTERS /////////     
    /** Get the start date of the simulation. */
    const stdair::Date_T& getStartDate () const {
      return _startDate;
    }
      
    /** Get the end date of the simulation. */
    const stdair::Date_T& getEndDate () const {
      return _endDate;
    }
    
    // /////////// Display support method /////////////
    /** Dump a Business Object into an output stream.
        @param ostream& the output stream. */
    void toStream (std::ostream& ioOut) const;

    /** Read a Business Object from an input stream.
        @param istream& the input stream. */
    void fromStream (std::istream& ioIn);

    /** Display of the structure. */
    const std::string describe() const;
    
        
    // /////////////// Constructors and Destructors /////////////////
  public:
    /** Constructor. */
    ConfigurationParameters (const stdair::Date_T&, const stdair::Date_T&);

    /** Copy constructor. */
    ConfigurationParameters (const ConfigurationParameters&);
    
  private:
    /** Default constructor.
        <br>It is private so that it can not be used. */
    ConfigurationParameters ();

  public:
    /** Destructor. */
    ~ConfigurationParameters();


  public:
    // ///////// Attributes //////////
    /** Start date of the simulation. */
    stdair::Date_T _startDate;

    /** End date of the simulation. */
    stdair::Date_T _endDate;
  };
  
}
#endif //__DSIM_BOM_CONFIGURATIONPARAMETERS_HPP
