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

// Forward declaration
namespace stdair {
  class AirlineFeatureSet;
  class STDAIR_Service;
  struct BasLogParams;
  struct BasDBParams;
  class Inventory;
  struct TravelSolutionStruct;
}

namespace AIRINV {
  
  /// Forward declaration
  class AIRINV_ServiceContext;

  
  /**
   * @brief Interface for the AIRINV Services.
   */
  class AIRINV_Service {
    
  public:   
    // ////////// Constructors and destructors //////////
    /**
     * Constructor.
     * The init() method is called; see the corresponding documentation
     * for more details.
     * A reference on an output stream is given, so that log
     * outputs can be directed onto that stream.
     * @param const stdair::Filename_T& Filename of the input demand file.
     */
    AIRINV_Service (const stdair::Filename_T& iInventoryInputFilename);

    /**
     * Constructor.
     *
     * The init() method is called; see the corresponding documentation
     * for more details.
     * @param const stdair::Filename_T& Filename of the input schedule file.
     * @param const stdair::Filename_T& Filename of the input O&D file.
     */
    AIRINV_Service (const stdair::Filename_T&, const stdair::Filename_T&);

    /**
     * Destructor.
     */
    ~AIRINV_Service();
    
    // /////////// Business Methods /////////////
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

  private:
    // /////// Construction and Destruction helper methods ///////
    /**
     * Default constructor. It should not be used.
     */
    AIRINV_Service ();
    /**
     * Default copy constructor. It should not be used.
     */
    AIRINV_Service (const AIRINV_Service&);

    /**
     * Initialise the STDAIR service (including the log service).
     *
     * A reference on the root of the BOM tree, namely the BomRoot object,
     * is stored within the service context for later use.
     */
    stdair::STDAIR_ServicePtr_T initStdAirService ();

    /**
     * Initialise the RMOL service (including the log service).
     */
    void initRMOLService (stdair::STDAIR_ServicePtr_T);
    
    /**
     * Attach the STDAIR service (holding the log and database services) to
     * the AIRINV_Service.
     * @param stdair::STDAIR_ServicePtr_T Reference on the STDAIR service.
     */
    void addStdAirService (stdair::STDAIR_ServicePtr_T ioSTDAIR_ServicePtr);
    
    /**
     * Initialise the (AIRINV) service context (i.e., the
     * AIRINV_ServiceContext object).
     */
    void initServiceContext ();

    /**
     * Initialise.
     *
     * The CSV file, describing the airline inventory for the
     * simulator, is parsed and instantiated in memory accordingly.
     * @param const stdair::Filename_T& Filename of the input demand file.
     */
    void init (const stdair::Filename_T& iInventoryInputFilename);

    /**
     * Initialise.
     *
     * The CSV file, describing the airline inventory for the
     * simulator, is parsed and instantiated in memory accordingly.
     * @param const stdair::Filename_T& Filename of the input schedule file.
     * @param const stdair::Filename_T& Filename of the input O&D file.
     */
    void init (const stdair::Filename_T&, const stdair::Filename_T&);

    /**
     * Finalise.
     */
    void finalise ();
    
  private:
    // ///////// Service Context /////////
    /**
     * Airinv context.
     */
    AIRINV_ServiceContext* _airinvServiceContext;
  };
}
#endif // __AIRINV_SVC_AIRINV_SERVICE_HPP
