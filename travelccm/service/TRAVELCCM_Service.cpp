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
#include <stdair/STDAIR_Service.hpp>
#include <stdair/service/Logger.hpp>
// TravelCCM
#include <travelccm/factory/FacTRAVELCCMServiceContext.hpp>
#include <travelccm/command/ChoiceManager.hpp>
#include <travelccm/service/TRAVELCCM_ServiceContext.hpp>
#include <travelccm/TRAVELCCM_Service.hpp>

namespace TRAVELCCM {
  // ////////////////////////////////////////////////////////////////////
  TRAVELCCM_Service::TRAVELCCM_Service () : _travelccmServiceContext (NULL) {
    assert (false);
  }

  // ////////////////////////////////////////////////////////////////////
  TRAVELCCM_Service::TRAVELCCM_Service (const TRAVELCCM_Service& iService)
  : _travelccmServiceContext (NULL) {
    assert (false);
  }

  // ////////////////////////////////////////////////////////////////////
  TRAVELCCM_Service::TRAVELCCM_Service (const stdair::BasLogParams& iLogParams)
    : _travelccmServiceContext (NULL) {
    
    // Initialise the STDAIR service handler
    stdair::STDAIR_ServicePtr_T lSTDAIR_Service_ptr =
      initStdAirService (iLogParams);
    
    // Initialise the service context
    initServiceContext ();

    // Add the StdAir service context to the TRAVELCCM service context
    addStdAirService (lSTDAIR_Service_ptr);
  }

  // ////////////////////////////////////////////////////////////////////
  TRAVELCCM_Service::
  TRAVELCCM_Service (stdair::STDAIR_ServicePtr_T ioSTDAIR_Service_ptr)
    : _travelccmServiceContext (NULL) {
        
    // Initialise the service context
    initServiceContext ();

    // Add the StdAir service context to the TRAVELCCM service context
    addStdAirService (ioSTDAIR_Service_ptr);
  }

  // ////////////////////////////////////////////////////////////////////
  TRAVELCCM_Service::~TRAVELCCM_Service () {
    // Delete/Clean all the objects from memory
    finalise();
  }
  
  // ////////////////////////////////////////////////////////////////////
  void TRAVELCCM_Service::finalise () {

  }

  // ////////////////////////////////////////////////////////////////////
  void TRAVELCCM_Service::initServiceContext () {
    // Initialise the context
    TRAVELCCM_ServiceContext& lTRAVELCCM_ServiceContext = 
      FacTRAVELCCMServiceContext::instance().create ();
    _travelccmServiceContext = &lTRAVELCCM_ServiceContext;
  }
  
  // ////////////////////////////////////////////////////////////////////
  stdair::STDAIR_ServicePtr_T TRAVELCCM_Service::
  initStdAirService (const stdair::BasLogParams& iLogParams) {

    // Retrieve the Travelccm service context
    assert (_travelccmServiceContext != NULL);
    TRAVELCCM_ServiceContext& lTRAVELCCM_ServiceContext =
      *_travelccmServiceContext;
    
    /**
       Initialise the STDAIR service handler.
       \note The track on the object memory is kept thanks to the Boost
       Smart Pointers component.
    */
    stdair::STDAIR_ServicePtr_T oSTDAIR_Service_ptr = 
      boost::make_shared<stdair::STDAIR_Service> (iLogParams);
    assert (oSTDAIR_Service_ptr != NULL);

    return oSTDAIR_Service_ptr;
  }
  
  // ////////////////////////////////////////////////////////////////////
  void TRAVELCCM_Service::
  addStdAirService (stdair::STDAIR_ServicePtr_T ioSTDAIR_Service_ptr) {
    // Retrieve the Travelccm service context
    assert (_travelccmServiceContext != NULL);
    TRAVELCCM_ServiceContext& lTRAVELCCM_ServiceContext =
      *_travelccmServiceContext;

    // Store the STDAIR service object within the (TRAVELCCM) service context
    lTRAVELCCM_ServiceContext.setSTDAIR_Service (ioSTDAIR_Service_ptr);
  }

  // ////////////////////////////////////////////////////////////////////
  const stdair::TravelSolutionStruct* TRAVELCCM_Service::
  chooseTravelSolution (stdair::TravelSolutionList_T& ioTravelSolutionList,
                        const stdair::BookingRequestStruct& iBookingRequest) {

    return ChoiceManager::chooseTravelSolution (ioTravelSolutionList,
                                                iBookingRequest);
  }
  
}
