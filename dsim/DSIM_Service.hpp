#ifndef __DSIM_SVC_DSIM_SERVICE_HPP
#define __DSIM_SVC_DSIM_SERVICE_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// StdAir
#include <stdair/stdair_basic_types.hpp>
#include <stdair/stdair_date_time_types.hpp>
#include <stdair/stdair_maths_types.hpp>
#include <stdair/stdair_service_types.hpp>
#include <stdair/basic/ForecastingMethod.hpp>
#include <stdair/basic/DemandGenerationMethod.hpp>
#include <stdair/basic/PartnershipTechnique.hpp>
#include <stdair/bom/TravelSolutionTypes.hpp>
// AIRRAC
#include <airrac/AIRRAC_Types.hpp>
// SimFQT
#include <simfqt/SIMFQT_Types.hpp>
// Dsim
#include <dsim/DSIM_Types.hpp>

// Forward declarations
namespace stdair {
  struct BasLogParams;
  struct BasDBParams;
  struct BookingRequestStruct;
}

namespace DSIM {

  // Forward declaration
  class DSIM_ServiceContext;
  struct RDSParameters;
  
  /**
   * Interface (API) for the simulator services.
   */
  class DSIM_Service {
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
     * @param const stdair::Date_T& Start date of the simulation.
     * @param const stdair::Date_T& End date of the simulation.
     * @param const stdair::RandomSeed_T& Seed for the random generation used
     *        by the demand generation component (TraDemGen).
     */
    DSIM_Service (const stdair::BasLogParams&, const stdair::BasDBParams&,
                  const stdair::Date_T& iStartDate,
                  const stdair::Date_T& iEndDate, const stdair::RandomSeed_T&);

    /**
     * Constructor.
     *
     * The init() method is called; see the corresponding documentation
     * for more details.
     *
     * The init() method is called; see the corresponding documentation
     * for more details.
     *
     * Moreover, a reference on an output stream is given, so
     * that log outputs can be directed onto that stream.
     *
     * @param const stdair::BasLogParams& Parameters for the output log stream.
     * @param const stdair::Date_T& Start date of the simulation.
     * @param const stdair::Date_T& End date of the simulation.
     * @param const stdair::RandomSeed_T& Seed for the random generation used
     *        by the demand generation component (TraDemGen).
     */
    DSIM_Service (const stdair::BasLogParams&, const stdair::Date_T& iStartDate,
                  const stdair::Date_T& iEndDate, const stdair::RandomSeed_T&);

    /**
     * Constructor.
     *
     * The init() method is called; see the corresponding documentation
     * for more details.
     *
     * Moreover, as no reference on any output stream is given,
     * it is assumed that the StdAir log service has already been
     * initialised with the proper log output stream by some other
     * methods in the calling chain (for instance, when the DSIM_Service
     * is itself being initialised by another library service. However,
     * there is currently no other known library service using/calling DSim).
     *
     * @param stdair::STDAIR_ServicePtr_T Reference on the STDAIR service.
     * @param const stdair::Date_T& Start date of the simulation.
     * @param const stdair::Date_T& End date of the simulation.
     * @param const stdair::RandomSeed_T& Seed for the random generation used
     *        by the demand generation component (TraDemGen).
     */
    DSIM_Service (stdair::STDAIR_ServicePtr_T, const stdair::Date_T& iStartDate,
                  const stdair::Date_T& iEndDate, const stdair::RandomSeed_T&);

    /**
     * Parse the schedule, O&D, fare and yield input files.
     *
     * The CSV files, describing the airline schedule, O&Ds, fares and yields
     * for the simulator, are parsed and instantiated in memory accordingly.
     *
     * @param const stdair::Filename_T& Filename of the input schedule file.
     * @param const stdair::Filename_T& Filename of the input O&D file.
     * @param const AIRRAC::YieldFilePath& Filename of the input yield file.
     * @param const SIMFQT::FareFilePath& Filename of the input fare file.
     * @param const stdair::Filename_T& Filename of the input demand file.
     */
    void parseAndLoad (const stdair::Filename_T& iScheduleInputFilename,
                       const stdair::Filename_T& iODInputFilename,
                       const AIRRAC::YieldFilePath& iYieldInputFilepath,
                       const SIMFQT::FareFilePath& iFareInputFilepath,
                       const stdair::Filename_T& iDemandInputFilename);

    /**
     * Initialise the snapshot and RM events for the inventories.
     */
    void initSnapshotAndRMEvents();

    /**
     * Re-initialise the simulation service, as well as all the other
     * component services (e.g., SimCRS, TraDemGen, TravelCCM).
     */
    void reinitServices();

    /**
     * Destructor.
     */
    ~DSIM_Service();

    
  public:
    // /////////// Business Methods /////////////
    /**
     * Perform a simulation.
     *
     * \note Currently, the multi-run piece of functionality does not work
     *       properly. Indeed, only the demand generation service context
     *       is resetted correctly; the inventories, schedules, etc, are not
     *       resetted at all. Achieving a fully working multi-run simulation
     *       framework will require some significant work. In the meantime,
     *       a work around is to launch in a row several mono-run simulations,
     *       with a distinct random generation seed for every simulation run.
     *
     * @param const NbOfRuns_T& Number of simulation runs to be performed.
     * @param const stdair::DemandGenerationMethod&
     *        States whether the demand generation must be performed
     *        following the method based on statistic orders.
     *        The alternative method, while more "intuitive", is also a
     *        sequential algorithm.
     * @param const stdair::ForecastingMethod&
     *        States which forecasting method should be used by the
     *        revenue management (RMOL component).
     * @param const stdair::PartnershipTechnique&
     *        States which partnership technique should be used by both
     *        the revenue management (RMOL) and inventory control (AirInv).
     */
    void simulate (const NbOfRuns_T&, const stdair::DemandGenerationMethod&,
                   const stdair::ForecastingMethod&, const stdair::PartnershipTechnique&);
    
    /**
     * Display the list of airlines.
     */
    void displayAirlineListFromDB() const;

    /**
     * Build a sample BOM tree, and attach it to the BomRoot instance.
     *
     * As for now, the BOM sample tree is the one built by the SimCRS and
     * TraDemGen components.
     *
     * \see SIMCRS::SIMCRS_Service, TRADEMGEN::TRADEMGEN_Service and
     *      stdair::CmdBomManager for more details.
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
     * \see stdair::CmdBomManager for more details.
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
     * \see stdair::CmdBomManager for more details.
     *
     * @param const bool isForCRS Whether the sample booking request is for CRS.
     * @return BookingRequestStruct& Sample booking request structure.
     */
    stdair::BookingRequestStruct
    buildSampleBookingRequest (const bool isForCRS = false);

    
  public:
    // //////////////// Export support methods /////////////////
    /**
     * Dispatch the JSon command string to the SimCRS service.
     * (Only SimCRS has json export commands for now).
     * 
     * @param std::string& Input string which contained the JSon command string.
     * @return std::string Output string in which the asking objects are
     *         logged/dumped with a JSon format.
     */
    std::string jsonHandler (const std::string& iJSONString) const;

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


    // /////// Construction and Destruction helper methods ///////
    /**
     * Default constructor. It should not be used.
     */
    DSIM_Service();

    /**
     * Default copy constructor. It should not be used.
     */
    DSIM_Service (const DSIM_Service&);

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
     * @param const stdair::BasLogParams& Parameters for the output log
     *        stream.
     */
    stdair::STDAIR_ServicePtr_T initStdAirService (const stdair::BasLogParams&);  

    /**
     * Initialise the SEVMGR service (including the log service).
     *
     * A reference on the root of the Event Queue, namely the EventQueue object,
     * is stored within the service context for later use.
     */
    void initSEVMGRService ();
    
    /**
     * Initialise the SimCRS service (including the log service).
     */
    void initSIMCRSService();

    /**
     * Initialise the TraDemGen service (including the log service).
     *
     * @param const stdair::RandomSeed_T& Seed for the random generation used
     *        by the demand generation component (TraDemGen).
     */
    void initTRADEMGENService (const stdair::RandomSeed_T&);

    /**
     * Initialise the TravelCCM service (including the log service).
     */
    void initTRAVELCCMService();

    /**
     * Attach the StdAir service (holding the log and database services) to
     * the SIMCRS_Service.
     *
     * @param stdair::STDAIR_ServicePtr_T Reference on the StdAir service.
     * @param const bool State whether or not SimCRS owns the StdAir service
     *        resources.
     */
    void addStdAirService (stdair::STDAIR_ServicePtr_T,
                           const bool iOwnStdairService);
    
    /**
     * Initialise the (DSim) service context (i.e., the
     * DSIM_ServiceContext object).
     *
     * @param const stdair::Date_T& Start date of the simulation.
     * @param const stdair::Date_T& End date of the simulation.
     */
    void initServiceContext (const stdair::Date_T& iStartDate,
                             const stdair::Date_T& iEndDate);

    /**
     * Initialise the (DSim) service context (i.e., the DSIM_ServiceContext
     * object).
     */
    void initDsimService();

    /**
     * Finalise.
     */
    void finalise();


  private:
    // ///////// Service Context /////////
    /**
     * DSim context.
     */
    DSIM_ServiceContext* _dsimServiceContext;
  };
}
#endif // __DSIM_SVC_DSIM_SERVICE_HPP
