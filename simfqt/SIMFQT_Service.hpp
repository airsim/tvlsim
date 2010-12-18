#ifndef __SIMFQT_SVC_SIMFQT_SERVICE_HPP
#define __SIMFQT_SVC_SIMFQT_SERVICE_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// StdAir
#include <stdair/stdair_basic_types.hpp>
// SimFQT
#include <simfqt/SIMFQT_Types.hpp>
#include <stdair/bom/TravelSolutionTypes.hpp>

// Forward declarations.
namespace stdair {
  class STDAIR_Service;
  struct BasLogParams;
  struct BasDBParams;
}

namespace SIMFQT {

  // Forward declaration
  class SIMFQT_ServiceContext;

  
  /** Interface for the SIMFQT Services. */
  class SIMFQT_Service {
  public:
    // /////////// Business Methods /////////////    
    // ////////////////// Constructors and Destructors //////////////////    
    /** Constructor.
        <br>The init() method is called; see the corresponding documentation
        for more details.
        <br>A reference on an output stream is given, so that log
        outputs can be directed onto that stream.
        <br>Moreover, database connection parameters are given, so that a
        session can be created on the corresponding database.
        @param const stdair::BasLogParams& Parameters for the output log stream.
        @param const stdair::BasDBParams& Parameters for the database access.
        @param const stdair::Filename_T& Filename of the input fare file. */
    SIMFQT_Service (const stdair::BasLogParams&, const stdair::BasDBParams&,
                    const stdair::Filename_T& iFareInputFilename);

    /** Constructor.
        <br>The init() method is called; see the corresponding documentation
        for more details.
        <br>Moreover, a reference on an output stream is given, so
        that log outputs can be directed onto that stream.       
        @param const stdair::BasLogParams& Parameters for the output log stream.
        @param const stdair::Date_T& Date for the beginning of analysis.
        @param const stdair::Filename_T& Filename of the input fare file. */
    SIMFQT_Service (const stdair::BasLogParams&,
                    const stdair::Filename_T& iFareInputFilename);

    /** Constructor.
        <br>The init() method is called; see the corresponding documentation
        for more details.
        <br>Moreover, as no reference on any output stream is given,
        it is assumed that the StdAir log service has already been
        initialised with the proper log output stream by some other
        methods in the calling chain (for instance, when the SIMFQT_Service
        is itself being initialised by another library service such as
        SIMCRS_Service).
        @param const stdair::Date_T& Date for the beginning of analysis.
        @param const stdair::Filename_T& Filename of the input fare file. */
    SIMFQT_Service (stdair::STDAIR_ServicePtr_T ioSTDAIR_ServicePtr,
                    const stdair::Filename_T& iFareInputFilename);

    /** Destructor. */
    ~SIMFQT_Service();

    
  private:
    // /////// Construction and Destruction helper methods ///////
    /** Default constructor. */
    SIMFQT_Service ();
    /** Default copy constructor. */
    SIMFQT_Service (const SIMFQT_Service&);

    /** Initialise the (SIMFQT) service context (i.e., the
        SIMFQT_ServiceContext object). */
    void initServiceContext ();

    /** Initialise the STDAIR service (including the log service).
        <br>A reference on the root of the BOM tree, namely the BomRoot object,
        is stored within the service context for later use.
        @param const stdair::BasLogParams& Parameters for the output log stream.
        @param const stdair::BasDBParams& Parameters for the database access. */
    void initStdAirService (const stdair::BasLogParams&,
                            const stdair::BasDBParams&);
    
    /** Initialise the STDAIR service (including the log service).
        <br>A reference on the root of the BOM tree, namely the BomRoot object,
        is stored within the service context for later use.
        @param const stdair::BasLogParams& Parameters for the output log
               stream. */
    void initStdAirService (const stdair::BasLogParams&);
    
    /** Initialise.
        <br>The CSV file, describing the airline fares for the
        simulator, is parsed and the inventories are generated accordingly.
        @param const stdair::Filename_T& Filename of the input fare file. */
    void init (const stdair::Filename_T& iFareInputFilename); 
    
    /** Calculate the fares corresponding to a given list of travel 
	solutions.
        <br>The stdair::Fare_T attribute of each travel solution of the list 
	is calculated.
        @param stdair::TravelSolutionList_T& List of travel solution. */
    void getFares (stdair::TravelSolutionList_T&);

    /** Finalise. */
    void finalise ();

    
  private:
    // ///////// Service Context /////////
    /** Simfqt context. */
    SIMFQT_ServiceContext* _simfqtServiceContext;
  };
}
#endif // __SIMFQT_SVC_SIMFQT_SERVICE_HPP
