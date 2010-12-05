#ifndef __AIRINV_SVC_AIRINV_MASTER_SERVICE_HPP
#define __AIRINV_SVC_AIRINV_MASTER_SERVICE_HPP

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

  // Forward declaration
  class AIRINV_Master_ServiceContext;

  
  /** Interface for the AIRINV Services. */
  class AIRINV_Master_Service {
  public:   
    // ////////// Constructors and destructors //////////
    /** Constructor.
        <br>The init() method is called; see the corresponding documentation
        for more details.
        <br>A reference on an output stream is given, so that log
        outputs can be directed onto that stream.
        <br>Moreover, database connection parameters are given, so that a
        session can be created on the corresponding database.
        @param const stdair::BasLogParams& Parameters for the output log stream.
        @param const stdair::BasDBParams& Parameters for the database access.
        @param const stdair::Filename_T& Filename of the input demand file. */
    AIRINV_Master_Service (const stdair::BasLogParams&, 
                           const stdair::BasDBParams&,
                           const stdair::Filename_T& iInventoryInputFilename);

    /** Constructor.
        <br>The init() method is called; see the corresponding documentation
        for more details.
        <br>Moreover, a reference on an output stream is given, so
        that log outputs can be directed onto that stream.       
        @param const stdair::BasLogParams& Parameters for the output log stream.
        @param const stdair::Filename_T& Filename of the input demand file.  */
    AIRINV_Master_Service (const stdair::BasLogParams&,
                           const stdair::Filename_T& iInventoryInputFilename);

    /** Constructor.
        <br>The init() method is called; see the corresponding documentation
        for more details.
        <br>Moreover, as no reference on any output stream is given,
        it is assumed that the StdAir log service has already been
        initialised with the proper log output stream by some other
        methods in the calling chain (for instance,
        when the AIRINV_Master_Service
        is itself being initialised by another library service such as
        SIMCRS_Service).
        <br>The same logic is true about the absence of inventory input file.
        @param stdair::STDAIR_ServicePtr_T Reference on the STDAIR service.
        @param const stdair::Filename_T& Filename of the input schedule file.
        @param const stdair::Filename_T& Filename of the input O&D file. */
    AIRINV_Master_Service (stdair::STDAIR_ServicePtr_T,
                           const stdair::Filename_T&, const stdair::Filename_T&);

    /** Constructor.
        <br>The init() method is called; see the corresponding documentation
        for more details.
        <br>Moreover, a reference on an output stream is given, so
        that log outputs can be directed onto that stream.       
        @param const stdair::BasLogParams& Parameters for the output log stream.
        @param const stdair::BasDBParams& Parameters for the database access.
        @param const stdair::Filename_T& Filename of the input schedule file.
        @param const stdair::Filename_T& Filename of the input O&D file.  */
    AIRINV_Master_Service (const stdair::BasLogParams&,
                           const stdair::BasDBParams&,
                           const stdair::Filename_T&, const stdair::Filename_T&);

    /** Constructor.
        <br>The init() method is called; see the corresponding documentation
        for more details.
        <br>Moreover, a reference on an output stream is given, so
        that log outputs can be directed onto that stream.       
        @param const stdair::BasLogParams& Parameters for the output log stream.
        @param const stdair::Filename_T& Filename of the input schedule file.
        @param const stdair::Filename_T& Filename of the input O&D file.  */
    AIRINV_Master_Service (const stdair::BasLogParams&,
                           const stdair::Filename_T&, const stdair::Filename_T&);

    /** Destructor. */
    ~AIRINV_Master_Service();
    
    // /////////// Business Methods /////////////
    /** Register a booking. */
    bool sell (const std::string&, const stdair::ClassCode_T&,
               const stdair::PartySize_T&);

  private:
    // /////// Construction and Destruction helper methods ///////
    /** Default constructor. It should not be used. */
    AIRINV_Master_Service ();
    /** Default copy constructor. It should not be used. */
    AIRINV_Master_Service (const AIRINV_Master_Service&);

    /** Initialise the STDAIR service (including the log service).
        <br>A reference on the root of the BOM tree, namely the BomRoot object,
        is stored within the service context for later use.
        @param const stdair::BasLogParams& Parameters for the output log stream.
        @param const stdair::BasDBParams& Parameters for the database access. */
    stdair::STDAIR_ServicePtr_T initStdAirService (const stdair::BasLogParams&,
                                                   const stdair::BasDBParams&);

    /** Initialise the STDAIR service (including the log service).
        <br>A reference on the root of the BOM tree, namely the BomRoot object,
        is stored within the service context for later use.
        @param const stdair::BasLogParams& Parameters for the output log
               stream. */
    stdair::STDAIR_ServicePtr_T initStdAirService (const stdair::BasLogParams&);
    
    /** Attach the STDAIR service (holding the log and database services) to
        the AIRINV_Master_Service.
        @param stdair::STDAIR_ServicePtr_T Reference on the STDAIR service. */
    void addStdAirService (stdair::STDAIR_ServicePtr_T ioSTDAIR_ServicePtr);
    
    /** Initialise the (AIRINV) service context (i.e., the
        AIRINV_Master_ServiceContext object). */
    void initServiceContext ();

    /** Initialise.
        <br>The CSV file, describing the airline inventory for the
        simulator, is parsed and instantiated in memory accordingly.
        @param const stdair::Filename_T& Filename of the input demand file. */
    void init (const stdair::Filename_T& iInventoryInputFilename);

    /** Initialise.
        <br>The CSV file, describing the airline inventory for the
        simulator, is parsed and instantiated in memory accordingly.
        @param const stdair::Filename_T& Filename of the input schedule file.
        @param const stdair::Filename_T& Filename of the input O&D file. */
    void init (const stdair::Filename_T&, const stdair::Filename_T&);

    /** Finalise. */
    void finalise ();
    
  private:
    // ///////// Service Context /////////
    /** Airinv context. */
    AIRINV_Master_ServiceContext* _airinvMasterServiceContext;
  };
}
#endif // __AIRINV_SVC_AIRINV_MASTER_SERVICE_HPP
