// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// Boost
#include <boost/make_shared.hpp>
// SOCI
#if defined(SOCI_HEADERS_BURIED)
#include <soci/core/soci.h>
#else // SOCI_HEADERS_BURIED
#include <soci/soci.h>
#endif // SOCI_HEADERS_BURIED
// StdAir
#include <stdair/stdair_json.hpp>
#include <stdair/basic/BasChronometer.hpp>
#include <stdair/basic/JSonCommand.hpp>
#include <stdair/bom/BomManager.hpp>
#include <stdair/bom/BomRoot.hpp>
#include <stdair/bom/AirlineStruct.hpp>
#include <stdair/bom/BookingRequestStruct.hpp>
#include <stdair/bom/BreakPointStruct.hpp>
#include <stdair/bom/EventStruct.hpp>
#include <stdair/bom/BomJSONImport.hpp>
#include <stdair/command/DBManagerForAirlines.hpp>
#include <stdair/service/FacSupervisor.hpp>
#include <stdair/service/Logger.hpp>
#include <stdair/service/DBSessionManager.hpp>
#include <stdair/STDAIR_Service.hpp>
// SEvMgr
#include <sevmgr/SEVMGR_Service.hpp>
// Distribution
#include <simcrs/SIMCRS_Service.hpp>
// TraDemGen
#include <trademgen/TRADEMGEN_Service.hpp>
// TravelCCM
#include <travelccm/TRAVELCCM_Service.hpp>
// Dsim
#include <dsim/basic/BasConst_DSIM_Service.hpp>
#include <dsim/basic/SimulationMode.hpp>
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
                              const stdair::Date_T& iEndDate,
                              const stdair::RandomSeed_T& iRandomSeed)
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
 
    // Initalise the SEVMGR service.
    initSEVMGRService();

    // Initalise the TraDemGen service.
    initTRADEMGENService (iRandomSeed);

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
                              const stdair::Date_T& iEndDate,
                              const stdair::RandomSeed_T& iRandomSeed)
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
 
    // Initalise the SEVMGR service.
    initSEVMGRService();

    // Initalise the TraDemGen service.
    initTRADEMGENService (iRandomSeed);

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
                              const stdair::Date_T& iEndDate,
                              const stdair::RandomSeed_T& iRandomSeed)
    : _dsimServiceContext (NULL) {
    
    // Initialise the service context
    initServiceContext (iStartDate, iEndDate);
    
    // Store the STDAIR service object within the (AIRINV) service context
    // \note AirInv does not own the STDAIR service resources here.
    const bool doesNotOwnStdairService = false;
    addStdAirService (ioSTDAIR_Service_ptr, doesNotOwnStdairService); 

    // Initalise the SEVMGR service.
    initSEVMGRService();

    // Initalise the TraDemGen service.
    initTRADEMGENService (iRandomSeed);

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
  void DSIM_Service::initSEVMGRService() {

    // Retrieve the Dsim service context
    assert (_dsimServiceContext != NULL);
    DSIM_ServiceContext& lDSIM_ServiceContext = *_dsimServiceContext;
    
    // Retrieve the StdAir service context
    stdair::STDAIR_ServicePtr_T lSTDAIR_Service_ptr =
      lDSIM_ServiceContext.getSTDAIR_ServicePtr();
    
    /**
     * Initialise the SEVMGR service handler.
     *
     * \note The (Boost.)Smart Pointer keeps track of the references
     *       on the Service object, and deletes that object when it is
     *       no longer referenced (e.g., at the end of the process).
     */
    SEVMGR::SEVMGR_ServicePtr_T lSEVMGR_Service_ptr = 
      boost::make_shared<SEVMGR::SEVMGR_Service> (lSTDAIR_Service_ptr);
    
    // Store the SEVMGR service object within the (DSim) service context
    lDSIM_ServiceContext.setSEVMGR_Service (lSEVMGR_Service_ptr);
  }
  
  
  // ////////////////////////////////////////////////////////////////////
  void DSIM_Service::initSIMCRSService() {

    // Retrieve the Dsim service context
    assert (_dsimServiceContext != NULL);
    DSIM_ServiceContext& lDSIM_ServiceContext = *_dsimServiceContext;
    
    // Retrieve the StdAir service context
    stdair::STDAIR_ServicePtr_T lSTDAIR_Service_ptr =
      lDSIM_ServiceContext.getSTDAIR_ServicePtr();   

    // Retrieve the StdAir service context
    SEVMGR::SEVMGR_ServicePtr_T lSEVMGR_Service_ptr =
      lDSIM_ServiceContext.getSEVMGR_ServicePtr();

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
						  lSEVMGR_Service_ptr,
                                                  lCRSCode);
    
    // Store the SIMCRS service object within the (DSim) service context
    lDSIM_ServiceContext.setSIMCRS_Service (lSIMCRS_Service_ptr);
  }
  
  // ////////////////////////////////////////////////////////////////////
  void DSIM_Service::
  initTRADEMGENService (const stdair::RandomSeed_T& iRandomSeed) {

    // Retrieve the Dsim service context
    assert (_dsimServiceContext != NULL);
    DSIM_ServiceContext& lDSIM_ServiceContext = *_dsimServiceContext;
    
    // Retrieve the StdAir service context
    stdair::STDAIR_ServicePtr_T lSTDAIR_Service_ptr =
      lDSIM_ServiceContext.getSTDAIR_ServicePtr();  

    // Retrieve the StdAir service context
    SEVMGR::SEVMGR_ServicePtr_T lSEVMGR_Service_ptr =
      lDSIM_ServiceContext.getSEVMGR_ServicePtr();

    /**
     * Initialise the TraDemGen service handler.
     *
     * \note The (Boost.)Smart Pointer keeps track of the references
     *       on the Service object, and deletes that object when it is
     *       no longer referenced (e.g., at the end of the process).
     */
    TRADEMGEN::TRADEMGEN_ServicePtr_T lTRADEMGEN_Service_ptr = 
      boost::make_shared<TRADEMGEN::TRADEMGEN_Service> (lSTDAIR_Service_ptr,
							lSEVMGR_Service_ptr,
                                                        iRandomSeed);
    
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

  // //////////////////////////////////////////////////////////////////////
  void DSIM_Service::reinitServices() {

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

    /**
     * 1. Have StdAir, only when it is owned by the current component
     *    (here, DSim), delete all the BOM objects, leaving alive only
     *    the service objects (e.g., service contexts), log and
     *    database services.
     */
    if (doesOwnStdairService == true) {
      // Retrieve the StdAir service object from the (DSim) service context
      /*
      stdair::STDAIR_Service& lSTDAIR_Service =
        lDSIM_ServiceContext.getSTDAIR_Service();
      */

      //
      //lSTDAIR_Service.cleanBomLayer();
    }

    /**
     * Then, re-initialise the components.
     */

    /**
     * Let the distribution component (and sub-components) re-load its BOM tree.
    SIMCRS::SIMCRS_Service& lSIMCRS_Service =
      lDSIM_ServiceContext.getSIMCRS_Service();
    lSIMCRS_Service.reinitServices();
     */

    /**
     * Let the demand generation component re-load its BOM tree.
    TRADEMGEN::TRADEMGEN_Service& lTRADEMGEN_Service =
      lDSIM_ServiceContext.getTRADEMGEN_Service();
    lTRADEMGEN_Service.reinitServices();
     */
  }

  // ////////////////////////////////////////////////////////////////////
  void DSIM_Service::
  parseAndLoad (const stdair::Filename_T& iScheduleInputFilename,
                const stdair::Filename_T& iODInputFilename,
                const AIRRAC::YieldFilePath& iYieldInputFilepath,
                const SIMFQT::FareFilePath& iFareInputFilepath,
                const stdair::Filename_T& iDemandInputFilename) {

    // Retrieve the DSim service context
    assert (_dsimServiceContext != NULL);
    DSIM_ServiceContext& lDSIM_ServiceContext = *_dsimServiceContext;
    
    /**
     * Let the schedule, inventory and pricing managers parse their input files.
     */
    SIMCRS::SIMCRS_Service& lSIMCRS_Service =
      lDSIM_ServiceContext.getSIMCRS_Service();
    lSIMCRS_Service.parseAndLoad (iScheduleInputFilename, iODInputFilename,
                                  iYieldInputFilepath, iFareInputFilepath);

    /**
     * Let the demand generation component parse its input file.
     */
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

    // Retrieve the StdAir service object from the (DSim) service context
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
  bool DSIM_Service::sell (const std::string& iSegmentDateKey,
                             const stdair::ClassCode_T& iClassCode,
                             const stdair::PartySize_T& iPartySize) {

    // Retrieve the DSim service context
    if (_dsimServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The DSim service "
                                                    "has not been initialised");
    }
    assert (_dsimServiceContext != NULL);

    // Retrieve the SimCRS service object from the (DSim) service context
    DSIM_ServiceContext& lDSIM_ServiceContext = *_dsimServiceContext;
    SIMCRS::SIMCRS_Service& lSIMCRS_Service =
      lDSIM_ServiceContext.getSIMCRS_Service(); 

    // Delegate the BOM display to the dedicated service
    return lSIMCRS_Service.sell (iSegmentDateKey, iClassCode,
                                 iPartySize);
  }

  // ////////////////////////////////////////////////////////////////////
  std::string DSIM_Service::
  jsonHandler (const stdair::JSONString& iJSONString) {

    // Retrieve the DSim service context
    if (_dsimServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The DSim service "
                                                    "has not been initialised");
    }
    assert (_dsimServiceContext != NULL);
    DSIM_ServiceContext& lDSIM_ServiceContext = *_dsimServiceContext; 

    //
    // Extract from the JSON-ified string the command
    //
    stdair::JSonCommand::EN_JSonCommand lEN_JSonCommand;
    const bool hasCommandBeenRetrieved =
      stdair::BomJSONImport::jsonImportCommand (iJSONString,
                                                lEN_JSonCommand);
    
    if (hasCommandBeenRetrieved == false) {
      // Return an error JSON-ified string
      std::ostringstream oErrorStream;
      oErrorStream << "{\"error\": \"Wrong JSON-ified string: "
                   << "the command is not understood.\"}";
      return oErrorStream.str();
    }
    assert (hasCommandBeenRetrieved == true);

    //
    // Dispatch the command to the right JSon service handler
    // 
    switch (lEN_JSonCommand) {
    case stdair::JSonCommand::FLIGHT_DATE:
    case stdair::JSonCommand::LIST:{
      
      // Get a reference on the SIMCRS service handler
      SIMCRS::SIMCRS_Service& lSIMCRS_Service =
	lDSIM_ServiceContext.getSIMCRS_Service();

      return lSIMCRS_Service.jsonHandler (iJSONString);
    }
    case stdair::JSonCommand::EVENT_LIST:{ 

      // Get a reference on the TRADEMGEN service handler
      TRADEMGEN::TRADEMGEN_Service& lTRADEMGEN_Service =
	lDSIM_ServiceContext.getTRADEMGEN_Service(); 

      return lTRADEMGEN_Service.jsonHandler (iJSONString);
    } 
    case stdair::JSonCommand::BREAK_POINT:{   
      return jsonBreakPointHandler (iJSONString);
    } 
    case stdair::JSonCommand::RUN:{ 
      return jsonRunHandler (iJSONString);
    } 
    case stdair::JSonCommand::RESET:{   
      reset();
      // Return a JSON-ified string
      std::ostringstream oStream;
      oStream << "{\"done\": \"1\"}";
      return oStream.str();
    }
    default: {
      // Return an Error string
      std::ostringstream lErrorCmdMessage;
      const std::string& lCommandStr =
	stdair::JSonCommand::getLabel(lEN_JSonCommand);
      lErrorCmdMessage << "{\"error\": \"The command '" << lCommandStr
		       << "' is not handled by the DSim service.\"}";
      return lErrorCmdMessage.str();
      break;
    }
    }
    
    // Return an error JSON-ified string
    assert (false);
    std::string lJSONDump ("{\"error\": \"Wrong JSON-ified string\"}");
    return lJSONDump;
  }  

  // ////////////////////////////////////////////////////////////////////
  std::string DSIM_Service::
  jsonBreakPointHandler (const stdair::JSONString& iJSONString) {

    stdair::BreakPointList_T lBreakPointList;   

    const bool hasBreakPointListBeenRetrieved =
      stdair::BomJSONImport::jsonImportBreakPoints(iJSONString,
						   lBreakPointList);    

    if (hasBreakPointListBeenRetrieved == false) {
      // Return an error JSON-ified string
	std::ostringstream oErrorStream;
	oErrorStream << "{\"error\": \"Wrong JSON-ified string: "
		     << "the break point list is not understood.\"}";
	return oErrorStream.str();
    }
    assert (hasBreakPointListBeenRetrieved == true);
      
    initBreakPointEvents (lBreakPointList);	

    // Return a JSON-ified string
    std::ostringstream oStream;
    oStream << "{\"done\": \"" << lBreakPointList.size() << "\"}";
    return oStream.str();
  } 

  // ////////////////////////////////////////////////////////////////////
  std::string DSIM_Service::
  jsonRunHandler (const stdair::JSONString& iJSONString) { 

    // Retrieve the DSim service context
    if (_dsimServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The DSim service "
                                                    "has not been initialised");
    }
    assert (_dsimServiceContext != NULL);
    DSIM_ServiceContext& lDSIM_ServiceContext = *_dsimServiceContext; 

    //
    // TODO: STOP hardcoding !!!
    // Making a service to enable Json import of those values.
    //
    // Number of Run
    const NbOfRuns_T lNbOfRuns = 1;
    // Forecasting method.
    const stdair::ForecastingMethod lForecastingMethod ('M');
    // Demand generation method.
    const stdair::DemandGenerationMethod lDemandGenerationMethod ('S');
    // Partnership technique.
    const stdair::PartnershipTechnique lPartnershipTechnique('N');

    simulate (lNbOfRuns, lDemandGenerationMethod,
	      lForecastingMethod, lPartnershipTechnique);

    // Get a reference on the Simulation Status
    SimulationStatus& lSimulationStatus =
	lDSIM_ServiceContext.getSimulationStatus(); 
      
    // Return a JSON-ified string
    std::ostringstream oStream;
    oStream << "{\"done\": \""
            << lSimulationStatus.getCurrentDate() << "\"}";
    return oStream.str();
  }

  // ////////////////////////////////////////////////////////////////////
  void DSIM_Service::initBreakPointEvents(const stdair::BreakPointList_T& iBreakPointList) {
    // Retrieve the DSim service context
    if (_dsimServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The DSim service "
                                                    "has not been initialised");
    }
    assert (_dsimServiceContext != NULL);
    DSIM_ServiceContext& lDSIM_ServiceContext = *_dsimServiceContext;

    // Retrieve the StdAir service context
    SEVMGR::SEVMGR_ServicePtr_T lSEVMGR_Service_ptr =
      lDSIM_ServiceContext.getSEVMGR_ServicePtr();   

    // Update the status of cancellation events within the event queue. 
    const bool hasProgressStatus = 
      lSEVMGR_Service_ptr->hasProgressStatus(stdair::EventType::BRK_PT); 
    const stdair::Count_T lBreakPointNumber = iBreakPointList.size();
    if (hasProgressStatus == false) {   
      lSEVMGR_Service_ptr->addStatus (stdair::EventType::BRK_PT, lBreakPointNumber); 
    } else {   
      stdair::Count_T lCurrentBPNumber = 
	lSEVMGR_Service_ptr->getActualTotalNumberOfEventsToBeGenerated (stdair::EventType::BRK_PT);
      lCurrentBPNumber += lBreakPointNumber;
      lSEVMGR_Service_ptr->updateStatus (stdair::EventType::BRK_PT, lCurrentBPNumber);   
    } 

    for (stdair::BreakPointList_T::const_iterator itBPEvent = iBreakPointList.begin();
         itBPEvent != iBreakPointList.end(); ++itBPEvent) {
      const stdair::BreakPointStruct& lBPEvent = *itBPEvent;   
      stdair::BreakPointPtr_T lBPEventPtr =
        boost::make_shared<stdair::BreakPointStruct> (lBPEvent);
      // Create an event structure
      stdair::EventStruct lEventStruct (stdair::EventType::BRK_PT,
					lBPEventPtr);
      lSEVMGR_Service_ptr->addEvent (lEventStruct);
    }

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

    // Get a reference on the Simulation Status
    SimulationStatus& lSimulationStatus =
      lDSIM_ServiceContext.getSimulationStatus();

    // Retrieve the simulation parameters
    const stdair::Date_T& lStartDate = lSimulationStatus.getStartDate();
    const stdair::Date_T& lEndDate = lSimulationStatus.getEndDate();

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

    // Retrieve the StdAir service object from the (DSim) service context
    DSIM_ServiceContext& lDSIM_ServiceContext = *_dsimServiceContext;
    stdair::STDAIR_Service& lSTDAIR_Service =
      lDSIM_ServiceContext.getSTDAIR_Service();

    // Delegate the BOM building to the dedicated service
    return lSTDAIR_Service.csvDisplay();
  } 

  // //////////////////////////////////////////////////////////////////////
  std::string DSIM_Service::listEvents () const {  

    // Retrieve the DSim service context
    if (_dsimServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The DSim service "
                                                    "has not been initialised");
    }
    assert (_dsimServiceContext != NULL);

    // Retrieve the TraDemGen service object from the (DSim) service context
    DSIM_ServiceContext& lDSIM_ServiceContext = *_dsimServiceContext;  

    // Get a reference on the TRADEMGEN service handler
    TRADEMGEN::TRADEMGEN_Service& lTRADEMGEN_Service =
      lDSIM_ServiceContext.getTRADEMGEN_Service(); 

    // Delegate the BOM building to the dedicated service
    return lTRADEMGEN_Service.list ();

  }

  // ////////////////////////////////////////////////////////////////////
  std::string DSIM_Service::
  list (const stdair::AirlineCode_T& iAirlineCode,
        const stdair::FlightNumber_T& iFlightNumber) const {

    // Retrieve the DSim service context
    if (_dsimServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The DSim service "
                                                    "has not been initialised");
    }
    assert (_dsimServiceContext != NULL);

    // Retrieve the SimCRS service object from the (DSim) service context
    DSIM_ServiceContext& lDSIM_ServiceContext = *_dsimServiceContext;
    SIMCRS::SIMCRS_Service& lSIMCRS_Service =
      lDSIM_ServiceContext.getSIMCRS_Service(); 

    // Delegate the BOM display to the dedicated service
    return lSIMCRS_Service.list (iAirlineCode, iFlightNumber);
  }

  // ////////////////////////////////////////////////////////////////////
  std::string DSIM_Service::
  csvDisplay (const stdair::AirlineCode_T& iAirlineCode,
              const stdair::FlightNumber_T& iFlightNumber,
              const stdair::Date_T& iDepartureDate) const {

    // Retrieve the DSim service context
    if (_dsimServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The DSim service "
                                                    "has not been initialised");
    }
    assert (_dsimServiceContext != NULL);

    // Retrieve the SimCRS service object from the (DSim) service context
    DSIM_ServiceContext& lDSIM_ServiceContext = *_dsimServiceContext;
    SIMCRS::SIMCRS_Service& lSIMCRS_Service =
      lDSIM_ServiceContext.getSIMCRS_Service(); 

    // Delegate the BOM display to the dedicated service
    return lSIMCRS_Service.csvDisplay (iAirlineCode, iFlightNumber,
                                       iDepartureDate);
  }

  // ////////////////////////////////////////////////////////////////////
  void DSIM_Service::reset() const {  

    // Retrieve the DSim service context
    if (_dsimServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The DSim service "
                                                    "has not been initialised");
    }
    assert (_dsimServiceContext != NULL);

    // Retrieve the StdAir service object from the (DSim) service context
    DSIM_ServiceContext& lDSIM_ServiceContext = *_dsimServiceContext; 

    // Get a reference on the TRADEMGEN service handler
    TRADEMGEN::TRADEMGEN_Service& lTRADEMGEN_Service =
      lDSIM_ServiceContext.getTRADEMGEN_Service(); 
    // Reset the TRADEMGEN service 
    lTRADEMGEN_Service.reset();

    // TODO
    // Get a reference on the SIMCRS service handler
    /**SIMCRS::SIMCRS_Service& lSIMCRS_Service =
       lDSIM_ServiceContext.getSIMCRS_Service();
       // Reset the SIMCRS service 
       lSIMCRS_Service.reset();*/ 

    // TODO
    // Get a reference on the TRAVELCCM service handler
    /**TRAVELCCM::TRAVELCCM_Service& lTRAVELCCM_Service =
       lDSIM_ServiceContext.getTRAVELCCM_Service();  
       // Reset the TRAVELCCM service 
       lTRAVELCCM_Service.reset();*/

    // Reset the simulation status object  
    // Get a reference on the Simulation Status
    SimulationStatus& lSimulationStatus =
      lDSIM_ServiceContext.getSimulationStatus();
    lSimulationStatus.reset();
    
  }  

  // //////////////////////////////////////////////////////////////////////
  std::string DSIM_Service::simulationStatusDisplay() const {

    // Retrieve the DSim service context
    if (_dsimServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The DSim service "
                                                    "has not been initialised");
    }
    assert (_dsimServiceContext != NULL);

    // Retrieve the StdAir service object from the (DSim) service context
    DSIM_ServiceContext& lDSIM_ServiceContext = *_dsimServiceContext;

    // Get a reference on the Simulation Status
    const SimulationStatus& lSimulationStatus =
      lDSIM_ServiceContext.getSimulationStatus();

    return lSimulationStatus.describe();
  }
  
  // //////////////////////////////////////////////////////////////////////
  void DSIM_Service::
  simulate (const NbOfRuns_T& iNbOfRuns,
            const stdair::DemandGenerationMethod& iDemandGenerationMethod,
            const stdair::ForecastingMethod& iForecastingMethod,
            const stdair::PartnershipTechnique& iPartnershipTechnique) {

    // Retrieve the DSim service context
    if (_dsimServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The DSim service has not "
                                                    "been initialised");
    }
    assert (_dsimServiceContext != NULL);
    DSIM_ServiceContext& lDSIM_ServiceContext= *_dsimServiceContext;

    // Get a reference on the Simulation Status
    SimulationStatus& lSimulationStatus =
      lDSIM_ServiceContext.getSimulationStatus();  

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

      if (idx > 0) {
	// Reset the service (including the event queue) for the next run
	reset();
      }

      // Delegate the booking to the dedicated command
      stdair::BasChronometer lSimulationChronometer;
      lSimulationChronometer.start();
      Simulator::simulate (lSIMCRS_Service, lTRADEMGEN_Service,
                           lTRAVELCCM_Service, lSTDAIR_Service,
                           lSimulationStatus, iDemandGenerationMethod,
                           iForecastingMethod, iPartnershipTechnique);
      const double lSimulationMeasure = lSimulationChronometer.elapsed();

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
