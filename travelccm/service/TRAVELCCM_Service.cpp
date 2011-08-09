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
#include <stdair/bom/BookingRequestStruct.hpp> 
#include <stdair/factory/FacBomManager.hpp>
#include <stdair/service/Logger.hpp>
#include <stdair/STDAIR_Service.hpp>
// TravelCCM
#include <travelccm/factory/FacTRAVELCCMServiceContext.hpp>
#include <travelccm/command/ChoiceManager.hpp>
#include <travelccm/service/TRAVELCCM_ServiceContext.hpp>
#include <travelccm/TRAVELCCM_Service.hpp>

namespace TRAVELCCM {

  // ////////////////////////////////////////////////////////////////////
  TRAVELCCM_Service::TRAVELCCM_Service() : _travelccmServiceContext (NULL) {
    assert (false);
  }

  // ////////////////////////////////////////////////////////////////////
  TRAVELCCM_Service::TRAVELCCM_Service (const TRAVELCCM_Service& iService)
  : _travelccmServiceContext (NULL) {
    assert (false);
  }

  // ////////////////////////////////////////////////////////////////////
  TRAVELCCM_Service::TRAVELCCM_Service (const stdair::BasLogParams& iLogParams,
                                        const stdair::BasDBParams& iDBParams)
    : _travelccmServiceContext (NULL) {
    
    // Initialise the STDAIR service handler
    stdair::STDAIR_ServicePtr_T lSTDAIR_Service_ptr =
      initStdAirService (iLogParams, iDBParams);
    
    // Initialise the service context
    initServiceContext();

    // Add the StdAir service context to the AIRINV service context
    // \note AIRINV owns the STDAIR service resources here.
    const bool ownStdairService = true;
    addStdAirService (lSTDAIR_Service_ptr, ownStdairService);
    
    // Initialise the (remaining of the) context
    initTravelCCMService();
  }

  // ////////////////////////////////////////////////////////////////////
  TRAVELCCM_Service::TRAVELCCM_Service (const stdair::BasLogParams& iLogParams)
    : _travelccmServiceContext (NULL) {
    
    // Initialise the STDAIR service handler
    stdair::STDAIR_ServicePtr_T lSTDAIR_Service_ptr =
      initStdAirService (iLogParams);
    
    // Initialise the service context
    initServiceContext();

    // Add the StdAir service context to the AIRINV service context
    // \note AIRINV owns the STDAIR service resources here.
    const bool ownStdairService = true;
    addStdAirService (lSTDAIR_Service_ptr, ownStdairService);
    
    // Initialise the (remaining of the) context
    initTravelCCMService();
  }

  // ////////////////////////////////////////////////////////////////////
  TRAVELCCM_Service::
  TRAVELCCM_Service (stdair::STDAIR_ServicePtr_T ioSTDAIR_Service_ptr)
    : _travelccmServiceContext (NULL) {
        
    // Initialise the service context
    initServiceContext();

    // Store the STDAIR service object within the (AIRINV) service context
    // \note AirInv does not own the STDAIR service resources here.
    const bool doesNotOwnStdairService = false;
    addStdAirService (ioSTDAIR_Service_ptr, doesNotOwnStdairService);

    // Initialise the (remaining of the) context
    initTravelCCMService();
  }

  // ////////////////////////////////////////////////////////////////////
  TRAVELCCM_Service::~TRAVELCCM_Service() {
    // Delete/Clean all the objects from memory
    finalise();
  }
  
  // ////////////////////////////////////////////////////////////////////
  void TRAVELCCM_Service::finalise() {
    assert (_travelccmServiceContext != NULL);
    // Reset the (Boost.)Smart pointer pointing on the STDAIR_Service object.
    _travelccmServiceContext->reset();
  }

  // ////////////////////////////////////////////////////////////////////
  void TRAVELCCM_Service::initServiceContext() {
    // Initialise the context
    TRAVELCCM_ServiceContext& lTRAVELCCM_ServiceContext = 
      FacTRAVELCCMServiceContext::instance().create();
    _travelccmServiceContext = &lTRAVELCCM_ServiceContext;
  }
  
  // ////////////////////////////////////////////////////////////////////
  void TRAVELCCM_Service::
  addStdAirService (stdair::STDAIR_ServicePtr_T ioSTDAIR_Service_ptr,
                    const bool iOwnStdairService) {
    // Retrieve the Travelccm service context
    assert (_travelccmServiceContext != NULL);
    TRAVELCCM_ServiceContext& lTRAVELCCM_ServiceContext =
      *_travelccmServiceContext;

    // Store the STDAIR service object within the (TRAVELCCM) service context
    lTRAVELCCM_ServiceContext.setSTDAIR_Service (ioSTDAIR_Service_ptr,
                                                 iOwnStdairService);
  }

  // ////////////////////////////////////////////////////////////////////
  stdair::STDAIR_ServicePtr_T TRAVELCCM_Service::
  initStdAirService (const stdair::BasLogParams& iLogParams,
                     const stdair::BasDBParams& iDBParams) {

    /**
     * Initialise the STDAIR service handler.
     *
     * \note The track on the object memory is kept thanks to the Boost
     * Smart Pointers component.
     */
    stdair::STDAIR_ServicePtr_T oSTDAIR_Service_ptr = 
      boost::make_shared<stdair::STDAIR_Service> (iLogParams, iDBParams);
    assert (oSTDAIR_Service_ptr != NULL);

    return oSTDAIR_Service_ptr;
  }
  
  // ////////////////////////////////////////////////////////////////////
  stdair::STDAIR_ServicePtr_T TRAVELCCM_Service::
  initStdAirService (const stdair::BasLogParams& iLogParams) {

    /**
     * Initialise the STDAIR service handler.
     *
     * \note The track on the object memory is kept thanks to the Boost
     * Smart Pointers component.
     */
    stdair::STDAIR_ServicePtr_T oSTDAIR_Service_ptr = 
      boost::make_shared<stdair::STDAIR_Service> (iLogParams);
    assert (oSTDAIR_Service_ptr != NULL);

    return oSTDAIR_Service_ptr;
  }
  
  // ////////////////////////////////////////////////////////////////////
  void TRAVELCCM_Service::initTravelCCMService() {
    // Do nothing at this stage. A sample BOM tree may be built by
    // calling the buildSampleBom() method
  }
  
  // //////////////////////////////////////////////////////////////////////
  void TRAVELCCM_Service::buildSampleBom() {

    // Retrieve the TravelCCM service context
    if (_travelccmServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The TravelCCM service has "
                                                    "not been initialised");
    }
    assert (_travelccmServiceContext != NULL);

    // Retrieve the TravelCCM service context and whether it owns the Stdair
    // service
    TRAVELCCM_ServiceContext& lTRAVELCCM_ServiceContext =
      *_travelccmServiceContext;
    const bool doesOwnStdairService =
      lTRAVELCCM_ServiceContext.getOwnStdairServiceFlag();

    // Retrieve the StdAir service object from the (TravelCCM) service context
    stdair::STDAIR_Service& lSTDAIR_Service =
      lTRAVELCCM_ServiceContext.getSTDAIR_Service();

    /**
     * 1. Have StdAir build the whole BOM tree, only when the StdAir service is
     *    owned by the current component (TravelCCM here)
     */
    if (doesOwnStdairService == true) {
      //
      lSTDAIR_Service.buildSampleBom();
    }

    /**
     * 2. Delegate the complementary building of objects and links by the
     *    appropriate levels/components
     * 
     * \note: Currently, no more things to do by TravelCCM at that stage,
     *        as there is no child
     */

    /**
     * 3. Build the complementary objects/links for the current component (here,
     *    TravelCCM)
     *
     * \note: Currently, no more things to do by TravelCCM at that stage.
     */
  }

  // //////////////////////////////////////////////////////////////////////
  void TRAVELCCM_Service::
  buildSampleTravelSolutions (stdair::TravelSolutionList_T& ioTSList) {

    // Retrieve the TRAVELCCM service context
    if (_travelccmServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The Travelccm service has "
                                                    "not been initialised");
    }
    assert (_travelccmServiceContext != NULL);

    TRAVELCCM_ServiceContext& lTRAVELCCM_ServiceContext =
      *_travelccmServiceContext;
  
    // Retrieve the STDAIR service object from the (TRAVELCCM) service context
    stdair::STDAIR_Service& lSTDAIR_Service =
      lTRAVELCCM_ServiceContext.getSTDAIR_Service();

    // Delegate the BOM building to the dedicated service
    lSTDAIR_Service.buildSampleTravelSolutions (ioTSList);
  }

  // //////////////////////////////////////////////////////////////////////
  stdair::BookingRequestStruct TRAVELCCM_Service::
  buildSampleBookingRequest (const bool isForCRS) {

    // Retrieve the TRAVELCCM service context
    if (_travelccmServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The Travelccm service has "
                                                    "not been initialised");
    }
    assert (_travelccmServiceContext != NULL);

    TRAVELCCM_ServiceContext& lTRAVELCCM_ServiceContext =
      *_travelccmServiceContext;
  
    // Retrieve the STDAIR service object from the (TRAVELCCM) service context
    stdair::STDAIR_Service& lSTDAIR_Service =
      lTRAVELCCM_ServiceContext.getSTDAIR_Service();

    // Delegate the BOM building to the dedicated service
    return lSTDAIR_Service.buildSampleBookingRequest (isForCRS);
  }

  // //////////////////////////////////////////////////////////////////////
  std::string TRAVELCCM_Service::csvDisplay() const {

    // Retrieve the TRAVELCCM service context
    if (_travelccmServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The TravelccmMaster service"
                                                    " has not been initialised");
    }
    assert (_travelccmServiceContext != NULL);

    TRAVELCCM_ServiceContext& lTRAVELCCM_ServiceContext =
      *_travelccmServiceContext;
  
    // Retrieve the STDAIR service object from the (TRAVELCCM) service context
    stdair::STDAIR_Service& lSTDAIR_Service =
      lTRAVELCCM_ServiceContext.getSTDAIR_Service();

    // Delegate the BOM building to the dedicated service
    return lSTDAIR_Service.csvDisplay();
  }
  
  // //////////////////////////////////////////////////////////////////////
  std::string TRAVELCCM_Service::
  csvDisplay (const stdair::TravelSolutionList_T& iTravelSolutionList) const {
    // Retrieve the TRAVELCCM service context
    if (_travelccmServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The TravelccmMaster service"
                                                    " has not been initialised");
    }
    assert (_travelccmServiceContext != NULL);

    TRAVELCCM_ServiceContext& lTRAVELCCM_ServiceContext =
      *_travelccmServiceContext;
  
    // Retrieve the STDAIR service object from the (TRAVELCCM) service context
    stdair::STDAIR_Service& lSTDAIR_Service =
      lTRAVELCCM_ServiceContext.getSTDAIR_Service();

    // Delegate the BOM building to the dedicated service
    return lSTDAIR_Service.csvDisplay (iTravelSolutionList);
  }

  // ////////////////////////////////////////////////////////////////////
  const stdair::TravelSolutionStruct* TRAVELCCM_Service::
  chooseTravelSolution (stdair::TravelSolutionList_T& ioTravelSolutionList,
                        const stdair::BookingRequestStruct& iBookingRequest) {

    const stdair::TravelSolutionStruct* oTravelSolution_ptr = 
      ChoiceManager::chooseTravelSolution (ioTravelSolutionList,
                                           iBookingRequest);
    return oTravelSolution_ptr;
  }
  
}
