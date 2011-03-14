#ifndef __TRAVELCCM_SVC_TRAVELCCM_SERVICE_HPP
#define __TRAVELCCM_SVC_TRAVELCCM_SERVICE_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// StdAir
#include <stdair/stdair_basic_types.hpp>
#include <stdair/stdair_service_types.hpp>
#include <stdair/bom/TravelSolutionTypes.hpp>
// TravelCCM
#include <travelccm/TRAVELCCM_Types.hpp>

/// Forward declarations
namespace stdair {
  class STDAIR_Service;
  struct BasLogParams;
  struct BasDBParams;
  struct BookingRequestStruct;
}

namespace TRAVELCCM {

  /// Forward declarations
  class TRAVELCCM_ServiceContext;


  /**
   * Interface for the TRAVELCCM Services.
   */
  class TRAVELCCM_Service {
  public:
    // ////////// Constructors and destructors //////////
    /**
     * Constructor.
     *
     * The init() method is called; see the corresponding
     * documentation for more details.
     *
     * A reference on an output stream is given, so that log outputs
     * can be directed onto that stream.
     *
     * Moreover, database connection parameters are given, so that a
     * session can be created on the corresponding database.
     *
     * @param const stdair::BasLogParams& Parameters for the output log stream.
     * @param const stdair::BasDBParams& Parameters for the database access.
     */
    TRAVELCCM_Service (const stdair::BasLogParams&, const stdair::BasDBParams&);

    /**
     * Constructor.
     *
     * The init() method is called; see the corresponding
     * documentation for more details.
     *
     * A reference on an output stream is given, so that log outputs
     * can be directed onto that stream.
     *
     * @param const stdair::BasLogParams& Parameters for the output log stream.
     */
    TRAVELCCM_Service (const stdair::BasLogParams&);
    
    /**
     * Constructor.
     *
     * The init() method is called; see the corresponding
     * documentation for more details.
     *
     * Moreover, as no reference on any output stream is given, it is
     * assumed that the StdAir log service has already been
     * initialised with the proper log output stream by some other
     * methods in the calling chain (for instance, when the
     * TRAVELCCM_Service is itself being initialised by another
     * library service such as DSIM_Service).
     *
     * @param stdair::STDAIR_ServicePtr_T Reference on the STDAIR service.
     */
    TRAVELCCM_Service (stdair::STDAIR_ServicePtr_T);
    
    /**
     * Destructor.
     */
    ~TRAVELCCM_Service();


  public:
    // ///////////// Business methods /////////////////
    /**
     * Build a sample BOM tree, and attach it to the BomRoot instance.
     */
    void buildSampleBom();

    /**
     * Build a sample list of travel solutions.
     *
     * As of now (March 2011), that list is made of the following
     * travel solutions:
     * <ul>
     *  <li>BA9</li>
     *  <li>LHR-SYD</li>
     *  <li>2011-06-10</li>
     *  <li>Q</li>
     *  <li>WTP: 900</li>
     *  <li>Change fee: 20; Non refundable; Saturday night stay</li>
     * </ul>
     *
     * @param stdair::TravelSolutionList_T& Sample list of travel solution
     *        structures. It should be given empty. It is altered with the
     *        returned sample.
     */
    void buildSampleTravelSolutions (stdair::TravelSolutionList_T&);

    /**
     * Build a sample booking request structure.
     *
     * As of now (March 2011), the sample booking request is made of the
     * following parameters:
     * <ul>
     *  <li>Return trip (inbound): LHR-SYD (POS: LHR, Channel: DN), </li>
     *  <li>Departing 10-JUN-2011 around 8:00, staying 7 days</li>
     *  <li>Requested on 15-MAY-2011 at 10:00</li>
     *  <li>Economy cabin, 3 persons, FF member</li>
     *  <li>WTP: 1000.0 EUR</li>
     *  <li>Dis-utility: 100.0 EUR/hour</li>
     * </ul>
     *
     * As of now (March 2011), the CRS-related booking request is made
     * of the following parameters:
     * <ul>
     *  <li>Return trip (inbound): SIN-BKK (POS: SIN, Channel: IN), </li>
     *  <li>Departing 30-JAN-2010 around 10:00, staying 7 days</li>
     *  <li>Requested on 22-JAN-2010 at 10:00</li>
     *  <li>Economy cabin, 3 persons, FF member</li>
     *  <li>WTP: 1000.0 EUR</li>
     *  <li>Dis-utility: 100.0 EUR/hour</li>
     * </ul>
     *
     * @param const bool isForCRS Whether the sample booking request is for CRS.
     * @return BookingRequestStruct& Sample booking request structure.
     */
    stdair::BookingRequestStruct
    buildSampleBookingRequest (const bool isForCRS = false);

    /**
     * Choose the travel solution and the fare option within the given
     * list of travel solutions.
     *
     * The returned pointer will be NULL if no travel solution is
     * chosen (e.g., when the Willingness-To-Pay is too low).
     *
     * @param stdair::TravelSolutionList_T& The list of travel solution
     *        to choose among.
     * @param const stdair::BookingRequestStruct& The booking request
     *        and its context.
     * @return stdair::TravelSolutionStruct* The chosen travel solution.
     *         NULL when not found.
     */
    const stdair::TravelSolutionStruct*
    chooseTravelSolution (stdair::TravelSolutionList_T&,
                          const stdair::BookingRequestStruct&);
    

  public:
    // //////////////// Display support methods /////////////////
    /**
     * Recursively display (dump in the returned string) the objects
     * of the BOM tree.
     *
     * @return std::string Output string in which the BOM tree is
     *        logged/dumped.
     */
    std::string csvDisplay() const;

    /**
     * Display (dump in the returned string) the full list of travel
     * solution structures.
     *
     * @return std::string Output string in which the list of travel
     *        solutions is logged/dumped.
     */
    std::string csvDisplay (const stdair::TravelSolutionList_T&) const;


  private:
    // /////// Construction and Destruction helper methods ///////
    /**
     * Default constructor. It should not be used.
     */
    TRAVELCCM_Service();
    /**
     * Copy constructor. It should not be used.
     */
    TRAVELCCM_Service (const TRAVELCCM_Service&);

    /**
     * Initialise the STDAIR service (including the log service).
     *
     * A reference on the root of the BOM tree, namely the BomRoot object,
     * is stored within the service context for later use.
     *
     * @param const stdair::BasLogParams& Parameters for the output log stream.
     * @param const stdair::BasDBParams& Parameters for the database access.
     */
    stdair::STDAIR_ServicePtr_T initStdAirService (const stdair::BasLogParams&,
                                                   const stdair::BasDBParams&);
    
    /**
     * Initialise the STDAIR service (including the log service).
     *
     * A reference on the root of the BOM tree, namely the BomRoot object,
     * is stored within the service context for later use.
     *
     * @param const stdair::BasLogParams& Parameters for the output log stream.
     */
    stdair::STDAIR_ServicePtr_T initStdAirService (const stdair::BasLogParams&);
    
    /**
     * Attach the STDAIR service (holding the log and database services) to
     * the TRAVELCCM_Service.
     *
     * @param stdair::STDAIR_ServicePtr_T Reference on the STDAIR service.
     * @param const bool State whether or not TravelCCM owns the STDAIR service
     *        resources.
     */
    void addStdAirService (stdair::STDAIR_ServicePtr_T ioSTDAIR_ServicePtr,
                           const bool iOwnStdairService);
    
    /**
     * Initialise the (TRADEMGEN) service context (i.e., the
     * TRADEMGEN_ServiceContext object).
     */
    void initServiceContext();

    /**
     * Initialise.
     *
     * No input file is given. A sample BOM tree may be built by
     * calling the buildSampleBom() method.
     */
    void initTravelCCMService();

    /**
     * Finalise.
     */
    void finalise();

  private:
    // ///////// Service Context /////////
    /**
     * TravelCCM service context.
     */
    TRAVELCCM_ServiceContext* _travelccmServiceContext;
  };
}
#endif // __TRAVELCCM_SVC_TRAVELCCM_SERVICE_HPP
