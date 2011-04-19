// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// Boost
#include <boost/make_shared.hpp>
// StdAir
#include <stdair/basic/BasChronometer.hpp>
#include <stdair/bom/BomManager.hpp> 
#include <stdair/bom/BomKeyManager.hpp> 
#include <stdair/bom/BomRoot.hpp>
#include <stdair/bom/Inventory.hpp>
#include <stdair/bom/AirlineFeature.hpp>
#include <stdair/factory/FacBomManager.hpp>
#include <stdair/service/Logger.hpp>
#include <stdair/STDAIR_Service.hpp>
// RMOL
#include <rmol/RMOL_Service.hpp>
// Airinv
#include <airinv/basic/BasConst_AIRINV_Service.hpp>
#include <airinv/factory/FacAirinvServiceContext.hpp>
#include <airinv/command/ScheduleParser.hpp>
#include <airinv/command/InventoryParser.hpp>
#include <airinv/command/InventoryManager.hpp>
#include <airinv/service/AIRINV_ServiceContext.hpp>
#include <airinv/AIRINV_Service.hpp>

namespace AIRINV {

  // ////////////////////////////////////////////////////////////////////
  AIRINV_Service::AIRINV_Service() : _airinvServiceContext (NULL) {
    assert (false);
  }

  // ////////////////////////////////////////////////////////////////////
  AIRINV_Service::AIRINV_Service (const AIRINV_Service& iService)
  : _airinvServiceContext (NULL) {
    assert (false);
  }

  // ////////////////////////////////////////////////////////////////////
  AIRINV_Service::AIRINV_Service (const stdair::BasLogParams& iLogParams)
    : _airinvServiceContext (NULL) {
    
    // Initialise the STDAIR service handler
    stdair::STDAIR_ServicePtr_T lSTDAIR_Service_ptr =
      initStdAirService (iLogParams);
    
    // Initialise the service context
    initServiceContext();

    // Add the StdAir service context to the AIRINV service context
    // \note AIRINV owns the STDAIR service resources here.
    const bool ownStdairService = true;
    addStdAirService (lSTDAIR_Service_ptr, ownStdairService);

    // Initalise the RMOL service.
    initRMOLService();
    
    // Initialise the (remaining of the) context
    initAirinvService();
  }

  // ////////////////////////////////////////////////////////////////////
  AIRINV_Service::AIRINV_Service (const stdair::BasLogParams& iLogParams,
                                  const stdair::BasDBParams& iDBParams)
    : _airinvServiceContext (NULL) {
    
    // Initialise the STDAIR service handler
    stdair::STDAIR_ServicePtr_T lSTDAIR_Service_ptr =
      initStdAirService (iLogParams, iDBParams);
    
    // Initialise the service context
    initServiceContext();

    // Add the StdAir service context to the AIRINV service context
    // \note AIRINV owns the STDAIR service resources here.
    const bool ownStdairService = true;
    addStdAirService (lSTDAIR_Service_ptr, ownStdairService);

    // Initalise the RMOL service.
    initRMOLService();
    
    // Initialise the (remaining of the) context
    initAirinvService();
  }

  // //////////////////////////////////////////////////////////////////////
  AIRINV_Service::
  AIRINV_Service (stdair::STDAIR_ServicePtr_T ioSTDAIR_Service_ptr)
    : _airinvServiceContext (NULL) {

    // Initialise the service context
    initServiceContext();

    // Store the STDAIR service object within the (AIRINV) service context
    // \note AirInv does not own the STDAIR service resources here.
    const bool doesNotOwnStdairService = false;
    addStdAirService (ioSTDAIR_Service_ptr, doesNotOwnStdairService);

    // Initialise the (remaining of the) context
    initAirinvService();
  }

  // ////////////////////////////////////////////////////////////////////
  AIRINV_Service::~AIRINV_Service() {
    // Delete/Clean all the objects from memory
    finalise();
  }

  // ////////////////////////////////////////////////////////////////////
  void AIRINV_Service::finalise() {
    assert (_airinvServiceContext != NULL);
    // Reset the (Boost.)Smart pointer pointing on the STDAIR_Service object.
    _airinvServiceContext->reset();
  }

  // ////////////////////////////////////////////////////////////////////
  void AIRINV_Service::initServiceContext() {
    // Initialise the context
    AIRINV_ServiceContext& lAIRINV_ServiceContext = 
      FacAirinvServiceContext::instance().create();
    _airinvServiceContext = &lAIRINV_ServiceContext;
  }

  // ////////////////////////////////////////////////////////////////////
  void AIRINV_Service::
  addStdAirService (stdair::STDAIR_ServicePtr_T ioSTDAIR_Service_ptr,
                    const bool iOwnStdairService) {

    // Retrieve the Airinv service context
    assert (_airinvServiceContext != NULL);
    AIRINV_ServiceContext& lAIRINV_ServiceContext = *_airinvServiceContext;

    // Store the STDAIR service object within the (AIRINV) service context
    lAIRINV_ServiceContext.setSTDAIR_Service (ioSTDAIR_Service_ptr,
                                              iOwnStdairService);
  }
  
  // //////////////////////////////////////////////////////////////////////
  stdair::STDAIR_ServicePtr_T AIRINV_Service::
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
  stdair::STDAIR_ServicePtr_T AIRINV_Service::
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
  void AIRINV_Service::initRMOLService() {

    // Retrieve the AirInv service context
    assert (_airinvServiceContext != NULL);
    AIRINV_ServiceContext& lAIRINV_ServiceContext = *_airinvServiceContext;

    // Retrieve the StdAir service context
    stdair::STDAIR_ServicePtr_T lSTDAIR_Service_ptr =
      lAIRINV_ServiceContext.getSTDAIR_ServicePtr();

    /**
     * Initialise the RMOL service handler.
     *
     * \note The (Boost.)Smart Pointer keeps track of the references
     *       on the Service object, and deletes that object when it is
     *       no longer referenced (e.g., at the end of the process).
     */
    const stdair::CabinCapacity_T lCabinCapacity = 100.0;
    RMOL::RMOL_ServicePtr_T lRMOL_Service_ptr = 
      boost::make_shared<RMOL::RMOL_Service> (lSTDAIR_Service_ptr,
                                              lCabinCapacity);
    
    // Store the RMOL service object within the (AIRINV) service context
    lAIRINV_ServiceContext.setRMOL_Service (lRMOL_Service_ptr);
  }
  
  // ////////////////////////////////////////////////////////////////////
  void AIRINV_Service::initAirinvService() {
    // Do nothing at this stage. A sample BOM tree may be built by
    // calling the buildSampleBom() method
  }
  
  // ////////////////////////////////////////////////////////////////////
  void AIRINV_Service::
  parseAndLoad (const stdair::Filename_T& iInventoryInputFilename) {

    // Retrieve the BOM root object.
    assert (_airinvServiceContext != NULL);
    AIRINV_ServiceContext& lAIRINV_ServiceContext = *_airinvServiceContext;
    stdair::STDAIR_Service& lSTDAIR_Service =
      lAIRINV_ServiceContext.getSTDAIR_Service();
    stdair::BomRoot& lBomRoot = lSTDAIR_Service.getBomRoot();
    
    // Initialise the airline inventories
    InventoryParser::buildInventory (iInventoryInputFilename, lBomRoot);
  }
  
  // ////////////////////////////////////////////////////////////////////
  void AIRINV_Service::
  parseAndLoad (const stdair::Filename_T& iScheduleInputFilename,
                const stdair::Filename_T& iODInputFilename) {

    // Retrieve the BOM root object.
    assert (_airinvServiceContext != NULL);
    AIRINV_ServiceContext& lAIRINV_ServiceContext = *_airinvServiceContext;
    stdair::STDAIR_Service& lSTDAIR_Service =
      lAIRINV_ServiceContext.getSTDAIR_Service();
    stdair::BomRoot& lBomRoot = lSTDAIR_Service.getBomRoot();
    
    // Initialise the airline inventories
    ScheduleParser::generateInventories (iScheduleInputFilename, lBomRoot);
  }
  
  // ////////////////////////////////////////////////////////////////////
  void AIRINV_Service::buildSampleBom (const bool isForRMOL,
                                       const stdair::CabinCapacity_T iCapacity) {

    // Retrieve the AIRINV service context
    if (_airinvServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The AirInv service has not "
                                                    "been initialised");
    }
    assert (_airinvServiceContext != NULL);

    AIRINV_ServiceContext& lAIRINV_ServiceContext = *_airinvServiceContext;
  
    // Retrieve the STDAIR service object from the (AIRINV) service context
    stdair::STDAIR_Service& lSTDAIR_Service =
      lAIRINV_ServiceContext.getSTDAIR_Service();

    // Delegate the BOM building to the dedicated service
    lSTDAIR_Service.buildSampleBom (isForRMOL, iCapacity);
  }

  // ////////////////////////////////////////////////////////////////////
  std::string AIRINV_Service::csvDisplay() const {

    // Retrieve the AIRINV service context
    if (_airinvServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The AirInv service "
                                                    "has not been initialised");
    }
    assert (_airinvServiceContext != NULL);

    AIRINV_ServiceContext& lAIRINV_ServiceContext = *_airinvServiceContext;
  
    // Retrieve the STDAIR service object from the (AIRINV) service context
    stdair::STDAIR_Service& lSTDAIR_Service =
      lAIRINV_ServiceContext.getSTDAIR_Service();

    // Delegate the BOM display to the dedicated service
    return lSTDAIR_Service.csvDisplay();
  }
  
  // //////////////////////////////////////////////////////////////////////
  std::string AIRINV_Service::
  csvDisplay (const stdair::AirlineCode_T& iAirlineCode,
              const stdair::FlightNumber_T& iFlightNumber,
              const stdair::Date_T& iDepartureDate) const {

    // Retrieve the AIRINV service context
    if (_airinvServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The AirInv service "
                                                    "has not been initialised");
    }
    assert (_airinvServiceContext != NULL);

    AIRINV_ServiceContext& lAIRINV_ServiceContext = *_airinvServiceContext;
  
    // Retrieve the STDAIR service object from the (AIRINV) service context
    stdair::STDAIR_Service& lSTDAIR_Service =
      lAIRINV_ServiceContext.getSTDAIR_Service();

    // Delegate the BOM display to the dedicated service
    return lSTDAIR_Service.csvDisplay (iAirlineCode, iFlightNumber,
                                       iDepartureDate);
  }
  
  // ////////////////////////////////////////////////////////////////////
  void AIRINV_Service::
  calculateAvailability (stdair::TravelSolutionStruct& ioTravelSolution) {
    
    if (_airinvServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The AirInv service "
                                                    "has not been initialised");
    }
    assert (_airinvServiceContext != NULL);
    AIRINV_ServiceContext& lAIRINV_ServiceContext = *_airinvServiceContext;

    // Retrieve the corresponding inventory.
    stdair::STDAIR_Service& lSTDAIR_Service =
      lAIRINV_ServiceContext.getSTDAIR_Service();
    stdair::BomRoot& lBomRoot = lSTDAIR_Service.getBomRoot();

    // Delegate the booking to the dedicated command
    stdair::BasChronometer lAvlChronometer;
    lAvlChronometer.start();
    InventoryManager::calculateAvailability (lBomRoot, ioTravelSolution);
    const double lAvlMeasure = lAvlChronometer.elapsed();

    // DEBUG
    STDAIR_LOG_DEBUG ("Availability retrieval: " << lAvlMeasure << " - "
                      << lAIRINV_ServiceContext.display());
  }
  
  // ////////////////////////////////////////////////////////////////////
  bool AIRINV_Service::sell (const std::string& iSegmentDateKey,
                             const stdair::ClassCode_T& iClassCode,
                             const stdair::PartySize_T& iPartySize) {

    if (_airinvServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The AirInv service "
                                                    "has not been initialised");
    }
    assert (_airinvServiceContext != NULL);
    AIRINV_ServiceContext& lAIRINV_ServiceContext = *_airinvServiceContext;

    // Retrieve the corresponding inventory.
    stdair::STDAIR_Service& lSTDAIR_Service =
      lAIRINV_ServiceContext.getSTDAIR_Service();
    stdair::BomRoot& lBomRoot = lSTDAIR_Service.getBomRoot();
    const stdair::InventoryKey lInventoryKey =
      stdair::BomKeyManager::extractInventoryKey (iSegmentDateKey);
    stdair::Inventory& lInventory =
      stdair::BomManager::getObject<stdair::Inventory>(lBomRoot,
                                                       lInventoryKey.toString());

    // Delegate the booking to the dedicated command
    stdair::BasChronometer lSellChronometer;
    lSellChronometer.start();
    const bool saleControl = 
      InventoryManager::sell(lInventory, iSegmentDateKey,
                             iClassCode, iPartySize);
    const double lSellMeasure = lSellChronometer.elapsed();

    // DEBUG
    STDAIR_LOG_DEBUG ("Booking sell: " << lSellMeasure << " - "
                      << lAIRINV_ServiceContext.display());

    return saleControl;
  }
  
  // ////////////////////////////////////////////////////////////////////
  void AIRINV_Service::takeSnapshots (const stdair::AirlineCode_T& iAirlineCode,
                                      const stdair::DateTime_T& iSnapshotTime) {

    if (_airinvServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The AirInv service "
                                                    "has not been initialised");
    }
    assert (_airinvServiceContext != NULL);
    AIRINV_ServiceContext& lAIRINV_ServiceContext = *_airinvServiceContext;

    // TODO: Retrieve the corresponding inventory.
    stdair::STDAIR_Service& lSTDAIR_Service =
      lAIRINV_ServiceContext.getSTDAIR_Service();
    stdair::BomRoot& lBomRoot = lSTDAIR_Service.getBomRoot();

    const stdair::InventoryList_T lInventoryList =
      stdair::BomManager::getList<stdair::Inventory> (lBomRoot);
    for (stdair::InventoryList_T::const_iterator itInv = lInventoryList.begin();
         itInv != lInventoryList.end(); ++itInv) {
      const stdair::Inventory* lInv_ptr = *itInv;
      assert (lInv_ptr != NULL);
      
      InventoryManager::takeSnapshots (*lInv_ptr, iSnapshotTime); 
    }
  }
}
