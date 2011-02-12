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
#include <stdair/bom/YieldStore.hpp>
#include <stdair/factory/FacBomManager.hpp>
#include <stdair/service/Logger.hpp>
#include <stdair/STDAIR_Service.hpp>
// Airrac
#include <airrac/basic/BasConst_AIRRAC_Service.hpp>
#include <airrac/factory/FacAirracServiceContext.hpp>
#include <airrac/command/YieldParser.hpp>
#include <airrac/command/YieldManager.hpp>
#include <airrac/service/AIRRAC_ServiceContext.hpp>
#include <airrac/AIRRAC_Service.hpp>

namespace AIRRAC {

  // //////////////////////////////////////////////////////////////////////
  AIRRAC_Service::AIRRAC_Service () : _airracServiceContext (NULL) {
    assert (false);
  }

  // //////////////////////////////////////////////////////////////////////
  AIRRAC_Service::AIRRAC_Service (const AIRRAC_Service& iService)
  : _airracServiceContext (NULL) {
    assert (false);
  }

  // //////////////////////////////////////////////////////////////////////
  AIRRAC_Service::
  AIRRAC_Service (stdair::STDAIR_ServicePtr_T ioSTDAIR_Service_ptr,
                  const stdair::AirlineCode_T& iAirlineCode)
    : _airracServiceContext (NULL) {

    // Retrieve the YieldStore object, at the root of the BOM tree, on
    // which all of the other BOM objects of the airline yield will be
    // attached
    assert (ioSTDAIR_Service_ptr != NULL);
    stdair::BomRoot& lBomRoot = ioSTDAIR_Service_ptr->getBomRoot();
    stdair::YieldStore* lYieldStore_ptr = stdair::BomManager::
      getObjectPtr<stdair::YieldStore> (lBomRoot, iAirlineCode);
    assert (lYieldStore_ptr != NULL);
            
    // Initialise the service context
    initServiceContext (iAirlineCode, *lYieldStore_ptr);

    // Retrieve the Airrac service context
    assert (_airracServiceContext != NULL);
    AIRRAC_ServiceContext& lAIRRAC_ServiceContext = *_airracServiceContext;
    
    // Store the STDAIR service object within the (AIRRAC) service context
    lAIRRAC_ServiceContext.setSTDAIR_Service (ioSTDAIR_Service_ptr);
    
    // Initialise the context
    init ();
  }

  // //////////////////////////////////////////////////////////////////////
  AIRRAC_Service::
  AIRRAC_Service (const stdair::BasLogParams& iLogParams,
                  const stdair::BasDBParams& iDBParams,
                  const stdair::AirlineCode_T& iAirlineCode,
                  const stdair::Filename_T& iYieldStoreInputFilename)
    : _airracServiceContext (NULL) {
    
    // Initialise the STDAIR service handler
    stdair::STDAIR_ServicePtr_T lSTDAIR_Service_ptr =
      initStdAirService (iLogParams, iDBParams);
    
    // Create a Yield Store object, at the root of the BOM tree, and on
    // which all of the other BOM objects of the airline yield store will be
    // attached
    assert (lSTDAIR_Service_ptr != NULL);
    stdair::BomRoot& lBomRoot = lSTDAIR_Service_ptr->getBomRoot();
    stdair::YieldStoreKey lKey (iAirlineCode);
    stdair::YieldStore& lYieldStore =
      stdair::FacBom<stdair::YieldStore>::instance().create (lKey);
    stdair::FacBomManager::instance().addToListAndMap (lBomRoot, lYieldStore);
    stdair::FacBomManager::instance().linkWithParent (lBomRoot, lYieldStore);

    // Initialise the service context
    initServiceContext (iAirlineCode, lYieldStore);

    // Add the StdAir service context to the AIRRAC service context
    addStdAirService (lSTDAIR_Service_ptr);
    
    // Initialise the (remaining of the) context
    init (iYieldStoreInputFilename);
  }

  // //////////////////////////////////////////////////////////////////////
  AIRRAC_Service::
  AIRRAC_Service (const stdair::BasLogParams& iLogParams,
                  const stdair::AirlineCode_T& iAirlineCode,
                  const stdair::Filename_T& iYieldStoreInputFilename)
    : _airracServiceContext (NULL) {
    
    // Initialise the STDAIR service handler
    stdair::STDAIR_ServicePtr_T lSTDAIR_Service_ptr =
      initStdAirService (iLogParams);
    
    // Create a YieldStore object, at the root of the BOM tree, and on
    // which all of the other BOM objects of the airline yield store will be
    // attached
    assert (lSTDAIR_Service_ptr != NULL);
    stdair::BomRoot& lBomRoot = lSTDAIR_Service_ptr->getBomRoot();
    stdair::YieldStoreKey lKey (iAirlineCode);
    stdair::YieldStore& lYieldStore =
      stdair::FacBom<stdair::YieldStore>::instance().create (lKey);
    stdair::FacBomManager::instance().addToListAndMap (lBomRoot, lYieldStore);
    stdair::FacBomManager::instance().linkWithParent (lBomRoot, lYieldStore);

    // Initialise the service context
    initServiceContext (iAirlineCode, lYieldStore);
    
    // Add the StdAir service context to the AIRRAC service context
    addStdAirService (lSTDAIR_Service_ptr);
    
    // Initialise the (remaining of the) context
    init (iYieldStoreInputFilename);
  }

  // //////////////////////////////////////////////////////////////////////
  AIRRAC_Service::~AIRRAC_Service () {
    // Delete/Clean all the objects from memory
    finalise();
  }

  // //////////////////////////////////////////////////////////////////////
  void AIRRAC_Service::
  initServiceContext (const stdair::AirlineCode_T& iAirlineCode,
                      stdair::YieldStore& ioYieldStore) {
    // Initialise the context
    AIRRAC_ServiceContext& lAIRRAC_ServiceContext = 
      FacAirracServiceContext::instance().create (iAirlineCode, ioYieldStore);
    _airracServiceContext = &lAIRRAC_ServiceContext;
  }

  // //////////////////////////////////////////////////////////////////////
  void AIRRAC_Service::
  addStdAirService (stdair::STDAIR_ServicePtr_T ioSTDAIR_Service_ptr) {

    // Retrieve the Airrac service context
    assert (_airracServiceContext != NULL);
    AIRRAC_ServiceContext& lAIRRAC_ServiceContext = *_airracServiceContext;

    // Store the STDAIR service object within the (AIRRAC) service context
    lAIRRAC_ServiceContext.setSTDAIR_Service (ioSTDAIR_Service_ptr);
  }
  
  // //////////////////////////////////////////////////////////////////////
  stdair::STDAIR_ServicePtr_T AIRRAC_Service::
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
  stdair::STDAIR_ServicePtr_T AIRRAC_Service::
  initStdAirService (const stdair::BasLogParams& iLogParams) {

    // Initialise the STDAIR service handler
    // Note that the track on the object memory is kept thanks to the Boost
    // Smart Pointers component.
    stdair::STDAIR_ServicePtr_T lSTDAIR_Service_ptr = 
      boost::make_shared<stdair::STDAIR_Service> (iLogParams);

    return lSTDAIR_Service_ptr;
  }
  
  // //////////////////////////////////////////////////////////////////////
  void AIRRAC_Service::init () {
  }
  
  // //////////////////////////////////////////////////////////////////////
  void AIRRAC_Service::
  init (const stdair::Filename_T& iYieldStoreInputFilename) {
    // Check that the file path given as input corresponds to an actual file
    const bool doesExistAndIsReadable =
      stdair::BasFileMgr::doesExistAndIsReadable (iYieldStoreInputFilename);
    if (doesExistAndIsReadable == false) {
      std::ostringstream oMessage;
      oMessage << "The yield store input file, '"
               << iYieldStoreInputFilename
               << "', can not be retrieved on the file-system";
      throw stdair::FileNotFoundException (oMessage.str());
    }

    // Retrieve the Airrac service context
    assert (_airracServiceContext != NULL);
    AIRRAC_ServiceContext& lAIRRAC_ServiceContext = *_airracServiceContext;

    // Retrieve the StdAir service context
    stdair::STDAIR_ServicePtr_T lSTDAIR_Service =
      lAIRRAC_ServiceContext.getSTDAIR_ServicePtr();
    assert (lSTDAIR_Service != NULL);
    
    // Get the root of the BOM tree, on which all of the other BOM objects
    // will be attached
    stdair::BomRoot& lBomRoot = lSTDAIR_Service->getBomRoot();

    // Initialise the airline yield store
    YieldParser::generateYieldStore (iYieldStoreInputFilename, lBomRoot);

    // DEBUG
    // STDAIR_LOG_DEBUG ("Generated BomRoot:");
    // std::ostringstream oStream;
    // stdair::BomManager::display (oStream, lBomRoot);
    // STDAIR_LOG_DEBUG (oStream.str());
  }
  
  // //////////////////////////////////////////////////////////////////////
  void AIRRAC_Service::finalise () {
    assert (_airracServiceContext != NULL);
  }

  // //////////////////////////////////////////////////////////////////////
  void AIRRAC_Service::
  calculateYield (const stdair::TravelSolutionStruct& iTravelSolution) {
    
    if (_airracServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The AirRAC service has "
                                            "not been initialised");
    }
    assert (_airracServiceContext != NULL);
    AIRRAC_ServiceContext& lAIRRAC_ServiceContext = *_airracServiceContext;

    try {
      
      // Retrieve the airline yield store
      stdair::YieldStore& lYieldStore = lAIRRAC_ServiceContext.getYieldStore();
      
      // Delegate the booking to the dedicated command
      stdair::BasChronometer lCalculationChronometer;
      lCalculationChronometer.start();
      YieldManager::calculateYield (lYieldStore, iTravelSolution);
      const double lCalculationMeasure = lCalculationChronometer.elapsed();
      
      // DEBUG
      STDAIR_LOG_DEBUG ("Yield calculation: " << lCalculationMeasure << " - "
                        << lAIRRAC_ServiceContext.display());

    } catch (const std::exception& error) {
      STDAIR_LOG_ERROR ("Exception: "  << error.what());
      throw YielCalculationException();
    }
  }
  
}
