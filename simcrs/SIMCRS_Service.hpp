#ifndef __SIMCRS_SVC_SIMCRS_SERVICE_HPP
#define __SIMCRS_SVC_SIMCRS_SERVICE_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// StdAir
#include <stdair/stdair_basic_types.hpp>
#include <stdair/stdair_service_types.hpp>
#include <stdair/basic/ForecastingMethod.hpp>
#include <stdair/bom/TravelSolutionTypes.hpp>
// SimCRS
#include <simcrs/SIMCRS_Types.hpp>

/// Forward declarations
namespace stdair {
  class STDAIR_Service;
  struct BasLogParams;
  struct BasDBParams;
  struct BookingRequestStruct;
  struct SnapshotStruct;
  struct RMEventStruct;
  struct TravelSolutionStruct;
}

namespace SIMCRS {

  /// Forward declaration
  class SIMCRS_ServiceContext;

  
  /**
   * Interface for the SIMCRS Services.
   */
  class SIMCRS_Service {  
  public:
    // ////////////////// Constructors and Destructors //////////////////    
    /**
     * Constructor.
     *
     * The init() method is called; see the corresponding documentation
     * for more details.
     *
     * A reference on an output stream is given, so that log
     * outputs can be directed onto that stream.
     *
     * Moreover, database connection parameters are given, so that a
     * session can be created on the corresponding database.
     *
     * @param const stdair::BasLogParams& Parameters for the output log stream.
     * @param const stdair::BasDBParams& Parameters for the database access.
     * @param const CRSCode_T& Code of the owner of the distribution system.
     * @param const stdair::Filename_T& Filename of the input schedule file.
     * @param const stdair::Filename_T& Filename of the input O&D file.
     * @param const stdair::Filename_T& Filename of the input fare file.
     * @param const stdair::Filename_T& Filename of the input yield file.
     */
    SIMCRS_Service (const stdair::BasLogParams&, const stdair::BasDBParams&,
                    const CRSCode_T&,
                    const stdair::Filename_T& iScheduleInputFilename,
                    const stdair::Filename_T& iODInputFilename,
                    const stdair::Filename_T& iFareInputFilename,
                    const stdair::Filename_T& iYieldInputFilename);

    /**
     * Constructor.
     *
     * The init() method is called; see the corresponding documentation
     * for more details.
     *
     * Moreover, a reference on an output stream is given, so
     * that log outputs can be directed onto that stream.
     *
     * @param const stdair::BasLogParams& Parameters for the output log stream.
     * @param const CRSCode_T& Code of the owner of the distribution system.
     * @param const stdair::Filename_T& Filename of the input schedule file.
     * @param const stdair::Filename_T& Filename of the input O&D file.
     * @param const stdair::Filename_T& Filename of the input fare file.
     * @param const stdair::Filename_T& Filename of the input yield file.
     */
    SIMCRS_Service (const stdair::BasLogParams&, const CRSCode_T&,
                    const stdair::Filename_T& iScheduleInputFilename,
                    const stdair::Filename_T& iODInputFilename,
                    const stdair::Filename_T& iFareInputFilename,
                    const stdair::Filename_T& iYieldInputFilename);

    /**
     * Constructor.
     *
     * The init() method is called; see the corresponding documentation
     * for more details.
     *
     * Moreover, as no reference on any output stream is given,
     * it is assumed that the StdAir log service has already been
     * initialised with the proper log output stream by some other
     * methods in the calling chain (for instance, when the SIMCRS_Service
     * is itself being initialised by another library service such as
     * DSIM_Service).
     *
     * @param stdair::STDAIR_ServicePtr_T Reference on the STDAIR service.
     * @param const CRSCode_T& Code of the owner of the distribution system.
     * @param const stdair::Filename_T& Filename of the input schedule file.
     * @param const stdair::Filename_T& Filename of the input O&D file.
     * @param const stdair::Filename_T& Filename of the input fare file.
     * @param const stdair::Filename_T& Filename of the input yield file.
     */
    SIMCRS_Service (stdair::STDAIR_ServicePtr_T, const CRSCode_T&, 
                    const stdair::Filename_T& iScheduleInputFilename,
                    const stdair::Filename_T& iODInputFilename,
                    const stdair::Filename_T& iFareInputFilename,
                    const stdair::Filename_T& iYieldInputFilename);

    /** Initialise the snapshot and RM events for the inventories.
        @param const stdiar::Date_T& Parameters for the start date.
        @param const stdiar::Date_T& Parameters for the end date.
     */
    void initSnapshotAndRMEvents (const stdair::Date_T&, const stdair::Date_T&);

    /**
     * Destructor.
     */
    ~SIMCRS_Service();

    
  public:
    // /////////// Business Methods /////////////
    /**
     * Construct the list of travel solutions corresponding to the
     * booking request.
     */
    stdair::TravelSolutionList_T 
    calculateSegmentPathList (const stdair::BookingRequestStruct&);
    
    /**
     * Calculate the fare of each travel solutions in the list.
     */
    void fareQuote (const stdair::BookingRequestStruct&,
                    stdair::TravelSolutionList_T&);

    /**
     * Compute the availability for each travel solution in the list.
     */
    void calculateAvailability (stdair::TravelSolutionList_T&);
    
    /**
     * Register a booking.
     */
    bool sell (const stdair::TravelSolutionStruct&, const stdair::PartySize_T&);

    /**
     * Take inventory snapshots.
     */
    void takeSnapshots (const stdair::SnapshotStruct&);
    
    /**
     * Optimise (revenue management) an flight-date/network-date
     */
    void optimise (const stdair::RMEventStruct&,
                   const stdair::ForecastingMethod::EN_ForecastingMethod&);
    
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
     * @param TravelSolutionList_T& Sample list of travel solution structures.
     *        It should be given empty. It is altered with the returned sample.
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
    SIMCRS_Service();
    /**
     * Default copy constructor. It should not be used.
     */
    SIMCRS_Service (const SIMCRS_Service&);

    /**
     * Initialise the (SIMCRS) service context (i.e., the
     * SIMCRS_ServiceContext object).
     *
     * @param const CRSCode_T& Code of the owner of the distribution system.
     */
    void initServiceContext (const CRSCode_T&);

    /**
     * Initialise the STDAIR service (including the log service).
     *
     * A reference on the root of the BOM tree, namely the BomRoot object,
     * is stored within the service context for later use.
     *
     * @param const stdair::BasLogParams& Parameters for the output log stream.
     * @param const stdair::BasDBParams& Parameters for the database access.
     */
    void initStdAirService (const stdair::BasLogParams&,
                            const stdair::BasDBParams&);
    
    /**
     * Initialise the STDAIR service (including the log service).
     *
     * A reference on the root of the BOM tree, namely the BomRoot object,
     * is stored within the service context for later use.
     *
     * @param const stdair::BasLogParams& Parameters for the output log
     *        stream.
     */
    void initStdAirService (const stdair::BasLogParams&);
    
    /**
     * Initialise.
     *
     * The CSV file, describing the airline schedules for the
     * simulator, is parsed and the inventories are generated accordingly.
     *
     * @param const stdair::Filename_T& Filename of the input schedule file.
     * @param const stdair::Filename_T& Filename of the input O&D file.
     * @param const stdair::Filename_T& Filename of the input fare file.
     * @param const stdair::Filename_T& Filename of the input yield file.
     */
    void init (const stdair::Filename_T& iScheduleInputFilename,
               const stdair::Filename_T& iODInputFilename,
               const stdair::Filename_T& iFareInputFilename,
               const stdair::Filename_T& iYieldInputFilename);

    /**
     * Initialise the AIRSCHED service with the given schedule file.
     *
     * @param const stdair::Filename_T& Filename of the input schedule file.
     * @param const stdair::Filename_T& Filename of the input O&D file.
     */
    void initAIRSCHEDService (const stdair::Filename_T& iScheduleInputFilename,
                              const stdair::Filename_T& iODInputFilename);
    
    /**
     * Initialise the SIMFQT service with the given schedule file.
     *
     * @param const stdair::Filename_T& Filename of the input schedule file.
     */
    void initSIMFQTService (const stdair::Filename_T& iFareInputFilename);

    /**
     * Initialise the AIRINV Master service with the given schedule file.
     *
     * @param const stdair::Filename_T& Filename of the input schedule file.
     * @param const stdair::Filename_T& Filename of the input O&D file.
     * @param const stdair::Filename_T& Filename of the input yield file.
     */
    void
    initAIRINV_Master_Service (const stdair::Filename_T& iScheduleInputFilename,
                               const stdair::Filename_T& iODInputFilename,
                               const stdair::Filename_T& iYieldInputFilename);

    /**
     * Finalise.
     */
    void finalise();


  private:
    // ///////// Service Context /////////
    /**
     * SimCRS context.
     */
    SIMCRS_ServiceContext* _simcrsServiceContext;
  };
}
#endif // __SIMCRS_SVC_SIMCRS_SERVICE_HPP
