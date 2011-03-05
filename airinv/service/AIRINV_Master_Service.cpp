// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// Boost
#include <boost/make_shared.hpp>
// StdAir
#include <stdair/basic/BasChronometer.hpp>
#include <stdair/basic/BasFileMgr.hpp>
#include <stdair/bom/BomManager.hpp> 
#include <stdair/bom/BomRoot.hpp>
#include <stdair/bom/Inventory.hpp>
#include <stdair/bom/AirlineFeature.hpp>
#include <stdair/factory/FacBomManager.hpp>
#include <stdair/service/Logger.hpp>
#include <stdair/STDAIR_Service.hpp>
// Airinv
#include <airinv/basic/BasConst_AIRINV_Service.hpp>
#include <airinv/factory/FacAirinvMasterServiceContext.hpp>
#include <airinv/command/InventoryParser.hpp>
#include <airinv/command/InventoryManager.hpp>
#include <airinv/service/AIRINV_Master_ServiceContext.hpp>
#include <airinv/AIRINV_Service.hpp>
#include <airinv/AIRINV_Master_Service.hpp>

namespace AIRINV {

  // //////////////////////////////////////////////////////////////////////
  AIRINV_Master_Service::AIRINV_Master_Service() 
    : _airinvMasterServiceContext (NULL) {
    assert (false);
  }

  // //////////////////////////////////////////////////////////////////////
  AIRINV_Master_Service::
  AIRINV_Master_Service (const AIRINV_Master_Service& iService)
    : _airinvMasterServiceContext (NULL) {
    assert (false);
  }

  // //////////////////////////////////////////////////////////////////////
  AIRINV_Master_Service::
  AIRINV_Master_Service (const stdair::BasLogParams& iLogParams,
                         const stdair::BasDBParams& iDBParams)
    : _airinvMasterServiceContext (NULL) {
    
    // Initialise the STDAIR service handler
    stdair::STDAIR_ServicePtr_T lSTDAIR_Service_ptr =
      initStdAirService (iLogParams, iDBParams);
    
    // Initialise the service context
    initServiceContext();

    // Add the StdAir service context to the AIRINV service context
    // \note RMOL owns the STDAIR service resources here.
    const bool ownStdairService = true;
    addStdAirService (lSTDAIR_Service_ptr, ownStdairService);
    
    // Initialise the (remaining of the) context
    initSlaveAirinvService (lSTDAIR_Service_ptr);
  }

  // //////////////////////////////////////////////////////////////////////
  AIRINV_Master_Service::
  AIRINV_Master_Service (const stdair::BasLogParams& iLogParams)
    : _airinvMasterServiceContext (NULL) {
    
    // Initialise the STDAIR service handler
    stdair::STDAIR_ServicePtr_T lSTDAIR_Service_ptr =
      initStdAirService (iLogParams);

    // Initialise the service context
    initServiceContext();
    
    // Add the StdAir service context to the AIRINV service context
    // \note RMOL owns the STDAIR service resources here.
    const bool ownStdairService = true;
    addStdAirService (lSTDAIR_Service_ptr, ownStdairService);

    // Initialise the (remaining of the) context
    initSlaveAirinvService (lSTDAIR_Service_ptr);
  }

  // //////////////////////////////////////////////////////////////////////
  AIRINV_Master_Service::
  AIRINV_Master_Service (stdair::STDAIR_ServicePtr_T ioSTDAIR_Service_ptr)
    : _airinvMasterServiceContext (NULL) {

    // Initialise the service context
    initServiceContext();

    // Retrieve the AirInv service context
    assert (_airinvMasterServiceContext != NULL);
    AIRINV_Master_ServiceContext& lAIRINV_Master_ServiceContext =
      *_airinvMasterServiceContext;
    
    // Store the STDAIR service object within the (AIRINV) service context
    // \note AirInv does not own the STDAIR service resources here.
    const bool doesNotOwnStdairService = false;
    assert (ioSTDAIR_Service_ptr);
    lAIRINV_Master_ServiceContext.setSTDAIR_Service (ioSTDAIR_Service_ptr,
                                                     doesNotOwnStdairService);
    
    // Initialise the (remaining of the) context
    initSlaveAirinvService (ioSTDAIR_Service_ptr);
  }

  // //////////////////////////////////////////////////////////////////////
  AIRINV_Master_Service::
  AIRINV_Master_Service (const stdair::BasLogParams& iLogParams,
                         const stdair::BasDBParams& iDBParams,
                         const stdair::Filename_T& iInventoryInputFilename)
    : _airinvMasterServiceContext (NULL) {
    
    // Initialise the STDAIR service handler
    stdair::STDAIR_ServicePtr_T lSTDAIR_Service_ptr =
      initStdAirService (iLogParams, iDBParams);
    
    // Initialise the service context
    initServiceContext();

    // Add the StdAir service context to the AIRINV service context
    // \note RMOL owns the STDAIR service resources here.
    const bool ownStdairService = true;
    addStdAirService (lSTDAIR_Service_ptr, ownStdairService);
    
    // Initialise the (remaining of the) context
    initSlaveAirinvService (lSTDAIR_Service_ptr, iInventoryInputFilename);
  }

  // //////////////////////////////////////////////////////////////////////
  AIRINV_Master_Service::
  AIRINV_Master_Service (const stdair::BasLogParams& iLogParams,
                         const stdair::Filename_T& iInventoryInputFilename)
    : _airinvMasterServiceContext (NULL) {
    
    // Initialise the STDAIR service handler
    stdair::STDAIR_ServicePtr_T lSTDAIR_Service_ptr =
      initStdAirService (iLogParams);

    // Initialise the service context
    initServiceContext();
    
    // Add the StdAir service context to the AIRINV service context
    // \note RMOL owns the STDAIR service resources here.
    const bool ownStdairService = true;
    addStdAirService (lSTDAIR_Service_ptr, ownStdairService);

    // Initialise the (remaining of the) context
    initSlaveAirinvService (lSTDAIR_Service_ptr, iInventoryInputFilename);
  }

  // //////////////////////////////////////////////////////////////////////
  AIRINV_Master_Service::
  AIRINV_Master_Service (stdair::STDAIR_ServicePtr_T ioSTDAIR_Service_ptr,
                         const stdair::Filename_T& iInventoryInputFilename)
    : _airinvMasterServiceContext (NULL) {

    // Initialise the service context
    initServiceContext();

    // Retrieve the AirInv service context
    assert (_airinvMasterServiceContext != NULL);
    AIRINV_Master_ServiceContext& lAIRINV_Master_ServiceContext =
      *_airinvMasterServiceContext;
    
    // Store the STDAIR service object within the (AIRINV) service context
    // \note AirInv does not own the STDAIR service resources here.
    const bool doesNotOwnStdairService = false;
    assert (ioSTDAIR_Service_ptr);
    lAIRINV_Master_ServiceContext.setSTDAIR_Service (ioSTDAIR_Service_ptr,
                                                     doesNotOwnStdairService);
    
    // Initialise the (remaining of the) context
    initSlaveAirinvService (ioSTDAIR_Service_ptr, iInventoryInputFilename);
  }

  // //////////////////////////////////////////////////////////////////////
  AIRINV_Master_Service::
  AIRINV_Master_Service (const stdair::BasLogParams& iLogParams,
                         const stdair::BasDBParams& iDBParams,
                         const stdair::Filename_T& iScheduleInputFilename,
                         const stdair::Filename_T& iODInputFilename)
    : _airinvMasterServiceContext (NULL) {
    
    // Initialise the STDAIR service handler
    stdair::STDAIR_ServicePtr_T lSTDAIR_Service_ptr =
      initStdAirService (iLogParams, iDBParams);
    
    // Initialise the service context
    initServiceContext();

    // Add the StdAir service context to the AIRINV service context
    // \note RMOL owns the STDAIR service resources here.
    const bool ownStdairService = true;
    addStdAirService (lSTDAIR_Service_ptr, ownStdairService);
    
    // Initialise the (remaining of the) context
    initSlaveAirinvService (lSTDAIR_Service_ptr, iScheduleInputFilename,
                            iODInputFilename);
  }

  // //////////////////////////////////////////////////////////////////////
  AIRINV_Master_Service::
  AIRINV_Master_Service (const stdair::BasLogParams& iLogParams,
                         const stdair::Filename_T& iScheduleInputFilename,
                         const stdair::Filename_T& iODInputFilename)
    : _airinvMasterServiceContext (NULL) {
    
    // Initialise the STDAIR service handler
    stdair::STDAIR_ServicePtr_T lSTDAIR_Service_ptr =
      initStdAirService (iLogParams);
    
    // Initialise the service context
    initServiceContext();

    // Add the StdAir service context to the AIRINV service context
    // \note RMOL owns the STDAIR service resources here.
    const bool ownStdairService = true;
    addStdAirService (lSTDAIR_Service_ptr, ownStdairService);
    
    // Initialise the (remaining of the) context
    initSlaveAirinvService (lSTDAIR_Service_ptr, iScheduleInputFilename,
                            iODInputFilename);
  }

  // //////////////////////////////////////////////////////////////////////
  AIRINV_Master_Service::
  AIRINV_Master_Service (stdair::STDAIR_ServicePtr_T ioSTDAIR_Service_ptr,
                         const stdair::Filename_T& iScheduleInputFilename,
                         const stdair::Filename_T& iODInputFilename)
    : _airinvMasterServiceContext (NULL) {

    // Initialise the service context
    initServiceContext();

    // Retrieve the AirInv service context
    assert (_airinvMasterServiceContext != NULL);
    AIRINV_Master_ServiceContext& lAIRINV_Master_ServiceContext =
      *_airinvMasterServiceContext;
    
    // Store the STDAIR service object within the (AIRINV) service context
    // \note AirInv does not own the STDAIR service resources here.
    const bool doesNotOwnStdairService = false;
    assert (ioSTDAIR_Service_ptr);
    lAIRINV_Master_ServiceContext.setSTDAIR_Service (ioSTDAIR_Service_ptr,
                                                     doesNotOwnStdairService);
    
    // Initialise the (remaining of the) context
    initSlaveAirinvService (ioSTDAIR_Service_ptr, iScheduleInputFilename,
                            iODInputFilename);
  }

  // //////////////////////////////////////////////////////////////////////
  AIRINV_Master_Service::~AIRINV_Master_Service() {
    // Delete/Clean all the objects from memory
    finalise();
  }

  // //////////////////////////////////////////////////////////////////////
  void AIRINV_Master_Service::initServiceContext() {
    // Initialise the context
    AIRINV_Master_ServiceContext& lAIRINV_Master_ServiceContext = 
      FacAirinvMasterServiceContext::instance().create ();
    _airinvMasterServiceContext = &lAIRINV_Master_ServiceContext;
  }

  // //////////////////////////////////////////////////////////////////////
  void AIRINV_Master_Service::
  addStdAirService (stdair::STDAIR_ServicePtr_T ioSTDAIR_Service_ptr,
                    const bool iOwnStdairService) {

    // Retrieve the Airinv service context
    assert (_airinvMasterServiceContext != NULL);
    AIRINV_Master_ServiceContext& lAIRINV_Master_ServiceContext =
      *_airinvMasterServiceContext;

    // Store the STDAIR service object within the (AIRINV) service context
    lAIRINV_Master_ServiceContext.setSTDAIR_Service (ioSTDAIR_Service_ptr,
                                                     iOwnStdairService);
  }
  
  // //////////////////////////////////////////////////////////////////////
  stdair::STDAIR_ServicePtr_T AIRINV_Master_Service::
  initStdAirService (const stdair::BasLogParams& iLogParams,
                     const stdair::BasDBParams& iDBParams) {

    // Initialise the STDAIR service handler
    // Note that the track on the object memory is kept thanks to the Boost
    // Smart Pointers component.
    stdair::STDAIR_ServicePtr_T lSTDAIR_Service_ptr = 
      boost::make_shared<stdair::STDAIR_Service> (iLogParams, iDBParams);
    
    return lSTDAIR_Service_ptr;
  }
  
  // //////////////////////////////////////////////////////////////////////
  stdair::STDAIR_ServicePtr_T AIRINV_Master_Service::
  initStdAirService (const stdair::BasLogParams& iLogParams) {

    // Initialise the STDAIR service handler
    // Note that the track on the object memory is kept thanks to the Boost
    // Smart Pointers component.
    stdair::STDAIR_ServicePtr_T lSTDAIR_Service_ptr = 
      boost::make_shared<stdair::STDAIR_Service> (iLogParams);

    return lSTDAIR_Service_ptr;
  }
  
  // //////////////////////////////////////////////////////////////////////
  void AIRINV_Master_Service::
  initSlaveAirinvService (stdair::STDAIR_ServicePtr_T ioSTDAIR_Service_ptr) {
    // Retrieve the AIRINV Master service context.
    assert (_airinvMasterServiceContext != NULL);
    AIRINV_Master_ServiceContext& lAIRINV_Master_ServiceContext =
      *_airinvMasterServiceContext;

    // Initialise the AIRINV service handler
    // Note that the (Boost.)Smart Pointer keeps track of the references
    // on the Service object, and deletes that object when it is no longer
    // referenced (e.g., at the end of the process).
    stdair::BasLogParams logParams = ioSTDAIR_Service_ptr->getLogParams();
    logParams.setForcedInitialisationFlag (true);
    AIRINV_ServicePtr_T lAIRINV_Service_ptr =
      boost::make_shared<AIRINV_Service> (ioSTDAIR_Service_ptr);

    // Store the AIRINV service object within the AIRINV Master service context.
    lAIRINV_Master_ServiceContext.setAIRINV_Service (lAIRINV_Service_ptr);
  }
  
  // //////////////////////////////////////////////////////////////////////
  void AIRINV_Master_Service::
  initSlaveAirinvService (stdair::STDAIR_ServicePtr_T ioSTDAIR_Service_ptr,
                          const stdair::Filename_T& iInventoryInputFilename) {
    // Retrieve the AIRINV Master service context.
    assert (_airinvMasterServiceContext != NULL);
    AIRINV_Master_ServiceContext& lAIRINV_Master_ServiceContext =
      *_airinvMasterServiceContext;

    // Initialise the AIRINV service handler
    // Note that the (Boost.)Smart Pointer keeps track of the references
    // on the Service object, and deletes that object when it is no longer
    // referenced (e.g., at the end of the process).
    stdair::BasLogParams logParams = ioSTDAIR_Service_ptr->getLogParams();
    logParams.setForcedInitialisationFlag (true);
    AIRINV_ServicePtr_T lAIRINV_Service_ptr =
      boost::make_shared<AIRINV_Service> (ioSTDAIR_Service_ptr,
                                          iInventoryInputFilename);

    // Store the AIRINV service object within the AIRINV Master service context.
    lAIRINV_Master_ServiceContext.setAIRINV_Service (lAIRINV_Service_ptr);
  }
  
  // //////////////////////////////////////////////////////////////////////
  void AIRINV_Master_Service::
  initSlaveAirinvService (stdair::STDAIR_ServicePtr_T ioSTDAIR_Service_ptr,
                          const stdair::Filename_T& iScheduleInputFilename,
                          const stdair::Filename_T& iODInputFilename) {
    // Retrieve the AIRINV Master service context.
    assert (_airinvMasterServiceContext != NULL);
    AIRINV_Master_ServiceContext& lAIRINV_Master_ServiceContext =
      *_airinvMasterServiceContext;

    // Initialise the AIRINV service handler
    // Note that the (Boost.)Smart Pointer keeps track of the references
    // on the Service object, and deletes that object when it is no longer
    // referenced (e.g., at the end of the process).
    stdair::BasLogParams logParams = ioSTDAIR_Service_ptr->getLogParams();
    logParams.setForcedInitialisationFlag (true);
    AIRINV_ServicePtr_T lAIRINV_Service_ptr =
      boost::make_shared<AIRINV_Service> (ioSTDAIR_Service_ptr,
                                          iScheduleInputFilename,
                                          iODInputFilename);

    // Store the AIRINV service object within the AIRINV Master service context.
    lAIRINV_Master_ServiceContext.setAIRINV_Service (lAIRINV_Service_ptr);
  }
  
  // //////////////////////////////////////////////////////////////////////
  void AIRINV_Master_Service::finalise() {
    assert (_airinvMasterServiceContext != NULL);
    // Reset the (Boost.)Smart pointer pointing on the STDAIR_Service object.
    _airinvMasterServiceContext->reset();
  }

  // //////////////////////////////////////////////////////////////////////
  void AIRINV_Master_Service::
  buildSampleBom (const bool isForRMOL,
                  const stdair::CabinCapacity_T iCapacity) {

    // Retrieve the AIRINV service context
    if (_airinvMasterServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The AirInvMaster service "
                                                    "has not been initialised");
    }
    assert (_airinvMasterServiceContext != NULL);

    AIRINV_Master_ServiceContext& lAIRINV_Master_ServiceContext =
      *_airinvMasterServiceContext;
  
    // Retrieve the slave AIRINV service object from the (AIRINV) service context
    AIRINV_ServicePtr_T lAIRINV_Service_ptr =
      lAIRINV_Master_ServiceContext.getAIRINV_Service();

    // Delegate the BOM building to the dedicated service
    lAIRINV_Service_ptr->buildSampleBom();
  }

  // //////////////////////////////////////////////////////////////////////
  std::string AIRINV_Master_Service::csvDisplay() const {

    // Retrieve the AIRINV service context
    if (_airinvMasterServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The AirInvMaster service "
                                                    "has not been initialised");
    }
    assert (_airinvMasterServiceContext != NULL);

    AIRINV_Master_ServiceContext& lAIRINV_Master_ServiceContext =
      *_airinvMasterServiceContext;
  
    // Retrieve the slave AIRINV service object from the (AIRINV) service context
    AIRINV_ServicePtr_T lAIRINV_Service_ptr =
      lAIRINV_Master_ServiceContext.getAIRINV_Service();

    // Delegate the BOM building to the dedicated service
    return lAIRINV_Service_ptr->csvDisplay();
  }

  // //////////////////////////////////////////////////////////////////////
  bool AIRINV_Master_Service::sell (const std::string& iSegmentDateKey,
                                    const stdair::ClassCode_T& iClassCode,
                                    const stdair::PartySize_T& iPartySize) {

    // Retrieve the AIRINV service context
    if (_airinvMasterServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The AirInvMaster service "
                                                    "has not been initialised");
    }
    assert (_airinvMasterServiceContext != NULL);

    AIRINV_Master_ServiceContext& lAIRINV_Master_ServiceContext =
      *_airinvMasterServiceContext;
  
    // Retrieve the slave AIRINV service object from the (AIRINV) service context
    AIRINV_ServicePtr_T lAIRINV_Service_ptr =
      lAIRINV_Master_ServiceContext.getAIRINV_Service();

    // Delegate the booking to the dedicated command
    stdair::BasChronometer lSellChronometer;
    lSellChronometer.start();

    // Delegate the BOM building to the dedicated service
    const bool hasBeenSaleSuccessful =
      lAIRINV_Service_ptr->sell (iSegmentDateKey, iClassCode, iPartySize);

    const double lSellMeasure = lSellChronometer.elapsed();

    // DEBUG
    STDAIR_LOG_DEBUG ("Booking sell: " << lSellMeasure << " - "
                      << lAIRINV_Master_ServiceContext.display());

    //
    return hasBeenSaleSuccessful;
  }
  
}
