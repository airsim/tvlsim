#ifndef __TVLSIM_BOM_RDSPARAMETERS_HPP
#define __TVLSIM_BOM_RDSPARAMETERS_HPP

// //////////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////////
// StdAir
#include <stdair/basic/StructAbstract.hpp>

namespace TVLSIM {

  /** Structure wrapping the Reference Data Set parameters. */
  struct RDSParameters : public stdair::StructAbstract {
    
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
    
    // /////////// Display support method /////////////
    /** Dump a Business Object into an output stream.
        @param ostream& the output stream. */
    void toStream (std::ostream& ioOut) const;

    /** Read a Business Object from an input stream.
        @param istream& the input stream. */
    void fromStream (std::istream& ioIn);

    /** Display of the structure. */
    const std::string describe() const;
    

  public:
    /** Costructor. */
    RDSParameters ();
    RDSParameters (const RDSParameters&);

    /** Destructor. */
    ~RDSParameters ();

  public:
    // ////////// Attributes ///////////
    /** Schedule input filename. */
    std::string _scheduleInputFilename;
  };
  
}
#endif //__TVLSIM_BOM_RDSPARAMETERS_HPP
