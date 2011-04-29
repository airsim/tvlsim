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
  SIMCRS_Service::
  SIMCRS_Service (stdair::STDAIR_ServicePtr_T ioSTDAIR_ServicePtr,
                  const CRSCode_T& iCRSCode,
                  const stdair::Filename_T& iScheduleInputFilename,
                  const stdair::Filename_T& iODInputFilename,
                  const stdair::Filename_T& iFareInputFilename)
    : _simcrsServiceContext (NULL) {

    // Initialise the service context
    initServiceContext (iCRSCode);

    // Retrieve the Simcrs service context
    assert (_simcrsServiceContext != NULL);
    SIMCRS_ServiceContext& lSIMCRS_ServiceContext = *_simcrsServiceContext;

    // Store the STDAIR service object within the (SIMCRS) service context
    lSIMCRS_ServiceContext.setSTDAIR_Service (ioSTDAIR_ServicePtr);
    
    // Initialise the context
    init (iScheduleInputFilename, iODInputFilename, iFareInputFilename);
  }

  // ////////////////////////////////////////////////////////////////////
  SIMCRS_Service::
  SIMCRS_Service (const stdair::BasLogParams& iLogParams,
                  const stdair::BasDBParams& iDBParams,
                  const CRSCode_T& iCRSCode,
                  const stdair::Filename_T& iScheduleInputFilename,
                  const stdair::Filename_T& iODInputFilename,
                  const stdair::Filename_T& iFareInputFilename)
    : _simcrsServiceContext (NULL) {
    
    // Initialise the service context
    initServiceContext (iCRSCode);
    
    // Initialise the STDAIR service handler
    initStdAirService (iLogParams, iDBParams);
    
    // Initialise the (remaining of the) context
    init (iScheduleInputFilename, iODInputFilename, iFareInputFilename);
  }

  // ////////////////////////////////////////////////////////////////////
  SIMCRS_Service::
  SIMCRS_Service (const stdair::BasLogParams& iLogParams,
                  const CRSCode_T& iCRSCode,
                  const stdair::Filename_T& iScheduleInputFilename,
                  const stdair::Filename_T& iODInputFilename,
                  const stdair::Filename_T& iFareInputFilename)
    : _simcrsServiceContext (NULL) {
    
    // Initialise the service context
    initServiceContext (iCRSCode);
    
    // Initialise the STDAIR service handler
    initStdAirService (iLogParams);
    
    // Initialise the (remaining of the) context
    init (iScheduleInputFilename, iODInputFilename, iFareInputFilename);
  }

  // ////////////////////////////////////////////////////////////////////
  SIMCRS_Service::~SIMCRS_Service() {
    // Delete/Clean all the objects from memory
    finalise();
  }

  // ////////////////////////////////////////////////////////////////////
  void SIMCRS_Service::finalise() {
    assert (_simcrsServiceContext != NULL);
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
  initStdAirService (const stdair::BasLogParams& iLogParams,
                     const stdair::BasDBParams& iDBParams) {

    // Retrieve the SimCRS service context
    assert (_simcrsServiceContext != NULL);
    SIMCRS_ServiceContext& lSIMCRS_ServiceContext = *_simcrsServiceContext;
    
    /**
     * Initialise the StdAir service handler
     *
     * \note The (Boost) Smart Pointer keeps track of the references
     * on the Service object, and deletes that object when it is no
     * longer referenced (e.g., at the end of the process).
     */
    stdair::STDAIR_ServicePtr_T lSTDAIR_Service_ptr = 
      boost::make_shared<stdair::STDAIR_Service> (iLogParams, iDBParams);
    assert (lSTDAIR_Service_ptr != NULL);

    // Store the STDAIR service object within the (SIMCRS) service context
    lSIMCRS_ServiceContext.setSTDAIR_Service (lSTDAIR_Service_ptr);
  }
  
  // ////////////////////////////////////////////////////////////////////
  void SIMCRS_Service::
  initStdAirService (const stdair::BasLogParams& iLogParams) {

    // Retrieve the SimCRS service context
    assert (_simcrsServiceContext != NULL);
    SIMCRS_ServiceContext& lSIMCRS_ServiceContext = *_simcrsServiceContext;
    
    /**
     * Initialise the StdAir service handler
     *
     * \note The (Boost) Smart Pointer keeps track of the references
     * on the Service object, and deletes that object when it is no
     * longer referenced (e.g., at the end of the process).
     */
    stdair::STDAIR_ServicePtr_T lSTDAIR_Service_ptr = 
      boost::make_shared<stdair::STDAIR_Service> (iLogParams);
    assert (lSTDAIR_Service_ptr != NULL);

    // Store the STDAIR service object within the (SIMCRS) service context
    lSIMCRS_ServiceContext.setSTDAIR_Service (lSTDAIR_Service_ptr);
  }
  
  // ////////////////////////////////////////////////////////////////////
  void SIMCRS_Service::init (const stdair::Filename_T& iScheduleInputFilename,
                             const stdair::Filename_T& iODInputFilename,
                             const stdair::Filename_T& iFareInputFilename) {

    // Initialise the children AirSched service context
    initAIRSCHEDService (iScheduleInputFilename, iODInputFilename);

    // Initialise the children AirInv service context
    initAIRINV_Master_Service (iScheduleInputFilename, iODInputFilename);

    // Initialise the children SimFQT service context
    initSIMFQTService (iFareInputFilename);
  }

  // ////////////////////////////////////////////////////////////////////
  void SIMCRS_Service::
  initAIRSCHEDService (const stdair::Filename_T& iScheduleInputFilename,
                       const stdair::Filename_T& iODInputFilename) {
    
    // Check that the file path given as input corresponds to an actual file
    bool doesExistAndIsReadable =
      stdair::BasFileMgr::doesExistAndIsReadable (iScheduleInputFilename);
    if (doesExistAndIsReadable == false) {
      STDAIR_LOG_ERROR ("The schedule input file, '" << iScheduleInputFilename
                        << "', can not be retrieved on the file-system");
      throw stdair::FileNotFoundException ("The schedule input file, '"
                                           + iScheduleInputFilename
                                           + "', can not be retrieved on the "
                                           + "file-system");
    }

    // Check that the file path given as input corresponds to an actual file
    doesExistAndIsReadable =
      stdair::BasFileMgr::doesExistAndIsReadable (iODInputFilename);
    if (doesExistAndIsReadable == false) {
      STDAIR_LOG_ERROR ("The O&D input file, '" << iODInputFilename
                        << "', can not be retrieved on the file-system");
      throw stdair::FileNotFoundException ("The O&D input file, '"
                                           + iODInputFilename
                                           + "', can not be retrieved on the "
                                           + "file-system");
    }

    // Retrieve the SimCRS service context
    assert (_simcrsServiceContext != NULL);
    SIMCRS_ServiceContext& lSIMCRS_ServiceContext = *_simcrsServiceContext;
    
    // Retrieve the StdAir service context
    stdair::STDAIR_ServicePtr_T lSTDAIR_Service_ptr =
      lSIMCRS_ServiceContext.getSTDAIR_ServicePtr();
    assert (lSTDAIR_Service_ptr != NULL);

    /**
     * Initialise the AirSched service handler
     *
     * \note The (Boost) Smart Pointer keeps track of the references
     * on the Service object, and deletes that object when it is no
     * longer referenced (e.g., at the end of the process).
     */
    AIRSCHED_ServicePtr_T lAIRSCHED_Service_ptr =
      boost::make_shared<AIRSCHED::AIRSCHED_Service> (lSTDAIR_Service_ptr,
                                                      iScheduleInputFilename,
                                                      iODInputFilename);

    // Store the AirSched service object within the (SimCRS) service context
    lSIMCRS_ServiceContext.setAIRSCHED_Service (lAIRSCHED_Service_ptr);
  }

  // ////////////////////////////////////////////////////////////////////
  void SIMCRS_Service::
  initSIMFQTService (const stdair::Filename_T& iFareInputFilename) {
    
    // Check that the file path given as input corresponds to an actual file
    const bool doesExistAndIsReadable =
      stdair::BasFileMgr::doesExistAndIsReadable (iFareInputFilename);
    if (doesExistAndIsReadable == false) {
      STDAIR_LOG_ERROR ("The fare input file, '" << iFareInputFilename
                        << "', can not be retrieved on the file-system");
      throw stdair::FileNotFoundException ("The fare input file, '"
                                           + iFareInputFilename
                                           + "', can not be retrieved on the "
                                           + "file-system");
    }

    // Retrieve the SimCRS service context
    assert (_simcrsServiceContext != NULL);
    SIMCRS_ServiceContext& lSIMCRS_ServiceContext = *_simcrsServiceContext;
    
    // Retrieve the StdAir service context
    stdair::STDAIR_ServicePtr_T lSTDAIR_Service_ptr =
      lSIMCRS_ServiceContext.getSTDAIR_ServicePtr();
    assert (lSTDAIR_Service_ptr != NULL);

    /**
     * Initialise the SimFQT service handler
     *
     * \note The (Boost) Smart Pointer keeps track of the references
     * on the Service object, and deletes that object when it is no
     * longer referenced (e.g., at the end of the process).
     */
    SIMFQT_ServicePtr_T lSIMFQT_Service_ptr =
      boost::make_shared<SIMFQT::SIMFQT_Service> (lSTDAIR_Service_ptr);

    // Store the Simfqt service object within the (SimLFS) service context
    lSIMLFS_ServiceContext.setSIMFQT_Service (lSIMFQT_Service_ptr); 

    // Parse the fare input file and load its content into memory
    lSIMFQT_Service_ptr->parseAndLoad (iFareInputFilename);
  }

  // ////////////////////////////////////////////////////////////////////
  void SIMCRS_Service::
  initAIRINV_Master_Service (const stdair::Filename_T& iScheduleInputFilename,
                             const stdair::Filename_T& iODInputFilename) {
    
    // Retrieve the SimCRS service context
    assert (_simcrsServiceContext != NULL);
    SIMCRS_ServiceContext& lSIMCRS_ServiceContext = *_simcrsServiceContext;
    
    // Retrieve the StdAir service context
    stdair::STDAIR_ServicePtr_T lSTDAIR_Service_ptr =
      lSIMCRS_ServiceContext.getSTDAIR_ServicePtr();
    assert (lSTDAIR_Service_ptr != NULL);

    /**
     * Initialise the AirInv service handler
     *
     * \note The (Boost) Smart Pointer keeps track of the references
     * on the Service object, and deletes that object when it is no
     * longer referenced (e.g., at the end of the process).
     */
    AIRINV_Master_ServicePtr_T lAIRINV_Master_Service_ptr =
      boost::make_shared<AIRINV::AIRINV_Master_Service> (lSTDAIR_Service_ptr);

    // Parse and load the schedule and O&D input files
    lAIRINV_Master_Service_ptr->parseAndLoad (iScheduleInputFilename,
                                              iODInputFilename);

    // Store the Airinv service object within the (SimCRS) service context
    lSIMCRS_ServiceContext.setAIRINV_Master_Service(lAIRINV_Master_Service_ptr);
  }

  // ////////////////////////////////////////////////////////////////////
  void SIMCRS_Service::initSnapshotEvents (const stdair::Date_T& iStartDate,
                                           const stdair::Date_T& iEndDate) {
    if (_simcrsServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The SimCRS service has "
                                                    "not been initialised");
    }
    assert (_simcrsServiceContext != NULL);
    SIMCRS_ServiceContext& lSIMCRS_ServiceContext = *_simcrsServiceContext;

    // Retrieve the AIRINV Master service.
    AIRINV::AIRINV_Master_Service& lAIRINV_Master_Service =
      lSIMCRS_ServiceContext.getAIRINV_Master_Service();

    lAIRINV_Master_Service.initSnapshotEvents (iStartDate, iEndDate);
  }
  
  // //////////////////////////////////////////////////////////////////////
  void SIMCRS_Service::
  buildSampleTravelSolutions(stdair::TravelSolutionList_T& ioTravelSolutionList){

    // Retrieve the SIMCRS service context
    if (_simcrsServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The SimCRS service has not "
                                                    "been initialised");
    }
    assert (_simcrsServiceContext != NULL);

    SIMCRS_ServiceContext& lSIMCRS_ServiceContext = *_simcrsServiceContext;
  
    // Retrieve the STDAIR service object from the (SIMCRS) service context
    stdair::STDAIR_Service& lSTDAIR_Service =
      lSIMCRS_ServiceContext.getSTDAIR_Service();

    // Delegate the BOM building to the dedicated service
    lSTDAIR_Service.buildSampleTravelSolutions (ioTravelSolutionList);
  }

  // //////////////////////////////////////////////////////////////////////
  stdair::BookingRequestStruct SIMCRS_Service::
  buildSampleBookingRequest (const bool isForCRS) {

    // Retrieve the SIMCRS service context
    if (_simcrsServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The SimCRS service has not "
                                                    "been initialised");
    }
    assert (_simcrsServiceContext != NULL);

    SIMCRS_ServiceContext& lSIMCRS_ServiceContext = *_simcrsServiceContext;
  
    // Retrieve the STDAIR service object from the (SIMCRS) service context
    stdair::STDAIR_Service& lSTDAIR_Service =
      lSIMCRS_ServiceContext.getSTDAIR_Service();

    // Delegate the BOM building to the dedicated service
    return lSTDAIR_Service.buildSampleBookingRequest (isForCRS);
  }

  // //////////////////////////////////////////////////////////////////////
  std::string SIMCRS_Service::csvDisplay() const {

    // Retrieve the SIMCRS service context
    if (_simcrsServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The SimCRS service has not "
                                                    "been initialised");
    }
    assert (_simcrsServiceContext != NULL);

    SIMCRS_ServiceContext& lSIMCRS_ServiceContext = *_simcrsServiceContext;
  
    // Retrieve the STDAIR service object from the (SIMCRS) service context
    stdair::STDAIR_Service& lSTDAIR_Service =
      lSIMCRS_ServiceContext.getSTDAIR_Service();

    // Delegate the BOM building to the dedicated service
    return lSTDAIR_Service.csvDisplay();
  }
  
  // //////////////////////////////////////////////////////////////////////
  std::string SIMCRS_Service::
  csvDisplay (const stdair::TravelSolutionList_T& ioTravelSolutionList) const {

    // Retrieve the SIMCRS service context
    if (_simcrsServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The SimCRS service has not "
                                                    "been initialised");
    }
    assert (_simcrsServiceContext != NULL);

    SIMCRS_ServiceContext& lSIMCRS_ServiceContext = *_simcrsServiceContext;
  
    // Retrieve the STDAIR service object from the (SIMCRS) service context
    stdair::STDAIR_Service& lSTDAIR_Service =
      lSIMCRS_ServiceContext.getSTDAIR_Service();

    // Delegate the BOM building to the dedicated service
    return lSTDAIR_Service.csvDisplay (ioTravelSolutionList);
  }
  
  // ////////////////////////////////////////////////////////////////////
  stdair::TravelSolutionList_T SIMCRS_Service::
  calculateSegmentPathList(const stdair::BookingRequestStruct& iBookingRequest){
     
    if (_simcrsServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The SimCRS service has "
                                                    "not been initialised");
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
  calculateAvailability (stdair::TravelSolutionList_T& ioTravelSolutionList) {
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
      lSIMCRS_ServiceContext.getAIRINV_Master_Service();
    
    // Delegate the availability retrieval to the dedicated command
    stdair::BasChronometer lAvlChronometer;
    lAvlChronometer.start();

    DistributionManager::calculateAvailability (lAIRINV_Master_Service,
                                                ioTravelSolutionList);
    
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
      lSIMCRS_ServiceContext.getAIRINV_Master_Service();
    
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
                      << ". Successful? " << hasSaleBeenSuccessful);
      
    // DEBUG
    const double lSellMeasure = lSellChronometer.elapsed();
    STDAIR_LOG_DEBUG ("Booking sell: " << lSellMeasure << " - "
                      << lSIMCRS_ServiceContext.display());

    return hasSaleBeenSuccessful;
  }

  // ////////////////////////////////////////////////////////////////////
  void SIMCRS_Service::takeSnapshots (const stdair::SnapshotStruct& iSnapshot) {
    if (_simcrsServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The SimCRS service has "
                                                    "not been initialised");
    }
    assert (_simcrsServiceContext != NULL);
    SIMCRS_ServiceContext& lSIMCRS_ServiceContext = *_simcrsServiceContext;

    // Retrieve the AIRINV Master service.
    AIRINV::AIRINV_Master_Service& lAIRINV_Master_Service =
      lSIMCRS_ServiceContext.getAIRINV_Master_Service();

    lAIRINV_Master_Service.takeSnapshots (iSnapshot);
  }
}
