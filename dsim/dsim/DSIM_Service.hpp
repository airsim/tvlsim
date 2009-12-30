#ifndef __DSIM_SVC_DSIM_SERVICE_HPP
#define __DSIM_SVC_DSIM_SERVICE_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <ostream>
#include <string>
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
        @param std::ostream& Output log stream (for instance, std::cout). */
    DSIM_Service (const std::string&, std::ostream& ioLogStream);

    /** Destructor. */
    ~DSIM_Service();

    
  private:
    // /////// Construction and Destruction helper methods ///////
    /** Default constructor. */
    DSIM_Service ();
    /** Default copy constructor. */
    DSIM_Service (const DSIM_Service&);

    /** Initialise. */
    void init (const std::string&, std::ostream& ioLogStream);

    /** Finalise. */
    void finalise ();

    
  private:
    // ///////// Service Context /////////
    /** Dsim context. */
    DSIM_ServiceContext* _dsimServiceContext;

  };
}
#endif // __DSIM_SVC_DSIM_SERVICE_HPP
