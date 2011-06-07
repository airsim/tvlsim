// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <sstream>
// Boost
#include <boost/make_shared.hpp>
// StdAir
#include <stdair/basic/BasChronometer.hpp>
#include <stdair/bom/BomManager.hpp> 
#include <stdair/bom/BookingRequestStruct.hpp>
#include <stdair/bom/TravelSolutionStruct.hpp>
#include <stdair/service/Logger.hpp>
#include <stdair/STDAIR_Service.hpp>
// AirSched
#include <airsched/basic/BasConst_AIRSCHED_Service.hpp>
#include <airsched/factory/FacAIRSCHEDServiceContext.hpp>
#include <airsched/command/Simulator.hpp>
#include <airsched/command/ScheduleParser.hpp>
#include <airsched/command/OnDParser.hpp>
#include <airsched/command/SegmentPathProvider.hpp>
#include <airsched/command/InventoryGenerator.hpp>
#include <airsched/service/AIRSCHED_ServiceContext.hpp>
#include <airsched/AIRSCHED_Service.hpp>

namespace AIRSCHED {

  // ////////////////////////////////////////////////////////////////////
  AIRSCHED_Service::AIRSCHED_Service() : _airschedServiceContext (NULL) {
    assert (false);
  }

  // ////////////////////////////////////////////////////////////////////
  AIRSCHED_Service::AIRSCHED_Service (const AIRSCHED_Service& iService)
    : _airschedServiceContext (NULL) {
    assert (false);
  }

  // ////////////////////////////////////////////////////////////////////
  AIRSCHED_Service::AIRSCHED_Service (const stdair::BasLogParams& iLogParams,
                                      const stdair::BasDBParams& iDBParams) 
    : _airschedServiceContext (NULL) {
    
    // Initialise the STDAIR service handler
    stdair::STDAIR_ServicePtr_T lSTDAIR_Service_ptr =
      initStdAirService (iLogParams, iDBParams);
    
    // Initialise the service context
    initServiceContext();
    
    // Add the StdAir service context to the AirSched service context
    // \note AirSched owns the STDAIR service resources here.
    const bool ownStdairService = true;
    addStdAirService (lSTDAIR_Service_ptr, ownStdairService);
    
    // Initialise the (remaining of the) context
    initAirschedService();
  }

  // ////////////////////////////////////////////////////////////////////
  AIRSCHED_Service::AIRSCHED_Service (const stdair::BasLogParams& iLogParams) 
    : _airschedServiceContext (NULL) {
    
    // Initialise the STDAIR service handler
    stdair::STDAIR_ServicePtr_T lSTDAIR_Service_ptr =
      initStdAirService (iLogParams);
    
    // Initialise the service context
    initServiceContext();
    
    // Add the StdAir service context to the AirSched service context
    // \note AirSched owns the STDAIR service resources here.
    const bool ownStdairService = true;
    addStdAirService (lSTDAIR_Service_ptr, ownStdairService);
    
    // Initialise the (remaining of the) context
    initAirschedService();
  }

  // ////////////////////////////////////////////////////////////////////
  AIRSCHED_Service::
  AIRSCHED_Service (stdair::STDAIR_ServicePtr_T ioSTDAIRServicePtr)
    : _airschedServiceContext (NULL) {

    // Initialise the service context
    initServiceContext();
    
    // Add the StdAir service context to the AirSched service context.
    // \note AirSched does not own the STDAIR service resources here.
    const bool doesNotOwnStdairService = false;
    addStdAirService (ioSTDAIRServicePtr, doesNotOwnStdairService);
    
    // Initialise the context
    initAirschedService();
  }

  // ////////////////////////////////////////////////////////////////////
  AIRSCHED_Service::
  AIRSCHED_Service (const stdair::BasLogParams& iLogParams,
                    const stdair::BasDBParams& iDBParams,
                    const stdair::Filename_T& iScheduleInputFilename) 
    : _airschedServiceContext (NULL) {
    
    // Initialise the STDAIR service handler
    stdair::STDAIR_ServicePtr_T lSTDAIR_Service_ptr =
      initStdAirService (iLogParams, iDBParams);
    
    // Initialise the service context
    initServiceContext();
    
    // Add the StdAir service context to the AirSched service context
    // \note AirSched owns the STDAIR service resources here.
    const bool ownStdairService = true;
    addStdAirService (lSTDAIR_Service_ptr, ownStdairService);
    
    // Initialise the (remaining of the) context
    initAirschedService (iScheduleInputFilename);
  }

  // ////////////////////////////////////////////////////////////////////
  AIRSCHED_Service::
  AIRSCHED_Service (const stdair::BasLogParams& iLogParams,
                    const stdair::Filename_T& iScheduleInputFilename) 
    : _airschedServiceContext (NULL) {
    
    // Initialise the STDAIR service handler
    stdair::STDAIR_ServicePtr_T lSTDAIR_Service_ptr =
      initStdAirService (iLogParams);
    
    // Initialise the service context
    initServiceContext();
    
    // Add the StdAir service context to the AirSched service context
    // \note AirSched owns the STDAIR service resources here.
    const bool ownStdairService = true;
    addStdAirService (lSTDAIR_Service_ptr, ownStdairService);
    
    // Initialise the (remaining of the) context
    initAirschedService (iScheduleInputFilename);
  }

  // ////////////////////////////////////////////////////////////////////
  AIRSCHED_Service::
  AIRSCHED_Service (stdair::STDAIR_ServicePtr_T ioSTDAIRServicePtr,
                    const stdair::Filename_T& iScheduleInputFilename)
    : _airschedServiceContext (NULL) {

    // Initialise the service context
    initServiceContext();
    
    // Add the StdAir service context to the AirSched service context.
    // \note AirSched does not own the STDAIR service resources here.
    const bool doesNotOwnStdairService = false;
    addStdAirService (ioSTDAIRServicePtr, doesNotOwnStdairService);
    
    // Initialise the context
    initAirschedService (iScheduleInputFilename);
  }

  // ////////////////////////////////////////////////////////////////////
  AIRSCHED_Service::
  AIRSCHED_Service (const stdair::BasLogParams& iLogParams,
                    const stdair::BasDBParams& iDBParams,
                    const stdair::Filename_T& iScheduleInputFilename,
                    const stdair::Filename_T& iODInputFilename) 
    : _airschedServiceContext (NULL) {
    
    // Initialise the STDAIR service handler
    stdair::STDAIR_ServicePtr_T lSTDAIR_Service_ptr =
      initStdAirService (iLogParams, iDBParams);
    
    // Initialise the service context
    initServiceContext();
    
    // Add the StdAir service context to the AirSched service context
    // \note AirSched owns the STDAIR service resources here.
    const bool ownStdairService = true;
    addStdAirService (lSTDAIR_Service_ptr, ownStdairService);
    
    // Initialise the (remaining of the) context
    initAirschedService (iScheduleInputFilename, iODInputFilename);
  }

  // ////////////////////////////////////////////////////////////////////
  AIRSCHED_Service::
  AIRSCHED_Service (const stdair::BasLogParams& iLogParams,
                    const stdair::Filename_T& iScheduleInputFilename,
                    const stdair::Filename_T& iODInputFilename) 
    : _airschedServiceContext (NULL) {
    
    // Initialise the STDAIR service handler
    stdair::STDAIR_ServicePtr_T lSTDAIR_Service_ptr =
      initStdAirService (iLogParams);
    
    // Initialise the service context
    initServiceContext();
    
    // Add the StdAir service context to the AirSched service context
    // \note AirSched owns the STDAIR service resources here.
    const bool ownStdairService = true;
    addStdAirService (lSTDAIR_Service_ptr, ownStdairService);
    
    // Initialise the (remaining of the) context
    initAirschedService (iScheduleInputFilename, iODInputFilename);
  }

  // ////////////////////////////////////////////////////////////////////
  AIRSCHED_Service::
  AIRSCHED_Service (stdair::STDAIR_ServicePtr_T ioSTDAIRServicePtr,
                    const stdair::Filename_T& iScheduleInputFilename,
                    const stdair::Filename_T& iODInputFilename)
    : _airschedServiceContext (NULL) {

    // Initialise the service context
    initServiceContext();
    
    // Add the StdAir service context to the AirSched service context.
    // \note AirSched does not own the STDAIR service resources here.
    const bool doesNotOwnStdairService = false;
    addStdAirService (ioSTDAIRServicePtr, doesNotOwnStdairService);
    
    // Initialise the context
    initAirschedService (iScheduleInputFilename, iODInputFilename);
  }

  // ////////////////////////////////////////////////////////////////////
  AIRSCHED_Service::~AIRSCHED_Service() {
    // Delete/Clean all the objects from memory
    finalise();
  }

  // ////////////////////////////////////////////////////////////////////
  void AIRSCHED_Service::finalise() {
    assert (_airschedServiceContext != NULL);
    // Reset the (Boost.)Smart pointer pointing on the STDAIR_Service object.
    _airschedServiceContext->reset();
  }

  // //////////////////////////////////////////////////////////////////////
  void AIRSCHED_Service::initServiceContext() {
    // Initialise the service context
    AIRSCHED_ServiceContext& lAIRSCHED_ServiceContext = 
      FacAIRSCHEDServiceContext::instance().create();
    _airschedServiceContext = &lAIRSCHED_ServiceContext;
  }

  // ////////////////////////////////////////////////////////////////////
  void AIRSCHED_Service::
  addStdAirService (stdair::STDAIR_ServicePtr_T ioSTDAIR_Service_ptr,
                    const bool iOwnStdairService) {

    // Retrieve the AirSched service context
    assert (_airschedServiceContext != NULL);
    AIRSCHED_ServiceContext& lAIRSCHED_ServiceContext =
      *_airschedServiceContext;

    // Store the STDAIR service object within the (AirSched) service context
    lAIRSCHED_ServiceContext.setSTDAIR_Service (ioSTDAIR_Service_ptr,
                                                iOwnStdairService);
  }

  // //////////////////////////////////////////////////////////////////////
  stdair::STDAIR_ServicePtr_T AIRSCHED_Service::
  initStdAirService (const stdair::BasLogParams& iLogParams,
                     const stdair::BasDBParams& iDBParams) {

    /**
     * Initialise the STDAIR service handler.
     *
     * \note The (Boost.)Smart Pointer keeps track of the references
     *       on the Service object, and deletes that object when it is
     *       no longer referenced (e.g., at the end of the process).
     */
    stdair::STDAIR_ServicePtr_T lSTDAIR_Service_ptr = 
      boost::make_shared<stdair::STDAIR_Service> (iLogParams, iDBParams);

    return lSTDAIR_Service_ptr;
  }
  
  // //////////////////////////////////////////////////////////////////////
  stdair::STDAIR_ServicePtr_T AIRSCHED_Service::
  initStdAirService (const stdair::BasLogParams& iLogParams) {

    /**
     * Initialise the STDAIR service handler.
     *
     * \note The (Boost.)Smart Pointer keeps track of the references
     *       on the Service object, and deletes that object when it is
     *       no longer referenced (e.g., at the end of the process).
     */
    stdair::STDAIR_ServicePtr_T lSTDAIR_Service_ptr = 
      boost::make_shared<stdair::STDAIR_Service> (iLogParams);

    return lSTDAIR_Service_ptr;
  }
  
  // ////////////////////////////////////////////////////////////////////
  void AIRSCHED_Service::initAirschedService() {
    // Do nothing at this stage. A sample BOM tree may be built by
    // calling the buildSampleBom() method
  }

  // ////////////////////////////////////////////////////////////////////
  void AIRSCHED_Service::
  initAirschedService (const stdair::Filename_T& iScheduleInputFilename) {

    // Retrieve the BOM tree root
    assert (_airschedServiceContext != NULL);
    AIRSCHED_ServiceContext& lAIRSCHED_ServiceContext =
      *_airschedServiceContext;
    stdair::STDAIR_Service& lSTDAIR_Service =
      lAIRSCHED_ServiceContext.getSTDAIR_Service();
    stdair::BomRoot& lBomRoot = lSTDAIR_Service.getBomRoot();

    // Parse the schedule input file, and generate the Inventories
    stdair::BasChronometer lINVGeneration; lINVGeneration.start();
    ScheduleParser::generateInventories (iScheduleInputFilename, lBomRoot);
    // const double lGenerationMeasure = lINVGeneration.elapsed();

    // DEBUG
    // STDAIR_LOG_DEBUG ("Inv generation time: " << lGenerationMeasure);
  }
  
  // ////////////////////////////////////////////////////////////////////
  void AIRSCHED_Service::
  initAirschedService (const stdair::Filename_T& iScheduleInputFilename,
                       const stdair::Filename_T& iODInputFilename) {

    // First, build the airline inventories from the schedule file
    initAirschedService (iScheduleInputFilename);

    // Retrieve the BOM tree root
    assert (_airschedServiceContext != NULL);
    AIRSCHED_ServiceContext& lAIRSCHED_ServiceContext =
      *_airschedServiceContext;
    stdair::STDAIR_Service& lSTDAIR_Service =
      lAIRSCHED_ServiceContext.getSTDAIR_Service();
    stdair::BomRoot& lBomRoot = lSTDAIR_Service.getBomRoot();

    // Parse the schedule input file, and generate the Inventories
    stdair::BasChronometer lOnDGeneration; lOnDGeneration.start();
    OnDParser::generateOnDPeriods (iODInputFilename, lBomRoot);
    // const double lGenerationMeasure = lOnDGeneration.elapsed();

    // DEBUG
    // STDAIR_LOG_DEBUG ("O&D generation time: " << lGenerationMeasure);
  }
  
  // //////////////////////////////////////////////////////////////////////
  void AIRSCHED_Service::buildSampleBom() {

    // Retrieve the AirSched service context
    if (_airschedServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The AirSched service has "
                                                    "not been initialised");
    }
    assert (_airschedServiceContext != NULL);

    // Retrieve the STDAIR service object from the (AirSched) service context
    AIRSCHED_ServiceContext& lAIRSCHED_ServiceContext =
      *_airschedServiceContext;
    stdair::STDAIR_Service& lSTDAIR_Service =
      lAIRSCHED_ServiceContext.getSTDAIR_Service();

    // Delegate the BOM building to the dedicated service
    lSTDAIR_Service.buildSampleBom();
  }

  // //////////////////////////////////////////////////////////////////////
  std::string AIRSCHED_Service::csvDisplay() const {

    // Retrieve the AirSched service context
    if (_airschedServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The AirSched service has "
                                                    "not been initialised");
    }
    assert (_airschedServiceContext != NULL);

    // Retrieve the STDAIR service object from the (AirSched) service context
    AIRSCHED_ServiceContext& lAIRSCHED_ServiceContext =
      *_airschedServiceContext;
    stdair::STDAIR_Service& lSTDAIR_Service =
      lAIRSCHED_ServiceContext.getSTDAIR_Service();

    // Delegate the BOM building to the dedicated service
    return lSTDAIR_Service.csvDisplay();
  }
  
  // ////////////////////////////////////////////////////////////////////
  void AIRSCHED_Service::simulate() {

    // Retrieve the AirSched service context
    if (_airschedServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The AirSched service has "
                                                    "not been initialised");
    }
    assert (_airschedServiceContext != NULL);

    // Retrieve the BOM tree root
    AIRSCHED_ServiceContext& lAIRSCHED_ServiceContext =
      *_airschedServiceContext;
    stdair::STDAIR_Service& lSTDAIR_Service =
      lAIRSCHED_ServiceContext.getSTDAIR_Service();
    stdair::BomRoot& lBomRoot = lSTDAIR_Service.getBomRoot();

    // Call the underlying Use Case (command)
    stdair::BasChronometer lSimulateChronometer; lSimulateChronometer.start();
    Simulator::simulate (lBomRoot);
    const double lSimulateMeasure = lSimulateChronometer.elapsed();

    // DEBUG
    STDAIR_LOG_DEBUG ("Simulation: " << lSimulateMeasure << " - "
                      << lAIRSCHED_ServiceContext.display());
  }

  // ////////////////////////////////////////////////////////////////////
  void AIRSCHED_Service::
  buildSegmentPathList (stdair::TravelSolutionList_T& ioTravelSolutionList,
			const stdair::BookingRequestStruct& iBookingRequest) {

    if (_airschedServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The AirSched service has "
                                                    "not been initialised");
    }
    assert (_airschedServiceContext != NULL);

    // Retrieve the BOM tree root
    AIRSCHED_ServiceContext& lAIRSCHED_ServiceContext =
      *_airschedServiceContext;
    stdair::STDAIR_Service& lSTDAIR_Service =
      lAIRSCHED_ServiceContext.getSTDAIR_Service();
    stdair::BomRoot& lBomRoot = lSTDAIR_Service.getBomRoot();
    
    // Delegate the call to the dedicated command
    stdair::BasChronometer lBuildChronometer; lBuildChronometer.start();
    SegmentPathProvider::buildSegmentPathList (ioTravelSolutionList,
					       lBomRoot, iBookingRequest);
    // const double lBuildMeasure = lBuildChronometer.elapsed();

    // DEBUG
    // STDAIR_LOG_DEBUG ("Segment-path build: " << lBuildMeasure << " - "
    //                   << lAIRSCHED_ServiceContext.display());
  }

}
