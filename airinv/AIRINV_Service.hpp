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
#include <stdair/basic/ForecastingMethod.hpp>
#include <stdair/bom/RMEventTypes.hpp>

/// Forward declarations
namespace stdair {
  class AirlineFeatureSet;
  class STDAIR_Service;
  class Inventory;
  struct TravelSolutionStruct;
  struct BasLogParams;
  struct BasDBParams;
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
     * Parse the inventory dump and load it into memory.
     *
     * The CSV file, describing the airline inventory for the
     * simulator, is parsed and instantiated in memory accordingly.
     *
     * @param const stdair::Filename_T& Filename of the input demand file.
     */
    void parseAndLoad (const stdair::Filename_T& iInventoryFilename);

    /**
     * Parse the schedule and O&D input files, and load them into memory.
     *
     * The CSV files, describing the airline schedule and the O&Ds for
     * the simulator, are parsed and instantiated in memory accordingly.
     *
     * @param const stdair::Filename_T& Filename of the input schedule file.
     * @param const stdair::Filename_T& Filename of the input O&D file.
     * @param const stdair::Filename_T& Filename of the input yield file.
     */
    void parseAndLoad (const stdair::Filename_T& iScheduleFilename,
                       const stdair::Filename_T& iODInputFilename,
                       const stdair::Filename_T& iYieldInputFilename);

    /**
     * Destructor.
     */
    ~AIRINV_Service();
    

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
     * Initialise the RM events for the inventory.
     * @param const stdair::Date_T& Parameters for the start date.
     * @param const stdair::Date_T& Parameters for the end date.
     */
    stdair::RMEventList_T initRMEvents (const stdair::Date_T& iStartDate,
                                        const stdair::Date_T& iEndDate);
    
    /**
     * Compute the availability for the given travel solution.
     */
    void calculateAvailability (stdair::TravelSolutionStruct&);

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

    /**
     * Take inventory snapshots.
     */
    void takeSnapshots (const stdair::AirlineCode_T&,
                        const stdair::DateTime_T&);
    
    /**
     * Optimise (revenue management) an flight-date/network-date
     */
    void optimise (const stdair::AirlineCode_T&,
                   const stdair::KeyDescription_T&,
                   const stdair::DateTime_T&,
                   const stdair::ForecastingMethod&);


  public:
    // //////////////// Export support methods /////////////////
    /**
     * Recursively dump, in the returned string and in JSON format,
     * the flight-date corresponding to the parameters given as input.
     *
     * @param const stdair::AirlineCode_T& Airline code of the flight to dump.
     * @param const stdair::FlightNumber_T& Flight number of the
     *        flight to dump.
     * @param const stdair::Date_T& Departure date of the flight to dump.
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
     * Recursively display (dump in the returned string) the flight-date
     * corresponding to the parameters given as input.
     *
     * @param const stdair::AirlineCode_T& Airline code of the flight to display
     * @param const stdair::FlightNumber_T& Flight number of the
     *        flight to display.
     * @param const stdair::Date_T& Departure date of the flight to display.
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
    void initRMOLService();
    
    /**
     * Initialise the AIRRAC service (including the log service).
     */
    void initAIRRACService();
    
    /**
     * Attach the STDAIR service (holding the log and database services) to
     * the AIRINV_Service.
     *
     * @param stdair::STDAIR_ServicePtr_T Reference on the STDAIR service.
     * @param const bool State whether or not AirInv owns the STDAIR service
     *        resources.
     */
    void addStdAirService (stdair::STDAIR_ServicePtr_T,
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
