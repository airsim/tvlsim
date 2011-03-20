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
     * The init() method is called; see the corresponding documentation
     * for more details.
     *
     * A reference on an output stream is given, so that log outputs
     * can be directed onto that stream.
     *
     * Moreover, database connection parameters are given, so that a
     * session can be created on the corresponding database.
     *
     * @param const stdair::BasLogParams& Parameters for the output log
     *        stream.
     * @param const stdair::BasDBParams& Parameters for the database access.
     * @param const stdair::Filename_T& Filename of the input fare file.
     */
    SIMFQT_Service (const stdair::BasLogParams&, const stdair::BasDBParams&,
                    const stdair::Filename_T& iFareInputFilename);

    /**
     * Constructor.
     *
     * The init() method is called; see the corresponding documentation
     * for more details.
     *
     * A reference on an output stream is given, so that log outputs
     * can be directed onto that stream.
     *
     * @param const stdair::BasLogParams& Parameters for the output log
     *        stream.
     * @param const stdair::Filename_T& Filename of the input fare file.
     */
    SIMFQT_Service (const stdair::BasLogParams&,
                    const stdair::Filename_T& iFareInputFilename);

    /**
     * Constructor.
     *
     * The init() method is called; see the corresponding documentation
     * for more details.
     *
     * Moreover, as no reference on any output stream is given, it is
     * assumed that the StdAir log service has already been initialised
     * with the proper log output stream by some other methods in the
     * calling chain (for instance, when the SIMFQT_Service is itself
     * being initialised by another library service such as SIMCRS_Service).
     *
     * @param const stdair::STDAIR_ServicePtr_T Reference on the STDAIR_Service
     *        holding log (and potentially database) parameters.
     * @param const stdair::Filename_T& Filename of the input fare file.
     */
    SIMFQT_Service (stdair::STDAIR_ServicePtr_T ioSTDAIR_ServicePtr,
                    const stdair::Filename_T& iFareInputFilename);

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
