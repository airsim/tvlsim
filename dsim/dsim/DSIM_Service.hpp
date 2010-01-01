#ifndef __DSIM_SVC_DSIM_SERVICE_HPP
#define __DSIM_SVC_DSIM_SERVICE_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <iosfwd>
#include <string>
// StdAir
#include <stdair/STDAIR_Types.hpp>
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
        @param std::ostream& Output log stream (for instance, std::cout).
        @param const stdair::Filename_T& Filename of the input schedule file. */
    DSIM_Service (std::ostream& ioLogStream, const stdair::Filename_T&);

    /** Destructor. */
    ~DSIM_Service();

    
  private:
    // /////// Construction and Destruction helper methods ///////
    /** Default constructor. */
    DSIM_Service ();
    /** Default copy constructor. */
    DSIM_Service (const DSIM_Service&);

    /** Initialise.
        @param std::ostream& Output log stream (for instance, std::cout).
        @param const stdair::Filename_T& Filename of the input schedule file. */
    void init (std::ostream& ioLogStream, const stdair::Filename_T&);

    /** Finalise. */
    void finalise ();

    
  private:
    // ///////// Service Context /////////
    /** Dsim context. */
    DSIM_ServiceContext* _dsimServiceContext;

  };
}
#endif // __DSIM_SVC_DSIM_SERVICE_HPP
