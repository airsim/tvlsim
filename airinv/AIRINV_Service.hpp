#ifndef __AIRINV_SVC_AIRINV_SERVICE_HPP
#define __AIRINV_SVC_AIRINV_SERVICE_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
// StdAir
#include <stdair/stdair_basic_types.hpp>
#include <stdair/stdair_service_types.hpp>

/// Forward declarations
namespace stdair {
  class AirlineFeatureSet;
  class STDAIR_Service;
  struct BasLogParams;
  struct BasDBParams;
  class Inventory;
  struct TravelSolutionStruct;
}

namespace AIRINV {
  
  /// Forward declarations
  class AIRINV_ServiceContext;

  
  /**
   * @brief Interface for the AIRINV Services.
   */
  class AIRINV_Service {
  public:   
    // ////////// Constructors and destructors //////////
    /**
     * Constructor.
     *
     * The initAirinvService() method is called; see the corresponding
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
     * @param const stdair::Filename_T& Filename of the input inventory file.
     */
    AIRINV_Service (const stdair::BasLogParams&, const stdair::BasDBParams&);

    /**
     * Constructor.
     *
     * The initAirinvService() method is called; see the corresponding
     * documentation for more details.
     *
     * A reference on an output stream is given, so that log outputs
     * can be directed onto that stream.
     *
     * @param const stdair::BasLogParams& Parameters for the output log stream.
     * @param const stdair::Filename_T& Filename of the input inventory file.
     */
    AIRINV_Service (const stdair::BasLogParams&);

    /**
     * Constructor.
     *
     * The initAirinvService() method is called; see the corresponding
     * documentation for more details.
     *
     * Moreover, as no reference on any output stream is given, it is
     * assumed that the StdAir log service has already been initialised
     * with the proper log output stream by some other methods in the
     * calling chain (for instance, when the AIRINV_Master_Service is
     * itself being initialised by another library service such as
     * SIMCRS_Service).
     *
     * @param stdair::STDAIR_ServicePtr_T Reference on the STDAIR service.
     * @param const stdair::Filename_T& Filename of the input inventory file.
     */
    AIRINV_Service (stdair::STDAIR_ServicePtr_T);

    /**
     * Constructor.
     *
     * The initAirinvService() method is called; see the corresponding
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
     * @param const stdair::Filename_T& Filename of the input inventory file.
     */
    AIRINV_Service (const stdair::BasLogParams&, const stdair::BasDBParams&,
                    const stdair::Filename_T& iInventoryInputFilename);

    /**
     * Constructor.
     *
     * The initAirinvService() method is called; see the corresponding
     * documentation for more details.
     *
     * A reference on an output stream is given, so that log outputs
     * can be directed onto that stream.
     *
     * @param const stdair::BasLogParams& Parameters for the output log stream.
     * @param const stdair::Filename_T& Filename of the input inventory file.
     */
    AIRINV_Service (const stdair::BasLogParams&, 
                    const stdair::Filename_T& iInventoryInputFilename);

    /**
     * Constructor.
     *
     * The initAirinvService() method is called; see the corresponding
     * documentation for more details.
     *
     * Moreover, as no reference on any output stream is given, it is
     * assumed that the StdAir log service has already been initialised
     * with the proper log output stream by some other methods in the
     * calling chain (for instance, when the AIRINV_Master_Service is
     * itself being initialised by another library service such as
     * SIMCRS_Service).
     *
     * @param stdair::STDAIR_ServicePtr_T Reference on the STDAIR service.
     * @param const stdair::Filename_T& Filename of the input inventory file.
     */
    AIRINV_Service (stdair::STDAIR_ServicePtr_T,
                    const stdair::Filename_T& iInventoryInputFilename);

    /**
     * Constructor.
     *
     * The initAirinvService() method is called; see the corresponding
     * documentation for more details.
     *
     * Moreover, a reference on an output stream is given, so that log
     * outputs can be directed onto that stream.
     *
     * @param const stdair::BasLogParams& Parameters for the output log stream.
     * @param const stdair::BasDBParams& Parameters for the database access.
     * @param const stdair::Filename_T& Filename of the input schedule file.
     * @param const stdair::Filename_T& Filename of the input O&D file.
     */
    AIRINV_Service (const stdair::BasLogParams&, const stdair::BasDBParams&,
                    const stdair::Filename_T& iScheduleInputFilename,
                    const stdair::Filename_T& iODInputFilename);

    /**
     * Constructor.
     *
     * The initAirinvService() method is called; see the corresponding
     * documentation for more details.
     *
     * Moreover, a reference on an output stream is given, so
     * that log outputs can be directed onto that stream.
     *
     * @param const stdair::BasLogParams& Parameters for the output log stream.
     * @param const stdair::Filename_T& Filename of the input schedule file.
     * @param const stdair::Filename_T& Filename of the input O&D file.
     */
    AIRINV_Service (const stdair::BasLogParams&,
                    const stdair::Filename_T& iScheduleInputFilename,
                    const stdair::Filename_T& iODInputFilename);

    /**
     * Constructor.
     *
     * The initAirinvService() method is called; see the corresponding
     * documentation for more details.
     *
     * Moreover, as no reference on any output stream is given, it is
     * assumed that the StdAir log service has already been initialised
     * with the proper log output stream by some other methods in the
     * calling chain (for instance, when the AIRINV_Master_Service is
     * itself being initialised by another library service such as
     * SIMCRS_Service).
     *
     * @param stdair::STDAIR_ServicePtr_T Reference on the STDAIR service.
     * @param const stdair::Filename_T& Filename of the input schedule file.
     * @param const stdair::Filename_T& Filename of the input O&D file.
     */
    AIRINV_Service (stdair::STDAIR_ServicePtr_T,
                    const stdair::Filename_T& iScheduleInputFilename,
                    const stdair::Filename_T& iODInputFilename);

    /**
     * Destructor.
     */
    ~AIRINV_Service();
    

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
     *
     * @param const bool isForRMOL Whether the sample BOM tree is for RMOL.
     * @param const CabinCapacity_T Capacity of the cabin for RMOL optimisation.
     */
    void buildSampleBom (const bool isForRMOL = false,
                         const stdair::CabinCapacity_T iCabinCapacity = 0);

    /**
     * Register a booking.
     *
     * @param const std::string& Key for the segment on which the sale is made.
     * @param const stdair::ClassCode_T& Class code where the sale is made.
     * @param const stdair::PartySize_T& Party size.
     * @return bool Whether or not the sale was successfull
     */
    bool sell (const std::string& iSegmentDateKey, const stdair::ClassCode_T&,
               const stdair::PartySize_T&);


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
     * Default constructor. It should not be used.
     */
    AIRINV_Service();
    /**
     * Default copy constructor. It should not be used.
     */
    AIRINV_Service (const AIRINV_Service&);

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
     * Initialise the RMOL service (including the log service).
     */
    void initRMOLService (stdair::STDAIR_ServicePtr_T);
    
    /**
     * Attach the STDAIR service (holding the log and database services) to
     * the AIRINV_Service.
     *
     * @param stdair::STDAIR_ServicePtr_T Reference on the STDAIR service.
     * @param const bool State whether or not AirInv owns the STDAIR service
     *        resources.
     */
    void addStdAirService (stdair::STDAIR_ServicePtr_T ioSTDAIR_ServicePtr,
                           const bool iOwnStdairService);
    
    /**
     * Initialise the (AIRINV) service context (i.e., the
     * AIRINV_ServiceContext object).
     */
    void initServiceContext();

    /**
     * Initialise.
     *
     * No input file is given. A sample BOM tree may be built by
     * calling the buildSampleBom() method.
     */
    void initAirinvService();

    /**
     * Initialise.
     *
     * The CSV file, describing the airline inventory for the
     * simulator, is parsed and instantiated in memory accordingly.
     *
     * @param const stdair::Filename_T& Filename of the input demand file.
     */
    void initAirinvService (const stdair::Filename_T& iInventoryInputFilename);

    /**
     * Initialise.
     *
     * The CSV file, describing the airline inventory for the
     * simulator, is parsed and instantiated in memory accordingly.
     *
     * @param const stdair::Filename_T& Filename of the input schedule file.
     * @param const stdair::Filename_T& Filename of the input O&D file.
     */
    void initAirinvService (const stdair::Filename_T& iScheduleInputFilename,
                            const stdair::Filename_T& iODInputFilename);

    /**
     * Finalise.
     */
    void finalise();
    

  private:
    // ///////// Service Context /////////
    /**
     * Airinv context.
     */
    AIRINV_ServiceContext* _airinvServiceContext;
  };
}
#endif // __AIRINV_SVC_AIRINV_SERVICE_HPP
