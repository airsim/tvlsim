#ifndef __DSIM_BOM_CONFIGURATIONPARAMETERS_HPP
#define __DSIM_BOM_CONFIGURATIONPARAMETERS_HPP

// //////////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////////
// DSIM
#include <dsim/bom/StructAbstract.hpp>

namespace DSIM {

  /** Structure wrapping the configuration parameters. */
  struct ConfigurationParameters : public StructAbstract {
  public:
    // //////// GETTERS /////////     
    
    // //////// SETTERS /////////
    
    /////////// DISPLAY METHOD ///////////
    const std::string describe() const;

    
  public:
    /** Costructor. */
    ConfigurationParameters ();
    ConfigurationParameters (const ConfigurationParameters&);
    
    /** Destructor. */
    ~ConfigurationParameters ();
    
  public:
    // ///////// Attributes //////////
  };
  
}
#endif //__DSIM_BOM_CONFIGURATIONPARAMETERS_HPP
