#ifndef __DSIM_BOM_RDSPARAMETERS_HPP
#define __DSIM_BOM_RDSPARAMETERS_HPP

// //////////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////////
// DSIM
#include <dsim/bom/StructAbstract.hpp>

namespace DSIM {

  /** Structure wrapping the Reference Data Set parameters. */
  struct RDSParameters : public StructAbstract {
    
  public:
    // ////////// Attributes ///////////
    /** Schedule input filename. */
    std::string _scheduleInputFilename;

  public:
    /** Costructor. */
    RDSParameters ();
    RDSParameters (const RDSParameters&);

    /** Destructor. */
    ~RDSParameters ();

  public:
    // //////// GETTERS /////////  
    /** Get the schedule input filename. */
    const std::string& getScheduleInputFilename () const {
      return _scheduleInputFilename;
    }   
    
    // //////// SETTERS /////////
    /** Set the schedule input filename. */
    void setScheduleInputFilename (const std::string& iInputFilename) {
      _scheduleInputFilename = iInputFilename;
    }
    
    /////////// DISPLAY METHOD ///////////
    const std::string describe() const;
    
  };
  
}
#endif //__DSIM_BOM_RDSPARAMETERS_HPP
