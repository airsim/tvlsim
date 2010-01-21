#ifndef __DSIM_SVC_DSIM_SERVICE_HPP
#define __DSIM_SVC_DSIM_SERVICE_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// StdAir
#include <stdair/STDAIR_Types.hpp>
#include <stdair/basic/BasLogParams.hpp>
// Dsim
#include <dsim/DSIM_Types.hpp>

namespace DSIM {

  // Forward declaration
  class DSIM_ServiceContext;
  struct RDSParameters;
  
  /** Interface for the DSIM Services. */
  class DSIM_Service {
  public:
    // /////////// Business Methods /////////////
    /** Perform a simulation. */
    void simulate();
    

  public:
    // ////////// Constructors and destructors //////////
    /** Constructor.
        <br>The init() method is called; see the corresponding documentation
        for more details.
        <br>Moreover, a reference on an output stream is given, so
        that log outputs can be directed onto that stream.       
        @param const stdair::BasLogParams& Parameters for the output log stream.
        @param const stdair::Filename_T& Filename of the input schedule file. */
    DSIM_Service (const stdair::BasLogParams&, const stdair::Filename_T&);

    /** Constructor.
        <br>The init() method is called; see the corresponding documentation
        for more details.
        <br>Moreover, as no reference on any output stream is given,
        it is assumed that the StdAir log service has already been
        initialised with the proper log output stream by some other
        methods in the calling chain (for instance, when the DSIM_Service
        is itself being initialised by another library service).
        @param const stdair::Filename_T& Filename of the input schedule file. */
    DSIM_Service (const stdair::Filename_T&);

    /** Destructor. */
    ~DSIM_Service();

    
  private:
    // /////// Construction and Destruction helper methods ///////
    /** Default constructor. */
    DSIM_Service ();
    /** Default copy constructor. */
    DSIM_Service (const DSIM_Service&);

    /** Initialise the log. */
    void logInit (const stdair::BasLogParams&);

    /** Initialise.
        @param const stdair::Filename_T& Filename of the input schedule file. */
    void init (const stdair::Filename_T&);

    /** Finalise. */
    void finalise ();

    
  private:
    // ///////// Service Context /////////
    /** Dsim context. */
    DSIM_ServiceContext* _dsimServiceContext;

  };
}
#endif // __DSIM_SVC_DSIM_SERVICE_HPP
