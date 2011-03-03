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
  AIRINV_Service::AIRINV_Service () : _airinvServiceContext (NULL) {
    assert (false);
  }

  // ////////////////////////////////////////////////////////////////////
  AIRINV_Service::AIRINV_Service (const AIRINV_Service& iService)
  : _airinvServiceContext (NULL) {
    assert (false);
  }

  // ////////////////////////////////////////////////////////////////////
  AIRINV_Service::
  AIRINV_Service (const stdair::Filename_T& iInventoryInputFilename)
    : _airinvServiceContext (NULL) {
    
    // Initialise the STDAIR service handler
    stdair::STDAIR_ServicePtr_T lSTDAIR_Service_ptr = initStdAirService ();
    
    // Initialise the service context
    initServiceContext ();

    // Add the StdAir service context to the AIRINV service context
    addStdAirService (lSTDAIR_Service_ptr);

    // Initalise the RMOL service.
    initRMOLService (lSTDAIR_Service_ptr);
    
    // Initialise the (remaining of the) context
    init (iInventoryInputFilename);
  }

  // ////////////////////////////////////////////////////////////////////
  AIRINV_Service::
  AIRINV_Service (const stdair::Filename_T& iScheduleInputFilename,
                  const stdair::Filename_T& iODInputFilename)
    : _airinvServiceContext (NULL) {
    
    // Initialise the STDAIR service handler
    stdair::STDAIR_ServicePtr_T lSTDAIR_Service_ptr = initStdAirService ();
    
    // Initialise the service context
    initServiceContext ();

    // Add the StdAir service context to the AIRINV service context
    addStdAirService (lSTDAIR_Service_ptr);

    // Initalise the RMOL service.
    initRMOLService (lSTDAIR_Service_ptr);
    
    // Initialise the (remaining of the) context
    init (iScheduleInputFilename, iODInputFilename);
  }

  // ////////////////////////////////////////////////////////////////////
  AIRINV_Service::~AIRINV_Service () {
    // Delete/Clean all the objects from memory
    finalise();
  }

  // ////////////////////////////////////////////////////////////////////
  void AIRINV_Service::initServiceContext () {
    // Initialise the context
    AIRINV_ServiceContext& lAIRINV_ServiceContext = 
      FacAirinvServiceContext::instance().create ();
    _airinvServiceContext = &lAIRINV_ServiceContext;
  }

  // ////////////////////////////////////////////////////////////////////
  void AIRINV_Service::
  addStdAirService (stdair::STDAIR_ServicePtr_T ioSTDAIR_Service_ptr) {
    // Retrieve the Airinv service context
    assert (_airinvServiceContext != NULL);
    AIRINV_ServiceContext& lAIRINV_ServiceContext = *_airinvServiceContext;

    // Store the STDAIR service object within the (AIRINV) service context
    lAIRINV_ServiceContext.setSTDAIR_Service (ioSTDAIR_Service_ptr);
  }
  
  // ////////////////////////////////////////////////////////////////////
  stdair::STDAIR_ServicePtr_T AIRINV_Service::initStdAirService () {
    // Initialise the STDAIR service handler
    // Note that the track on the object memory is kept thanks to the Boost
    // Smart Pointers component.
    stdair::STDAIR_ServicePtr_T lSTDAIR_Service_ptr = 
      boost::make_shared<stdair::STDAIR_Service> ();
    
    return lSTDAIR_Service_ptr;
  }

  // ////////////////////////////////////////////////////////////////////
  void AIRINV_Service::
  initRMOLService (stdair::STDAIR_ServicePtr_T ioStdAir_Service) {
    // Initialise the RMOL service handler
    // Note that the track on the object memory is kept thanks to the Boost
    // Smart Pointers component.
    RMOL::RMOL_ServicePtr_T lRMOL_Service_ptr = 
      boost::make_shared<RMOL::RMOL_Service> (ioStdAir_Service);
    
    // Retrieve the Airinv service context
    assert (_airinvServiceContext != NULL);
    AIRINV_ServiceContext& lAIRINV_ServiceContext = *_airinvServiceContext;

    // Store the RMOL service object within the (AIRINV) service context
    lAIRINV_ServiceContext.setRMOL_Service (lRMOL_Service_ptr);
  }
  
  // ////////////////////////////////////////////////////////////////////
  void AIRINV_Service::
  init (const stdair::Filename_T& iInventoryInputFilename) {
    // Check that the file path given as input corresponds to an actual file
    const bool doesExistAndIsReadable =
      stdair::BasFileMgr::doesExistAndIsReadable (iInventoryInputFilename);
    if (doesExistAndIsReadable == false) {
      std::ostringstream oMessage;
      oMessage << "The inventory input file, '" << iInventoryInputFilename
               << "', can not be retrieved on the file-system";
      throw stdair::FileNotFoundException (oMessage.str());
    }

    // Retrieve the bom root object.
    assert (_airinvServiceContext != NULL);
    AIRINV_ServiceContext& lAIRINV_ServiceContext = *_airinvServiceContext;
    stdair::STDAIR_Service& lSTDAIR_Service =
      lAIRINV_ServiceContext.getSTDAIR_Service ();
    stdair::BomRoot& lBomRoot = lSTDAIR_Service.getBomRoot ();
    
    // Initialise the airline inventories
    InventoryParser::buildInventory (iInventoryInputFilename, lBomRoot);
  }
  
  // ////////////////////////////////////////////////////////////////////
  void AIRINV_Service::
  init (const stdair::Filename_T& iScheduleInputFilename,
        const stdair::Filename_T& iODInputFilename) {
    // Check that the file path given as input corresponds to an actual file
    bool doesExistAndIsReadable =
      stdair::BasFileMgr::doesExistAndIsReadable (iScheduleInputFilename);
    if (doesExistAndIsReadable == false) {
      std::ostringstream oMessage;
      oMessage << "The inventory input file, '" << iScheduleInputFilename
               << "', can not be retrieved on the file-system";
      throw stdair::FileNotFoundException (oMessage.str());
    }
    doesExistAndIsReadable =
      stdair::BasFileMgr::doesExistAndIsReadable (iODInputFilename);
    if (doesExistAndIsReadable == false) {
      std::ostringstream oMessage;
      oMessage << "The inventory input file, '" << iODInputFilename
               << "', can not be retrieved on the file-system";
      throw stdair::FileNotFoundException (oMessage.str());
    }

    // Retrieve the inventory object.
    assert (_airinvServiceContext != NULL);
    AIRINV_ServiceContext& lAIRINV_ServiceContext = *_airinvServiceContext;
    stdair::STDAIR_Service& lSTDAIR_Service =
      lAIRINV_ServiceContext.getSTDAIR_Service ();
    stdair::BomRoot& lBomRoot = lSTDAIR_Service.getBomRoot ();
    
    // Initialise the airline inventories
    ScheduleParser::generateInventories (iScheduleInputFilename, lBomRoot);
  }
  
  // ////////////////////////////////////////////////////////////////////
  void AIRINV_Service::finalise () {
    assert (_airinvServiceContext != NULL);
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

    // Delegate the booking to the dedicated command
    stdair::BasChronometer lSellChronometer;
    lSellChronometer.start();
    const bool saleControl = true;
    // InventoryManager::sell (lInventory, iSegmentDateKey,
    //                         iClassCode, iPartySize);
    const double lSellMeasure = lSellChronometer.elapsed();
      
    // DEBUG
    STDAIR_LOG_DEBUG ("Booking sell: " << lSellMeasure << " - "
                      << lAIRINV_ServiceContext.display());

    return saleControl;
  }
  
}
