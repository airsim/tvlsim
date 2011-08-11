// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// Boost
#include <boost/make_shared.hpp>
// SOCI
#include <soci/core/soci.h>
// StdAir
#include <stdair/basic/BasChronometer.hpp>
#include <stdair/bom/BomManager.hpp>
#include <stdair/bom/BomRoot.hpp>
#include <stdair/bom/AirlineStruct.hpp>
#include <stdair/bom/BookingRequestStruct.hpp>
#include <stdair/command/DBManagerForAirlines.hpp>
#include <stdair/service/Logger.hpp>
#include <stdair/service/DBSessionManager.hpp>
#include <stdair/STDAIR_Service.hpp>
// Distribution
#include <simcrs/SIMCRS_Service.hpp>
// TraDemGen
#include <trademgen/TRADEMGEN_Service.hpp>
// TravelCCM
#include <travelccm/TRAVELCCM_Service.hpp>
// Dsim
#include <dsim/basic/BasConst_DSIM_Service.hpp>
#include <dsim/factory/FacDsimServiceContext.hpp>
#include <dsim/command/Simulator.hpp>
#include <dsim/service/DSIM_ServiceContext.hpp>
#include <dsim/DSIM_Service.hpp>

namespace DSIM {

  // //////////////////////////////////////////////////////////////////////
  DSIM_Service::DSIM_Service () : _dsimServiceContext (NULL) {
    assert (false);
  }

  // //////////////////////////////////////////////////////////////////////
  DSIM_Service::DSIM_Service (const DSIM_Service& iService) {
    assert (false);
  }

  // //////////////////////////////////////////////////////////////////////
  DSIM_Service::DSIM_Service (const stdair::BasLogParams& iLogParams,
                              const stdair::Date_T& iStartDate,
                              const stdair::Date_T& iEndDate)
    : _dsimServiceContext (NULL) {
    
    // Initialise the StdAir service handler
    stdair::STDAIR_ServicePtr_T lSTDAIR_Service_ptr =
      initStdAirService (iLogParams);
    
    // Initialise the service context
    initServiceContext (iStartDate, iEndDate);
    
    // Add the StdAir service context to the DSim service context
    // \note DSim owns the StdAir service resources here.
    const bool ownStdairService = true;
    addStdAirService (lSTDAIR_Service_ptr, ownStdairService);

    // Initalise the TraDemGen service.
    initTRADEMGENService();

    // Initalise the TravelCCM service.
    initTRAVELCCMService();    

    // Initalise the SimCRS service, itself initialising AirInv, Airsched
    // and SimFQT.
    initSIMCRSService();

    // Initialise the (remaining of the) context
    initDsimService();
  }

  // //////////////////////////////////////////////////////////////////////
  DSIM_Service::DSIM_Service (const stdair::BasLogParams& iLogParams,
                              const stdair::BasDBParams& iDBParams,
                              const stdair::Date_T& iStartDate,
                              const stdair::Date_T& iEndDate)
    : _dsimServiceContext (NULL) {
    
    // Initialise the StdAir service handler
    stdair::STDAIR_ServicePtr_T lSTDAIR_Service_ptr =
      initStdAirService (iLogParams, iDBParams);
    
    // Initialise the service context
    initServiceContext (iStartDate, iEndDate);
    
    // Add the StdAir service context to the DSim service context
    // \note DSim owns the StdAir service resources here.
    const bool ownStdairService = true;
    addStdAirService (lSTDAIR_Service_ptr, ownStdairService);

    // Initalise the TraDemGen service.
    initTRADEMGENService();

    // Initalise the TravelCCM service.
    initTRAVELCCMService();    

    // Initalise the SimCRS service, itself initialising AirInv, Airsched
    // and SimFQT.
    initSIMCRSService();

    // Initialise the (remaining of the) context
    initDsimService();
  }

  // //////////////////////////////////////////////////////////////////////
  DSIM_Service::DSIM_Service (stdair::STDAIR_ServicePtr_T ioSTDAIR_Service_ptr,
                              const stdair::Date_T& iStartDate,
                              const stdair::Date_T& iEndDate)
    : _dsimServiceContext (NULL) {
    
    // Initialise the service context
    initServiceContext (iStartDate, iEndDate);
    
    // Store the STDAIR service object within the (AIRINV) service context
    // \note AirInv does not own the STDAIR service resources here.
    const bool doesNotOwnStdairService = false;
    addStdAirService (ioSTDAIR_Service_ptr, doesNotOwnStdairService);

    // Initalise the TraDemGen service.
    initTRADEMGENService();

    // Initalise the TravelCCM service.
    initTRAVELCCMService();    

    // Initalise the SimCRS service, itself initialising AirInv, Airsched
    // and SimFQT.
    initSIMCRSService();

    // Initialise the (remaining of the) context
    initDsimService();
  }

  // //////////////////////////////////////////////////////////////////////
  DSIM_Service::~DSIM_Service() {
    // Delete/Clean all the objects from memory
    finalise();
  }

  // //////////////////////////////////////////////////////////////////////
  void DSIM_Service::finalise() {
    assert (_dsimServiceContext != NULL);
    // Reset the (Boost.)Smart pointer pointing on the STDAIR_Service object.
    _dsimServiceContext->reset();
  }

  // ////////////////////////////////////////////////////////////////////
  void DSIM_Service::initServiceContext (const stdair::Date_T& iStartDate,
                                         const stdair::Date_T& iEndDate) {
    // Initialise the context
    DSIM_ServiceContext& lDSIM_ServiceContext =
      FacDsimServiceContext::instance().create (iStartDate, iEndDate);
    _dsimServiceContext = &lDSIM_ServiceContext;
  }

  // ////////////////////////////////////////////////////////////////////
  void DSIM_Service::
  addStdAirService (stdair::STDAIR_ServicePtr_T ioSTDAIR_Service_ptr,
                    const bool iOwnStdairService) {

    // Retrieve the Dsim service context
    assert (_dsimServiceContext != NULL);
    DSIM_ServiceContext& lDSIM_ServiceContext = *_dsimServiceContext;
    
    // Store the StdAir service object within the (DSim) service context
    lDSIM_ServiceContext.setSTDAIR_Service (ioSTDAIR_Service_ptr,
                                            iOwnStdairService);
  }
  
  // //////////////////////////////////////////////////////////////////////
  stdair::STDAIR_ServicePtr_T DSIM_Service::
  initStdAirService (const stdair::BasLogParams& iLogParams) {

    /**
     * Initialise the StdAir service handler.
     *
     * \note The (Boost.)Smart Pointer keeps track of the references
     *       on the Service object, and deletes that object when it is
     *       no longer referenced (e.g., at the end of the process).
     */
    stdair::STDAIR_ServicePtr_T lSTDAIR_Service_ptr = 
      boost::make_shared<stdair::STDAIR_Service> (iLogParams);

    return lSTDAIR_Service_ptr;
  }
  
  // //////////////////////////////////////////////////////////////////////
  stdair::STDAIR_ServicePtr_T DSIM_Service::
  initStdAirService (const stdair::BasLogParams& iLogParams,
                     const stdair::BasDBParams& iDBParams) {

    /**
     * Initialise the StdAir service handler.
     *
     * \note The (Boost.)Smart Pointer keeps track of the references
     *       on the Service object, and deletes that object when it is
     *       no longer referenced (e.g., at the end of the process).
     */
    stdair::STDAIR_ServicePtr_T lSTDAIR_Service_ptr = 
      boost::make_shared<stdair::STDAIR_Service> (iLogParams, iDBParams);

    return lSTDAIR_Service_ptr;
  }
  
  // ////////////////////////////////////////////////////////////////////
  void DSIM_Service::initSIMCRSService() {

    // Retrieve the Dsim service context
    assert (_dsimServiceContext != NULL);
    DSIM_ServiceContext& lDSIM_ServiceContext = *_dsimServiceContext;
    
    // Retrieve the StdAir service context
    stdair::STDAIR_ServicePtr_T lSTDAIR_Service_ptr =
      lDSIM_ServiceContext.getSTDAIR_ServicePtr();

    // TODO: do not hardcode the CRS code (e.g., take it from a
    // configuration file).
    // Initialise the SIMCRS service handler
    const SIMCRS::CRSCode_T lCRSCode = "1S";
    
    /**
     * Initialise the SIMCRS service handler.
     *
     * \note The (Boost.)Smart Pointer keeps track of the references
     *       on the Service object, and deletes that object when it is
     *       no longer referenced (e.g., at the end of the process).
     */
    SIMCRS::SIMCRS_ServicePtr_T lSIMCRS_Service_ptr = 
      boost::make_shared<SIMCRS::SIMCRS_Service> (lSTDAIR_Service_ptr,
                                                  lCRSCode);
    
    // Store the SIMCRS service object within the (DSim) service context
    lDSIM_ServiceContext.setSIMCRS_Service (lSIMCRS_Service_ptr);
  }
  
  // ////////////////////////////////////////////////////////////////////
  void DSIM_Service::initTRADEMGENService() {

    // Retrieve the Dsim service context
    assert (_dsimServiceContext != NULL);
    DSIM_ServiceContext& lDSIM_ServiceContext = *_dsimServiceContext;
    
    // Retrieve the StdAir service context
    stdair::STDAIR_ServicePtr_T lSTDAIR_Service_ptr =
      lDSIM_ServiceContext.getSTDAIR_ServicePtr();

    /**
     * Initialise the TraDemGen service handler.
     *
     * \note The (Boost.)Smart Pointer keeps track of the references
     *       on the Service object, and deletes that object when it is
     *       no longer referenced (e.g., at the end of the process).
     */
    TRADEMGEN::TRADEMGEN_ServicePtr_T lTRADEMGEN_Service_ptr = 
      boost::make_shared<TRADEMGEN::TRADEMGEN_Service> (lSTDAIR_Service_ptr);
    
    // Store the TRADEMGEN service object within the (DSim) service context
    lDSIM_ServiceContext.setTRADEMGEN_Service (lTRADEMGEN_Service_ptr);
  }
  
  // ////////////////////////////////////////////////////////////////////
  void DSIM_Service::initTRAVELCCMService() {

    // Retrieve the Dsim service context
    assert (_dsimServiceContext != NULL);
    DSIM_ServiceContext& lDSIM_ServiceContext = *_dsimServiceContext;
    
    // Retrieve the StdAir service context
    stdair::STDAIR_ServicePtr_T lSTDAIR_Service_ptr =
      lDSIM_ServiceContext.getSTDAIR_ServicePtr();

    /**
     * Initialise the TravelCCM service handler.
     *
     * \note The (Boost.)Smart Pointer keeps track of the references
     *       on the Service object, and deletes that object when it is
     *       no longer referenced (e.g., at the end of the process).
     */
    TRAVELCCM::TRAVELCCM_ServicePtr_T lTRAVELCCM_Service_ptr = 
      boost::make_shared<TRAVELCCM::TRAVELCCM_Service> (lSTDAIR_Service_ptr);
    
    // Store the TRAVELCCM service object within the (DSim) service context
    lDSIM_ServiceContext.setTRAVELCCM_Service (lTRAVELCCM_Service_ptr);
  }
  
  // //////////////////////////////////////////////////////////////////////
  void DSIM_Service::initDsimService() {
    // Do nothing at this stage. A sample BOM tree may be built by
    // calling the buildSampleBom() method
  }

  // ////////////////////////////////////////////////////////////////////
  void DSIM_Service::
  parseAndLoad (const stdair::Filename_T& iScheduleInputFilename,
                const stdair::Filename_T& iODInputFilename,
                const stdair::Filename_T& iYieldInputFilename,
                const stdair::Filename_T& iFareInputFilename,
                const stdair::Filename_T& iDemandInputFilename) {

    // Retrieve the Dsim service context
    assert (_dsimServiceContext != NULL);
    DSIM_ServiceContext& lDSIM_ServiceContext = *_dsimServiceContext;
    
    /**
     * Let the schedule, inventory and pricing managers parse their input files.
     */
    SIMCRS::SIMCRS_Service& lSIMCRS_Service =
      lDSIM_ServiceContext.getSIMCRS_Service();
    lSIMCRS_Service.parseAndLoad (iScheduleInputFilename, iODInputFilename,
                                  iYieldInputFilename, iFareInputFilename);

    // Parse the demand input file.
    TRADEMGEN::TRADEMGEN_Service& lTRADEMGEN_Service =
      lDSIM_ServiceContext.getTRADEMGEN_Service();
    lTRADEMGEN_Service.parseAndLoad (iDemandInputFilename);
  }
  
  // ////////////////////////////////////////////////////////////////////
  void DSIM_Service::buildSampleBom() {

    // Retrieve the DSim service context
    if (_dsimServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The DSim service "
                                                    "has not been initialised");
    }
    assert (_dsimServiceContext != NULL);

    // Retrieve the DSim service context and whether it owns the Stdair
    // service
    DSIM_ServiceContext& lDSIM_ServiceContext = *_dsimServiceContext;
    const bool doesOwnStdairService =
      lDSIM_ServiceContext.getOwnStdairServiceFlag();

    // Retrieve the StdAir service object from the (SimCRS) service context
    stdair::STDAIR_Service& lSTDAIR_Service =
      lDSIM_ServiceContext.getSTDAIR_Service();

    /**
     * 1. Have StdAir build the whole BOM tree, only when the StdAir service is
     *    owned by the current component (DSim here).
     */
    if (doesOwnStdairService == true) {
      //
      lSTDAIR_Service.buildSampleBom();
    }

    /**
     * 2. Delegate the complementary building of objects and links by the
     *    appropriate levels/components.
     */
    /**
     * Let the CRS (i.e., the SimCRS component) build the schedules, O&Ds,
     * inventories, yields and fares.
     */
    SIMCRS::SIMCRS_Service& lSIMCRS_Service =
      lDSIM_ServiceContext.getSIMCRS_Service();
    lSIMCRS_Service.buildSampleBom();

    /**
     * Let the demand manager (i.e., the TraDemGen component) build the
     * demand generators/streams.
     */
    TRADEMGEN::TRADEMGEN_Service& lTRADEMGEN_Service =
      lDSIM_ServiceContext.getTRADEMGEN_Service();
    lTRADEMGEN_Service.buildSampleBom();

    /**
     * Let the customer choice manager (i.e., the TravelCCM component) build the
     * customer choice sets.
     */
    TRAVELCCM::TRAVELCCM_Service& lTRAVELCCM_Service =
      lDSIM_ServiceContext.getTRAVELCCM_Service();
    lTRAVELCCM_Service.buildSampleBom();

    /**
     * 3. Build the complementary objects/links for the current component (here,
     *    DSim).
     *
     * \note: Currently, no more things to do by DSim.
     */
  }

  // //////////////////////////////////////////////////////////////////////
  void DSIM_Service::
  buildSampleTravelSolutions(stdair::TravelSolutionList_T& ioTravelSolutionList){

    // Retrieve the DSim service context
    if (_dsimServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The DSim service "
                                                    "has not been initialised");
    }
    assert (_dsimServiceContext != NULL);

    // Retrieve the StdAir service object from the (AirSched) service context
    DSIM_ServiceContext& lDSIM_ServiceContext = *_dsimServiceContext;
    stdair::STDAIR_Service& lSTDAIR_Service =
      lDSIM_ServiceContext.getSTDAIR_Service();

    // Delegate the BOM building to the dedicated service
    lSTDAIR_Service.buildSampleTravelSolutions (ioTravelSolutionList);
  }

  // //////////////////////////////////////////////////////////////////////
  stdair::BookingRequestStruct DSIM_Service::
  buildSampleBookingRequest (const bool isForCRS) {

    // Retrieve the DSim service context
    if (_dsimServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The DSim service "
                                                    "has not been initialised");
    }
    assert (_dsimServiceContext != NULL);

    // Retrieve the StdAir service object from the (AirSched) service context
    DSIM_ServiceContext& lDSIM_ServiceContext = *_dsimServiceContext;
    stdair::STDAIR_Service& lSTDAIR_Service =
      lDSIM_ServiceContext.getSTDAIR_Service();

    // Delegate the BOM building to the dedicated service
    return lSTDAIR_Service.buildSampleBookingRequest (isForCRS);
  }

  // ////////////////////////////////////////////////////////////////////
  std::string DSIM_Service::
  jsonExport (const stdair::AirlineCode_T& iAirlineCode,
              const stdair::FlightNumber_T& iFlightNumber,
              const stdair::Date_T& iDepartureDate) const {

    // Retrieve the DSim service context
    if (_dsimServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The DSim service "
                                                    "has not been initialised");
    }
    assert (_dsimServiceContext != NULL);

    // Retrieve the StdAir service object from the (AirSched) service context
    DSIM_ServiceContext& lDSIM_ServiceContext = *_dsimServiceContext;
    stdair::STDAIR_Service& lSTDAIR_Service =
      lDSIM_ServiceContext.getSTDAIR_Service();

    // Delegate the JSON export to the dedicated service
    return lSTDAIR_Service.jsonExport (iAirlineCode, iFlightNumber,
                                       iDepartureDate);
  }

  // ////////////////////////////////////////////////////////////////////
  void DSIM_Service::initSnapshotAndRMEvents() {
    // Retrieve the DSim service context
    if (_dsimServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The DSim service "
                                                    "has not been initialised");
    }
    assert (_dsimServiceContext != NULL);
    DSIM_ServiceContext& lDSIM_ServiceContext = *_dsimServiceContext;

    // Retrieve the simulation parameters
    const stdair::Date_T& lStartDate = lDSIM_ServiceContext.getStartDate();
    const stdair::Date_T& lEndDate = lDSIM_ServiceContext.getEndDate();

    // Get a reference on the SIMCRS service handler
    SIMCRS::SIMCRS_Service& lSIMCRS_Service =
      lDSIM_ServiceContext.getSIMCRS_Service();

    lSIMCRS_Service.initSnapshotAndRMEvents (lStartDate, lEndDate);
  }
  
  // //////////////////////////////////////////////////////////////////////
  std::string DSIM_Service::csvDisplay() const {

    // Retrieve the DSim service context
    if (_dsimServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The DSim service "
                                                    "has not been initialised");
    }
    assert (_dsimServiceContext != NULL);

    // Retrieve the StdAir service object from the (AirSched) service context
    DSIM_ServiceContext& lDSIM_ServiceContext = *_dsimServiceContext;
    stdair::STDAIR_Service& lSTDAIR_Service =
      lDSIM_ServiceContext.getSTDAIR_Service();

    // Delegate the BOM building to the dedicated service
    return lSTDAIR_Service.csvDisplay();
  }
  
  // //////////////////////////////////////////////////////////////////////
  void DSIM_Service::
  simulate (const NbOfRuns_T& iNbOfRuns,
            const stdair::DemandGenerationMethod& iDemandGenerationMethod,
            const stdair::ForecastingMethod& iForecastingMethod) {

    // Retrieve the DSim service context
    if (_dsimServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The DSim service has not "
                                                    "been initialised");
    }
    assert (_dsimServiceContext != NULL);
    DSIM_ServiceContext& lDSIM_ServiceContext= *_dsimServiceContext;

    // Get a reference on the SIMCRS service handler
    SIMCRS::SIMCRS_Service& lSIMCRS_Service =
      lDSIM_ServiceContext.getSIMCRS_Service();

    // Get a reference on the TRADEMGEN service handler
    TRADEMGEN::TRADEMGEN_Service& lTRADEMGEN_Service =
      lDSIM_ServiceContext.getTRADEMGEN_Service();
    
    // Get a reference on the TRAVELCCM service handler
    TRAVELCCM::TRAVELCCM_Service& lTRAVELCCM_Service =
      lDSIM_ServiceContext.getTRAVELCCM_Service();

    // Get a reference on the STDAIR service handler
    stdair::STDAIR_Service& lSTDAIR_Service =
      lDSIM_ServiceContext.getSTDAIR_Service();

    for (NbOfRuns_T idx = 0; idx != iNbOfRuns; ++idx) {
      // DEBUG
      STDAIR_LOG_DEBUG ("Simulation[" << idx << "] begins - "
                        << lDSIM_ServiceContext.display());

      // Delegate the booking to the dedicated command
      stdair::BasChronometer lSimulationChronometer;
      lSimulationChronometer.start();
      Simulator::simulate (lSIMCRS_Service, lTRADEMGEN_Service,
                           lTRAVELCCM_Service, lSTDAIR_Service,
                           iDemandGenerationMethod, iForecastingMethod);
      const double lSimulationMeasure = lSimulationChronometer.elapsed();

      // Reset the service (including the event queue) for the next run
      lTRADEMGEN_Service.reset();

      // DEBUG
      STDAIR_LOG_DEBUG ("Simulation[" << idx << "] ends: " << lSimulationMeasure
                        << " - " << lDSIM_ServiceContext.display());
    }
  }

  // //////////////////////////////////////////////////////////////////////
  void DSIM_Service::displayAirlineListFromDB () const {

    // Retrieve the DSim service context
    if (_dsimServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The DSim service has not "
                                                    "been initialised");
    }
    assert (_dsimServiceContext != NULL);
    //DSIM_ServiceContext& lDSIM_ServiceContext = *_dsimServiceContext;

    // Get the date-time for the present time
    boost::posix_time::ptime lNowDateTime =
      boost::posix_time::second_clock::local_time();
    //boost::gregorian::date lNowDate = lNowDateTime.date();

    // DEBUG
    STDAIR_LOG_DEBUG (std::endl
                      << "==================================================="
                      << std::endl
                      << lNowDateTime);

    // Delegate the query execution to the dedicated command
    stdair::BasChronometer lDsimChronometer;
    lDsimChronometer.start();

    // Retrieve the database session handler
    stdair::DBSession_T& lDBSession =
      stdair::DBSessionManager::instance().getDBSession();
    
    // Prepare and execute the select statement
    stdair::AirlineStruct lAirline;
    stdair::DBRequestStatement_T lSelectStatement (lDBSession);
    stdair::DBManagerForAirlines::prepareSelectStatement (lDBSession,
                                                          lSelectStatement,
                                                          lAirline);
    
    // Prepare the SQL request corresponding to the select statement
    bool hasStillData = true;
    unsigned int idx = 0;
    while (hasStillData == true) {
      hasStillData =
        stdair::DBManagerForAirlines::iterateOnStatement (lSelectStatement,
                                                          lAirline);
      
      // DEBUG
      STDAIR_LOG_DEBUG ("[" << idx << "]: " << lAirline);
      
      // Iteration
      ++idx;
    }
    
    const double lDsimMeasure = lDsimChronometer.elapsed();
    
    // DEBUG
    STDAIR_LOG_DEBUG ("Sample service for Dsim: " << lDsimMeasure);
  }

}
