// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <cmath>
// Boost
#include <boost/make_shared.hpp>
// StdAir
#include <stdair/bom/BomManager.hpp>
#include <stdair/bom/BomRoot.hpp>
#include <stdair/basic/BasChronometer.hpp>
#include <stdair/basic/EventType.hpp>
#include <stdair/bom/EventQueue.hpp>
#include <stdair/bom/SnapshotStruct.hpp>
#include <stdair/bom/RMEventStruct.hpp>
#include <stdair/service/Logger.hpp>
#include <stdair/STDAIR_Service.hpp>
#include <stdair/bom/YieldFeatures.hpp>
#include <stdair/bom/AirportPair.hpp>
#include <stdair/bom/PosChannel.hpp>
#include <stdair/bom/DatePeriod.hpp>
#include <stdair/bom/TimePeriod.hpp>
#include <stdair/bom/AirlineClassList.hpp>
#include <stdair/basic/BasConst_Request.hpp>
// AirInv
#include <airinv/basic/BasConst_AIRINV_Service.hpp>
#include <airinv/factory/FacAirinvMasterServiceContext.hpp>
#include <airinv/command/InventoryParser.hpp>
#include <airinv/command/InventoryManager.hpp>
#include <airinv/service/AIRINV_Master_ServiceContext.hpp>
#include <airinv/AIRINV_Service.hpp>
#include <airinv/AIRINV_Master_Service.hpp>
// Trademgen
#include <trademgen/bom/DemandStream.hpp>

namespace AIRINV {

  // ////////////////////////////////////////////////////////////////////
  AIRINV_Master_Service::AIRINV_Master_Service() 
    : _airinvMasterServiceContext (NULL) {
    assert (false);
  }

  // ////////////////////////////////////////////////////////////////////
  AIRINV_Master_Service::
  AIRINV_Master_Service (const AIRINV_Master_Service& iService)
    : _airinvMasterServiceContext (NULL) {
    assert (false);
  }

  // ////////////////////////////////////////////////////////////////////
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
    initSlaveAirinvService();
  }

  // ////////////////////////////////////////////////////////////////////
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
    initSlaveAirinvService();
  }

  // ////////////////////////////////////////////////////////////////////
  AIRINV_Master_Service::
  AIRINV_Master_Service (stdair::STDAIR_ServicePtr_T ioSTDAIR_Service_ptr)
    : _airinvMasterServiceContext (NULL) {

    // Initialise the service context
    initServiceContext();

    // Store the STDAIR service object within the (AIRINV) service context
    // \note AirInv does not own the STDAIR service resources here.
    const bool doesNotOwnStdairService = false;
    addStdAirService (ioSTDAIR_Service_ptr, doesNotOwnStdairService);
    
    // Initialise the (remaining of the) context
    initSlaveAirinvService();
  }

  // ////////////////////////////////////////////////////////////////////
  AIRINV_Master_Service::~AIRINV_Master_Service() {
    // Delete/Clean all the objects from memory
    finalise();
  }

  // ////////////////////////////////////////////////////////////////////
  void AIRINV_Master_Service::finalise() {
    assert (_airinvMasterServiceContext != NULL);
    // Reset the (Boost.)Smart pointer pointing on the STDAIR_Service object.
    _airinvMasterServiceContext->reset();
  }

  // ////////////////////////////////////////////////////////////////////
  void AIRINV_Master_Service::initServiceContext() {
    // Initialise the context
    AIRINV_Master_ServiceContext& lAIRINV_Master_ServiceContext = 
      FacAirinvMasterServiceContext::instance().create();
    _airinvMasterServiceContext = &lAIRINV_Master_ServiceContext;
  }

  // ////////////////////////////////////////////////////////////////////
  void AIRINV_Master_Service::
  addStdAirService (stdair::STDAIR_ServicePtr_T ioSTDAIR_Service_ptr,
                    const bool iOwnStdairService) {

    // Retrieve the AirInv service context
    assert (_airinvMasterServiceContext != NULL);
    AIRINV_Master_ServiceContext& lAIRINV_Master_ServiceContext =
      *_airinvMasterServiceContext;

    // Store the STDAIR service object within the (AIRINV) service context
    lAIRINV_Master_ServiceContext.setSTDAIR_Service (ioSTDAIR_Service_ptr,
                                                     iOwnStdairService);
  }
  
  // ////////////////////////////////////////////////////////////////////
  stdair::STDAIR_ServicePtr_T AIRINV_Master_Service::
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
  stdair::STDAIR_ServicePtr_T AIRINV_Master_Service::
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
  void AIRINV_Master_Service::initSlaveAirinvService() {

    // Retrieve the AIRINV Master service context.
    assert (_airinvMasterServiceContext != NULL);
    AIRINV_Master_ServiceContext& lAIRINV_Master_ServiceContext =
      *_airinvMasterServiceContext;

    /**
     * Initialise the AirInv service handler.
     *
     * \note The (Boost.)Smart Pointer keeps track of the references
     *       on the Service object, and deletes that object when it is
     *       no longer referenced (e.g., at the end of the process).
     *
     * \note Each Airinv slave service has its own StdAir Service.
     */
    AIRINV_ServicePtr_T lAIRINV_Service_ptr =
      boost::make_shared<AIRINV_Service> ();

    // Store the AIRINV service object within the AIRINV Master service context.
    lAIRINV_Master_ServiceContext.setAIRINV_Service (lAIRINV_Service_ptr);
  }
  
  // ////////////////////////////////////////////////////////////////////
  void AIRINV_Master_Service::
  parseAndLoad (const stdair::Filename_T& iInventoryInputFilename) {
    // Retrieve the AIRINV service context
    if (_airinvMasterServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The AirInvMaster service "
                                                    "has not been initialised");
    }
    assert (_airinvMasterServiceContext != NULL);

    AIRINV_Master_ServiceContext& lAIRINV_Master_ServiceContext =
      *_airinvMasterServiceContext;
  
    // Retrieve the slave AIRINV service object from the (AIRINV)
    // service context
    AIRINV_Service& lAIRINV_Service =
      lAIRINV_Master_ServiceContext.getAIRINV_Service();

    // Delegate the file parsing and BOM building to the dedicated service
    lAIRINV_Service.parseAndLoad (iInventoryInputFilename);
  }

  // ////////////////////////////////////////////////////////////////////
  void AIRINV_Master_Service::
  parseAndLoad (const stdair::Filename_T& iScheduleInputFilename,
                const stdair::Filename_T& iODInputFilename,
                const stdair::Filename_T& iYieldInputFilename) {
    
    // Retrieve the AIRINV service context
    if (_airinvMasterServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The AirInvMaster service "
                                                    "has not been initialised");
    }
    assert (_airinvMasterServiceContext != NULL);

    AIRINV_Master_ServiceContext& lAIRINV_Master_ServiceContext =
      *_airinvMasterServiceContext;
  
    // Retrieve the slave AIRINV service object from the (AIRINV)
    // service context
    AIRINV_Service& lAIRINV_Service =
      lAIRINV_Master_ServiceContext.getAIRINV_Service();

    // Delegate the file parsing and BOM building to the dedicated service
    lAIRINV_Service.parseAndLoad (iScheduleInputFilename, iODInputFilename,
                                  iYieldInputFilename);
  }
  
  // ////////////////////////////////////////////////////////////////////
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
  
    // Retrieve the slave AIRINV service object from the (AIRINV)
    // service context
    AIRINV_Service& lAIRINV_Service =
      lAIRINV_Master_ServiceContext.getAIRINV_Service();

    // Delegate the BOM building to the dedicated service
    lAIRINV_Service.buildSampleBom();
  }

  // ////////////////////////////////////////////////////////////////////
  std::string AIRINV_Master_Service::csvDisplay() const {

    // Retrieve the AIRINV service context
    if (_airinvMasterServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The AirInvMaster service "
                                                    "has not been initialised");
    }
    assert (_airinvMasterServiceContext != NULL);

    AIRINV_Master_ServiceContext& lAIRINV_Master_ServiceContext =
      *_airinvMasterServiceContext;
  
    // Retrieve the slave AIRINV service object from
    // the (AIRINV) service context
    AIRINV_Service& lAIRINV_Service =
      lAIRINV_Master_ServiceContext.getAIRINV_Service();

    // Delegate the BOM display to the dedicated service
    return lAIRINV_Service.csvDisplay();
  }

  // ////////////////////////////////////////////////////////////////////
  std::string AIRINV_Master_Service::
  csvDisplay (const stdair::AirlineCode_T& iAirlineCode,
              const stdair::FlightNumber_T& iFlightNumber,
              const stdair::Date_T& iDepartureDate) const {

    // Retrieve the AIRINV service context
    if (_airinvMasterServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The AirInvMaster service "
                                                    "has not been initialised");
    }
    assert (_airinvMasterServiceContext != NULL);

    AIRINV_Master_ServiceContext& lAIRINV_Master_ServiceContext =
      *_airinvMasterServiceContext;
  
    // Retrieve the slave AIRINV service object from
    // the (AIRINV) service context
    AIRINV_Service& lAIRINV_Service =
      lAIRINV_Master_ServiceContext.getAIRINV_Service();

    // Delegate the BOM display to the dedicated service
    return lAIRINV_Service.csvDisplay (iAirlineCode, iFlightNumber,
                                       iDepartureDate);
  }

  // ////////////////////////////////////////////////////////////////////
  void AIRINV_Master_Service::
  initSnapshotAndRMEvents (const stdair::Date_T& iStartDate,
                           const stdair::Date_T& iEndDate) {

    // Retrieve the AIRINV service context
    if (_airinvMasterServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The AirInvMaster service "
                                                    "has not been initialised");
    }
    assert (_airinvMasterServiceContext != NULL);

    AIRINV_Master_ServiceContext& lAIRINV_Master_ServiceContext =
      *_airinvMasterServiceContext;
    
    // Retrieve the StdAir service context
    stdair::STDAIR_ServicePtr_T lSTDAIR_Service_ptr =
      lAIRINV_Master_ServiceContext.getSTDAIR_ServicePtr();
    assert (lSTDAIR_Service_ptr != NULL);

    // Retrieve the event queue object instance
    stdair::EventQueue& lQueue = lSTDAIR_Service_ptr->getEventQueue();

    // Initialise the snapshot events
    InventoryManager::initSnapshotEvents (iStartDate, iEndDate, lQueue);

    // Browse the list of inventories and itinialise the RM events of each
    // inventory.
  
    // Retrieve the slave AIRINV service object from the (AIRINV)
    // service context
    AIRINV_Service& lAIRINV_Service =
      lAIRINV_Master_ServiceContext.getAIRINV_Service();
    lQueue.addStatus (stdair::EventType::RM, 0);
    stdair::RMEventList_T lRMEventList =
      lAIRINV_Service.initRMEvents (iStartDate, iEndDate);
    InventoryManager::addRMEventsToEventQueue (lQueue, lRMEventList);
  }

  // ////////////////////////////////////////////////////////////////////
  void AIRINV_Master_Service::
  calculateAvailability (stdair::TravelSolutionStruct& ioTravelSolution) {

    // Retrieve the AIRINV service context
    if (_airinvMasterServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The AirInvMaster service "
                                                    "has not been initialised");
    }
    assert (_airinvMasterServiceContext != NULL);

    AIRINV_Master_ServiceContext& lAIRINV_Master_ServiceContext =
      *_airinvMasterServiceContext;
  
    // Retrieve the slave AIRINV service object from the (AIRINV)
    // service context
    AIRINV_Service& lAIRINV_Service =
      lAIRINV_Master_ServiceContext.getAIRINV_Service();

    // Delegate the availability retrieval to the dedicated service
    stdair::BasChronometer lAvlChronometer;
    lAvlChronometer.start();

    lAIRINV_Service.calculateAvailability (ioTravelSolution);

    // DEBUG
    const double lAvlMeasure = lAvlChronometer.elapsed();
    STDAIR_LOG_DEBUG ("Availability retrieval: " << lAvlMeasure << " - "
                      << lAIRINV_Master_ServiceContext.display());
    
  }

  // ////////////////////////////////////////////////////////////////////
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
  
    // Retrieve the slave AIRINV service object from the (AIRINV)
    // service context
    AIRINV_Service& lAIRINV_Service =
      lAIRINV_Master_ServiceContext.getAIRINV_Service();

    // Delegate the booking to the dedicated command
    stdair::BasChronometer lSellChronometer;
    lSellChronometer.start();

    // Delegate the BOM building to the dedicated service
    const bool hasBeenSaleSuccessful =
      lAIRINV_Service.sell (iSegmentDateKey, iClassCode, iPartySize);

    const double lSellMeasure = lSellChronometer.elapsed();

    // DEBUG
    STDAIR_LOG_DEBUG ("Booking sell: " << lSellMeasure << " - "
                      << lAIRINV_Master_ServiceContext.display());

    //
    return hasBeenSaleSuccessful;
  }

  // ////////////////////////////////////////////////////////////////////
  void AIRINV_Master_Service::
  takeSnapshots (const stdair::SnapshotStruct& iSnapshot) {

    // Retrieve the AIRINV service context
    if (_airinvMasterServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The AirInvMaster service "
                                                    "has not been initialised");
    }
    assert (_airinvMasterServiceContext != NULL);

    AIRINV_Master_ServiceContext& lAIRINV_Master_ServiceContext =
      *_airinvMasterServiceContext;
  
    // Retrieve the slave AIRINV service object from the (AIRINV)
    // service context
    AIRINV_Service& lAIRINV_Service =
      lAIRINV_Master_ServiceContext.getAIRINV_Service();

    // Retrieve  the snapshot time and the airline code.
    const stdair::DateTime_T& lSnapshotTime = iSnapshot.getSnapshotTime();
    const stdair::AirlineCode_T lAirlineCode = iSnapshot.getAirlineCode();

    lAIRINV_Service.takeSnapshots (lAirlineCode, lSnapshotTime);
  }

  // ////////////////////////////////////////////////////////////////////
  void AIRINV_Master_Service::
  optimise (const stdair::RMEventStruct& iRMEvent) {

    // Retrieve the AIRINV service context
    if (_airinvMasterServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The AirInvMaster service "
                                                    "has not been initialised");
    }
    assert (_airinvMasterServiceContext != NULL);

    AIRINV_Master_ServiceContext& lAIRINV_Master_ServiceContext =
      *_airinvMasterServiceContext;
  
    // Retrieve the slave AIRINV service object from the (AIRINV)
    // service context
    AIRINV_Service& lAIRINV_Service =
      lAIRINV_Master_ServiceContext.getAIRINV_Service();

    // Retrieve  the snapshot time and the airline code.
    const stdair::DateTime_T& lRMEventTime = iRMEvent.getRMEventTime();
    const stdair::AirlineCode_T lAirlineCode = iRMEvent.getAirlineCode();
    const stdair::KeyDescription_T lFDDescription =
      iRMEvent.getFlightDateDescription();

    lAIRINV_Service.optimise (lAirlineCode, lFDDescription, lRMEventTime);
  }

  // ////////////////////////////////////////////////////////////////////
  void AIRINV_Master_Service::forecast () {

    // Retrieve the AIRINV service context
    if (_airinvMasterServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The AirInvMaster service "
                                                    "has not been initialised");
    }
    assert (_airinvMasterServiceContext != NULL);

    // Retrieve the airinv context
    AIRINV_Master_ServiceContext& lAIRINV_Master_ServiceContext =
      *_airinvMasterServiceContext;


    // Retrieve the master bom root
    stdair::STDAIR_Service& lSTDAIR_Service =
      lAIRINV_Master_ServiceContext.getSTDAIR_Service();
    stdair::BomRoot& lBomRoot = lSTDAIR_Service.getBomRoot();

    // Retrieve the demand stream list
    const stdair::EventQueueList_T lEventQueueList =
        stdair::BomManager::getList<stdair::EventQueue> (lBomRoot);
    assert(lEventQueueList.begin() != lEventQueueList.end());
    const stdair::EventQueue* lEventQueue_ptr = lEventQueueList.front();
    const TRADEMGEN::DemandStreamList_T lDemandStreamList =
      stdair::BomManager::getList<TRADEMGEN::DemandStream> (*lEventQueue_ptr);

    // Browse the demand stream list
    for (TRADEMGEN::DemandStreamList_T::const_iterator itDS = lDemandStreamList.begin();
         itDS != lDemandStreamList.end(); ++itDS) {
      TRADEMGEN::DemandStream* lDemandStream_ptr = *itDS;

      stdair::YieldFeatures* lYieldFeatures_ptr =
        getYieldFeatures(*lDemandStream_ptr, lBomRoot);

      if (lYieldFeatures_ptr == NULL) {
        const TRADEMGEN::DemandStreamKey& lDemandStreamKey =
          lDemandStream_ptr->getKey();
        STDAIR_LOG_ERROR ("Cannot find yield corresponding to the Demand"
                          << " Stream: " << lDemandStreamKey.toString());
        assert (false);
      }

      forecast (*lYieldFeatures_ptr, *lDemandStream_ptr, lBomRoot);
     
    }
  }

  // ///////////////////////////////////////////////////////////////////
  stdair::YieldFeatures* AIRINV_Master_Service::
  getYieldFeatures(const TRADEMGEN::DemandStream& iDemandStream,
                   stdair::BomRoot& iBomRoot) {

    const stdair::AirportCode_T& lOrigin = iDemandStream.getOrigin();
    const stdair::AirportCode_T& lDestination = iDemandStream.getDestination();

    const stdair::Date_T& lPreferredDepartureDate =
      iDemandStream.getPreferredDepartureDate();
    const stdair::CabinCode_T& lPreferredCabin = iDemandStream.getPreferredCabin();

    // Build the airport pair key out of O&D and get the airport pair object
    const stdair::AirportPairKey lAirportPairKey(lOrigin, lDestination);
    stdair::AirportPair* lAirportPair_ptr = stdair::BomManager::
      getObjectPtr<stdair::AirportPair> (iBomRoot,
                                         lAirportPairKey.toString());
    if (lAirportPair_ptr == NULL) {
      STDAIR_LOG_ERROR ("Cannot find yield corresponding to the airport "
                        << "pair: " << lAirportPairKey.toString());
      assert (false);
    }
    // Retrieve the PoS-Channel.
    const stdair::PosChannelKey lPosChannelKey (stdair::DEFAULT_POS,
                                                stdair::DEFAULT_CHANNEL);
    stdair::PosChannel* lPosChannel_ptr = stdair::BomManager::
      getObjectPtr<stdair::PosChannel> (*lAirportPair_ptr,
                                        lPosChannelKey.toString());
    if (lPosChannel_ptr == NULL) {
      STDAIR_LOG_ERROR ("Cannot find yield corresponding to the PoS-"
                        << "Channel: " << lPosChannelKey.toString());
      assert (false);
    }

    // Retrieve the corresponding date period to lPreferredDepartureDate.
    const stdair::DatePeriodList_T lDatePeriodList =
      stdair::BomManager::getList<stdair::DatePeriod> (*lPosChannel_ptr);
    for (stdair::DatePeriodList_T::const_iterator itDatePeriod =
           lDatePeriodList.begin();
         itDatePeriod != lDatePeriodList.end(); ++itDatePeriod) {
      const stdair::DatePeriod* lDatePeriod_ptr = *itDatePeriod;
      assert (lDatePeriod_ptr != NULL);
      
      const bool isDepartureDateValid =
        lDatePeriod_ptr->isDepartureDateValid (lPreferredDepartureDate);
      
      if (isDepartureDateValid == true) {
          // Retrieve the corresponding time period.
          const stdair::TimePeriodList_T lTimePeriodList = stdair::
            BomManager::getList<stdair::TimePeriod> (*lDatePeriod_ptr);
          for (stdair::TimePeriodList_T::const_iterator itTimePeriod =
                 lTimePeriodList.begin();
               itTimePeriod != lTimePeriodList.end(); ++itTimePeriod) {
            const stdair::TimePeriod* lTimePeriod_ptr = *itTimePeriod;
            assert (lTimePeriod_ptr != NULL);
            
            
            const stdair::YieldFeaturesKey lYieldFeaturesKey (lPreferredCabin);
            stdair::YieldFeatures* oYieldFeatures_ptr = stdair::BomManager::
              getObjectPtr<stdair::YieldFeatures>(*lTimePeriod_ptr,
                                                  lYieldFeaturesKey.toString());
            if (oYieldFeatures_ptr != NULL) {
              return oYieldFeatures_ptr;
            }
          }
        }
      }
    return NULL;

  }

  // ///////////////////////////////////////////////////////////////////
  void AIRINV_Master_Service::
  forecast(const stdair::YieldFeatures& iYieldFeatures,
           const TRADEMGEN::DemandStream& iDemandStream,
           stdair::BomRoot& iBomRoot) {

    const stdair::AirlineClassListList_T lAirlineClassListList = stdair::BomManager::
      getList<stdair::AirlineClassList> (iYieldFeatures);
    assert (lAirlineClassListList.begin() != lAirlineClassListList.end());

    // Yield order check
    stdair::AirlineClassListList_T::const_iterator  itACL = lAirlineClassListList.begin();
    stdair::Yield_T lPreviousYield((*itACL)->getYield());
    ++itACL;
    for (; itACL != lAirlineClassListList.end(); ++itACL) {
      const stdair::AirlineClassList* lAirlineClassList = *itACL;
      const stdair::Yield_T& lYield = lAirlineClassList->getYield();
      if (lYield <= lPreviousYield) {
        lPreviousYield = lYield;
      }
      else{
        STDAIR_LOG_ERROR ("Yields should be given in a descendant order"
                          << " in the yield input file") ;
        assert (false);
      }
    }
    // Proportion factor list initialisation
    // Each element corresponds to a yield rule
    stdair::ProportionFactorList_T lProportionFactorList;
    stdair::ProportionFactor_T lPreviousProportionFactor = 0;

    
    const TRADEMGEN::DemandCharacteristics& lDemandCharacteristics =
      iDemandStream.getDemandCharacteristics();
    const stdair::WTP_T& lMinWTP = lDemandCharacteristics.getMinWTP();

    // TODO define lDTD properly
    stdair::FloatDuration_T lDTD = -40;

    // Retrieve the remaining percentage of booking requests
    const TRADEMGEN::ContinuousFloatDuration_T& lArrivalPattern =
     lDemandCharacteristics.getArrivalPattern();
    const stdair::Probability_T lRemainingProportion =
      lArrivalPattern.getRemainingProportion(lDTD);

    const stdair::NbOfRequests_T& lMeanNumberOfRequests =
      lRemainingProportion*iDemandStream.getMeanNumberOfRequests();
    const stdair::StdDevValue_T& lStdDevNumberOfRequests =
      lRemainingProportion*iDemandStream.getStdDevNumberOfRequests();

    // TODO define the frat5 coef properly
    stdair::RealNumber_T lFrat5Coef = 2.0;

    // Proportion list computation
    itACL = lAirlineClassListList.begin();
    for (; itACL != lAirlineClassListList.end(); ++itACL) {
      const stdair::AirlineClassList* lAirlineClassList = *itACL;
      const stdair::Yield_T& lYield = lAirlineClassList->getYield();
      stdair::ProportionFactor_T lProportionFactor =
        exp ((lYield - lMinWTP)*log(0.5)/(lMinWTP*(lFrat5Coef-1.0)))/lMeanNumberOfRequests;
      lProportionFactorList.push_back(lProportionFactor - lPreviousProportionFactor);      
      lPreviousProportionFactor = lProportionFactor;
    }

    // Sanity check
    assert (lAirlineClassListList.size() == lProportionFactorList.size());

    // Store the forecast in the booking classes
    stdair::ProportionFactorList_T::const_iterator itPF = lProportionFactorList.begin();
    itACL = lAirlineClassListList.begin();
    for (; itACL != lAirlineClassListList.end(); ++itACL, ++itPF) {
      const stdair::AirlineClassList* lAirlineClassList = *itACL;
      const stdair::ProportionFactor_T& lProportionFactor = *itPF;
      stdair::NbOfRequests_T lNumberOfRequests = lProportionFactor*lMeanNumberOfRequests;
      stdair::StdDevValue_T lStdDevValue = lProportionFactor*lStdDevNumberOfRequests;
      setForecast(*lAirlineClassList, lNumberOfRequests, lStdDevValue,
                  iDemandStream, iBomRoot);
    }  
    
  }

  // ///////////////////////////////////////////////////////////////////
  void AIRINV_Master_Service::
  setForecast (const stdair::AirlineClassList& iAirlineClassList,
               const stdair::NbOfRequests_T& iNumberOfRequests,
               const stdair::StdDevValue_T& iStdDevValue,
               const TRADEMGEN::DemandStream& iDemandStream,
               stdair::BomRoot& iBomRoot) {
    
    const stdair::AirportCode_T& lOrigin = iDemandStream.getOrigin();
    const stdair::AirportCode_T& lDestination = iDemandStream.getDestination();

    const stdair::Date_T& lPreferredDepartureDate =
      iDemandStream.getPreferredDepartureDate();
    const stdair::CabinCode_T& lPreferredCabin = iDemandStream.getPreferredCabin();

    const stdair::AirlineCodeList_T& lAirlineCodeList =
      iAirlineClassList.getAirlineCodeList();

    const stdair::ClassList_StringList_T& lClassCodeList =
      iAirlineClassList.getClassCodeList();

    // Sanity check
    assert (lAirlineCodeList.size() == lClassCodeList.size());
    assert (!lAirlineCodeList.empty());
    
    if (lAirlineCodeList.size() == 1) {
      stdair::AirlineCode_T lAirlineCode = lAirlineCodeList.front();
      //setForecast(lAirlineCode, iNumberOfRequests, iStdDevValue,
      //iDemandStream,iBomRoot);
    }
    else {
      stdair::AirlineCodeList_T::const_iterator itAC = lAirlineCodeList.begin();
      stdair::ClassList_StringList_T::const_iterator itCC = lClassCodeList.begin();

      for (; itAC != lAirlineCodeList.end(); ++itAC, ++itCC) {
        
      }
    }
  }

}
