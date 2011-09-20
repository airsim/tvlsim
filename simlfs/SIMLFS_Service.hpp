#ifndef __SIMLFS_SVC_SIMLFS_SERVICE_HPP
#define __SIMLFS_SVC_SIMLFS_SERVICE_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// StdAir
#include <stdair/stdair_basic_types.hpp>
#include <stdair/stdair_service_types.hpp>
#include <stdair/bom/TravelSolutionTypes.hpp>
// SimFQT
#include <simfqt/SIMFQT_Types.hpp>
// SimLFS
#include <simlfs/SIMLFS_Types.hpp>

// Forward declarations.
namespace stdair {
  class STDAIR_Service;
  struct BasLogParams;
  struct BasDBParams;
  struct BookingRequestStruct;
  struct TravelSolutionStruct;
}

namespace SIMLFS {

  // Forward declaration
  class SIMLFS_ServiceContext;

  
  /**
   * Interface for the SIMLFS Services.
   */
  class SIMLFS_Service {
  public:
    // ////////////////// Constructors and Destructors //////////////////    
    /**
     * Constructor.
     *
     * The init() method is called; see the corresponding
     * documentation for more details.
     *
     * A reference on an output stream is given, so that log outputs
     * can be directed onto that stream.  Moreover, database connection
     * parameters are given, so that a session can be created on the
     * corresponding database.
     *     
     * @param const stdair::BasLogParams& Parameters for the output log stream.
     * @param const stdair::BasDBParams& Parameters for the database access.
     * @param const stdair::Filename_T& Filename of the input schedule file.
     * @param const stdair::Filename_T& Filename of the input O&D file.
     * @param const SIMFQT::FareFilePath& Filename of the input fare file.
     * @param const stdair::Filename_T& Filename of the input yield file.
     */
    SIMLFS_Service (const stdair::BasLogParams&, const stdair::BasDBParams&,
                    const stdair::Filename_T& iScheduleInputFilename,
                    const stdair::Filename_T& iODInputFilename,
                    const SIMFQT::FareFilePath& iFareInputFilepath,
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
     * @param const stdair::Date_T& Date for the beginning of analysis.
     * @param const stdair::Filename_T& Filename of the input schedule file.
     * @param const stdair::Filename_T& Filename of the input O&D file.
     * @param const SIMFQT::FareFilePath& Filename of the input fare file.
     * @param const stdair::Filename_T& Filename of the input yield file.
     */
    SIMLFS_Service (const stdair::BasLogParams&,
                    const stdair::Filename_T& iScheduleInputFilename,
                    const stdair::Filename_T& iODInputFilename,
                    const SIMFQT::FareFilePath& iFareInputFilepath,
                    const stdair::Filename_T& iYieldInputFilename);

    /**
     * Constructor.
     *
     * The init() method is called; see the corresponding documentation
     * for more details.
     *
     * Moreover, as no reference on any output stream is given, it is
     * assumed that the StdAir log service has already been
     * initialised with the proper log output stream by some other
     * methods in the calling chain (for instance, when the
     * SIMLFS_Service is itself being initialised by another library
     * service such as SIMCRS_Service).
     *
     * @param const stdair::Date_T& Date for the beginning of analysis.
     * @param const stdair::Filename_T& Filename of the input schedule file.
     * @param const stdair::Filename_T& Filename of the input O&D file.
     * @param const SIMFQT::FareFilePath& Filename of the input fare file.
     * @param const stdair::Filename_T& Filename of the input yield file.
     */
    SIMLFS_Service (stdair::STDAIR_ServicePtr_T,
                    const stdair::Filename_T& iScheduleInputFilename,
                    const stdair::Filename_T& iODInputFilename,
                    const SIMFQT::FareFilePath& iFareInputFilepath,
                    const stdair::Filename_T& iYieldInputFilename);

    /** Destructor. */
    ~SIMLFS_Service();

    
  public:
    // /////////// Business Methods /////////////
    /**
     * Construct a travel solution list by calculating the fare of
     * each segment paths in the list.
     */
    void fareQuote (const stdair::BookingRequestStruct&,
                    stdair::TravelSolutionList_T&);

    /**
     * Compute the availability for each travel solution in the list.
     */
    void calculateAvailability (stdair::TravelSolutionList_T&);


  private:
    // /////// Construction and Destruction helper methods ///////
    /**
     * Default constructor.
     */
    SIMLFS_Service ();
    
    /**
     * Default copy constructor.
     */
    SIMLFS_Service (const SIMLFS_Service&);


    /**
     * Initialise the (SIMLFS) service context (i.e., the
     * SIMLFS_ServiceContext object).
     */
    void initServiceContext();

    /**
     * Initialise the STDAIR service (including the log service).
     *
     * A reference on the root of the BOM tree, namely the BomRoot
     * object, is stored within the service context for later use.
     *
     * @param const stdair::BasLogParams& Parameters for the output
     *   log stream.
     * @param const stdair::BasDBParams& Parameters for the database
     *   access.
     */
    void initStdAirService (const stdair::BasLogParams&,
                            const stdair::BasDBParams&);
    
    /**
     * Initialise the STDAIR service (including the log service).
     *
     * A reference on the root of the BOM tree, namely the BomRoot
     * object, is stored within the service context for later use.
     *
     * @param const stdair::BasLogParams& Parameters for the output
     *   log stream.
     */
    void initStdAirService (const stdair::BasLogParams&);
    
    /**
     * Initialise.
     *
     * The CSV file, describing the airline fares for the simulator,
     * is parsed and the inventories are generated accordingly.
     *
     * @param const stdair::Filename_T& Filename of the input schedule file.
     * @param const stdair::Filename_T& Filename of the input O&D file.
     * @param const SIMFQT::FareFilePath& Filename of the input fare file.
     * @param const stdair::Filename_T& Filename of the input yield file.
     */
    void init (const stdair::Filename_T& iScheduleInputFilename,
               const stdair::Filename_T& iODInputFilename,
               const SIMFQT::FareFilePath& iFareInputFilepath,
               const stdair::Filename_T& iYieldInputFilename);

    /**
     * Initialise the SIMFQT service with the given schedule file.
     *
     * @param const stdair::Filename_T& Filename of the input schedule file.
     */
    void initSIMFQTService (const SIMFQT::FareFilePath& iFareInputFilepath);

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
    void finalise ();

    
  private:
    // ///////// Service Context /////////
    /**
     * SimLFS context.
     */
    SIMLFS_ServiceContext* _simlfsServiceContext;
  };
}
#endif // __SIMLFS_SVC_SIMLFS_SERVICE_HPP
