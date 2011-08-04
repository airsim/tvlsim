#ifndef __AIRSCHED_SVC_AIRSCHED_SERVICE_HPP
#define __AIRSCHED_SVC_AIRSCHED_SERVICE_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// StdAir
#include <stdair/stdair_basic_types.hpp>
#include <stdair/stdair_service_types.hpp>
#include <stdair/bom/TravelSolutionTypes.hpp>
// AirSched
#include <airsched/AIRSCHED_Types.hpp>

/// Forward declarations
namespace stdair {
  class STDAIR_Service;
  struct BasLogParams;
  struct BasDBParams;
  struct BookingRequestStruct;
  struct TravelSolutionStruct;
}

namespace AIRSCHED {

  /// Forward declarations
  class AIRSCHED_ServiceContext;

  
  /**
   * @brief Interface for the AirSched Services.
   */
  class AIRSCHED_Service {
  public:
    // ////////////////// Constructors and Destructors //////////////////    
    /**
     * Constructor.
     *
     * The initAirschedService() method is called; see the corresponding
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
     */
    AIRSCHED_Service (const stdair::BasLogParams&, const stdair::BasDBParams&);  
    
    /**
     * Constructor.
     *
     * The initAirschedService() method is called; see the corresponding
     * documentation for more details.
     *
     * A reference on an output stream is given, so that log outputs
     * can be directed onto that stream.
     *
     * @param const stdair::BasLogParams& Parameters for the output log stream.
     */
    AIRSCHED_Service (const stdair::BasLogParams&);

    /**
     * Constructor.
     *
     * The initAirschedService() method is called; see the corresponding
     * documentation for more details.
     *
     * Moreover, as no reference on any output stream is given, it is
     * assumed that the StdAir log service has already been initialised
     * with the proper log output stream by some other methods in the
     * calling chain (for instance, when the AIRSCHED_Service is
     * itself being initialised by another library service such as
     * SIMCRS_Service).
     *
     * @param stdair::STDAIR_ServicePtr_T Reference on the STDAIR service.
     */
    AIRSCHED_Service (stdair::STDAIR_ServicePtr_T ioSTDAIR_ServicePtr);

    /**
     * Parse the schedule input file and load it into memory.
     *
     * The CSV file, describing the airline schedule for the
     * simulator, is parsed and instantiated in memory accordingly.
     *
     * @param const stdair::Filename_T& Filename of the input schedule file.
     */
    void parseAndLoad (const stdair::Filename_T& iScheduleInputFilename);

    /**
     * Parse the schedule and O&D input files, and load them into memory.
     *
     * The CSV files, describing the airline schedule and the O&Ds for
     * the simulator, are parsed and instantiated in memory accordingly.
     *
     * @param const stdair::Filename_T& Filename of the input schedule file.
     * @param const stdair::Filename_T& Filename of the input O&D file.
     */
    void parseAndLoad (const stdair::Filename_T& iScheduleFilename,
                       const stdair::Filename_T& iODInputFilename);

    /**
     * Destructor.
     */
    ~AIRSCHED_Service();

    
  public:
    // /////////// Business Methods /////////////
    /**
     * Build a sample BOM tree, and attach it to the BomRoot instance.
     *
     * The BOM tree is based on two actual inventories (one for BA,
     * another for AF). Each inventory contains one flight. One of
     * those flights has two legs (and therefore three segments).
     */
    void buildSampleBom();

    /**
     * Calculate and return a list of travel solutions corresponding to
     * a given product demand.
     */
    void buildSegmentPathList (stdair::TravelSolutionList_T&,
                               const stdair::BookingRequestStruct&);

    /**
     * Perform a small simulation, which uses the Customer Choice Model (CCM).
     *
     * Currently, that method does nothing.
     */
    void simulate();


  public:
    // //////////////// Export support methods /////////////////
    /**
     * Recursively dump, in the returned string and in JSON format,
     * the flight-period corresponding to the parameters given as input.
     *
     * @param const stdair::AirlineCode_T& Airline code of the flight to dump.
     * @param const stdair::FlightNumber_T& Flight number of the
     *        flight to dump.
     * @param const stdair::Date_T& Departure date of a flight within the
     *        flight period to dump.
     * @return std::string Output string in which the BOM tree is JSON-ified.
     */
    std::string jsonExport (const stdair::AirlineCode_T&,
                            const stdair::FlightNumber_T&,
                            const stdair::Date_T& iDepartureDate) const;


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
     * Recursively display (dump in the returned string) the flight-period
     * corresponding to the parameters given as input.
     *
     * @param const stdair::AirlineCode_T& Airline code of the flight period
     *        to display.
     * @param const stdair::FlightNumber_T& Flight number of the
     *        flight to display.
     * @param const stdair::Date_T& Departure date of a flight within the
     *        flight-period to display.
     * @return std::string Output string in which the BOM tree is
     *        logged/dumped.
     */
    std::string csvDisplay (const stdair::AirlineCode_T&,
                            const stdair::FlightNumber_T&,
                            const stdair::Date_T& iDepartureDate) const;


  private:
    // /////// Construction and Destruction helper methods ///////
    /**
     * Default constructor. It should not be used.
     */
    AIRSCHED_Service();

    /**
     * Default copy constructor. It should not be used.
     */
    AIRSCHED_Service (const AIRSCHED_Service&);

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
     * the AIRSCHED_Service.
     *
     * @param stdair::STDAIR_ServicePtr_T Reference on the STDAIR service.
     * @param const bool State whether or not AirInv owns the STDAIR service
     *        resources.
     */
    void addStdAirService (stdair::STDAIR_ServicePtr_T,
                           const bool iOwnStdairService);
    
    /**
     * Initialise the (AirSched) service context (i.e., the
     * AIRSCHED_ServiceContext object).
     */
    void initServiceContext();

    /**
     * Initialise.
     *
     * No input file is given. A sample BOM tree may be built by
     * calling the buildSampleBom() method.
     */
    void initAirschedService();

    /**
     * Finalise.
     */
    void finalise();

    
  private:
    // ///////// Service Context /////////
    /**
     * AirSched context.
     */
    AIRSCHED_ServiceContext* _airschedServiceContext;
  };
}
#endif // __AIRSCHED_SVC_AIRSCHED_SERVICE_HPP
