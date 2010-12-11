#ifndef __AIRRAC_SVC_AIRRAC_SERVICE_HPP
#define __AIRRAC_SVC_AIRRAC_SERVICE_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// StdAir
#include <stdair/stdair_basic_types.hpp>
#include <stdair/stdair_service_types.hpp>
#include <stdair/basic/BasLogParams.hpp>
#include <stdair/basic/BasDBParams.hpp>

// Forward declaration
namespace stdair {
  class AirlineFeatureSet;
  class STDAIR_Service;
  struct BasLogParams;
  struct BasDBParams;
  class YieldStore;
  struct TravelSolutionStruct;
}

namespace AIRRAC {

  // Forward declaration
  class AIRRAC_ServiceContext;

  
  /** Interface for the AIRRAC Services. */
  class AIRRAC_Service {
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
        @param const stdair::AirlineCode_T& Code of the owner airline.
        @param const stdair::Filename_T& Filename of the input demand file. */
    AIRRAC_Service (const stdair::BasLogParams&, const stdair::BasDBParams&,
                    const stdair::AirlineCode_T&,
                    const stdair::Filename_T& iYieldStoreInputFilename);

    /** Constructor.
        <br>The init() method is called; see the corresponding documentation
        for more details.
        <br>Moreover, a reference on an output stream is given, so
        that log outputs can be directed onto that stream.       
        @param const stdair::BasLogParams& Parameters for the output log stream.
        @param const stdair::AirlineCode_T& Code of the owner airline.
        @param const stdair::Filename_T& Filename of the input demand file.  */
    AIRRAC_Service (const stdair::BasLogParams&, const stdair::AirlineCode_T&,
                    const stdair::Filename_T& iYieldStoreInputFilename);

    /** Constructor.
        <br>The init() method is called; see the corresponding documentation
        for more details.
        <br>Moreover, as no reference on any output stream is given,
        it is assumed that the StdAir log service has already been
        initialised with the proper log output stream by some other
        methods in the calling chain (for instance, when the AIRRAC_Service
        is itself being initialised by another library service such as
        SIMCRS_Service).
        <br>The same logic is true about the absence of yield store input file.
        @param stdair::STDAIR_ServicePtr_T Reference on the STDAIR service.
        @param AirlineCode_T& Code of the owner airline. */
    AIRRAC_Service (stdair::STDAIR_ServicePtr_T, const stdair::AirlineCode_T&);

    /** Destructor. */
    ~AIRRAC_Service();

    
    // /////////// Business Methods /////////////
    /** Calculate/retrieve a yield. */
    void calculateYield (const stdair::TravelSolutionStruct&);

    
  private:
    // /////// Construction and Destruction helper methods ///////
    /** Default constructor. It should not be used. */
    AIRRAC_Service ();
    /** Default copy constructor. It should not be used. */
    AIRRAC_Service (const AIRRAC_Service&);

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
        the AIRRAC_Service.
        @param stdair::STDAIR_ServicePtr_T Reference on the STDAIR service. */
    void addStdAirService (stdair::STDAIR_ServicePtr_T ioSTDAIR_ServicePtr);
    
    /** Initialise the (AIRRAC) service context (i.e., the
        AIRRAC_ServiceContext object).
        @param const stdair::AirlineCode_T& Code of the owner airline.
        @param stdair::YieldStore& Root of the Airline-dedicated BOM tree. */
    void initServiceContext (const stdair::AirlineCode_T&, stdair::YieldStore&);

    /** Initialise.
        <br>The CSV file, describing the airline yield store for the
        simulator, is parsed and instantiated in memory accordingly.
        @param const stdair::Filename_T& Filename of the input demand file. */
    void init (const stdair::Filename_T& iYieldStoreInputFilename);

    /** Initialise.
        <br>No yield store input file is given, because the YieldStore is
        assumed to have been built in memory from an external component
        (typically, from the SimCRS library). */
    void init ();

    /** Finalise. */
    void finalise ();

    
  private:
    // ///////// Service Context /////////
    /** Airrac context. */
    AIRRAC_ServiceContext* _airracServiceContext;
  };
}
#endif // __AIRRAC_SVC_AIRRAC_SERVICE_HPP
