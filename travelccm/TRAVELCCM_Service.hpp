#ifndef __TRAVELCCM_SVC_TRAVELCCM_SERVICE_HPP
#define __TRAVELCCM_SVC_TRAVELCCM_SERVICE_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// StdAir
#include <stdair/stdair_basic_types.hpp>
#include <stdair/stdair_service_types.hpp>
#include <stdair/basic/BasLogParams.hpp>
#include <stdair/bom/TravelSolutionTypes.hpp>
// TravelCCM
#include <travelccm/TRAVELCCM_Types.hpp>

// Forward declarations
namespace stdair {
  struct BookingRequestStruct;
}

namespace TRAVELCCM {

  // Forward declarations
  class TRAVELCCM_ServiceContext;

  /** Interface for the TRAVELCCM Services. */
  class TRAVELCCM_Service {
  public:
    // ////////// Constructors and destructors //////////
    /** Constructor.
        <br>The init() method is called; see the corresponding documentation
        for more details.
        <br>Moreover, a reference on an output stream is given, so
        that log outputs can be directed onto that stream.       
        @param const stdair::BasLogParams& Parameters for the output log stream.
    */
    TRAVELCCM_Service (const stdair::BasLogParams&);
    
    /** Constructor.
        <br>The init() method is called; see the corresponding documentation
        for more details.
        <br>Moreover, as no reference on any output stream is given,
        it is assumed that the StdAir log service has already been
        initialised with the proper log output stream by some other
        methods in the calling chain (for instance, when the TRAVELCCM_Service
        is itself being initialised by another library service such as
        DSIM_Service).
        @param stdair::STDAIR_ServicePtr_T Handler on the STDAIR_Service.
    */
    TRAVELCCM_Service (stdair::STDAIR_ServicePtr_T);
    
    /** Destructor. */
    ~TRAVELCCM_Service();

  public:
    // ///////////// Business methodes /////////////////
    /** Choose the travel solution and the fare option within the given
        list of travel solutions.
        <br> the returned pointer will be NULL if no travel solution is
        chosen (e.g. Willingness-To-Pay too low). */
    const stdair::TravelSolutionStruct*
    chooseTravelSolution (stdair::TravelSolutionList_T&,
                          const stdair::BookingRequestStruct&);
    
  private:
    // /////// Construction and Destruction helper methods ///////
    /** Default Constructors. */
    TRAVELCCM_Service ();
    TRAVELCCM_Service (const TRAVELCCM_Service&);

    /** Initialise the (TRADEMGEN) service context (i.e., the
        TRADEMGEN_ServiceContext object). */
    void initServiceContext ();
    
    /** Initialise the STDAIR service (including the log service).
        <br>A reference on the root of the BOM tree, namely the BomRoot object,
        is stored within the service context for later use.
        @param const stdair::BasLogParams& Parameters for the output log stream.
    */
    stdair::STDAIR_ServicePtr_T initStdAirService (const stdair::BasLogParams&);
    
    /** Attach the STDAIR service (holding the log and database services) to
        the AIRINV_Service.
        @param stdair::STDAIR_ServicePtr_T Reference on the STDAIR service. */
    void addStdAirService (stdair::STDAIR_ServicePtr_T ioSTDAIR_ServicePtr);
    
    /** Finaliser. */
    void finalise ();

  private:
    // ////////// Service Context //////////
    /** Service Context. */
    TRAVELCCM_ServiceContext* _travelccmServiceContext;
  };
}
#endif // __TRAVELCCM_SVC_TRAVELCCM_SERVICE_HPP
