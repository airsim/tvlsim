#ifndef __DSIM_SVC_DSIM_SERVICE_HPP
#define __DSIM_SVC_DSIM_SERVICE_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// StdAir
#include <stdair/stdair_basic_types.hpp>
#include <stdair/stdair_date_time_types.hpp>
#include <stdair/stdair_service_types.hpp>
// Dsim
#include <dsim/DSIM_Types.hpp>

// Forward declarations
namespace stdair {
  struct BasLogParams;
  struct BasDBParams;
}

namespace DSIM {

  // Forward declaration
  class DSIM_ServiceContext;
  struct RDSParameters;
  
  /** Interface for the DSIM Services. */
  class DSIM_Service {
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
        @param const stdiar::Date_T& Parameters for the start date.
        @param const stdiar::Date_T& Parameters for the end date.
        @param const stdair::Filename_T& Filename of the input schedule file.
        @param const stdair::Filename_T& Filename of the input O&D file.
        @param const stdair::Filename_T& Filename of the input fare file.
        @param const stdair::Filename_T& Filename of the input yield file.
        @param const stdair::Filename_T& Filename of the input demand file. */
    DSIM_Service (const stdair::BasLogParams&, const stdair::BasDBParams&,
                  const stdair::Date_T&, const stdair::Date_T&,
                  const stdair::Filename_T& iScheduleInputFilename,
                  const stdair::Filename_T& iODInputFilename,
                  const stdair::Filename_T& iFareInputFilename,
                  const stdair::Filename_T& iYieldInputFilename,
                  const stdair::Filename_T& iDemandInputFilenames);

    /** Constructor.
        <br>The init() method is called; see the corresponding documentation
        for more details.
        <br>Moreover, as no reference on any output stream is given,
        neither any database access parameter is given, it is assumed
        that the StdAir log service has already been initialised with
        the proper log output stream by some other methods in the
        calling chain (for instance, when the DSIM_Service is
        itself being initialised by another library service).
        @param const stdiar::Date_T& Parameters for the start date.
        @param const stdiar::Date_T& Parameters for the end date.
        @param const stdair::Filename_T& Filename of the input schedule file.
        @param const stdair::Filename_T& Filename of the input O&D file.
        @param const stdair::Filename_T& Filename of the input Fare file.
        @param const stdair::Filename_T& Filename of the input Yield file.
        @param const stdair::Filename_T& Filename of the input demand file. */
    DSIM_Service (stdair::STDAIR_ServicePtr_T,
                  const stdair::Date_T&, const stdair::Date_T&,
                  const stdair::Filename_T& iScheduleInputFilename,
                  const stdair::Filename_T& iODInputFilename,
                  const stdair::Filename_T& iFareInputFilename,
                  const stdair::Filename_T& iYieldInputFilename,
                  const stdair::Filename_T& iDemandInputFilenames);

    /** Destructor. */
    ~DSIM_Service();

    
  public:
    // /////////// Business Methods /////////////
    /** Perform a simulation. */
    void simulate();
    
    /** Display the list of airlines. */
    void displayAirlineListFromDB() const;

    
  private:
    // /////// Construction and Destruction helper methods ///////
    /** Default constructor. */
    DSIM_Service ();
    /** Default copy constructor. */
    DSIM_Service (const DSIM_Service&);

    /** Initialise the (DSIM) service context (i.e., the
        DSIM_ServiceContext object). 
        @param const stdiar::Date_T& Parameters for the start date.
        @param const stdiar::Date_T& Parameters for the end date.
    */
    void initServiceContext (const stdair::Date_T&, const stdair::Date_T&);

    /** Initialise the STDAIR service (including the log service).
        <br>A reference on the root of the BOM tree, namely the BomRoot object,
        is stored within the service context for later use.
        @param const stdair::BasLogParams& Parameters for the output log stream.
        @param const stdair::BasDBParams& Parameters for the database access.
        @param const stdair::AirlineFeatureSet& Set of airline features. */
    void initStdAirService (const stdair::BasLogParams&,
                            const stdair::BasDBParams&);
    
    /** Initialise.
        <br>The CSV file, describing the airline schedules for the
        simulator, is parsed and the inventories are generated accordingly.
        @param const stdair::Filename_T& Filename of the input schedule file.
        @param const stdair::Filename_T& Filename of the input O&D file.
        @param const stdair::Filename_T& Filename of the input Fare file.
        @param const stdair::Filename_T& Filename of the input Yield file.
        @param const stdair::Filename_T& Filename of the input demand file. */
    void init (const stdair::Filename_T& iScheduleInputFilename,
               const stdair::Filename_T& iODInputFilename,
               const stdair::Filename_T& iFareInputFilename,
               const stdair::Filename_T& iYieldInputFilename,
               const stdair::Filename_T& iDemandInputFilename);

    /** Initialise the snapshot and RM events for the inventories.
        @param const stdiar::Date_T& Parameters for the start date.
        @param const stdiar::Date_T& Parameters for the end date.
     */
    void initSnapshotAndRMEvents (const stdair::Date_T&, const stdair::Date_T&);

    /** Finalise. */
    void finalise ();

    
  private:
    // ///////// Service Context /////////
    /** Dsim context. */
    DSIM_ServiceContext* _dsimServiceContext;

  };
}
#endif // __DSIM_SVC_DSIM_SERVICE_HPP
