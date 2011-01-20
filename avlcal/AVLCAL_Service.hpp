#ifndef __AVLCAL_SVC_AVLCAL_SERVICE_HPP
#define __AVLCAL_SVC_AVLCAL_SERVICE_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// StdAir
#include <stdair/stdair_basic_types.hpp>
#include <stdair/basic/BasLogParams.hpp>
// Avlcal
#include <avlcal/AVLCAL_Types.hpp>

namespace AVLCAL {

  // Forward declaration
  class AVLCAL_ServiceContext;

  
  /** Interface for the AVLCAL Services. */
  class AVLCAL_Service {
  public:
    // /////////// Business Methods /////////////
    /** Perform a seat availability calculation. */
    void avlCalculate (const stdair::PartySize_T&);

    
    // ////////// Constructors and destructors //////////
    /** Constructor.
        <br>The init() method is called; see the corresponding documentation
        for more details.
        <br>Moreover, a reference on an output stream is given, so
        that log outputs can be directed onto that stream.       
        @param const stdair::BasLogParams& Parameters for the output log stream.
        @param AirlineCode_T& Code of the owner airline. */
    AVLCAL_Service (const stdair::BasLogParams&, const stdair::AirlineCode_T&);

    /** Constructor.
        <br>The init() method is called; see the corresponding documentation
        for more details.
        <br>Moreover, as no reference on any output stream is given,
        it is assumed that the StdAir log service has already been
        initialised with the proper log output stream by some other
        methods in the calling chain (for instance, when the AVLCAL_Service
        is itself being initialised by another library service such as
        SIMCRS_Service).
        @param AirlineCode_T& Code of the owner airline. */
    AVLCAL_Service (const stdair::AirlineCode_T&);

    /** Destructor. */
    ~AVLCAL_Service();

    
  private:
    // /////// Construction and Destruction helper methods ///////
    /** Default constructor. */
    AVLCAL_Service ();
    /** Default copy constructor. */
    AVLCAL_Service (const AVLCAL_Service&);

    /** Initialise the log. */
    void logInit (const stdair::BasLogParams&);

    /** Initialise.
        @param const stdair::AirlineCode_T& Airline code of the inventory
               owner. */
    void init (const stdair::AirlineCode_T&);

    /** Finalise. */
    void finalise ();

    
  private:
    // ///////// Service Context /////////
    /** Avlcal context. */
    AVLCAL_ServiceContext* _avlcalServiceContext;
  };
}
#endif // __AVLCAL_SVC_AVLCAL_SERVICE_HPP
