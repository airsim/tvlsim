#ifndef __SIMFQT_SVC_SIMFQT_SERVICE_HPP
#define __SIMFQT_SVC_SIMFQT_SERVICE_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// StdAir
#include <stdair/stdair_basic_types.hpp>
#include <stdair/stdair_service_types.hpp>
#include <stdair/bom/TravelSolutionTypes.hpp>
// SimFQT
#include <simfqt/SIMFQT_Types.hpp>

/// Forward declarations.
namespace stdair {
  class STDAIR_Service;
  struct BookingRequestStruct;
  struct BasLogParams;
  struct BasDBParams;
  struct BookingRequestStruct;
}

namespace SIMFQT {

  /// Forward declarations
  class SIMFQT_ServiceContext;

  
  /**
   * @brief Interface for the SIMFQT Services.
   */
  class SIMFQT_Service {
  public:
    
    // ////////////////// Constructors and Destructors //////////////////    
    /**
     * Constructor.
     *
     * The initSimfqtService() method is called; see the corresponding
     * documentation for more details.
     *
     * A reference on an output stream is given, so that log outputs
     * can be directed onto that stream.
     *
     * @param const stdair::BasLogParams& Parameters for the output log stream.
     */
    SIMFQT_Service (const stdair::BasLogParams&);
    
    /**
     * Constructor.
     *
     * The initSimfqtService() method is called; see the corresponding
     * documentation for more details.
     *
     * A reference on an output stream is given, so that log outputs
     * can be directed onto that stream.
     *
     * @param const stdair::BasLogParams& Parameters for the output log stream.
     * @param const stdair::BasDBParams& Parameters for the database access.
     */
    SIMFQT_Service (const stdair::BasLogParams&, const stdair::BasDBParams&);

    /**
     * Constructor.
     *
     * The initSimfqtService() method is called; see the corresponding
     * documentation for more details.
     *
     * Moreover, as no reference on any output stream is given,
     * it is assumed that the StdAir log service has already been
     * initialised with the proper log output stream by some other
     * methods in the calling chain (for instance, when the SIMFQT_Service
     * is itself being initialised by another library service such as
     * SIMCRS_Service).
     *
     * @param stdair::STDAIR_ServicePtr_T Reference on the STDAIR service.
     */
    SIMFQT_Service (stdair::STDAIR_ServicePtr_T ioSTDAIR_ServicePtr);

    /**
     * Parse the fare dump and load it into memory.
     *
     * The CSV file, describing the fare rule for the
     * simulator, is parsed and instantiated in memory accordingly.
     *
     * @param const stdair::Filename_T& Filename of the input fare file.
     */
    void parseAndLoad (const stdair::Filename_T& iFareInputFilename);

    /**
     * Destructor.
     */
    ~SIMFQT_Service();


  public:
    // /////////// Business Methods /////////////    
    /**
     * Build a sample BOM tree, and attach it to the BomRoot instance.
     *
     * As for now, two sample BOM trees can be built.
     * <ul>
     *   <li>One BOM tree is based on two actual inventories (one for BA,
     *     another for AF). Each inventory contains one flight. One of
     *     those flights has two legs (and therefore three segments).</li>
     *   <li>The other BOM tree is fake, as a hook for RMOL to work.</li>
     * </ul>
     */
    void buildSampleBom();

	/**
	 * Build a BookingRequest structure (for test purposes).
	 *
	 * @return stdair::BookingRequestStruct The created BookingRequest
	 *         structure.
	 */ 
    stdair::BookingRequestStruct buildBookingRequest();

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
     * Calculate the prices for a given list of travel solutions.
     * 
	 * A stdair::Fare_T attribute is calculated for every travel
	 * solution of the list.
     *
     * @param stdair::BookingRequestStruct& Booking request. 
     * @param stdair::TravelSolutionList_T& List of travel solution.
     */
    void quotePrices (const stdair::BookingRequestStruct&,
                      stdair::TravelSolutionList_T&);


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
     * Default constructor.
     */
    SIMFQT_Service();

    /**
     * Copy constructor.
     */
    SIMFQT_Service (const SIMFQT_Service&);

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
     * the SimFQT Service.
     *
     * @param stdair::STDAIR_ServicePtr_T Reference on the STDAIR service.
     * @param const bool State whether or not SimFQT owns the STDAIR service
     *        resources.
     */
    void addStdAirService (stdair::STDAIR_ServicePtr_T ioSTDAIR_ServicePtr,
                           const bool iOwnStdairService);
    
    /**
     * Initialise the (SIMFQT) service context (i.e., the
     * SIMFQT_ServiceContext object).
     */
    void initServiceContext();

    /**
     * Initialise.
     *
     * No input file is given. A sample BOM tree may be built by
     * calling the buildSampleBom() method.
     */
    void initSimfqtService(); 

    /**
     * Initialise.
     *
     * The CSV file, describing the airline fares for the
     * simulator, is parsed and the inventories are generated accordingly.
     *
     * @param const stdair::Filename_T& Filename of the input fare file.
     */
    void initSimfqtService (const stdair::Filename_T& iFareInputFilename); 

    /**
     * Finaliser.
     */
    void finalise();


  private:
    // ///////// Service Context /////////
    /**
     * SimFQT context.
     */
    SIMFQT_ServiceContext* _simfqtServiceContext;
  };
}
#endif // __SIMFQT_SVC_SIMFQT_SERVICE_HPP
