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
#include <stdair/basic/BasFileMgr.hpp>
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
#include <airsched/command/SegmentPathGenerator.hpp>
#include <airsched/command/SegmentPathProvider.hpp>
#include <airsched/command/InventoryGenerator.hpp>
#include <airsched/service/AIRSCHED_ServiceContext.hpp>
#include <airsched/AIRSCHED_Service.hpp>

namespace AIRSCHED {

  // ////////////////////////////////////////////////////////////////////
  AIRSCHED_Service::AIRSCHED_Service () : _airschedServiceContext (NULL) {
    assert (false);
  }

  // ////////////////////////////////////////////////////////////////////
  AIRSCHED_Service::AIRSCHED_Service (const AIRSCHED_Service& iService) {
    assert (false);
  }

  // ////////////////////////////////////////////////////////////////////
  AIRSCHED_Service::
  AIRSCHED_Service (stdair::STDAIR_ServicePtr_T ioSTDAIR_ServicePtr,
                    const stdair::Filename_T& iScheduleInputFilename)
    : _airschedServiceContext (NULL) {

    // Initialise the service context
    initServiceContext ();
    
    // Retrieve the AirSched service context
    assert (_airschedServiceContext != NULL);
    AIRSCHED_ServiceContext& lAIRSCHED_ServiceContext =
      *_airschedServiceContext;
    
    // Store the STDAIR service object within the (AIRSCHED) service context
    lAIRSCHED_ServiceContext.setSTDAIR_Service (ioSTDAIR_ServicePtr);
    
    // Initialise the context
    init (iScheduleInputFilename);
  }

  // ////////////////////////////////////////////////////////////////////
  AIRSCHED_Service::
  AIRSCHED_Service (stdair::STDAIR_ServicePtr_T ioSTDAIR_ServicePtr,
                    const stdair::Filename_T& iScheduleInputFilename,
                    const stdair::Filename_T& iODInputFilename)
    : _airschedServiceContext (NULL) {

    // Initialise the service context
    initServiceContext ();
    
    // Retrieve the AirSched service context
    assert (_airschedServiceContext != NULL);
    AIRSCHED_ServiceContext& lAIRSCHED_ServiceContext =
      *_airschedServiceContext;
    
    // Store the STDAIR service object within the (AIRSCHED) service context
    lAIRSCHED_ServiceContext.setSTDAIR_Service (ioSTDAIR_ServicePtr);
    
    // Initialise the context
    init (iScheduleInputFilename, iODInputFilename);
  }

  // ////////////////////////////////////////////////////////////////////
  AIRSCHED_Service::
  AIRSCHED_Service (const stdair::BasLogParams& iLogParams,
                    const stdair::BasDBParams& iDBParams,
                    const stdair::Filename_T& iScheduleInputFilename) 
    : _airschedServiceContext (NULL) {
    
    // Initialise the service context
    initServiceContext ();
    
    // Initialise the STDAIR service handler
    initStdAirService (iLogParams, iDBParams);
    
    // Initialise the (remaining of the) context
    init (iScheduleInputFilename);
  }

  // ////////////////////////////////////////////////////////////////////
  AIRSCHED_Service::
  AIRSCHED_Service (const stdair::BasLogParams& iLogParams,
                    const stdair::BasDBParams& iDBParams,
                    const stdair::Filename_T& iScheduleInputFilename,
                    const stdair::Filename_T& iODInputFilename) 
    : _airschedServiceContext (NULL) {
    
    // Initialise the service context
    initServiceContext ();
    
    // Initialise the STDAIR service handler
    initStdAirService (iLogParams, iDBParams);
    
    // Initialise the (remaining of the) context
    init (iScheduleInputFilename, iODInputFilename);
  }

  // ////////////////////////////////////////////////////////////////////
  AIRSCHED_Service::
  AIRSCHED_Service (const stdair::BasLogParams& iLogParams,
                    const stdair::Filename_T& iScheduleInputFilename) 
    : _airschedServiceContext (NULL) {
    
    // Initialise the service context
    initServiceContext ();
    
    // Initialise the STDAIR service handler
    initStdAirService (iLogParams);
    
    // Initialise the (remaining of the) context
    init (iScheduleInputFilename);
  }

  // ////////////////////////////////////////////////////////////////////
  AIRSCHED_Service::
  AIRSCHED_Service (const stdair::BasLogParams& iLogParams,
                    const stdair::Filename_T& iScheduleInputFilename,
                    const stdair::Filename_T& iODInputFilename) 
    : _airschedServiceContext (NULL) {
    
    // Initialise the service context
    initServiceContext ();
    
    // Initialise the STDAIR service handler
    initStdAirService (iLogParams);
    
    // Initialise the (remaining of the) context
    init (iScheduleInputFilename, iODInputFilename);
  }

  // ////////////////////////////////////////////////////////////////////
  AIRSCHED_Service::~AIRSCHED_Service () {
  }

  // //////////////////////////////////////////////////////////////////////
  void AIRSCHED_Service::initServiceContext () {
    // Initialise the service context
    AIRSCHED_ServiceContext& lAIRSCHED_ServiceContext = 
      FacAIRSCHEDServiceContext::instance().create ();
    _airschedServiceContext = &lAIRSCHED_ServiceContext;
  }

  // //////////////////////////////////////////////////////////////////////
  void AIRSCHED_Service::
  initStdAirService (const stdair::BasLogParams& iLogParams,
                     const stdair::BasDBParams& iDBParams) {

    // Retrieve the AirSched service context
    assert (_airschedServiceContext != NULL);
    AIRSCHED_ServiceContext& lAIRSCHED_ServiceContext =
      *_airschedServiceContext;
    
    // Initialise the STDAIR service handler
    // Note that the track on the object memory is kept thanks to the Boost
    // Smart Pointers component.
    stdair::STDAIR_ServicePtr_T lSTDAIR_Service_ptr = 
      boost::make_shared<stdair::STDAIR_Service> (iLogParams, iDBParams);

    // Store the STDAIR service object within the (AIRSCHED) service context
    lAIRSCHED_ServiceContext.setSTDAIR_Service (lSTDAIR_Service_ptr);
  }
  
  // //////////////////////////////////////////////////////////////////////
  void AIRSCHED_Service::
  initStdAirService (const stdair::BasLogParams& iLogParams) {

    // Retrieve the AirSched service context
    assert (_airschedServiceContext != NULL);
    AIRSCHED_ServiceContext& lAIRSCHED_ServiceContext =
      *_airschedServiceContext;
    
    // Initialise the STDAIR service handler
    // Note that the track on the object memory is kept thanks to the Boost
    // Smart Pointers component.
    stdair::STDAIR_ServicePtr_T lSTDAIR_Service_ptr = 
      boost::make_shared<stdair::STDAIR_Service> (iLogParams);

    // Store the STDAIR service object within the (AIRSCHED) service context
    lAIRSCHED_ServiceContext.setSTDAIR_Service (lSTDAIR_Service_ptr);
  }
  
  // ////////////////////////////////////////////////////////////////////
  void AIRSCHED_Service::
  init (const stdair::Filename_T& iScheduleInputFilename) {

    // Check that the file path given as input corresponds to an actual file
    const bool doesExistAndIsReadable =
      stdair::BasFileMgr::doesExistAndIsReadable (iScheduleInputFilename);

    if (doesExistAndIsReadable == false) {
      throw ScheduleInputFileNotFoundException
        ("The schedule file " + iScheduleInputFilename
         + " does not exist or can not be read");
    }

    // Retrieve the AirSched service context
    assert (_airschedServiceContext != NULL);
    AIRSCHED_ServiceContext& lAIRSCHED_ServiceContext =
      *_airschedServiceContext;

    // Retrieve the StdAir service context
    stdair::STDAIR_ServicePtr_T lSTDAIR_Service_ptr =
      lAIRSCHED_ServiceContext.getSTDAIR_Service();
    assert (lSTDAIR_Service_ptr != NULL);
    
    // Get the root of the BOM tree, on which all of the other BOM objects
    // will be attached
    stdair::BomRoot& lBomRoot = lSTDAIR_Service_ptr->getBomRoot();

    // Parse the schedule input file, and generate the Inventories
    stdair::BasChronometer lINVGeneration; lINVGeneration.start();
    ScheduleParser::generateInventories (iScheduleInputFilename, lBomRoot);
    STDAIR_LOG_DEBUG ("Inv generation time: " << lINVGeneration.elapsed());
    

    // Build the network from the schedule.
    SegmentPathGenerator::createSegmentPathNetwork (lBomRoot);
    
    // DEBUG
    // STDAIR_LOG_DEBUG ("Generated BomRoot:");
    // std::ostringstream oStream;
    // STDAIR_LOG_DEBUG (oStream.str());
  }
  
  // ////////////////////////////////////////////////////////////////////
  void AIRSCHED_Service::
  init (const stdair::Filename_T& iScheduleInputFilename,
        const stdair::Filename_T& iODInputFilename) {
    init (iScheduleInputFilename);

    // Check that the file path given as input corresponds to an actual file
    const bool doesExistAndIsReadable =
      stdair::BasFileMgr::doesExistAndIsReadable (iODInputFilename);

    if (doesExistAndIsReadable == false) {
      throw OnDInputFileNotFoundException ("The O&D file " + iODInputFilename
                                           + " does not exist or can not be read");
    }

    // Retrieve the AirSched service context
    assert (_airschedServiceContext != NULL);
    AIRSCHED_ServiceContext& lAIRSCHED_ServiceContext =
      *_airschedServiceContext;

    // Retrieve the StdAir service context
    stdair::STDAIR_ServicePtr_T lSTDAIR_Service_ptr =
      lAIRSCHED_ServiceContext.getSTDAIR_Service();
    assert (lSTDAIR_Service_ptr != NULL);
    
    // Get the root of the BOM tree, on which all of the other BOM objects
    // will be attached
    stdair::BomRoot& lBomRoot = lSTDAIR_Service_ptr->getBomRoot();

    // Parse the schedule input file, and generate the Inventories
    stdair::BasChronometer lOnDGeneration; lOnDGeneration.start();
    OnDParser::generateOnDPeriods (iODInputFilename, lBomRoot);
    STDAIR_LOG_DEBUG ("O&D generation time: " << lOnDGeneration.elapsed());
  }
  
  // ////////////////////////////////////////////////////////////////////
  void AIRSCHED_Service::simulate()  {

    if (_airschedServiceContext == NULL) {
      throw NonInitialisedServiceException();
    }
    assert (_airschedServiceContext != NULL);
    AIRSCHED_ServiceContext& lAIRSCHED_ServiceContext =
      *_airschedServiceContext;

    // Retrieve the StdAir service context
    stdair::STDAIR_ServicePtr_T lSTDAIR_Service_ptr =
      lAIRSCHED_ServiceContext.getSTDAIR_Service();
    assert (lSTDAIR_Service_ptr != NULL);
    
    // Create the root of the BOM tree, on which all of the other BOM objects
    // will be attached
    stdair::BomRoot& lBomRoot = lSTDAIR_Service_ptr->getBomRoot();

    // Call the underlying Use Case (command)
    stdair::BasChronometer lSimulateChronometer;
    lSimulateChronometer.start();
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
      throw NonInitialisedServiceException();
    }
    assert (_airschedServiceContext != NULL);
    AIRSCHED_ServiceContext& lAIRSCHED_ServiceContext = *_airschedServiceContext;

    // Retrieve the network.
    const stdair::STDAIR_ServicePtr_T lSTDAIR_Service_ptr =
      lAIRSCHED_ServiceContext.getSTDAIR_Service();
    assert (lSTDAIR_Service_ptr != NULL);
    
    const stdair::BomRoot& lBomRoot = lSTDAIR_Service_ptr->getBomRoot();
    
    SegmentPathProvider::buildSegmentPathList (ioTravelSolutionList,
					       lBomRoot, iBookingRequest);
  }

}
