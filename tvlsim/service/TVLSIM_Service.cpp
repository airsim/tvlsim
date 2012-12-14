
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
#include <stdair/stdair_file.hpp>
#include <stdair/basic/BasConst_General.hpp>
#include <stdair/basic/BasChronometer.hpp>
#include <stdair/basic/JSonCommand.hpp>
#include <stdair/bom/BomManager.hpp>
#include <stdair/bom/BomRoot.hpp>
#include <stdair/bom/AirlineStruct.hpp>
#include <stdair/bom/BookingRequestStruct.hpp>
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
#include <tvlsim/basic/BasConst_TVLSIM_Service.hpp>
#include <tvlsim/basic/SimulationMode.hpp>
#include <tvlsim/bom/BomJSONExport.hpp>
#include <tvlsim/bom/ConfigImport.hpp>
#include <tvlsim/bom/ConfigExport.hpp>
#include <tvlsim/factory/FacDsimServiceContext.hpp>
#include <tvlsim/command/Simulator.hpp>
#include <tvlsim/service/TVLSIM_ServiceContext.hpp>
#include <tvlsim/TVLSIM_Service.hpp>

namespace TVLSIM {

  // //////////////////////////////////////////////////////////////////////
  TVLSIM_Service::TVLSIM_Service () : _tvlsimServiceContext (NULL) {
    assert (false);
  }

  // //////////////////////////////////////////////////////////////////////
  TVLSIM_Service::TVLSIM_Service (const TVLSIM_Service& iService) {
    assert (false);
  }

  // //////////////////////////////////////////////////////////////////////
  TVLSIM_Service::TVLSIM_Service 
  (const stdair::BasLogParams& iLogParams,
   const stdair::Date_T& iStartDate,
   const stdair::Date_T& iEndDate,
   const stdair::RandomSeed_T& iRandomSeed,
   const stdair::DemandGenerationMethod& iDemandGenerationMethod,
   const NbOfRuns_T& iNbOfRuns)
    : _tvlsimServiceContext (NULL) {
    
    // Initialise the StdAir service handler
    stdair::STDAIR_ServicePtr_T lSTDAIR_Service_ptr =
      initStdAirService (iLogParams);
    
    // Initialise the service context
    initServiceContext ();
    
    // Add the StdAir service context to the TvlSim service context
    // \note TvlSim owns the StdAir service resources here.
    const bool ownStdairService = true;
    addStdAirService (lSTDAIR_Service_ptr, ownStdairService); 
   
    // Init Config
    initConfig (iStartDate, iEndDate, iRandomSeed, 
		iDemandGenerationMethod, iNbOfRuns);
 
    // Initalise the SEVMGR service.
    initSEVMGRService();

    // Initalise the TraDemGen service.
    initTRADEMGENService ();

    // Initalise the TravelCCM service.
    initTRAVELCCMService();    

    // Initalise the SimCRS service, itself initialising AirInv, Airsched
    // and SimFQT.
    initSIMCRSService();

    // Initialise the (remaining of the) context
    initDsimService();
  }

  // //////////////////////////////////////////////////////////////////////
  TVLSIM_Service::TVLSIM_Service 
  (const stdair::BasLogParams& iLogParams,
   const stdair::BasDBParams& iDBParams,
   const stdair::Date_T& iStartDate,
   const stdair::Date_T& iEndDate,
   const stdair::RandomSeed_T& iRandomSeed,
   const stdair::DemandGenerationMethod& iDemandGenerationMethod,
   const NbOfRuns_T& iNbOfRuns)
    : _tvlsimServiceContext (NULL) {
    
    // Initialise the StdAir service handler
    stdair::STDAIR_ServicePtr_T lSTDAIR_Service_ptr =
      initStdAirService (iLogParams, iDBParams);

    // Initialise the service context
    initServiceContext ();
    
    // Add the StdAir service context to the TvlSim service context
    // \note TvlSim owns the StdAir service resources here.
    const bool ownStdairService = true;
    addStdAirService (lSTDAIR_Service_ptr, ownStdairService); 

    // Init Config
    initConfig (iStartDate, iEndDate, iRandomSeed, 
		iDemandGenerationMethod, iNbOfRuns);

    // Initalise the SEVMGR service.
    initSEVMGRService();

    // Initalise the TraDemGen service.
    initTRADEMGENService ();

    // Initalise the TravelCCM service.
    initTRAVELCCMService();    

    // Initalise the SimCRS service, itself initialising AirInv, Airsched
    // and SimFQT.
    initSIMCRSService();

    // Initialise the (remaining of the) context
    initDsimService();
  }

  // //////////////////////////////////////////////////////////////////////
  TVLSIM_Service::TVLSIM_Service 
  (stdair::STDAIR_ServicePtr_T ioSTDAIR_Service_ptr,
   const stdair::Date_T& iStartDate,
   const stdair::Date_T& iEndDate,
   const stdair::RandomSeed_T& iRandomSeed,
   const stdair::DemandGenerationMethod& iDemandGenerationMethod,
   const NbOfRuns_T& iNbOfRuns)
    : _tvlsimServiceContext (NULL) {
    
    // Initialise the service context
    initServiceContext ();
    
    // Store the STDAIR service object within the (AIRINV) service context
    // \note AirInv does not own the STDAIR service resources here.
    const bool doesNotOwnStdairService = false;
    addStdAirService (ioSTDAIR_Service_ptr, doesNotOwnStdairService);     

    // Init Config
    initConfig (iStartDate, iEndDate, iRandomSeed, 
		iDemandGenerationMethod, iNbOfRuns);

    // Initalise the SEVMGR service.
    initSEVMGRService();

    // Initalise the TraDemGen service. 
    initTRADEMGENService ();

    // Initalise the TravelCCM service.
    initTRAVELCCMService();    

    // Initalise the SimCRS service, itself initialising AirInv, Airsched
    // and SimFQT.
    initSIMCRSService();

    // Initialise the (remaining of the) context
    initDsimService();
  }

  // //////////////////////////////////////////////////////////////////////
  TVLSIM_Service::~TVLSIM_Service() {
    // Delete/Clean all the objects from memory
    finalise();
  }

  // //////////////////////////////////////////////////////////////////////
  void TVLSIM_Service::finalise() {
    assert (_tvlsimServiceContext != NULL);
    // Reset the (Boost.)Smart pointer pointing on the STDAIR_Service object.
    _tvlsimServiceContext->reset();
  }

  // ////////////////////////////////////////////////////////////////////
  void TVLSIM_Service::initServiceContext () {

    // Initialise the context
    TVLSIM_ServiceContext& lTVLSIM_ServiceContext =
      FacDsimServiceContext::instance().create ();
    _tvlsimServiceContext = &lTVLSIM_ServiceContext;
  }

  // ////////////////////////////////////////////////////////////////////
  void TVLSIM_Service::
  addStdAirService (stdair::STDAIR_ServicePtr_T ioSTDAIR_Service_ptr,
                    const bool iOwnStdairService) {

    // Retrieve the Dsim service context
    assert (_tvlsimServiceContext != NULL);
    TVLSIM_ServiceContext& lTVLSIM_ServiceContext = *_tvlsimServiceContext;
    
    // Store the StdAir service object within the (TvlSim) service context
    lTVLSIM_ServiceContext.setSTDAIR_Service (ioSTDAIR_Service_ptr,
                                            iOwnStdairService);
  }
  
  // //////////////////////////////////////////////////////////////////////
  stdair::STDAIR_ServicePtr_T TVLSIM_Service::
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
  stdair::STDAIR_ServicePtr_T TVLSIM_Service::
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
  void TVLSIM_Service::initSEVMGRService() {

    // Retrieve the Dsim service context
    assert (_tvlsimServiceContext != NULL);
    TVLSIM_ServiceContext& lTVLSIM_ServiceContext = *_tvlsimServiceContext;
    
    // Retrieve the StdAir service context
    stdair::STDAIR_ServicePtr_T lSTDAIR_Service_ptr =
      lTVLSIM_ServiceContext.getSTDAIR_ServicePtr();
    
    /**
     * Initialise the SEVMGR service handler.
     *
     * \note The (Boost.)Smart Pointer keeps track of the references
     *       on the Service object, and deletes that object when it is
     *       no longer referenced (e.g., at the end of the process).
     */
    SEVMGR::SEVMGR_ServicePtr_T lSEVMGR_Service_ptr = 
      boost::make_shared<SEVMGR::SEVMGR_Service> (lSTDAIR_Service_ptr);
    
    // Store the SEVMGR service object within the (TvlSim) service context
    lTVLSIM_ServiceContext.setSEVMGR_Service (lSEVMGR_Service_ptr);
  }
  
  
  // ////////////////////////////////////////////////////////////////////
  void TVLSIM_Service::initSIMCRSService() {

    // Retrieve the Dsim service context
    assert (_tvlsimServiceContext != NULL);
    TVLSIM_ServiceContext& lTVLSIM_ServiceContext = *_tvlsimServiceContext;
    
    // Retrieve the StdAir service context
    stdair::STDAIR_ServicePtr_T lSTDAIR_Service_ptr =
      lTVLSIM_ServiceContext.getSTDAIR_ServicePtr();   

    // Retrieve the StdAir service context
    SEVMGR::SEVMGR_ServicePtr_T lSEVMGR_Service_ptr =
      lTVLSIM_ServiceContext.getSEVMGR_ServicePtr();

    // Look for the CRS code
    SIMCRS::CRSCode_T lCRSCode ("1S");  
    lSTDAIR_Service_ptr->exportConfigValue<SIMCRS::CRSCode_T> (lCRSCode, 
							       "CRS.code");
    
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
    
    // Store the SIMCRS service object within the (TvlSim) service context
    lTVLSIM_ServiceContext.setSIMCRS_Service (lSIMCRS_Service_ptr);
  }
  
  // ////////////////////////////////////////////////////////////////////
  void TVLSIM_Service::initTRADEMGENService () {

    // Retrieve the Dsim service context
    assert (_tvlsimServiceContext != NULL);
    TVLSIM_ServiceContext& lTVLSIM_ServiceContext = *_tvlsimServiceContext;
    
    // Retrieve the StdAir service context
    stdair::STDAIR_ServicePtr_T lSTDAIR_Service_ptr =
      lTVLSIM_ServiceContext.getSTDAIR_ServicePtr();  

    // Retrieve the StdAir service context
    SEVMGR::SEVMGR_ServicePtr_T lSEVMGR_Service_ptr =
      lTVLSIM_ServiceContext.getSEVMGR_ServicePtr();

    // Look for the random seed in the configuration
    stdair::RandomSeed_T lRandomSeed (stdair::DEFAULT_RANDOM_SEED);
    const bool hasSeedBeenRetrieved = 
      lSTDAIR_Service_ptr->exportConfigValue<stdair::RandomSeed_T> (lRandomSeed, 
								    "random.seed");
    assert (hasSeedBeenRetrieved == true);

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
                                                        lRandomSeed);
    
    // Store the TRADEMGEN service object within the (TvlSim) service context
    lTVLSIM_ServiceContext.setTRADEMGEN_Service (lTRADEMGEN_Service_ptr);
  }
  
  // ////////////////////////////////////////////////////////////////////
  void TVLSIM_Service::initTRAVELCCMService() {

    // Retrieve the Dsim service context
    assert (_tvlsimServiceContext != NULL);
    TVLSIM_ServiceContext& lTVLSIM_ServiceContext = *_tvlsimServiceContext;
    
    // Retrieve the StdAir service context
    stdair::STDAIR_ServicePtr_T lSTDAIR_Service_ptr =
      lTVLSIM_ServiceContext.getSTDAIR_ServicePtr();

    /**
     * Initialise the TravelCCM service handler.
     *
     * \note The (Boost.)Smart Pointer keeps track of the references
     *       on the Service object, and deletes that object when it is
     *       no longer referenced (e.g., at the end of the process).
     */
    TRAVELCCM::TRAVELCCM_ServicePtr_T lTRAVELCCM_Service_ptr = 
      boost::make_shared<TRAVELCCM::TRAVELCCM_Service> (lSTDAIR_Service_ptr);
    
    // Store the TRAVELCCM service object within the (TvlSim) service context
    lTVLSIM_ServiceContext.setTRAVELCCM_Service (lTRAVELCCM_Service_ptr);
  }
  
  // //////////////////////////////////////////////////////////////////////
  void TVLSIM_Service::initDsimService() {
    // Do nothing at this stage. A sample BOM tree may be built by
    // calling the buildSampleBom() method
  } 

  // //////////////////////////////////////////////////////////////////////
  void TVLSIM_Service::
  initConfig (const stdair::Date_T& iStartDate,
	      const stdair::Date_T& iEndDate,
	      const stdair::RandomSeed_T& iRandomSeed,
	      const stdair::DemandGenerationMethod& iDemandGenerationMethod,
	      const NbOfRuns_T& iNbOfRuns) {

    // Retrieve the TvlSim service context
    if (_tvlsimServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The TvlSim service "
                                                    "has not been initialised");
    }
    assert (_tvlsimServiceContext != NULL);
    TVLSIM_ServiceContext& lTVLSIM_ServiceContext = *_tvlsimServiceContext;  

    // Retrieve the StdAir service object from the (TVLSIM) service context
    stdair::STDAIR_Service& lSTDAIR_Service =
      lTVLSIM_ServiceContext.getSTDAIR_Service();

    // Load the input config parameters and try to retrieve the input config
    // files.
    ConfigImport::importINI (lSTDAIR_Service, iStartDate, iEndDate,
                             iRandomSeed, iDemandGenerationMethod, iNbOfRuns);
    
    // Retrieve the Simulation Status
    SimulationStatus& lSimulationStatus =
	lTVLSIM_ServiceContext.getSimulationStatus();

    // Update the simulation status with the correct start date, end date and
    // number of runs
    ConfigExport::updateSimulationStatus (lSTDAIR_Service, lSimulationStatus,
                                          iStartDate, iEndDate, iNbOfRuns);
    
  }

  // //////////////////////////////////////////////////////////////////////
  void TVLSIM_Service::reinitServices() {

    // Retrieve the TvlSim service context
    if (_tvlsimServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The TvlSim service "
                                                    "has not been initialised");
    }
    assert (_tvlsimServiceContext != NULL);

    // Retrieve the TvlSim service context and whether it owns the Stdair
    // service
    TVLSIM_ServiceContext& lTVLSIM_ServiceContext = *_tvlsimServiceContext;
    const bool doesOwnStdairService =
      lTVLSIM_ServiceContext.getOwnStdairServiceFlag();

    /**
     * 1. Have StdAir, only when it is owned by the current component
     *    (here, TvlSim), delete all the BOM objects, leaving alive only
     *    the service objects (e.g., service contexts), log and
     *    database services.
     */
    if (doesOwnStdairService == true) {
      // Retrieve the StdAir service object from the (TvlSim) service context
      /*
      stdair::STDAIR_Service& lSTDAIR_Service =
        lTVLSIM_ServiceContext.getSTDAIR_Service();
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
      lTVLSIM_ServiceContext.getSIMCRS_Service();
    lSIMCRS_Service.reinitServices();
     */

    /**
     * Let the demand generation component re-load its BOM tree.
    TRADEMGEN::TRADEMGEN_Service& lTRADEMGEN_Service =
      lTVLSIM_ServiceContext.getTRADEMGEN_Service();
    lTRADEMGEN_Service.reinitServices();
     */
  } 

  // ////////////////////////////////////////////////////////////////////
  void TVLSIM_Service::
  setInputFiles (const stdair::ScheduleFilePath& iScheduleInputFilename,
		 const stdair::ODFilePath& iODInputFilename,
		 const stdair::FRAT5FilePath& iFRAT5InputFilename,
		 const stdair::FFDisutilityFilePath& iFFDisutilityInputFilename,
		 const AIRRAC::YieldFilePath& iYieldInputFilepath,
		 const SIMFQT::FareFilePath& iFareInputFilepath,
		 const TRADEMGEN::DemandFilePath& iDemandFilepath) {   

    // Retrieve the TvlSim service context
    if (_tvlsimServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The TvlSim service "
                                                    "has not been initialised");
    }
    assert (_tvlsimServiceContext != NULL);

    // Retrieve the TvlSim service context and whether it owns the Stdair
    // service
    TVLSIM_ServiceContext& lTVLSIM_ServiceContext = *_tvlsimServiceContext;

    // Retrieve the StdAir service object from the (TVLSIM) service context
    stdair::STDAIR_Service& lSTDAIR_Service =
      lTVLSIM_ServiceContext.getSTDAIR_Service();

    // Load the file names into the configuration holder
    ConfigImport::importFiles (lSTDAIR_Service, iScheduleInputFilename,
                               iODInputFilename, iFRAT5InputFilename,
                               iFFDisutilityInputFilename, iYieldInputFilepath,
                               iFareInputFilepath, iDemandFilepath);

  } 

  // ////////////////////////////////////////////////////////////////////
  void TVLSIM_Service::
  parseAndLoad () {   

    // Retrieve the TvlSim service context
    if (_tvlsimServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The TvlSim service "
                                                    "has not been initialised");
    }
    assert (_tvlsimServiceContext != NULL);

    // Retrieve the TvlSim service context and whether it owns the Stdair
    // service
    TVLSIM_ServiceContext& lTVLSIM_ServiceContext = *_tvlsimServiceContext;
    const bool doesOwnStdairService =
      lTVLSIM_ServiceContext.getOwnStdairServiceFlag();

    // Retrieve the StdAir service object from the (TVLSIM) service context
    stdair::STDAIR_Service& lSTDAIR_Service =
      lTVLSIM_ServiceContext.getSTDAIR_Service();

    // Look for the input files in the configuration holder
    stdair::Filename_T lFilename (""); 
    const bool hasScheduleFileBeenRetrieved = 
      lSTDAIR_Service.exportConfigValue<stdair::Filename_T> (lFilename, 
							     "input.schedule"); 
    assert (hasScheduleFileBeenRetrieved == true);     
    const stdair::ScheduleFilePath lScheduleInputFilename (lFilename);  
    const bool hasODFileBeenRetrieved = 
      lSTDAIR_Service.exportConfigValue<stdair::Filename_T> (lFilename, 
							     "input.ond");  
     assert (hasODFileBeenRetrieved == true);      
    const stdair::ODFilePath lODInputFilename (lFilename);  
    const bool hasFrat5FileBeenRetrieved = 
      lSTDAIR_Service.exportConfigValue<stdair::Filename_T> (lFilename, 
							     "input.frat5");  
    assert (hasFrat5FileBeenRetrieved == true);  
    const stdair::FRAT5FilePath lFRAT5InputFilename (lFilename);  
    const bool hasFFdisutilityFileBeenRetrieved = 
      lSTDAIR_Service.exportConfigValue<stdair::Filename_T> (lFilename, 
							     "input.ffdisutility");  
    assert (hasFFdisutilityFileBeenRetrieved == true);      
    const stdair::FFDisutilityFilePath lFFDisutilityInputFilename (lFilename);     
    const bool hasYieldFileBeenRetrieved = 
      lSTDAIR_Service.exportConfigValue<stdair::Filename_T> (lFilename, 
							     "input.yield");  
    assert (hasYieldFileBeenRetrieved == true);     
    const AIRRAC::YieldFilePath lYieldInputFilepath (lFilename);     
    const bool hasFareFileBeenRetrieved = 
      lSTDAIR_Service.exportConfigValue<stdair::Filename_T> (lFilename, 
							     "input.fare");  
    assert (hasFareFileBeenRetrieved == true);     
    const SIMFQT::FareFilePath lFareInputFilepath (lFilename);     
    const bool hasDemandFileBeenRetrieved = 
      lSTDAIR_Service.exportConfigValue<stdair::Filename_T> (lFilename, 
							     "input.demand");  
    assert (hasDemandFileBeenRetrieved == true);     
    const TRADEMGEN::DemandFilePath lDemandFilepath (lFilename);
      
    /**
     * 1. Delegate the complementary building of objects and links by the
     *    appropriate levels/components
     */ 
    /**
     * Let the schedule, inventory and pricing managers parse their input files.
     */
    SIMCRS::SIMCRS_Service& lSIMCRS_Service =
      lTVLSIM_ServiceContext.getSIMCRS_Service();
    lSIMCRS_Service.parseAndLoad (lScheduleInputFilename, lODInputFilename,
                                  lFRAT5InputFilename,
                                  lFFDisutilityInputFilename,
                                  lYieldInputFilepath, lFareInputFilepath);

    /**
     * Let the demand generation component parse its input file.
     */
    TRADEMGEN::TRADEMGEN_Service& lTRADEMGEN_Service =
      lTVLSIM_ServiceContext.getTRADEMGEN_Service(); 
    lTRADEMGEN_Service.parseAndLoad (lDemandFilepath);  

    /**
     * 2. Build the complementary objects/links for the current component (here,
     *    TravelCCM)
     */ 
    // Retrieve the persistent BOM root object.
    stdair::BomRoot& lPersistentBomRoot = 
      lSTDAIR_Service.getPersistentBomRoot();
    buildComplementaryLinks (lPersistentBomRoot);  

    /**
     * 3. Have TVLSIM clone the whole persistent BOM tree, only when the StdAir
     *    service is owned by the current component (TvlSim here).
     */
    if (doesOwnStdairService == true) {
      //
      clonePersistentBom ();
    }

    /**
     *  Update the Simulation Status
     */
    SimulationStatus& lSimulationStatus =
	lTVLSIM_ServiceContext.getSimulationStatus(); 
    Simulator::updateStatus(lTRADEMGEN_Service, stdair::EventType::BKG_REQ,
			    lSimulationStatus);
  }
  
  // ////////////////////////////////////////////////////////////////////
  void TVLSIM_Service::buildSampleBom() {

    // Retrieve the TvlSim service context
    if (_tvlsimServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The TvlSim service "
                                                    "has not been initialised");
    }
    assert (_tvlsimServiceContext != NULL);

    // Retrieve the TvlSim service context and whether it owns the Stdair
    // service
    TVLSIM_ServiceContext& lTVLSIM_ServiceContext = *_tvlsimServiceContext;
    const bool doesOwnStdairService =
      lTVLSIM_ServiceContext.getOwnStdairServiceFlag();

    // Retrieve the StdAir service object from the (TvlSim) service context
    stdair::STDAIR_Service& lSTDAIR_Service =
      lTVLSIM_ServiceContext.getSTDAIR_Service();

    // Retrieve the persistent BOM root object.
    stdair::BomRoot& lPersistentBomRoot = 
      lSTDAIR_Service.getPersistentBomRoot();

    /**
     * 1. Have StdAir build the whole BOM tree, only when the StdAir service is
     *    owned by the current component (TvlSim here).
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
      lTVLSIM_ServiceContext.getSIMCRS_Service();
    lSIMCRS_Service.buildSampleBom();

    /**
     * Let the demand manager (i.e., the TraDemGen component) build the
     * demand generators/streams.
     */
    TRADEMGEN::TRADEMGEN_Service& lTRADEMGEN_Service =
      lTVLSIM_ServiceContext.getTRADEMGEN_Service();
    lTRADEMGEN_Service.buildSampleBom();    

    // Update the Simulation Status
    SimulationStatus& lSimulationStatus =
	lTVLSIM_ServiceContext.getSimulationStatus(); 
    Simulator::updateStatus(lTRADEMGEN_Service, stdair::EventType::BKG_REQ,
			    lSimulationStatus);

    /**
     * Let the customer choice manager (i.e., the TravelCCM component) build the
     * customer choice sets.
     */
    TRAVELCCM::TRAVELCCM_Service& lTRAVELCCM_Service =
      lTVLSIM_ServiceContext.getTRAVELCCM_Service();
    lTRAVELCCM_Service.buildSampleBom();

    /**
     * 3. Build the complementary objects/links for the current component (here,
     *    TvlSim).
     */   
    buildComplementaryLinks (lPersistentBomRoot);    

    /**
     * 4. Have TVLSIM clone the whole persistent BOM tree, only when the StdAir
     *    service is owned by the current component (TvlSim here).
     */
    if (doesOwnStdairService == true) {
      //
      clonePersistentBom ();
    }
  } 

  // ////////////////////////////////////////////////////////////////////
  void TVLSIM_Service::clonePersistentBom () {  

    // Retrieve the TvlSim service context
    if (_tvlsimServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The TvlSim service "
                                                    "has not been initialised");
    }
    assert (_tvlsimServiceContext != NULL);

    // Retrieve the TvlSim service context and whether it owns the Stdair
    // service
    TVLSIM_ServiceContext& lTVLSIM_ServiceContext = *_tvlsimServiceContext;
    const bool doesOwnStdairService =
      lTVLSIM_ServiceContext.getOwnStdairServiceFlag();

    // Retrieve the StdAir service object from the (TvlSim) service context
    stdair::STDAIR_Service& lSTDAIR_Service =
      lTVLSIM_ServiceContext.getSTDAIR_Service(); 

    /**
     * 1. Have StdAir clone the whole persistent BOM tree, only when the StdAir
     *    service is owned by the current component (TVLSIM here)
     */
    if (doesOwnStdairService == true) {
      //
      lSTDAIR_Service.clonePersistentBom ();
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
      lTVLSIM_ServiceContext.getSIMCRS_Service();
    lSIMCRS_Service.clonePersistentBom ();

    /**
     * Let the demand manager (i.e., the TraDemGen component) build the
     * demand generators/streams.
     */
    TRADEMGEN::TRADEMGEN_Service& lTRADEMGEN_Service =
      lTVLSIM_ServiceContext.getTRADEMGEN_Service();
    lTRADEMGEN_Service.clonePersistentBom ();

   /**
    * 3. Build the complementary objects/links for the current component (here,
    *    TvlSim).
    */  
    stdair::BomRoot& lBomRoot = lSTDAIR_Service.getBomRoot();   
    buildComplementaryLinks (lBomRoot);    

  } 

  // ////////////////////////////////////////////////////////////////////
  void TVLSIM_Service::buildComplementaryLinks (stdair::BomRoot& ioBomRoot) {
    // Currently, no more things to do by TVLSIM at that stage.
  }

  // //////////////////////////////////////////////////////////////////////
  void TVLSIM_Service::
  buildSampleTravelSolutions(stdair::TravelSolutionList_T& ioTravelSolutionList){

    // Retrieve the TvlSim service context
    if (_tvlsimServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The TvlSim service "
                                                    "has not been initialised");
    }
    assert (_tvlsimServiceContext != NULL);

    // Retrieve the StdAir service object from the (AirSched) service context
    TVLSIM_ServiceContext& lTVLSIM_ServiceContext = *_tvlsimServiceContext;
    stdair::STDAIR_Service& lSTDAIR_Service =
      lTVLSIM_ServiceContext.getSTDAIR_Service();

    // Delegate the BOM building to the dedicated service
    lSTDAIR_Service.buildSampleTravelSolutions (ioTravelSolutionList);
  }

  // //////////////////////////////////////////////////////////////////////
  stdair::BookingRequestStruct TVLSIM_Service::
  buildSampleBookingRequest (const bool isForCRS) {

    // Retrieve the TvlSim service context
    if (_tvlsimServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The TvlSim service "
                                                    "has not been initialised");
    }
    assert (_tvlsimServiceContext != NULL);

    // Retrieve the StdAir service object from the (AirSched) service context
    TVLSIM_ServiceContext& lTVLSIM_ServiceContext = *_tvlsimServiceContext;
    stdair::STDAIR_Service& lSTDAIR_Service =
      lTVLSIM_ServiceContext.getSTDAIR_Service();

    // Delegate the BOM building to the dedicated service
    return lSTDAIR_Service.buildSampleBookingRequest (isForCRS);
  }
  
  // ////////////////////////////////////////////////////////////////////
  bool TVLSIM_Service::sell (const std::string& iSegmentDateKey,
                             const stdair::ClassCode_T& iClassCode,
                             const stdair::PartySize_T& iPartySize) {

    // Retrieve the TvlSim service context
    if (_tvlsimServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The TvlSim service "
                                                    "has not been initialised");
    }
    assert (_tvlsimServiceContext != NULL);

    // Retrieve the SimCRS service object from the (TvlSim) service context
    TVLSIM_ServiceContext& lTVLSIM_ServiceContext = *_tvlsimServiceContext;
    SIMCRS::SIMCRS_Service& lSIMCRS_Service =
      lTVLSIM_ServiceContext.getSIMCRS_Service(); 

    // Delegate the BOM display to the dedicated service
    return lSIMCRS_Service.sell (iSegmentDateKey, iClassCode,
                                 iPartySize);
  }

  // ////////////////////////////////////////////////////////////////////
  std::string TVLSIM_Service::
  jsonHandler (const stdair::JSONString& iJSONString) {

    // Retrieve the TvlSim service context
    if (_tvlsimServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The TvlSim service "
                                                    "has not been initialised");
    }
    assert (_tvlsimServiceContext != NULL);
    TVLSIM_ServiceContext& lTVLSIM_ServiceContext = *_tvlsimServiceContext; 

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
	lTVLSIM_ServiceContext.getSIMCRS_Service();

      return lSIMCRS_Service.jsonHandler (iJSONString);
    }
    case stdair::JSonCommand::EVENT_LIST:{ 

      // Get a reference on the TRADEMGEN service handler
      TRADEMGEN::TRADEMGEN_Service& lTRADEMGEN_Service =
	lTVLSIM_ServiceContext.getTRADEMGEN_Service(); 

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
    case stdair::JSonCommand::STATUS:{  
      // Get a reference on the Simulation Status
      SimulationStatus& lSimulationStatus =
	lTVLSIM_ServiceContext.getSimulationStatus(); 
      std::ostringstream oStream;
      BomJSONExport::jsonExportSimulationStatus(oStream, lSimulationStatus); 
      return oStream.str();
    }	  
    case stdair::JSonCommand::CONFIG:{ 
      // Retrieve the StdAir service object from the (TvlSim) service context
      stdair::STDAIR_Service& lSTDAIR_Service =
	lTVLSIM_ServiceContext.getSTDAIR_Service();
      // Try to import the JSON-ified string
      lSTDAIR_Service.jsonImportConfiguration (iJSONString);
      // Display the new configuration tree
      return lSTDAIR_Service.jsonExportConfiguration();
    }						  
    default: {
      // Return an Error string
      std::ostringstream lErrorCmdMessage;
      const std::string& lCommandStr =
	stdair::JSonCommand::getLabel(lEN_JSonCommand);
      lErrorCmdMessage << "{\"error\": \"The command '" << lCommandStr
		       << "' is not handled by the TvlSim service.\"}";
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
  std::string TVLSIM_Service::
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
      
    const stdair::Count_T lBreakPointsListSize = 
      lBreakPointList.size();
    const stdair::Count_T lNumberOfBreakPointsAdded = 
      initBreakPointEvents (lBreakPointList);	
   
    // Return a JSON-ified string
    std::ostringstream oStream;
    if (lNumberOfBreakPointsAdded != lBreakPointsListSize) {
      oStream << "{\"failed: \"" << lNumberOfBreakPointsAdded << "\"}";
    } else {
      oStream << "{\"done\": \"" << lBreakPointList.size() << "\"}";
    }
    return oStream.str();
  } 

  // ////////////////////////////////////////////////////////////////////
  std::string TVLSIM_Service::
  jsonRunHandler (const stdair::JSONString& iJSONString) { 

    // Retrieve the TvlSim service context
    if (_tvlsimServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The TvlSim service "
                                                    "has not been initialised");
    }
    assert (_tvlsimServiceContext != NULL);
    TVLSIM_ServiceContext& lTVLSIM_ServiceContext = *_tvlsimServiceContext;
    
    // Launch the simulation
    simulate ();

    // Get a reference on the Simulation Status
    SimulationStatus& lSimulationStatus =
	lTVLSIM_ServiceContext.getSimulationStatus(); 
      
    // Return a JSON-ified string
    std::ostringstream oStream;
    oStream << "{\"done\": \""
            << lSimulationStatus.getCurrentDate() << "\"}";
    return oStream.str();
  }

  // ////////////////////////////////////////////////////////////////////
  const stdair::Count_T TVLSIM_Service::
  initBreakPointEvents(const stdair::BreakPointList_T& iBreakPointList) {
    // Retrieve the TvlSim service context
    if (_tvlsimServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The TvlSim service "
                                                    "has not been initialised");
    }
    assert (_tvlsimServiceContext != NULL);
    TVLSIM_ServiceContext& lTVLSIM_ServiceContext = *_tvlsimServiceContext; 

    // Get a reference on the TRADEMGEN service handler
    const TRADEMGEN::TRADEMGEN_Service& lTRADEMGEN_Service =
      lTVLSIM_ServiceContext.getTRADEMGEN_Service(); 

    // Retrieve the SEvMgr service context
    SEVMGR::SEVMGR_Service& lSEVMGR_Service =
      lTVLSIM_ServiceContext.getSEVMGR_Service();      

    // Get a reference on the Simulation Status
    SimulationStatus& lSimulationStatus =
      lTVLSIM_ServiceContext.getSimulationStatus(); 

    return Simulator::initialiseBreakPoint (lTRADEMGEN_Service,
					    lSEVMGR_Service,
					    iBreakPointList,
					    lSimulationStatus);

  }

  // ////////////////////////////////////////////////////////////////////
  void TVLSIM_Service::initSnapshotAndRMEvents() {
    // Retrieve the TvlSim service context
    if (_tvlsimServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The TvlSim service "
                                                    "has not been initialised");
    }
    assert (_tvlsimServiceContext != NULL);
    TVLSIM_ServiceContext& lTVLSIM_ServiceContext = *_tvlsimServiceContext;

    // Get a reference on the Simulation Status
    SimulationStatus& lSimulationStatus =
      lTVLSIM_ServiceContext.getSimulationStatus();

    // Retrieve the simulation parameters
    const stdair::Date_T& lStartDate = lSimulationStatus.getStartDate();
    const stdair::Date_T& lEndDate = lSimulationStatus.getEndDate();

    // Get a reference on the SIMCRS service handler
    SIMCRS::SIMCRS_Service& lSIMCRS_Service =
      lTVLSIM_ServiceContext.getSIMCRS_Service();

    lSIMCRS_Service.initSnapshotAndRMEvents (lStartDate, lEndDate);    

    // Get a reference on the TRADEMGEN service handler
    TRADEMGEN::TRADEMGEN_Service& lTRADEMGEN_Service =
      lTVLSIM_ServiceContext.getTRADEMGEN_Service(); 
 
    // Update the Simulation Status
    Simulator::updateStatus(lTRADEMGEN_Service, stdair::EventType::RM,
			    lSimulationStatus);
    Simulator::updateStatus(lTRADEMGEN_Service, stdair::EventType::SNAPSHOT,
			    lSimulationStatus);
 
  }
  
  // //////////////////////////////////////////////////////////////////////
  std::string TVLSIM_Service::csvDisplay() const {

    // Retrieve the TvlSim service context
    if (_tvlsimServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The TvlSim service "
                                                    "has not been initialised");
    }
    assert (_tvlsimServiceContext != NULL);

    // Retrieve the StdAir service object from the (TvlSim) service context
    TVLSIM_ServiceContext& lTVLSIM_ServiceContext = *_tvlsimServiceContext;
    stdair::STDAIR_Service& lSTDAIR_Service =
      lTVLSIM_ServiceContext.getSTDAIR_Service(); 
    const stdair::BomRoot& lBomRoot = lSTDAIR_Service.getBomRoot();

    // Delegate the BOM building to the dedicated service
    return lSTDAIR_Service.csvDisplay(lBomRoot);
  } 

  // //////////////////////////////////////////////////////////////////////
  std::string TVLSIM_Service::listEvents () const {  

    // Retrieve the TvlSim service context
    if (_tvlsimServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The TvlSim service "
                                                    "has not been initialised");
    }
    assert (_tvlsimServiceContext != NULL);

    // Retrieve the TraDemGen service object from the (TvlSim) service context
    TVLSIM_ServiceContext& lTVLSIM_ServiceContext = *_tvlsimServiceContext;  

    // Get a reference on the TRADEMGEN service handler
    TRADEMGEN::TRADEMGEN_Service& lTRADEMGEN_Service =
      lTVLSIM_ServiceContext.getTRADEMGEN_Service(); 

    // Delegate the BOM building to the dedicated service
    return lTRADEMGEN_Service.list ();

  }   

  // //////////////////////////////////////////////////////////////////////
  std::string TVLSIM_Service::
  listEvents (const stdair::EventType::EN_EventType& iEventType) const {

    // Retrieve the TvlSim service context
    if (_tvlsimServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The TvlSim service "
                                                    "has not been initialised");
    }
    assert (_tvlsimServiceContext != NULL);

    // Retrieve the TraDemGen service object from the (TvlSim) service context
    TVLSIM_ServiceContext& lTVLSIM_ServiceContext = *_tvlsimServiceContext;  

    // Get a reference on the TRADEMGEN service handler
    TRADEMGEN::TRADEMGEN_Service& lTRADEMGEN_Service =
      lTVLSIM_ServiceContext.getTRADEMGEN_Service(); 

    // Delegate the BOM building to the dedicated service
    return lTRADEMGEN_Service.list (iEventType);

  }

  // ////////////////////////////////////////////////////////////////////
  std::string TVLSIM_Service::
  list (const stdair::AirlineCode_T& iAirlineCode,
        const stdair::FlightNumber_T& iFlightNumber) const {

    // Retrieve the TvlSim service context
    if (_tvlsimServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The TvlSim service "
                                                    "has not been initialised");
    }
    assert (_tvlsimServiceContext != NULL);

    // Retrieve the SimCRS service object from the (TvlSim) service context
    TVLSIM_ServiceContext& lTVLSIM_ServiceContext = *_tvlsimServiceContext;
    SIMCRS::SIMCRS_Service& lSIMCRS_Service =
      lTVLSIM_ServiceContext.getSIMCRS_Service(); 

    // Delegate the BOM display to the dedicated service
    return lSIMCRS_Service.list (iAirlineCode, iFlightNumber);
  }

  // ////////////////////////////////////////////////////////////////////
  std::string TVLSIM_Service::
  csvDisplay (const stdair::AirlineCode_T& iAirlineCode,
              const stdair::FlightNumber_T& iFlightNumber,
              const stdair::Date_T& iDepartureDate) const {

    // Retrieve the TvlSim service context
    if (_tvlsimServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The TvlSim service "
                                                    "has not been initialised");
    }
    assert (_tvlsimServiceContext != NULL);

    // Retrieve the SimCRS service object from the (TvlSim) service context
    TVLSIM_ServiceContext& lTVLSIM_ServiceContext = *_tvlsimServiceContext;
    SIMCRS::SIMCRS_Service& lSIMCRS_Service =
      lTVLSIM_ServiceContext.getSIMCRS_Service(); 

    // Delegate the BOM display to the dedicated service
    return lSIMCRS_Service.csvDisplay (iAirlineCode, iFlightNumber,
                                       iDepartureDate);
  }

  // ////////////////////////////////////////////////////////////////////
  void TVLSIM_Service::reset() {  

    // Retrieve the TvlSim service context
    if (_tvlsimServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The TvlSim service "
                                                    "has not been initialised");
    }
    assert (_tvlsimServiceContext != NULL);

    // Retrieve the StdAir service object from the (TvlSim) service context
    TVLSIM_ServiceContext& lTVLSIM_ServiceContext = *_tvlsimServiceContext;
 
    // TODO: gsabatier
    // Reset everything except break points
    // Get a reference on the TRADEMGEN service handler
    TRADEMGEN::TRADEMGEN_Service& lTRADEMGEN_Service =
      lTVLSIM_ServiceContext.getTRADEMGEN_Service(); 
    // Reset the TRADEMGEN service 
    lTRADEMGEN_Service.reset();

    // TODO
    // Get a reference on the SIMCRS service handler
    /**SIMCRS::SIMCRS_Service& lSIMCRS_Service =
       lTVLSIM_ServiceContext.getSIMCRS_Service();
       // Reset the SIMCRS service 
       lSIMCRS_Service.reset();*/ 

    // TODO
    // Get a reference on the TRAVELCCM service handler
    /**TRAVELCCM::TRAVELCCM_Service& lTRAVELCCM_Service =
       lTVLSIM_ServiceContext.getTRAVELCCM_Service();  
       // Reset the TRAVELCCM service 
       lTRAVELCCM_Service.reset();*/

    // Reset the simulation status object  
    SimulationStatus& lSimulationStatus =
      lTVLSIM_ServiceContext.getSimulationStatus();
    lSimulationStatus.reset();

    // Re-init Snapshot and RMEvents
    initSnapshotAndRMEvents();

    // Clone again the persistent BOM tree
    clonePersistentBom ();
    
  }    

  // ////////////////////////////////////////////////////////////////////
  void TVLSIM_Service::prepareNewRun() {  

    // Retrieve the TvlSim service context
    if (_tvlsimServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The TvlSim service "
                                                    "has not been initialised");
    }
    assert (_tvlsimServiceContext != NULL);

    // Retrieve the StdAir service object from the (TvlSim) service context
    TVLSIM_ServiceContext& lTVLSIM_ServiceContext = *_tvlsimServiceContext; 
 
    // Get a reference on the TRADEMGEN service handler
    TRADEMGEN::TRADEMGEN_Service& lTRADEMGEN_Service =
      lTVLSIM_ServiceContext.getTRADEMGEN_Service(); 
    // Reset the TRADEMGEN service 
    lTRADEMGEN_Service.reset();

    // TODO
    // Get a reference on the SIMCRS service handler
    /**SIMCRS::SIMCRS_Service& lSIMCRS_Service =
       lTVLSIM_ServiceContext.getSIMCRS_Service();
       // Reset the SIMCRS service 
       lSIMCRS_Service.reset();*/ 

    // TODO
    // Get a reference on the TRAVELCCM service handler
    /**TRAVELCCM::TRAVELCCM_Service& lTRAVELCCM_Service =
       lTVLSIM_ServiceContext.getTRAVELCCM_Service();  
       // Reset the TRAVELCCM service 
       lTRAVELCCM_Service.reset();*/

    // Prepare a new run in the simulation status object  
    SimulationStatus& lSimulationStatus =
      lTVLSIM_ServiceContext.getSimulationStatus();
    lSimulationStatus.prepareNewRun(); 

    // Re-init Snapshot and RMEvents
    initSnapshotAndRMEvents();
    
  }  

  // //////////////////////////////////////////////////////////////////////
  std::string TVLSIM_Service::simulationStatusDisplay() const {

    // Retrieve the TvlSim service context
    if (_tvlsimServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The TvlSim service "
                                                    "has not been initialised");
    }
    assert (_tvlsimServiceContext != NULL);

    // Retrieve the StdAir service object from the (TvlSim) service context
    TVLSIM_ServiceContext& lTVLSIM_ServiceContext = *_tvlsimServiceContext;

    // Get a reference on the Simulation Status
    const SimulationStatus& lSimulationStatus =
      lTVLSIM_ServiceContext.getSimulationStatus();

    return lSimulationStatus.describe();
  }

  // //////////////////////////////////////////////////////////////////////
  void TVLSIM_Service::
  simulate () {

    // Retrieve the TvlSim service context
    if (_tvlsimServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The TvlSim service has not "
                                                    "been initialised");
    }
    assert (_tvlsimServiceContext != NULL);
    TVLSIM_ServiceContext& lTVLSIM_ServiceContext = *_tvlsimServiceContext;

    // Get a reference on the Simulation Status
    SimulationStatus& lSimulationStatus =
      lTVLSIM_ServiceContext.getSimulationStatus();

    // Get a reference on the SIMCRS service handler
    SIMCRS::SIMCRS_Service& lSIMCRS_Service =
      lTVLSIM_ServiceContext.getSIMCRS_Service();

    // Get a reference on the TRADEMGEN service handler
    TRADEMGEN::TRADEMGEN_Service& lTRADEMGEN_Service =
      lTVLSIM_ServiceContext.getTRADEMGEN_Service(); 
    
    // Get a reference on the TRAVELCCM service handler
    TRAVELCCM::TRAVELCCM_Service& lTRAVELCCM_Service =
      lTVLSIM_ServiceContext.getTRAVELCCM_Service();

    // Get a reference on the STDAIR service handler
    stdair::STDAIR_Service& lSTDAIR_Service =
      lTVLSIM_ServiceContext.getSTDAIR_Service(); 

    // Extract the demand generation method from the config holder
    char lChar;
    const bool hasDemandGenMethodBeenRetrieved = 
      lSTDAIR_Service.exportConfigValue<char> (lChar, 
					       "demand generation.method"); 
    assert (hasDemandGenMethodBeenRetrieved == true);
    const stdair::DemandGenerationMethod lDemandGenerationMethod (lChar);
 
    // Get the number of the current run
    while (lSimulationStatus.isTheSimulationDone() == false) { 

      if (lSimulationStatus.getMode() == SimulationMode::DONE) {
	prepareNewRun(); 
      }	 

      const NbOfRuns_T& lCurrentRun = lSimulationStatus.getCurrentRun();

      // DEBUG
      lSimulationStatus.displayStartStatusMessage();	
      STDAIR_LOG_DEBUG ("Simulation[" << lCurrentRun << "] begins"
			<< " - " << lTVLSIM_ServiceContext.display());

      // Delegate the bookings to the dedicated command
      stdair::BasChronometer lSimulationChronometer;
      lSimulationChronometer.start();
      Simulator::simulate (lSIMCRS_Service, lTRADEMGEN_Service,
                           lTRAVELCCM_Service, lSTDAIR_Service,
                           lSimulationStatus, lDemandGenerationMethod);
      const double lSimulationMeasure = lSimulationChronometer.elapsed();	

      // DEBUG 
      lSimulationStatus.displayEndStatusMessage();	
      STDAIR_LOG_DEBUG ("Simulation[" << lCurrentRun << "] ends: " << lSimulationMeasure
			<< " - " << lTVLSIM_ServiceContext.display());

       if (lSimulationStatus.getMode() == SimulationMode::BREAK) {
	 return;
      }	
    }
  }

  // //////////////////////////////////////////////////////////////////////
  void TVLSIM_Service::displayAirlineListFromDB () const {

    // Retrieve the TvlSim service context
    if (_tvlsimServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The TvlSim service has not "
                                                    "been initialised");
    }
    assert (_tvlsimServiceContext != NULL);
    //TVLSIM_ServiceContext& lTVLSIM_ServiceContext = *_tvlsimServiceContext;

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

  // //////////////////////////////////////////////////////////////////////
  std::string TVLSIM_Service::configDisplay () const {

    // Retrieve the TvlSim service context
    if (_tvlsimServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The TvlSim service "
                                                    "has not been initialised");
    }
    assert (_tvlsimServiceContext != NULL);
    TVLSIM_ServiceContext& lTVLSIM_ServiceContext = *_tvlsimServiceContext;  

    // Retrieve the StdAir service object from the (TVLSIM) service context
    stdair::STDAIR_Service& lSTDAIR_Service =
      lTVLSIM_ServiceContext.getSTDAIR_Service();

    // Display (dump in the returned string) the configuration.
    return lSTDAIR_Service.configDisplay (); 
  }


  // ////////////////////////////////////////////////////////////////////
  void TVLSIM_Service::optimise (const stdair::RMEventStruct& iRMEvent) {

    // Retrieve the TvlSim service context
    if (_tvlsimServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The TvlSim service has not "
                                                    "been initialised");
    }
    assert (_tvlsimServiceContext != NULL);
    TVLSIM_ServiceContext& lTVLSIM_ServiceContext = *_tvlsimServiceContext;

    // Get a reference on the SIMCRS service handler
    SIMCRS::SIMCRS_Service& lSIMCRS_Service =
      lTVLSIM_ServiceContext.getSIMCRS_Service();

    // Call the dedicated service
    lSIMCRS_Service.optimise (iRMEvent);
  }

  // ////////////////////////////////////////////////////////////////////
  const SimulationStatus& TVLSIM_Service::getSimulationStatus() {

    // Retrieve the TvlSim service context
    if (_tvlsimServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The TvlSim service has not "
                                                    "been initialised");
    }
    assert (_tvlsimServiceContext != NULL);
    TVLSIM_ServiceContext& lTVLSIM_ServiceContext = *_tvlsimServiceContext;

    return lTVLSIM_ServiceContext.getSimulationStatus();

  }
}
