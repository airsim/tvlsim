#ifndef __AVLCAL_SVC_AVLCAL_SERVICE_HPP
#define __AVLCAL_SVC_AVLCAL_SERVICE_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// StdAir
#include <stdair/stdair_basic_types.hpp>
#include <stdair/stdair_service_types.hpp>
// Avlcal
#include <avlcal/AVLCAL_Types.hpp>

namespace AVLCAL {

  /// Forward declarations
  class AVLCAL_ServiceContext;

  
  /**
   * @brief Interface for the AVLCAL Services.
   */
  class AVLCAL_Service {
  public:
    // /////////// Business Methods /////////////
    /** Perform a seat availability calculation. */
    void avlCalculate (const stdair::PartySize_T&);

    
    // ////////// Constructors and destructors //////////
    /**
     * Constructor.
     *
     * The init() method is called; see the corresponding documentation
     * for more details.
     *
     * Moreover, a reference on an output stream is given, so
     * that log outputs can be directed onto that stream.
     *
     * @param const stdair::BasLogParams& Parameters for the output log
     *        stream.
     * @param AirlineCode_T& Code of the owner airline.
     */
    AVLCAL_Service (const stdair::BasLogParams&, const stdair::AirlineCode_T&);

    /**
     * Constructor.
     *
     * The init() method is called; see the corresponding documentation
     * for more details.
     *
     * Moreover, as no reference on any output stream is given,
     * it is assumed that the StdAir log service has already been
     * initialised with the proper log output stream by some other
     * methods in the calling chain (for instance, when the AVLCAL_Service
     * is itself being initialised by another library service such as
     * SIMCRS_Service).
     *
     * @param STDAIR_ServicePtr_T the shared pointer of stdair service.
     * @param AirlineCode_T& Code of the owner airline.
     */
    AVLCAL_Service (stdair::STDAIR_ServicePtr_T, const stdair::AirlineCode_T&);
        
    /**
     * Destructor.
     */
    ~AVLCAL_Service();

    /**
     * Clear the context (cabin capacity, bucket holder).
     */
    void reset();
  

  private:
    // /////// Construction and Destruction helper methods ///////
    /**
     * Default constructor.
     */
    AVLCAL_Service();
    /**
     * Copy constructor.
     */
    AVLCAL_Service (const AVLCAL_Service&);

    /**
     * Initialise the (AVLCAL) service context (i.e., the
     * AVLCAL_ServiceContext object).
     */
    void initServiceContext (const stdair::AirlineCode_T&);
    
    /**
     * Initialise the STDAIR service (including the log service).
     */
    void initStdAirService (const stdair::BasLogParams&);

    /**
     * Attach the STDAIR service (holding the log and database services) to
     * the AVLCAL_Service.
     *
     * @param stdair::STDAIR_ServicePtr_T Reference on the STDAIR service.
     */
    void addStdAirService (stdair::STDAIR_ServicePtr_T,
                           const bool iOwnStdairService);

    /**
     * Finaliser.
     */
    void finalise();

    
  private:
    // ///////// Service Context /////////
    /**
     * Service Context.
     */
    AVLCAL_ServiceContext* _avlcalServiceContext;
  };
}
#endif // __AVLCAL_SVC_AVLCAL_SERVICE_HPP
