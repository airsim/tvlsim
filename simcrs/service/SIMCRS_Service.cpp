// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <sstream>
// Boost
#include <boost/make_shared.hpp>
// Standard Airline Object Model
#include <stdair/stdair_exceptions.hpp>
#include <stdair/stdair_basic_types.hpp>
#include <stdair/basic/BasChronometer.hpp>
#include <stdair/basic/BasFileMgr.hpp>
#include <stdair/bom/BomManager.hpp> 
#include <stdair/bom/BookingRequestStruct.hpp>
#include <stdair/bom/TravelSolutionStruct.hpp>
#include <stdair/bom/CancellationStruct.hpp>
#include <stdair/bom/BomRoot.hpp>
#include <stdair/bom/Inventory.hpp>
#include <stdair/service/Logger.hpp>
#include <stdair/STDAIR_Service.hpp>
// Airline Inventory
#include <airinv/AIRINV_Master_Service.hpp>
// Airline Schedule
#include <airsched/AIRSCHED_Service.hpp>
// Fare Quote
#include <simfqt/SIMFQT_Service.hpp>
// SimCRS
#include <simcrs/basic/BasConst_SIMCRS_Service.hpp>
#include <simcrs/command/DistributionManager.hpp>
#include <simcrs/factory/FacSimcrsServiceContext.hpp>
#include <simcrs/service/SIMCRS_ServiceContext.hpp>
#include <simcrs/SIMCRS_Service.hpp>

namespace SIMCRS {

  // ////////////////////////////////////////////////////////////////////
  SIMCRS_Service::SIMCRS_Service() : _simcrsServiceContext (NULL) {
    assert (false);
  }

  // ////////////////////////////////////////////////////////////////////
  SIMCRS_Service::SIMCRS_Service (const SIMCRS_Service& iService) {
    assert (false);
  }

  // ////////////////////////////////////////////////////////////////////
  SIMCRS_Service::SIMCRS_Service (const stdair::BasLogParams& iLogParams,
                                  const CRSCode_T& iCRSCode)
    : _simcrsServiceContext (NULL) {
    
    // Initialise the StdAir service handler
    stdair::STDAIR_ServicePtr_T lSTDAIR_Service_ptr =
      initStdAirService (iLogParams);
    
    // Initialise the service context
    initServiceContext (iCRSCode);
    
    // Add the StdAir service context to the SimCRS service context
    // \note SIMCRS owns the STDAIR service resources here.
    const bool ownStdairService = true;
    addStdAirService (lSTDAIR_Service_ptr, ownStdairService);

    // Initalise the SimFQT service.
    initSIMFQTService();
    
    // Initalise the AirSched service.
    initAIRSCHEDService();
    
    // Initalise the AirInv service.
    initAIRINVService();
    
    // Initialise the (remaining of the) context
    initSimcrsService();
  }

  // ////////////////////////////////////////////////////////////////////
  SIMCRS_Service::SIMCRS_Service (const stdair::BasLogParams& iLogParams,
                                  const stdair::BasDBParams& iDBParams,
                                  const CRSCode_T& iCRSCode)
    : _simcrsServiceContext (NULL) {
    
    // Initialise the STDAIR service handler
    stdair::STDAIR_ServicePtr_T lSTDAIR_Service_ptr =
      initStdAirService (iLogParams, iDBParams);
    
    // Initialise the service context
    initServiceContext (iCRSCode);
    
    // Add the StdAir service context to the SIMCRS service context
    // \note SIMCRS owns the STDAIR service resources here.
    const bool ownStdairService = true;
    addStdAirService (lSTDAIR_Service_ptr, ownStdairService);

    // Initalise the SIMFQT service.
    initSIMFQTService();
    
    // Initalise the AIRSCHED service.
    initAIRSCHEDService();
    
    // Initalise the AIRINV service.
    initAIRINVService();
    
    // Initialise the (remaining of the) context
    initSimcrsService();
  }

  // ////////////////////////////////////////////////////////////////////
  SIMCRS_Service::
  SIMCRS_Service (stdair::STDAIR_ServicePtr_T ioSTDAIR_Service_ptr,
                  const CRSCode_T& iCRSCode)
    : _simcrsServiceContext (NULL) {

    // Initialise the service context
    initServiceContext (iCRSCode);

    // Store the STDAIR service object within the (AIRINV) service context
    // \note AirInv does not own the STDAIR service resources here.
    const bool doesNotOwnStdairService = false;
    addStdAirService (ioSTDAIR_Service_ptr, doesNotOwnStdairService);

    // Initalise the SIMFQT service.
    initSIMFQTService();
    
    // Initalise the AIRSCHED service.
    initAIRSCHEDService();
    
    // Initalise the AIRINV service.
    initAIRINVService();
    
    // Initialise the (remaining of the) context
    initSimcrsService();
  }

  // ////////////////////////////////////////////////////////////////////
  SIMCRS_Service::~SIMCRS_Service() {
    // Delete/Clean all the objects from memory
    finalise();
  }

  // ////////////////////////////////////////////////////////////////////
  void SIMCRS_Service::finalise() {
    assert (_simcrsServiceContext != NULL);
    // Reset the (Boost.)Smart pointer pointing on the STDAIR_Service object.
    _simcrsServiceContext->reset();
  }

  // ////////////////////////////////////////////////////////////////////
  void SIMCRS_Service::initServiceContext (const CRSCode_T& iCRSCode) {
    // Initialise the service context
    SIMCRS_ServiceContext& lSIMCRS_ServiceContext = 
      FacSimcrsServiceContext::instance().create (iCRSCode);
    _simcrsServiceContext = &lSIMCRS_ServiceContext;
  }

  // ////////////////////////////////////////////////////////////////////
  void SIMCRS_Service::
  addStdAirService (stdair::STDAIR_ServicePtr_T ioSTDAIR_Service_ptr,
                    const bool iOwnStdairService) {

    // Retrieve the SimCRS service context
    assert (_simcrsServiceContext != NULL);
    SIMCRS_ServiceContext& lSIMCRS_ServiceContext = *_simcrsServiceContext;

    // Store the StdAir service object within the (SimCRS) service context
    lSIMCRS_ServiceContext.setSTDAIR_Service (ioSTDAIR_Service_ptr,
                                              iOwnStdairService);
  }
  
  // ////////////////////////////////////////////////////////////////////
  stdair::STDAIR_ServicePtr_T SIMCRS_Service::
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
  
  // ////////////////////////////////////////////////////////////////////
  stdair::STDAIR_ServicePtr_T SIMCRS_Service::
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
  
  // ////////////////////////////////////////////////////////////////////
  void SIMCRS_Service::initAIRSCHEDService() {

    // Retrieve the SimCRS service context
    assert (_simcrsServiceContext != NULL);
    SIMCRS_ServiceContext& lSIMCRS_ServiceContext = *_simcrsServiceContext;

    // Retrieve the StdAir service context
    stdair::STDAIR_ServicePtr_T lSTDAIR_Service_ptr =
      lSIMCRS_ServiceContext.getSTDAIR_ServicePtr();

    /**
     * Initialise the AIRSCHED service handler.
     *
     * \note The (Boost.)Smart Pointer keeps track of the references
     *       on the Service object, and deletes that object when it is
     *       no longer referenced (e.g., at the end of the process).
     */
    AIRSCHED::AIRSCHED_ServicePtr_T lAIRSCHED_Service_ptr = 
      boost::make_shared<AIRSCHED::AIRSCHED_Service> (lSTDAIR_Service_ptr);
    
    // Store the AIRSCHED service object within the (SimCRS) service context
    lSIMCRS_ServiceContext.setAIRSCHED_Service (lAIRSCHED_Service_ptr);
  }
  
  // ////////////////////////////////////////////////////////////////////
  void SIMCRS_Service::initSIMFQTService() {

    // Retrieve the SimCRS service context
    assert (_simcrsServiceContext != NULL);
    SIMCRS_ServiceContext& lSIMCRS_ServiceContext = *_simcrsServiceContext;

    // Retrieve the StdAir service context
    stdair::STDAIR_ServicePtr_T lSTDAIR_Service_ptr =
      lSIMCRS_ServiceContext.getSTDAIR_ServicePtr();

    /**
     * Initialise the SIMFQT service handler.
     *
     * \note The (Boost.)Smart Pointer keeps track of the references
     *       on the Service object, and deletes that object when it is
     *       no longer referenced (e.g., at the end of the process).
     */
    SIMFQT::SIMFQT_ServicePtr_T lSIMFQT_Service_ptr = 
      boost::make_shared<SIMFQT::SIMFQT_Service> (lSTDAIR_Service_ptr);
    
    // Store the SIMFQT service object within the (SimCRS) service context
    lSIMCRS_ServiceContext.setSIMFQT_Service (lSIMFQT_Service_ptr);
  }
  
  // ////////////////////////////////////////////////////////////////////
  void SIMCRS_Service::initAIRINVService() {

    // Retrieve the SimCRS service context
    assert (_simcrsServiceContext != NULL);
    SIMCRS_ServiceContext& lSIMCRS_ServiceContext = *_simcrsServiceContext;

    // Retrieve the StdAir service context
    stdair::STDAIR_ServicePtr_T lSTDAIR_Service_ptr =
      lSIMCRS_ServiceContext.getSTDAIR_ServicePtr();

    /**
     * Initialise the AIRINV service handler.
     *
     * \note The (Boost.)Smart Pointer keeps track of the references
     *       on the Service object, and deletes that object when it is
     *       no longer referenced (e.g., at the end of the process).
     */
    AIRINV::AIRINV_Master_ServicePtr_T lAIRINV_Service_ptr = 
      boost::make_shared<AIRINV::AIRINV_Master_Service> (lSTDAIR_Service_ptr);
    
    // Store the AIRINV service object within the (SimCRS) service context
    lSIMCRS_ServiceContext.setAIRINV_Service (lAIRINV_Service_ptr);
  }
  
  // ////////////////////////////////////////////////////////////////////
  void SIMCRS_Service::initSimcrsService() {
    // Do nothing at this stage. A sample BOM tree may be built by
    // calling the buildSampleBom() method
  }

  // ////////////////////////////////////////////////////////////////////
  void SIMCRS_Service::
  parseAndLoad (const stdair::Filename_T& iScheduleInputFilename,
                const stdair::Filename_T& iODInputFilename,
                const AIRRAC::YieldFilePath& iYieldInputFilepath,
                const SIMFQT::FareFilePath& iFareInputFilepath) {

    // Retrieve the SimCRS service context
    assert (_simcrsServiceContext != NULL);
    SIMCRS_ServiceContext& lSIMCRS_ServiceContext = *_simcrsServiceContext;
    
    /**
     * Let the schedule manager (i.e., the AirSched component) parse
     * the schedules and O&Ds. AirSched holds the flight-periods (aka schedule)
     * only, not the flight-dates (aka the inventory).
     */
    AIRSCHED::AIRSCHED_Service& lAIRSCHED_Service =
      lSIMCRS_ServiceContext.getAIRSCHED_Service();
    lAIRSCHED_Service.parseAndLoad (iScheduleInputFilename);

    /**
     * Let the inventory manager (i.e., the AirInv component) parse
     * the schedules, O&Ds and yields. From the schedules and O&Ds,
     * AirInv builds the flight-dates (aka the inventory).
     * The flight-periods (aka schedule) are then dropped.
     */
    AIRINV::AIRINV_Master_Service& lAIRINV_Service =
      lSIMCRS_ServiceContext.getAIRINV_Service();
    lAIRINV_Service.parseAndLoad (iScheduleInputFilename, iODInputFilename,
                                  iYieldInputFilepath);

    /**
     * Let the pricing component to build the fare rule structures.
     */
    SIMFQT::SIMFQT_Service& lSIMFQT_Service =
      lSIMCRS_ServiceContext.getSIMFQT_Service();
    lSIMFQT_Service.parseAndLoad (iFareInputFilepath);
  }
  
  // ////////////////////////////////////////////////////////////////////
  void SIMCRS_Service::buildSampleBom() {

    // Retrieve the SimCRS service context
    if (_simcrsServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The SimCRS service "
                                                    "has not been initialised");
    }
    assert (_simcrsServiceContext != NULL);

    // Retrieve the SimCRS service context and whether it owns the Stdair
    // service
    SIMCRS_ServiceContext& lSIMCRS_ServiceContext = *_simcrsServiceContext;
    const bool doesOwnStdairService =
      lSIMCRS_ServiceContext.getOwnStdairServiceFlag();

    // Retrieve the StdAir service object from the (SimCRS) service context
    stdair::STDAIR_Service& lSTDAIR_Service =
      lSIMCRS_ServiceContext.getSTDAIR_Service();

    /**
     * 1. Have StdAir build the whole BOM tree, only when the StdAir service is
     *    owned by the current component (SimCRS here).
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
     * Let the schedule manager (i.e., the AirSched component) build
     * the schedules and O&Ds. AirSched holds the flight-periods (aka schedule)
     * only, not the flight-dates (aka the inventory).
     */
    AIRSCHED::AIRSCHED_Service& lAIRSCHED_Service =
      lSIMCRS_ServiceContext.getAIRSCHED_Service();
    lAIRSCHED_Service.buildSampleBom();

    /**
     * Let the inventory manager (i.e., the AirInv component) build
     * the schedules, O&Ds and yields. From the schedules and O&Ds,
     * AirInv builds the flight-dates (aka the inventory).
     * The flight-periods (aka schedule) are then dropped.
     */
    AIRINV::AIRINV_Master_Service& lAIRINV_Service =
      lSIMCRS_ServiceContext.getAIRINV_Service();
    lAIRINV_Service.buildSampleBom();

    /**
     * Let the pricing component to build the fare rules.
     */
    SIMFQT::SIMFQT_Service& lSIMFQT_Service =
      lSIMCRS_ServiceContext.getSIMFQT_Service();
    lSIMFQT_Service.buildSampleBom();

    /**
     * 3. Build the complementary objects/links for the current component (here,
     *    SimCRS).
     *
     * \note: Currently, no more things to do by SimCRS.
     */
  }

  // //////////////////////////////////////////////////////////////////////
  void SIMCRS_Service::
  buildSampleTravelSolutions(stdair::TravelSolutionList_T& ioTravelSolutionList){

    // Retrieve the SimCRS service context
    if (_simcrsServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The SimCRS service "
                                                    "has not been initialised");
    }
    assert (_simcrsServiceContext != NULL);

    // Retrieve the StdAir service object from the (SimCRS) service context
    SIMCRS_ServiceContext& lSIMCRS_ServiceContext = *_simcrsServiceContext;
    stdair::STDAIR_Service& lSTDAIR_Service =
      lSIMCRS_ServiceContext.getSTDAIR_Service();

    // Delegate the BOM building to the dedicated service
    lSTDAIR_Service.buildSampleTravelSolutions (ioTravelSolutionList);
  }

  // //////////////////////////////////////////////////////////////////////
  stdair::BookingRequestStruct SIMCRS_Service::
  buildSampleBookingRequest (const bool isForCRS) {

    // Retrieve the SimCRS service context
    if (_simcrsServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The SimCRS service "
                                                    "has not been initialised");
    }
    assert (_simcrsServiceContext != NULL);

    // Retrieve the StdAir service object from the (SimCRS) service context
    SIMCRS_ServiceContext& lSIMCRS_ServiceContext = *_simcrsServiceContext;
    stdair::STDAIR_Service& lSTDAIR_Service =
      lSIMCRS_ServiceContext.getSTDAIR_Service();

    // Delegate the BOM building to the dedicated service
    return lSTDAIR_Service.buildSampleBookingRequest (isForCRS);
  }

  // ////////////////////////////////////////////////////////////////////
  std::string SIMCRS_Service::
  jsonExport (const stdair::AirlineCode_T& iAirlineCode,
              const stdair::FlightNumber_T& iFlightNumber,
              const stdair::Date_T& iDepartureDate) const {

    // Retrieve the SimCRS service context
    if (_simcrsServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The SimCRS service "
                                                    "has not been initialised");
    }
    assert (_simcrsServiceContext != NULL);

    // Retrieve the StdAir service object from the (SimCRS) service context
    SIMCRS_ServiceContext& lSIMCRS_ServiceContext = *_simcrsServiceContext;
    stdair::STDAIR_Service& lSTDAIR_Service =
      lSIMCRS_ServiceContext.getSTDAIR_Service();

    // Delegate the JSON export to the dedicated service
    return lSTDAIR_Service.jsonExport (iAirlineCode, iFlightNumber,
                                       iDepartureDate);
  }

  // ////////////////////////////////////////////////////////////////////
  void SIMCRS_Service::
  initSnapshotAndRMEvents (const stdair::Date_T& iStartDate,
                           const stdair::Date_T& iEndDate) {

    // Retrieve the SimCRS service context
    if (_simcrsServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The SimCRS service has "
                                                    "not been initialised");
    }
    assert (_simcrsServiceContext != NULL);
    SIMCRS_ServiceContext& lSIMCRS_ServiceContext = *_simcrsServiceContext;

    // Retrieve the AIRINV Master service.
    AIRINV::AIRINV_Master_Service& lAIRINV_Master_Service =
      lSIMCRS_ServiceContext.getAIRINV_Service();

    lAIRINV_Master_Service.initSnapshotAndRMEvents (iStartDate, iEndDate);
  }
  
  // //////////////////////////////////////////////////////////////////////
  std::string SIMCRS_Service::csvDisplay() const {

    // Retrieve the SimCRS service context
    if (_simcrsServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The SimCRS service "
                                                    "has not been initialised");
    }
    assert (_simcrsServiceContext != NULL);

    // Retrieve the StdAir service object from the (SimCRS) service context
    SIMCRS_ServiceContext& lSIMCRS_ServiceContext = *_simcrsServiceContext;
    stdair::STDAIR_Service& lSTDAIR_Service =
      lSIMCRS_ServiceContext.getSTDAIR_Service();

    // Delegate the BOM building to the dedicated service
    return lSTDAIR_Service.csvDisplay();
  }
  
  // //////////////////////////////////////////////////////////////////////
  std::string SIMCRS_Service::
  csvDisplay (const stdair::TravelSolutionList_T& ioTravelSolutionList) const {

    // Retrieve the SimCRS service context
    if (_simcrsServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The SimCRS service "
                                                    "has not been initialised");
    }
    assert (_simcrsServiceContext != NULL);

    // Retrieve the StdAir service object from the (SimCRS) service context
    SIMCRS_ServiceContext& lSIMCRS_ServiceContext = *_simcrsServiceContext;
    stdair::STDAIR_Service& lSTDAIR_Service =
      lSIMCRS_ServiceContext.getSTDAIR_Service();

    // Delegate the BOM building to the dedicated service
    return lSTDAIR_Service.csvDisplay (ioTravelSolutionList);
  }
  
  // ////////////////////////////////////////////////////////////////////
  stdair::TravelSolutionList_T SIMCRS_Service::
  calculateSegmentPathList(const stdair::BookingRequestStruct& iBookingRequest){
     
    // Retrieve the SimCRS service context
    if (_simcrsServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The SimCRS service "
                                                    "has not been initialised");
    }
    assert (_simcrsServiceContext != NULL);

    SIMCRS_ServiceContext& lSIMCRS_ServiceContext = *_simcrsServiceContext;

    stdair::TravelSolutionList_T oTravelSolutionList;
    
    // Get a reference on the AIRSCHED service handler
    AIRSCHED::AIRSCHED_Service& lAIRSCHED_Service =
      lSIMCRS_ServiceContext.getAIRSCHED_Service();
    
    // Delegate the booking to the dedicated service
    stdair::BasChronometer lTravelSolutionRetrievingChronometer;
    lTravelSolutionRetrievingChronometer.start();

    lAIRSCHED_Service.buildSegmentPathList (oTravelSolutionList,
                                            iBookingRequest);
      
    // DEBUG
    const double lSegmentPathRetrievingMeasure =
      lTravelSolutionRetrievingChronometer.elapsed();
    STDAIR_LOG_DEBUG ("Travel solution retrieving: "
                      << lSegmentPathRetrievingMeasure << " - "
                      << lSIMCRS_ServiceContext.display());

    return oTravelSolutionList;
  }

  // ////////////////////////////////////////////////////////////////////
  void SIMCRS_Service::
  fareQuote (const stdair::BookingRequestStruct& iBookingRequest,
             stdair::TravelSolutionList_T& ioTravelSolutionList) {
     
    // Retrieve the SimCRS service context
    if (_simcrsServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The SimCRS service has "
                                                    "not been initialised");
    }
    assert (_simcrsServiceContext != NULL);
    
    SIMCRS_ServiceContext& lSIMCRS_ServiceContext = *_simcrsServiceContext;
    
    // Get a reference on the SIMFQT service handler
    SIMFQT::SIMFQT_Service& lSIMFQT_Service =
      lSIMCRS_ServiceContext.getSIMFQT_Service();
      
    // Delegate the action to the dedicated command
    stdair::BasChronometer lFareQuoteRetrievalChronometer;
    lFareQuoteRetrievalChronometer.start();

    lSIMFQT_Service.quotePrices (iBookingRequest, ioTravelSolutionList);

    // DEBUG
    const double lFareQuoteRetrievalMeasure =
      lFareQuoteRetrievalChronometer.elapsed();
    STDAIR_LOG_DEBUG ("Fare Quote retrieving: " << lFareQuoteRetrievalMeasure
                      << " - " << lSIMCRS_ServiceContext.display());
  }

  // ////////////////////////////////////////////////////////////////////
  void SIMCRS_Service::
  calculateAvailability (stdair::TravelSolutionList_T& ioTravelSolutionList,
                         const stdair::PartnershipTechnique& iPartnershipTechnique) {

    // Retrieve the SimCRS service context
    if (_simcrsServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The SimCRS service has "
                                                    "not been initialised");
    }
    assert (_simcrsServiceContext != NULL);
    
    SIMCRS_ServiceContext& lSIMCRS_ServiceContext = *_simcrsServiceContext;

    // Retrieve the CRS code
    //const CRSCode_T& lCRSCode = lSIMCRS_ServiceContext.getCRSCode();

    // Retrieve the AIRINV Master service.
    AIRINV::AIRINV_Master_Service& lAIRINV_Master_Service =
      lSIMCRS_ServiceContext.getAIRINV_Service();
    
    // Delegate the availability retrieval to the dedicated command
    stdair::BasChronometer lAvlChronometer;
    lAvlChronometer.start();

    DistributionManager::calculateAvailability (lAIRINV_Master_Service,
                                                ioTravelSolutionList,
                                                iPartnershipTechnique);
    
    // DEBUG
    const double lAvlMeasure = lAvlChronometer.elapsed();
    STDAIR_LOG_DEBUG ("Availability retrieval: " << lAvlMeasure << " - "
                      << lSIMCRS_ServiceContext.display());
  }
  
  // ////////////////////////////////////////////////////////////////////
  bool SIMCRS_Service::
  sell (const stdair::TravelSolutionStruct& iTravelSolution,
        const stdair::PartySize_T& iPartySize) {
    bool hasSaleBeenSuccessful = false;

    // Retrieve the SimCRS service context
    if (_simcrsServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The SimCRS service has "
                                                    "not been initialised");
    }
    assert (_simcrsServiceContext != NULL);
    
    SIMCRS_ServiceContext& lSIMCRS_ServiceContext = *_simcrsServiceContext;

    // Retrieve the CRS code
    //const CRSCode_T& lCRSCode = lSIMCRS_ServiceContext.getCRSCode();

    // Retrieve the AIRINV Master service.
    AIRINV::AIRINV_Master_Service& lAIRINV_Master_Service =
      lSIMCRS_ServiceContext.getAIRINV_Service();
    
    // Delegate the booking to the dedicated command
    stdair::BasChronometer lSellChronometer;
    lSellChronometer.start();

    hasSaleBeenSuccessful = DistributionManager::sell (lAIRINV_Master_Service,
                                                       iTravelSolution,
                                                       iPartySize);

    // DEBUG
    STDAIR_LOG_DEBUG ("Made a sell of " << iPartySize
                      << " persons on the following travel solution: "
                      << iTravelSolution.describe()
		      << " with the chosen fare option: "
		      << iTravelSolution.getChosenFareOption().describe()
                      << ". Successful? " << hasSaleBeenSuccessful);
      
    // DEBUG
    const double lSellMeasure = lSellChronometer.elapsed();
    STDAIR_LOG_DEBUG ("Booking sell: " << lSellMeasure << " - "
                      << lSIMCRS_ServiceContext.display());

    return hasSaleBeenSuccessful;
  }

  
  // ////////////////////////////////////////////////////////////////////
  bool SIMCRS_Service::
  playCancellation (const stdair::CancellationStruct& iCancellation) {
    bool hasCancellationBeenSuccessful = false;

    // Retrieve the SimCRS service context
    if (_simcrsServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The SimCRS service has "
                                                    "not been initialised");
    }
    assert (_simcrsServiceContext != NULL);
    
    SIMCRS_ServiceContext& lSIMCRS_ServiceContext = *_simcrsServiceContext;

    // Retrieve the CRS code
    //const CRSCode_T& lCRSCode = lSIMCRS_ServiceContext.getCRSCode();

    // Retrieve the AIRINV Master service.
    AIRINV::AIRINV_Master_Service& lAIRINV_Master_Service =
      lSIMCRS_ServiceContext.getAIRINV_Service();
    
    // Delegate the booking to the dedicated command
    stdair::BasChronometer lCancellationChronometer;
    lCancellationChronometer.start();

    hasCancellationBeenSuccessful =
      DistributionManager::playCancellation (lAIRINV_Master_Service,
                                             iCancellation);
                                             
    // DEBUG
    STDAIR_LOG_DEBUG ("Made a cancellation of " << iCancellation.describe());
      
    // DEBUG
    const double lCancellationMeasure = lCancellationChronometer.elapsed();
    STDAIR_LOG_DEBUG ("Booking cancellation: " << lCancellationMeasure << " - "
                      << lSIMCRS_ServiceContext.display());

    return hasCancellationBeenSuccessful;
  }
  
  // ////////////////////////////////////////////////////////////////////
  void SIMCRS_Service::takeSnapshots (const stdair::SnapshotStruct& iSnapshot) {

    // Retrieve the SimCRS service context
    if (_simcrsServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The SimCRS service has "
                                                    "not been initialised");
    }
    assert (_simcrsServiceContext != NULL);
    SIMCRS_ServiceContext& lSIMCRS_ServiceContext = *_simcrsServiceContext;

    // Retrieve the AIRINV Master service.
    AIRINV::AIRINV_Master_Service& lAIRINV_Master_Service =
      lSIMCRS_ServiceContext.getAIRINV_Service();

    lAIRINV_Master_Service.takeSnapshots (iSnapshot);
  }

  // ////////////////////////////////////////////////////////////////////
  void SIMCRS_Service::
  optimise (const stdair::RMEventStruct& iRMEvent,
            const stdair::ForecastingMethod& iForecastingMethod,
            const stdair::PartnershipTechnique& iPartnershipTechnique) {

    // Retrieve the SimCRS service context
    if (_simcrsServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The SimCRS service has "
                                                    "not been initialised");
    }
    assert (_simcrsServiceContext != NULL);
    SIMCRS_ServiceContext& lSIMCRS_ServiceContext = *_simcrsServiceContext;

    // Retrieve the AIRINV Master service.
    AIRINV::AIRINV_Master_Service& lAIRINV_Master_Service =
      lSIMCRS_ServiceContext.getAIRINV_Service();

    lAIRINV_Master_Service.optimise (iRMEvent, iForecastingMethod, iPartnershipTechnique);
  }
}
