// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// Boost
#include <boost/make_shared.hpp>
// StdAir
#include <stdair/stdair_inventory_types.hpp>
#include <stdair/basic/BasChronometer.hpp>
#include <stdair/bom/BomManager.hpp>
#include <stdair/bom/BomRoot.hpp>
#include <stdair/bom/Inventory.hpp>
#include <stdair/bom/FlightDate.hpp>
#include <stdair/bom/LegCabin.hpp>
#include <stdair/command/CmdBomManager.hpp>
#include <stdair/service/Logger.hpp>
#include <stdair/STDAIR_Service.hpp>
#include <stdair/bom/LegDate.hpp>
// StdAir
#include <stdair/bom/EventQueue.hpp>
#include <stdair/bom/YieldFeatures.hpp>
#include <stdair/bom/AirportPair.hpp>
#include <stdair/bom/PosChannel.hpp>
#include <stdair/bom/DatePeriod.hpp>
#include <stdair/bom/TimePeriod.hpp>
#include <stdair/bom/AirlineClassList.hpp>
#include <stdair/basic/BasConst_Request.hpp>
#include <stdair/basic/BasConst_Inventory.hpp>
#include <stdair/bom/Inventory.hpp>
#include <stdair/bom/FlightDate.hpp>
#include <stdair/bom/SegmentDate.hpp>
#include <stdair/bom/SegmentCabin.hpp>
#include <stdair/bom/BookingClass.hpp>
#include <stdair/bom/OnDDate.hpp>
// RMOL
#include <rmol/basic/BasConst_RMOL_Service.hpp>
#include <rmol/factory/FacRmolServiceContext.hpp>
#include <rmol/command/InventoryParser.hpp>
#include <rmol/command/Optimiser.hpp>
#include <rmol/command/Forecaster.hpp>
#include <rmol/service/RMOL_ServiceContext.hpp>
#include <rmol/RMOL_Service.hpp>
// Trademgen
#include <trademgen/bom/DemandStream.hpp>

namespace RMOL {

  // ////////////////////////////////////////////////////////////////////
  RMOL_Service::RMOL_Service() : _rmolServiceContext (NULL),
                                 _previousForecastDate (stdair::Date_T (2000, 1, 1)) {
    assert (false);
  }

  // ////////////////////////////////////////////////////////////////////
  RMOL_Service::RMOL_Service (const RMOL_Service& iService) :
    _rmolServiceContext (NULL), _previousForecastDate (stdair::Date_T (2000, 1, 1)) {
    assert (false);
  }

  // ////////////////////////////////////////////////////////////////////
  RMOL_Service::RMOL_Service (const stdair::BasLogParams& iLogParams,
                              const stdair::BasDBParams& iDBParams,
                              const stdair::CabinCapacity_T& iCabinCapacity,
                              const stdair::Filename_T& iInputFileName) :
    _rmolServiceContext (NULL), _previousForecastDate (stdair::Date_T (2000, 1, 1)) {

    // Initialise the STDAIR service handler
    stdair::STDAIR_ServicePtr_T lSTDAIR_Service_ptr =
      initStdAirService (iLogParams, iDBParams);
    
    // Initialise the service context
    initServiceContext();

    // Add the StdAir service context to the RMOL service context
    // \note RMOL owns the STDAIR service resources here.
    const bool ownStdairService = true;
    addStdAirService (lSTDAIR_Service_ptr, ownStdairService);

    // Initialise the (remaining of the) context
    initRmolService (iCabinCapacity, iInputFileName);
  }

  // ////////////////////////////////////////////////////////////////////
  RMOL_Service::RMOL_Service (const stdair::BasLogParams& iLogParams,
                              const stdair::CabinCapacity_T& iCabinCapacity,
                              const stdair::Filename_T& iInputFileName) :
    _rmolServiceContext (NULL), _previousForecastDate (stdair::Date_T (2000, 1, 1)) {

    // Initialise the STDAIR service handler
    stdair::STDAIR_ServicePtr_T lSTDAIR_Service_ptr =
      initStdAirService (iLogParams);
    
    // Initialise the service context
    initServiceContext();

    // Add the StdAir service context to the RMOL service context
    // \note RMOL owns the STDAIR service resources here.
    const bool ownStdairService = true;
    addStdAirService (lSTDAIR_Service_ptr, ownStdairService);

    // Initialise the (remaining of the) context
    initRmolService (iCabinCapacity, iInputFileName);
  }

  // ////////////////////////////////////////////////////////////////////
  RMOL_Service::RMOL_Service (const stdair::BasLogParams& iLogParams,
                              const stdair::BasDBParams& iDBParams,
                              const stdair::CabinCapacity_T& iCabinCapacity) :
    _rmolServiceContext (NULL), _previousForecastDate (stdair::Date_T (2000, 1, 1)) {

    // Initialise the STDAIR service handler
    stdair::STDAIR_ServicePtr_T lSTDAIR_Service_ptr =
      initStdAirService (iLogParams, iDBParams);
    
    // Initialise the service context
    initServiceContext();

    // Add the StdAir service context to the RMOL service context
    // \note RMOL owns the STDAIR service resources here.
    const bool ownStdairService = true;
    addStdAirService (lSTDAIR_Service_ptr, ownStdairService);

    // Initialise the (remaining of the) context
    initRmolService (iCabinCapacity);
  }

  // ////////////////////////////////////////////////////////////////////
  RMOL_Service::RMOL_Service (const stdair::BasLogParams& iLogParams,
                              const stdair::CabinCapacity_T& iCabinCapacity) :
    _rmolServiceContext (NULL), _previousForecastDate (stdair::Date_T (2000, 1, 1)) {

    // Initialise the STDAIR service handler
    stdair::STDAIR_ServicePtr_T lSTDAIR_Service_ptr =
      initStdAirService (iLogParams);
    
    // Initialise the service context
    initServiceContext();

    // Add the StdAir service context to the RMOL service context
    // \note RMOL owns the STDAIR service resources here.
    const bool ownStdairService = true;
    addStdAirService (lSTDAIR_Service_ptr, ownStdairService);

    // Initialise the (remaining of the) context
    initRmolService (iCabinCapacity);
  }

  // ////////////////////////////////////////////////////////////////////
  RMOL_Service::RMOL_Service (stdair::STDAIR_ServicePtr_T ioSTDAIRServicePtr,
                              const stdair::CabinCapacity_T& iCabinCapacity,
                              const stdair::Filename_T& iInputFileName)
    : _rmolServiceContext (NULL), _previousForecastDate (stdair::Date_T (2000, 1, 1)) {
    
    // Initialise the context
    initServiceContext();
    
    // Add the StdAir service context to the RMOL service context.
    // \note RMOL does not own the STDAIR service resources here.
    const bool doesNotOwnStdairService = false;
    addStdAirService (ioSTDAIRServicePtr, doesNotOwnStdairService);

    // Initialise the (remaining of the) context
    initRmolService (iCabinCapacity, iInputFileName);
  }

  // ////////////////////////////////////////////////////////////////////
  RMOL_Service::RMOL_Service (stdair::STDAIR_ServicePtr_T ioSTDAIRServicePtr)
    : _rmolServiceContext (NULL), _previousForecastDate (stdair::Date_T (2000, 1, 1)) {

    // Initialise the context
    initServiceContext();
    
    // Add the StdAir service context to the RMOL service context.
    // \note RMOL does not own the STDAIR service resources here.
    const bool doesNotOwnStdairService = false;
    addStdAirService (ioSTDAIRServicePtr, doesNotOwnStdairService);
  }

  // ////////////////////////////////////////////////////////////////////
  RMOL_Service::~RMOL_Service() {
    // Delete/Clean all the objects from memory
    finalise();
  }

  // ////////////////////////////////////////////////////////////////////
  void RMOL_Service::finalise() {
    assert (_rmolServiceContext != NULL);
    // Reset the (Boost.)Smart pointer pointing on the STDAIR_Service object.
    _rmolServiceContext->reset();
  }

  // ////////////////////////////////////////////////////////////////////
  void RMOL_Service::initServiceContext() {
    // Initialise the service context
    RMOL_ServiceContext& lRMOL_ServiceContext = 
      FacRmolServiceContext::instance().create();
    _rmolServiceContext = &lRMOL_ServiceContext;
  }

  // ////////////////////////////////////////////////////////////////////
  void RMOL_Service::
  addStdAirService (stdair::STDAIR_ServicePtr_T ioSTDAIR_Service_ptr,
                    const bool iOwnStdairService) {

    // Retrieve the RMOL service context
    assert (_rmolServiceContext != NULL);
    RMOL_ServiceContext& lRMOL_ServiceContext = *_rmolServiceContext;

    // Store the STDAIR service object within the (AIRINV) service context
    lRMOL_ServiceContext.setSTDAIR_Service (ioSTDAIR_Service_ptr,
                                            iOwnStdairService);
  }

  // //////////////////////////////////////////////////////////////////////
  stdair::STDAIR_ServicePtr_T RMOL_Service::
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
  stdair::STDAIR_ServicePtr_T RMOL_Service::
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
  void RMOL_Service::initRmolService() {
    // Do nothing at this stage. A sample BOM tree may be built by
    // calling the buildSampleBom() method
  }

  // ////////////////////////////////////////////////////////////////////
  void RMOL_Service::
  initRmolService (const stdair::CabinCapacity_T& iCabinCapacity) {
    // Build a dummy inventory with a leg-cabin which has the given capacity.
    // The STDAIR-held BOM tree is altered correspondingly.
    const bool isForDemo = true;
    buildSampleBom (isForDemo, iCabinCapacity);
  }

  // ////////////////////////////////////////////////////////////////////
  void RMOL_Service::
  initRmolService (const stdair::CabinCapacity_T& iCabinCapacity,
                   const stdair::Filename_T& iInputFileName) {

    // Build a dummy inventory with a leg-cabin which has the given capacity.
    // The STDAIR-held BOM tree is altered correspondingly.
    const bool isForDemo = true;
    buildSampleBom (isForDemo, iCabinCapacity);

    // Retrieve the BOM tree root
    assert (_rmolServiceContext != NULL);
    RMOL_ServiceContext& lRMOL_ServiceContext = *_rmolServiceContext;
    stdair::STDAIR_Service& lSTDAIR_Service =
      lRMOL_ServiceContext.getSTDAIR_Service();
    stdair::BomRoot& lBomRoot = lSTDAIR_Service.getBomRoot();

    // Complete the BOM tree with the optimisation problem specification
    InventoryParser::parseInputFileAndBuildBom (iInputFileName, lBomRoot);
  }
  
  // ////////////////////////////////////////////////////////////////////
  void RMOL_Service::
  buildSampleBom (const bool isForDemo,
                  const stdair::CabinCapacity_T iCabinCapacity) {

    // Retrieve the BOM tree root
    assert (_rmolServiceContext != NULL);
    RMOL_ServiceContext& lRMOL_ServiceContext = *_rmolServiceContext;

    // Retrieve the StdAir service
    stdair::STDAIR_Service& lSTDAIR_Service =
      lRMOL_ServiceContext.getSTDAIR_Service();
    stdair::BomRoot& lBomRoot = lSTDAIR_Service.getBomRoot();

    // Delegate the action to the dedicated command
    if (isForDemo == true) {
      stdair::CmdBomManager::buildSampleBomForRMOL (lBomRoot, iCabinCapacity);

    } else {
      stdair::CmdBomManager::buildSampleBom (lBomRoot);
    }
  }

  // ////////////////////////////////////////////////////////////////////
  void RMOL_Service::optimalOptimisationByMCIntegration (const int K) {
    assert (_rmolServiceContext != NULL);
    RMOL_ServiceContext& lRMOL_ServiceContext = *_rmolServiceContext;

    // Retrieve the StdAir service
    stdair::STDAIR_Service& lSTDAIR_Service =
      lRMOL_ServiceContext.getSTDAIR_Service();
    stdair::BomRoot& lBomRoot = lSTDAIR_Service.getBomRoot();

    //
    stdair::LegCabin& lLegCabin = InventoryParser::getSampleLegCabin (lBomRoot);

    stdair::BasChronometer lOptimisationChronometer;
    lOptimisationChronometer.start();

    Optimiser::optimalOptimisationByMCIntegration (K, lLegCabin);

    const double lOptimisationMeasure = lOptimisationChronometer.elapsed();
    
    // DEBUG
    STDAIR_LOG_DEBUG ("Optimisation by Monte-Carlo performed in "
                      << lOptimisationMeasure);
    STDAIR_LOG_DEBUG ("Result: " << lLegCabin.displayVirtualClassList());

    std::ostringstream logStream;
    stdair::BidPriceVector_T lBidPriceVector = lLegCabin.getBidPriceVector();
    logStream << "Bid-Price Vector (BPV): ";
    unsigned int size = lBidPriceVector.size();
    
    for (unsigned int i = 0; i < size - 1; ++i) {
      const double bidPrice = lBidPriceVector.at(i);
      logStream << std::fixed << std::setprecision (2) << bidPrice << ", ";
    }
    const double bidPrice = lBidPriceVector.at(size -1);
    logStream << std::fixed << std::setprecision (2) << bidPrice;
    STDAIR_LOG_DEBUG (logStream.str());
  }

  // ////////////////////////////////////////////////////////////////////
  void RMOL_Service::optimalOptimisationByDP() {
  }
  
  // ////////////////////////////////////////////////////////////////////
  void RMOL_Service::heuristicOptimisationByEmsr() {
    assert (_rmolServiceContext != NULL);
    RMOL_ServiceContext& lRMOL_ServiceContext = *_rmolServiceContext;

    // Retrieve the StdAir service
    stdair::STDAIR_Service& lSTDAIR_Service =
      lRMOL_ServiceContext.getSTDAIR_Service();
    stdair::BomRoot& lBomRoot = lSTDAIR_Service.getBomRoot();

    //
    stdair::LegCabin& lLegCabin = InventoryParser::getSampleLegCabin (lBomRoot);

    stdair::BasChronometer lOptimisationChronometer;
    lOptimisationChronometer.start();
    
    Optimiser::heuristicOptimisationByEmsr (lLegCabin);
    
    const double lOptimisationMeasure = lOptimisationChronometer.elapsed();
    // DEBUG
    STDAIR_LOG_DEBUG ("Optimisation EMSR performed in "
                      << lOptimisationMeasure);
    STDAIR_LOG_DEBUG ("Result: " << lLegCabin.displayVirtualClassList());

    std::ostringstream logStream;
    stdair::BidPriceVector_T lBidPriceVector = lLegCabin.getBidPriceVector();
    logStream << "Bid-Price Vector (BPV): ";
    const unsigned int size = lBidPriceVector.size();
    
    for (unsigned int i = 0; i < size - 1; ++i) {
      const double bidPrice = lBidPriceVector.at(i);
      logStream << std::fixed << std::setprecision (2) << bidPrice << ", ";
    }
    const double bidPrice = lBidPriceVector.at(size -1);
    logStream << std::fixed << std::setprecision (2) << bidPrice;
    STDAIR_LOG_DEBUG (logStream.str());
  }

  // ////////////////////////////////////////////////////////////////////
  void RMOL_Service::heuristicOptimisationByEmsrA() {
    assert (_rmolServiceContext != NULL);
    RMOL_ServiceContext& lRMOL_ServiceContext = *_rmolServiceContext;

    // Retrieve the StdAir service
    stdair::STDAIR_Service& lSTDAIR_Service =
      lRMOL_ServiceContext.getSTDAIR_Service();
    stdair::BomRoot& lBomRoot = lSTDAIR_Service.getBomRoot();

    //
    stdair::LegCabin& lLegCabin = InventoryParser::getSampleLegCabin (lBomRoot);

    Optimiser::heuristicOptimisationByEmsrA (lLegCabin);

    // DEBUG
    STDAIR_LOG_DEBUG ("Result: " << lLegCabin.displayVirtualClassList());
    
  }

  // ////////////////////////////////////////////////////////////////////
  void RMOL_Service::heuristicOptimisationByEmsrB() {
    assert (_rmolServiceContext != NULL);
    RMOL_ServiceContext& lRMOL_ServiceContext = *_rmolServiceContext;

    // Retrieve the StdAir service
    stdair::STDAIR_Service& lSTDAIR_Service =
      lRMOL_ServiceContext.getSTDAIR_Service();
    stdair::BomRoot& lBomRoot = lSTDAIR_Service.getBomRoot();

    //
    stdair::LegCabin& lLegCabin = InventoryParser::getSampleLegCabin (lBomRoot);

    Optimiser::heuristicOptimisationByEmsrB (lLegCabin);

    // DEBUG
    STDAIR_LOG_DEBUG ("Result: " << lLegCabin.displayVirtualClassList());
  }

  // ////////////////////////////////////////////////////////////////////
  bool RMOL_Service::optimise (stdair::FlightDate& ioFlightDate,
                               const stdair::DateTime_T& iRMEventTime) {

    STDAIR_LOG_DEBUG ("RMOL::optimise called");    

    if (_previousForecastDate < iRMEventTime.date()) {
      forecast (iRMEventTime);
      // optimiseBPWithDemandAggregation (iRMEventTime);
      optimiseBPWithYieldProration (iRMEventTime);
    }    
    return false;    
    //Call the functions in the forecaster and the optimiser.
    //DEBUG
    // STDAIR_LOG_DEBUG ("Forecast");
    // bool isForecasted = Forecaster::forecast (ioFlightDate, iRMEventTime);
    // STDAIR_LOG_DEBUG ("Forecast successful: " << isForecasted);
    // if (isForecasted == true) {
    //   STDAIR_LOG_DEBUG ("Optimise");
    //   Optimiser::optimise (ioFlightDate);
    //   return true;
    // } else {
    //   return false;
    // }
  }

    // ////////////////////////////////////////////////////////////////////
  void RMOL_Service::forecast (const stdair::DateTime_T& iRMEventTime) {

    if (_rmolServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The Rmol service "
                                                    "has not been initialised");
    }
    assert (_rmolServiceContext != NULL);
    RMOL_ServiceContext& lRMOL_ServiceContext = *_rmolServiceContext;

    // Retrieve the bom root
    stdair::STDAIR_Service& lSTDAIR_Service =
      lRMOL_ServiceContext.getSTDAIR_Service();
    stdair::BomRoot& lBomRoot = lSTDAIR_Service.getBomRoot();

    // Retrieve the demand stream list
    stdair::EventQueue& lEventQueue = lSTDAIR_Service.getEventQueue();    
    const TRADEMGEN::DemandStreamList_T lDemandStreamList =
      stdair::BomManager::getList<TRADEMGEN::DemandStream> (lEventQueue);

    // Retrieve the date from the RM event
    const stdair::Date_T lDate = iRMEventTime.date();

    _previousForecastDate = lDate;

    // Browse the demand stream list
    for (TRADEMGEN::DemandStreamList_T::const_iterator itDS = lDemandStreamList.begin();
         itDS != lDemandStreamList.end(); ++itDS) {
      TRADEMGEN::DemandStream* lDemandStream_ptr = *itDS;
      assert (lDemandStream_ptr != NULL);

      const stdair::Date_T& lPreferredDepartureDate =
        lDemandStream_ptr->getPreferredDepartureDate();
      stdair::DateOffset_T lDateOffset = lPreferredDepartureDate - lDate;
      stdair::DTD_T lDTD = short (lDateOffset.days());
      
      stdair::DCPList_T::const_iterator itDCP =
        std::find (stdair::DEFAULT_DCP_LIST.begin(), stdair::DEFAULT_DCP_LIST.end(), lDTD);
      if (itDCP != stdair::DEFAULT_DCP_LIST.end()) {        

        // Retrieve the yield features object associated to the demand
        stdair::YieldFeatures* lYieldFeatures_ptr =
          getYieldFeatures(*lDemandStream_ptr, lBomRoot);
        
        if (lYieldFeatures_ptr == NULL) {
          const TRADEMGEN::DemandStreamKey& lDemandStreamKey =
            lDemandStream_ptr->getKey();
          STDAIR_LOG_ERROR ("Cannot find yield corresponding to the Demand"
                            << " Stream: " << lDemandStreamKey.toString());
          assert (false);
        }        
        forecast (lDTD, *lYieldFeatures_ptr, *lDemandStream_ptr, lBomRoot);
      }
      
    }
  }

  // ///////////////////////////////////////////////////////////////////
  stdair::YieldFeatures* RMOL_Service::
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

    // Retrieve the corresponding date period to lPreferredDepartureDate.
    const stdair::DatePeriodList_T lDatePeriodList =
      stdair::BomManager::getList<stdair::DatePeriod> (*lAirportPair_ptr);
    for (stdair::DatePeriodList_T::const_iterator itDatePeriod =
           lDatePeriodList.begin();
         itDatePeriod != lDatePeriodList.end(); ++itDatePeriod) {
      const stdair::DatePeriod* lDatePeriod_ptr = *itDatePeriod;
      assert (lDatePeriod_ptr != NULL);
      
      const bool isDepartureDateValid =
        lDatePeriod_ptr->isDepartureDateValid (lPreferredDepartureDate);
      
      if (isDepartureDateValid == true) {
        // Retrieve the PoS-Channel.
        // TODO: Use POS and Channel from demand instead of default
        const stdair::PosChannelKey lPosChannelKey (stdair::DEFAULT_POS,
                                                    stdair::DEFAULT_CHANNEL);
        stdair::PosChannel* lPosChannel_ptr = stdair::BomManager::
          getObjectPtr<stdair::PosChannel> (*lDatePeriod_ptr,
                                            lPosChannelKey.toString());
        if (lPosChannel_ptr == NULL) {
          STDAIR_LOG_ERROR ("Cannot find yield corresponding to the PoS-"
                            << "Channel: " << lPosChannelKey.toString());
          assert (false);
        }
        // Retrieve the yield features.
        const stdair::TimePeriodList_T lTimePeriodList = stdair::
          BomManager::getList<stdair::TimePeriod> (*lPosChannel_ptr);
        for (stdair::TimePeriodList_T::const_iterator itTimePeriod =
               lTimePeriodList.begin();
             itTimePeriod != lTimePeriodList.end(); ++itTimePeriod) {
          const stdair::TimePeriod* lTimePeriod_ptr = *itTimePeriod;
          assert (lTimePeriod_ptr != NULL);
          
          // TODO: Use trip type from demand instead of default value.
          const stdair::YieldFeaturesKey lYieldFeaturesKey (stdair::DEFAULT_TRIP_TYPE,
                                                            lPreferredCabin);
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
  void RMOL_Service::
  forecast(const stdair::DTD_T& iDTD,
           const stdair::YieldFeatures& iYieldFeatures,
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

    // Retrieve the minimal willingness to pay associated to the demand
    const TRADEMGEN::DemandCharacteristics& lDemandCharacteristics =
      iDemandStream.getDemandCharacteristics();
    const stdair::WTP_T& lMinWTP = lDemandCharacteristics.getMinWTP();
   
    // Retrieve the remaining percentage of booking requests
    const TRADEMGEN::ContinuousFloatDuration_T& lArrivalPattern =
     lDemandCharacteristics.getArrivalPattern();
    STDAIR_LOG_DEBUG (lArrivalPattern.displayCumulativeDistribution());
    const stdair::Probability_T lRemainingProportion =
      lArrivalPattern.getRemainingProportion(-float(iDTD));

    

    // Compute the characteristics (mean and std dev) of the total forecast demand to come
    const stdair::NbOfRequests_T& lMeanNumberOfRequests =
      lRemainingProportion*iDemandStream.getMeanNumberOfRequests();
    const stdair::StdDevValue_T& lStdDevNumberOfRequests =
      lRemainingProportion*iDemandStream.getStdDevNumberOfRequests();

    // Retrieve the frat5 coef corresponding to the input dtd
    stdair::DTDFratMap_T::const_iterator itDFC = stdair::DEFAULT_DTD_FRAT5COEF_MAP.find(iDTD);
    if (itDFC == stdair::DEFAULT_DTD_FRAT5COEF_MAP.end()) {
      STDAIR_LOG_ERROR ("Cannot find frat5 coef for DTD = " << iDTD );
      assert (false);
    }
    stdair::RealNumber_T lFrat5Coef = stdair::DEFAULT_DTD_FRAT5COEF_MAP.at(iDTD);

    STDAIR_LOG_DEBUG ("Remaining proportion " << lRemainingProportion << " Total "
                      << iDemandStream.getMeanNumberOfRequests() << " StdDev "
                      << iDemandStream.getStdDevNumberOfRequests() <<"Frat5 Coef "
                      << lFrat5Coef);

    std::ostringstream oStr;
    // Compute the "forecast demand to come" proportion by class
    itACL = lAirlineClassListList.begin();
    for (; itACL != lAirlineClassListList.end(); ++itACL) {
      const stdair::AirlineClassList* lAirlineClassList = *itACL;
      const stdair::Yield_T& lYield = lAirlineClassList->getYield();
      stdair::ProportionFactor_T lProportionFactor =
        exp ((lYield - lMinWTP)*log(0.5)/(lMinWTP*(lFrat5Coef-1.0)));
      // If the yield is smaller than minimal WTP, the factor is greater than 1.
      // In that case it should be modified and put to 1.
      lProportionFactor = std::min (lProportionFactor, 1.0);
      lProportionFactorList.push_back(lProportionFactor - lPreviousProportionFactor);      
      lPreviousProportionFactor = lProportionFactor;
      oStr << lPreviousProportionFactor << " ";
    }

    STDAIR_LOG_DEBUG (oStr.str());

    // Sanity check
    assert (lAirlineClassListList.size() == lProportionFactorList.size());

    STDAIR_LOG_DEBUG ("Forecast for " << iDemandStream.describeKey()
                      << " " << iDTD << " days to departure");

    // store the forecast demand to come characteristics in the booking classes
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
  void RMOL_Service::
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

    // Retrieve the class list (one class per airline)
    const stdair::ClassList_StringList_T& lClassList_StringList =
      iAirlineClassList.getClassCodeList();
    assert (!lClassList_StringList.empty());
    stdair::ClassCodeList_T lClassCodeList;
    for (stdair::ClassList_StringList_T::const_iterator itCL = lClassList_StringList.begin();
         itCL != lClassList_StringList.end(); ++itCL){
      const stdair::ClassList_String_T& lClassList_String = *itCL;
      assert (lClassList_String.size() > 0);
      stdair::ClassCode_T lFirstClass;
      lFirstClass.append (lClassList_String, 0, 1);
      lClassCodeList.push_back(lFirstClass);
    }

    // Sanity check
    assert (lAirlineCodeList.size() == lClassCodeList.size());
    assert (!lAirlineCodeList.empty());

    if (lAirlineCodeList.size() == 1) {
      // Store the forecast information in the case of a single segment
      stdair::AirlineCode_T lAirlineCode = lAirlineCodeList.front();
      stdair::ClassCode_T lClassCode = lClassCodeList.front();
      setForecast(lAirlineCode, lPreferredDepartureDate, lOrigin,
                  lDestination, lPreferredCabin, lClassCode,
                  iNumberOfRequests, iStdDevValue, iBomRoot);

      // TODO: add yield to the input parameters.
    }
    else {
      // Store the forecast information in the case of a multiple segment
      stdair::AirlineCodeList_T::const_iterator beginItAC = lAirlineCodeList.begin();
      stdair::ClassCodeList_T::const_iterator beginItCC = lClassCodeList.begin();
      const stdair::Yield_T& lYield = iAirlineClassList.getYield();
      stdair::AirportCode_T lCurrentOrigin(lOrigin);
      stdair::AirportCodeList_T lAirportCodeList;
      bool lDoneForecast =
        setForecast(beginItAC, lAirlineCodeList, lPreferredDepartureDate, lCurrentOrigin,
                    lDestination, lPreferredCabin, beginItCC, lClassCodeList,
                    iNumberOfRequests, iStdDevValue, lYield, lAirportCodeList, iBomRoot);
      if (!lDoneForecast) {
        STDAIR_LOG_ERROR ("Cannot find the required multiline classpath for the segment "
                          << lOrigin << "-" << lDestination);
        assert(false);
      }
    }
  }

  // ///////////////////////////////////////////////////////////////////
  void RMOL_Service::
  setForecast (const stdair::AirlineCode_T& iAirlineCode,
               const stdair::Date_T& iPreferredDepartureDate,
               const stdair::AirportCode_T& iOrigin,
               const stdair::AirportCode_T& iDestination,
               const stdair::CabinCode_T& iPreferredCabin,
               const stdair::ClassCode_T& iClassCode,
               const stdair::NbOfRequests_T& iNumberOfRequests,
               const stdair::StdDevValue_T& iStdDevValue,                    
               stdair::BomRoot& iBomRoot) {
    stdair::Inventory* lInventory_ptr = iBomRoot.getInventory(iAirlineCode);
    if (lInventory_ptr == NULL) {
      STDAIR_LOG_ERROR ("Cannot find the inventory corresponding"
                        << " to the airline" << iAirlineCode) ;
      assert(false);
    }
    const stdair::FlightDateList_T lFlightDateList =
      stdair::BomManager::getList<stdair::FlightDate> (*lInventory_ptr);
    assert (!lFlightDateList.empty());
    bool lFoundBookingClass = false;
    for (stdair::FlightDateList_T::const_iterator itFD = lFlightDateList.begin();
         itFD != lFlightDateList.end(); ++itFD) {
      const stdair::FlightDate* lFlightDate_ptr = *itFD;
      if (lFlightDate_ptr != NULL) {
        const stdair::Date_T& lDepartureDate = lFlightDate_ptr->getDepartureDate();
        if (lDepartureDate == iPreferredDepartureDate) {
          const stdair::SegmentDateKey lSegmentDateKey(iOrigin, iDestination);
          stdair::SegmentDate* lSegmentDate_ptr = lFlightDate_ptr->getSegmentDate(lSegmentDateKey);
          if (lSegmentDate_ptr != NULL) {
            stdair::SegmentCabin* lSegmentCabin_ptr = lSegmentDate_ptr->getSegmentCabin(iPreferredCabin);
            if (lSegmentCabin_ptr != NULL) {
              stdair::BookingClass* lBookingClass_ptr =
                stdair::BomManager::getObjectPtr<stdair::BookingClass> (*lSegmentCabin_ptr, iClassCode);
              if (lBookingClass_ptr != NULL) {
                lBookingClass_ptr->setMean(iNumberOfRequests);
                lBookingClass_ptr->setStdDev(iStdDevValue);
                lFoundBookingClass = true;

                STDAIR_LOG_DEBUG (iAirlineCode << " Class " << iClassCode << " Mean " << iNumberOfRequests
                                  << " Std Dev " << iStdDevValue);
              }
            }
          }
        }
      }
    }
    if (!lFoundBookingClass) {
      STDAIR_LOG_ERROR ("Cannot find class " << iClassCode << " in cabin " << iPreferredCabin
                        << " for the segment " << iOrigin << "-" << iDestination << " with"
                        << " the airline " << iAirlineCode);
      assert(false);
    }
  }

  // ///////////////////////////////////////////////////////////////////
  bool RMOL_Service::
  setForecast (stdair::AirlineCodeList_T::const_iterator& beginItAC,
               const stdair::AirlineCodeList_T& iAirlineCodeList,
               const stdair::Date_T& iPreferredDepartureDate,
               stdair::AirportCode_T& iOrigin,
               const stdair::AirportCode_T& iDestination,
               const stdair::CabinCode_T& iPreferredCabin,
               stdair::ClassCodeList_T::const_iterator& beginItCC,
               const stdair::ClassCodeList_T& iClassCodeList,
               const stdair::NbOfRequests_T& iNumberOfRequests,
               const stdair::StdDevValue_T& iStdDevValue,
               const stdair::Yield_T& iYield,
               stdair::AirportCodeList_T& iAirportCodeList,
               stdair::BomRoot& iBomRoot) {
    const stdair::AirlineCode_T& lAirlineCode = *beginItAC;
    stdair::Inventory* lInventory_ptr = iBomRoot.getInventory(lAirlineCode);
    if (lInventory_ptr == NULL) {
      STDAIR_LOG_ERROR ("Cannot find the inventory corresponding"
                        << " to the airline" << lAirlineCode) ;
      assert(false);
    }
    const stdair::FlightDateList_T lFlightDateList =
      stdair::BomManager::getList<stdair::FlightDate> (*lInventory_ptr);
    assert (!lFlightDateList.empty());
    iAirportCodeList.push_back(iOrigin);
    stdair::AirlineCodeList_T::const_iterator endItAC = iAirlineCodeList.end();
    bool lFoundBookingClasses = false;
    for (stdair::FlightDateList_T::const_iterator itFD = lFlightDateList.begin();
         itFD != lFlightDateList.end(); ++itFD) {
      const stdair::FlightDate* lFlightDate_ptr = *itFD;
      assert(lFlightDate_ptr != NULL);
      const stdair::Date_T& lDepartureDate = lFlightDate_ptr->getDepartureDate();
      if (lDepartureDate == iPreferredDepartureDate) {
        if (std::distance(beginItAC,endItAC) > 1) {
          const stdair::SegmentDateList_T lSegmentDateList =
            stdair::BomManager::getList<stdair::SegmentDate> (*lFlightDate_ptr);
          assert(!lSegmentDateList.empty());
          for (stdair::SegmentDateList_T::const_iterator itSD = lSegmentDateList.begin();
               itSD != lSegmentDateList.end(); ++itSD) {
            const stdair::SegmentDate* lSegmentDate_ptr = *itSD;
            assert(lSegmentDate_ptr != NULL);
            const stdair::AirportCode_T& lOrigin = lSegmentDate_ptr->getBoardingPoint();
            if (lOrigin == iOrigin) {
              stdair::SegmentCabin* lSegmentCabin_ptr = lSegmentDate_ptr->getSegmentCabin(iPreferredCabin);
              if (lSegmentCabin_ptr != NULL) {
                const stdair::ClassCode_T& lClassCode = *beginItCC;
                stdair::BookingClass* lBookingClass_ptr =
                  stdair::BomManager::getObjectPtr<stdair::BookingClass> (*lSegmentCabin_ptr, lClassCode);
                if (lBookingClass_ptr != NULL) {
                  const stdair::AirportCode_T& lDestination = lSegmentDate_ptr->getOffPoint();
                  iOrigin = lDestination;
                  ++beginItAC;
                  ++beginItCC;
                  lFoundBookingClasses = setForecast(beginItAC, iAirlineCodeList, iPreferredDepartureDate,
                                                     iOrigin, iDestination, iPreferredCabin, beginItCC,
                                                     iClassCodeList, iNumberOfRequests, iStdDevValue,
                                                     iYield, iAirportCodeList, iBomRoot);
                  if (lFoundBookingClasses) {
                    lBookingClass_ptr->updateForecast(iAirlineCodeList, iClassCodeList, iAirportCodeList,
                                                      iNumberOfRequests, iStdDevValue, iYield);
                    break;
                  }
                }
              }
            }
          }
        }
        else {
          const stdair::SegmentDateKey lSegmentDateKey(iOrigin, iDestination);
          stdair::SegmentDate* lSegmentDate_ptr = lFlightDate_ptr->getSegmentDate(lSegmentDateKey);
          if (lSegmentDate_ptr != NULL) {
            stdair::SegmentCabin* lSegmentCabin_ptr = lSegmentDate_ptr->getSegmentCabin(iPreferredCabin);
            if (lSegmentCabin_ptr != NULL) {
              const stdair::ClassCode_T& lClassCode = *beginItCC;
              stdair::BookingClass* lBookingClass_ptr =
                stdair::BomManager::getObjectPtr<stdair::BookingClass> (*lSegmentCabin_ptr, lClassCode);
              if (lBookingClass_ptr != NULL) {
                iAirportCodeList.push_back(iDestination);
                lBookingClass_ptr->updateForecast(iAirlineCodeList, iClassCodeList, iAirportCodeList,
                                                  iNumberOfRequests, iStdDevValue, iYield);
                lFoundBookingClasses = true;
                
                std::ostringstream oACStr;
                for (stdair::AirlineCodeList_T::const_iterator itAC = iAirlineCodeList.begin();
                     itAC != iAirlineCodeList.end(); ++itAC) {
                  if (itAC == iAirlineCodeList.begin()) {
                    oACStr << *itAC;
                  }
                  else {
                    oACStr << "-" << *itAC;
                  }
                }
                std::ostringstream oCCStr;
                for (stdair::ClassCodeList_T::const_iterator itCC = iClassCodeList.begin();
                     itCC != iClassCodeList.end(); ++itCC) {
                  if (itCC == iClassCodeList.begin()) {
                    oCCStr << *itCC;
                  }
                  else {
                    oCCStr << "-" << *itCC;
                  }
                }
                  
                STDAIR_LOG_DEBUG (oACStr.str() << " Classes " << oCCStr.str()
                                  << " Mean " << iNumberOfRequests << " Std Dev " << iStdDevValue);
              }
            }
          }
        }
      }
      if (lFoundBookingClasses) {
        break;
      }
    }
    --beginItAC;
    --beginItCC;
    if (!lFoundBookingClasses) {iAirportCodeList.pop_back();}
    return lFoundBookingClasses;
  }

  // ///////////////////////////////////////////////////////////////////
  void RMOL_Service::
  optimiseBPWithDemandAggregation (const stdair::DateTime_T& iRMEventTime) {

    if (_rmolServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The Rmol service "
                                                    "has not been initialised");
    }
    assert (_rmolServiceContext != NULL);
    RMOL_ServiceContext& lRMOL_ServiceContext = *_rmolServiceContext;

    // Retrieve the bom root
    stdair::STDAIR_Service& lSTDAIR_Service =
      lRMOL_ServiceContext.getSTDAIR_Service();
    stdair::BomRoot& lBomRoot = lSTDAIR_Service.getBomRoot();

    // Retrieve the date from the RM event
    const stdair::Date_T lDate = iRMEventTime.date();

    const stdair::InventoryList_T& lInvList =
      stdair::BomManager::getList<stdair::Inventory> (lBomRoot);
    for (stdair::InventoryList_T::const_iterator itInv = lInvList.begin();
         itInv != lInvList.end(); ++itInv) {
      stdair::Inventory* lCurrentInv_ptr = *itInv;
      assert (lCurrentInv_ptr != NULL);

      const stdair::FlightDateList_T& lFlightDateList =
	stdair::BomManager::getList<stdair::FlightDate> (*lCurrentInv_ptr);
      for (stdair::FlightDateList_T::const_iterator itFlightDate =
	     lFlightDateList.begin();
	   itFlightDate != lFlightDateList.end(); ++itFlightDate) {
	stdair::FlightDate* lCurrentFlightDate_ptr = *itFlightDate;
	assert (lCurrentFlightDate_ptr != NULL);

        const stdair::Date_T& lCurrentDepartureDate = lCurrentFlightDate_ptr->getDepartureDate();
        stdair::DateOffset_T lDateOffset = lCurrentDepartureDate - lDate;
        stdair::DTD_T lDTD = short (lDateOffset.days());
      
        stdair::DCPList_T::const_iterator itDCP =
          std::find (stdair::DEFAULT_DCP_LIST.begin(), stdair::DEFAULT_DCP_LIST.end(), lDTD);
        if (itDCP != stdair::DEFAULT_DCP_LIST.end()) {
          STDAIR_LOG_DEBUG ("Optimisation with demand aggregation: " << lCurrentInv_ptr->getAirlineCode()
                            << " Departure " << lCurrentDepartureDate << " DTD " << lDTD);
          Optimiser::optimiseBPWithDemandAggregation (*lCurrentFlightDate_ptr);
        }
      }
    }
  }

  // ///////////////////////////////////////////////////////////////////
  void RMOL_Service::
  optimiseBPWithYieldProration (const stdair::DateTime_T& iRMEventTime) {

    if (_rmolServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The Rmol service "
                                                    "has not been initialised");
    }
    assert (_rmolServiceContext != NULL);
    RMOL_ServiceContext& lRMOL_ServiceContext = *_rmolServiceContext;

    // Retrieve the bom root
    stdair::STDAIR_Service& lSTDAIR_Service =
      lRMOL_ServiceContext.getSTDAIR_Service();
    stdair::BomRoot& lBomRoot = lSTDAIR_Service.getBomRoot();

    // Retrieve the date from the RM event
    const stdair::Date_T lDate = iRMEventTime.date();

    const stdair::InventoryList_T& lInvList =
      stdair::BomManager::getList<stdair::Inventory> (lBomRoot);
    for (stdair::InventoryList_T::const_iterator itInv = lInvList.begin();
         itInv != lInvList.end(); ++itInv) {
      stdair::Inventory* lCurrentInv_ptr = *itInv;
      assert (lCurrentInv_ptr != NULL);

      const stdair::FlightDateList_T& lFlightDateList =
	stdair::BomManager::getList<stdair::FlightDate> (*lCurrentInv_ptr);
      for (stdair::FlightDateList_T::const_iterator itFlightDate =
	     lFlightDateList.begin();
	   itFlightDate != lFlightDateList.end(); ++itFlightDate) {
	stdair::FlightDate* lCurrentFlightDate_ptr = *itFlightDate;
	assert (lCurrentFlightDate_ptr != NULL);

        const stdair::Date_T& lCurrentDepartureDate = lCurrentFlightDate_ptr->getDepartureDate();
        stdair::DateOffset_T lDateOffset = lCurrentDepartureDate - lDate;
        stdair::DTD_T lDTD = short (lDateOffset.days());
      
        stdair::DCPList_T::const_iterator itDCP =
          std::find (stdair::DEFAULT_DCP_LIST.begin(), stdair::DEFAULT_DCP_LIST.end(), lDTD);
        if (itDCP != stdair::DEFAULT_DCP_LIST.end()) {
          STDAIR_LOG_DEBUG ("Optimisation with demand aggregation: " << lCurrentInv_ptr->getAirlineCode()
                            << " Departure " << lCurrentDepartureDate << " DTD " << lDTD);
          Optimiser::optimiseBPWithYieldProration (*lCurrentFlightDate_ptr);
        }
      }
    }
  }

  // ///////////////////////////////////////////////////////////////////
  void RMOL_Service::
  setOnDForecast (const stdair::AirlineCode_T& iAirlineCode,
                  const stdair::Date_T& iPreferredDepartureDate,
                  const stdair::AirportCode_T& iOrigin,
                  const stdair::AirportCode_T& iDestination,
                  const stdair::CabinCode_T& iPreferredCabin,
                  const stdair::ClassCode_T& iClassCode,
                  const stdair::MeanValue_T& iMeanValue,
                  const stdair::StdDevValue_T& iStdDevValue,
                  const stdair::Yield_T& iYield,
                  stdair::BomRoot& iBomRoot) {
    stdair::Inventory* lInventory_ptr = iBomRoot.getInventory(iAirlineCode);
    if (lInventory_ptr == NULL) {
      STDAIR_LOG_ERROR ("Cannot find the inventory corresponding"
                        << " to the airline" << iAirlineCode) ;
      assert(false);
    }
    const stdair::OnDDateList_T lOnDDateList =
      stdair::BomManager::getList<stdair::OnDDate> (*lInventory_ptr);
    assert (!lOnDDateList.empty());
    bool lFoundOnDDate = false;
    for (stdair::OnDDateList_T::const_iterator itOD = lOnDDateList.begin();
         itOD != lOnDDateList.end(); ++itOD) {
      stdair::OnDDate* lOnDDate_ptr = *itOD;
      assert (lOnDDate_ptr != NULL);
      const stdair::Date_T& lDepartureDate = lOnDDate_ptr->getDate();
      const stdair::AirportCode_T& lOrigin = lOnDDate_ptr->getOrigin();
      const stdair::AirportCode_T& lDestination = lOnDDate_ptr->getDestination();
      if (!stdair::BomManager::hasList<stdair::SegmentDate> (*lOnDDate_ptr)) {
        STDAIR_LOG_ERROR ("The O&D date " << lOnDDate_ptr->describeKey()
                          << "has not been correctly initialized : SegmentDate list is missing");
        assert (false);
      }
      const stdair::SegmentDateList_T& lSegmentDateList =
        stdair::BomManager::getList<stdair::SegmentDate> (*lOnDDate_ptr);
      if (lDepartureDate == iPreferredDepartureDate && lOrigin == iOrigin &&
          lDestination == iDestination && lSegmentDateList.size() == 1) {       
        stdair::CabinClassPair_T lCabinClassPair (iPreferredCabin, iClassCode);
        stdair::CabinClassPairList_T lCabinClassPairList;
        lCabinClassPairList.push_back(lCabinClassPair);
        stdair::DemandStruct lDemandStruct (iYield, iMeanValue, iStdDevValue);
        lOnDDate_ptr->setDemandInformation(lCabinClassPairList, lDemandStruct);
        lFoundOnDDate = true;
        STDAIR_LOG_DEBUG (iAirlineCode << " Class " << iClassCode << " Mean " << iMeanValue
                          << " Std Dev " << iStdDevValue);
        break;
      }
    }

    if (!lFoundOnDDate) {
      STDAIR_LOG_ERROR ("Cannot find class " << iClassCode << " in cabin " << iPreferredCabin
                        << " for the segment " << iOrigin << "-" << iDestination << " with"
                        << " the airline " << iAirlineCode);
      assert(false);
    }
  }
  
  // ///////////////////////////////////////////////////////////////////       
  void RMOL_Service::
  setOnDForecast (const stdair::AirlineCodeList_T& iAirlineCodeList,
                  const stdair::AirlineCode_T& iAirlineCode,
                  const stdair::Date_T& iPreferredDepartureDate,
                  const stdair::AirportCode_T& iOrigin,
                  const stdair::AirportCode_T& iDestination,
                  const stdair::CabinCode_T& iPreferredCabin,
                  const stdair::ClassCodeList_T& iClassCodeList,
                  const stdair::MeanValue_T& iMeanValue,
                  const stdair::StdDevValue_T& iStdDevValue,
                  const stdair::Yield_T& iYield,
                  stdair::BomRoot& iBomRoot) {    
    stdair::Inventory* lInventory_ptr = iBomRoot.getInventory(iAirlineCode);
    if (lInventory_ptr == NULL) {
      STDAIR_LOG_ERROR ("Cannot find the inventory corresponding"
                        << " to the airline" << iAirlineCode) ;
      assert(false);
    }
    const stdair::OnDDateList_T lOnDDateList =
      stdair::BomManager::getList<stdair::OnDDate> (*lInventory_ptr);
    assert (!lOnDDateList.empty());
    bool lFoundOnDDate = false;
    for (stdair::OnDDateList_T::const_iterator itOD = lOnDDateList.begin();
         itOD != lOnDDateList.end(); ++itOD) {
      stdair::OnDDate* lOnDDate_ptr = *itOD;
      assert (lOnDDate_ptr != NULL);
      const stdair::Date_T& lDepartureDate = lOnDDate_ptr->getDate();
      const stdair::AirportCode_T& lOrigin = lOnDDate_ptr->getOrigin();
      const stdair::AirportCode_T& lDestination = lOnDDate_ptr->getDestination();
      if (!stdair::BomManager::hasList<stdair::SegmentDate> (*lOnDDate_ptr)) {
        STDAIR_LOG_ERROR ("The O&D date " << lOnDDate_ptr->describeKey()
                          << "has not been correctly initialized : SegmentDate list is missing");
        assert (false);
      }
      const stdair::SegmentDateList_T& lSegmentDateList =
        stdair::BomManager::getList<stdair::SegmentDate> (*lOnDDate_ptr);
      if (lDepartureDate == iPreferredDepartureDate && lOrigin == iOrigin &&
          lDestination == iDestination && lSegmentDateList.size() == iAirlineCodeList.size()) {
        const stdair::SegmentDateList_T& lSegmentDateList =
          stdair::BomManager::getList<stdair::SegmentDate> (*lOnDDate_ptr);        
        stdair::AirlineCodeList_T::const_iterator itAC = iAirlineCodeList.begin();
        stdair::SegmentDateList_T::const_iterator itSD = lSegmentDateList.begin();
        for (;itAC != iAirlineCodeList.end(); ++itAC, ++itSD) {
          const stdair::AirlineCode_T lForecastAirlineCode = *itAC;
          if ((*itSD)->isOtherAirlineOperating()) {
            const stdair::SegmentDate* lOperatingSegmentDate_ptr =
              (*itSD)->getOperatingSegmentDate();
            stdair::FlightDate* lFlightDate_ptr =
              stdair::BomManager::getParentPtr<stdair::FlightDate>(*lOperatingSegmentDate_ptr);
            const stdair::AirlineCode_T lOperatingAirlineCode = lFlightDate_ptr->getAirlineCode();
            if (lOperatingAirlineCode != lForecastAirlineCode) {break;}
          } else {
            const stdair::AirlineCode_T lOperatingAirlineCode = lOnDDate_ptr->getAirlineCode();
            if (lOperatingAirlineCode != lForecastAirlineCode) {break;}
          }          
        }
        if (itAC == iAirlineCodeList.end()) {lFoundOnDDate = true;}
      }
      if (lFoundOnDDate) {        
        stdair::CabinClassPairList_T lCabinClassPairList;
        for (stdair::ClassCodeList_T::const_iterator itCC = iClassCodeList.begin();
             itCC != iClassCodeList.end(); ++itCC) {
          const stdair::ClassCode_T lClassCode = *itCC;
          stdair::CabinClassPair_T lCabinClassPair (iPreferredCabin, lClassCode);
          lCabinClassPairList.push_back(lCabinClassPair);
        }
        stdair::DemandStruct lDemandStruct (iYield, iMeanValue, iStdDevValue);
        lOnDDate_ptr->setDemandInformation(lCabinClassPairList, lDemandStruct);
        lFoundOnDDate = true;
        std::ostringstream oACStr;
        for (stdair::AirlineCodeList_T::const_iterator itAC = iAirlineCodeList.begin();
             itAC != iAirlineCodeList.end(); ++itAC) {
          if (itAC == iAirlineCodeList.begin()) {
            oACStr << *itAC;
          }
          else {
            oACStr << "-" << *itAC;
          }
        }
        std::ostringstream oCCStr;
        for (stdair::ClassCodeList_T::const_iterator itCC = iClassCodeList.begin();
             itCC != iClassCodeList.end(); ++itCC) {
          if (itCC == iClassCodeList.begin()) {
            oCCStr << *itCC;
          }
          else {
            oCCStr << "-" << *itCC;
          }
        }
        
        STDAIR_LOG_DEBUG (oACStr.str() << " Classes " << oCCStr.str()
                          << " Mean " << iMeanValue << " Std Dev " << iStdDevValue);        
        break;
      }
    }
    if (!lFoundOnDDate) {
      STDAIR_LOG_ERROR ("Cannot find the required multi-segment O&D date:  "
                        << iOrigin << "-" << iDestination << " " << iPreferredDepartureDate);
      assert(false);
    }
  }
  
  // ///////////////////////////////////////////////////////////////////
  void RMOL_Service::resetDemandInformation (const stdair::DateTime_T& iRMEventTime) {
    if (_rmolServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The Rmol service "
                                                    "has not been initialised");
    }
    assert (_rmolServiceContext != NULL);
    RMOL_ServiceContext& lRMOL_ServiceContext = *_rmolServiceContext;
    
    // Retrieve the bom root
    stdair::STDAIR_Service& lSTDAIR_Service =
      lRMOL_ServiceContext.getSTDAIR_Service();
    stdair::BomRoot& lBomRoot = lSTDAIR_Service.getBomRoot();

    // Retrieve the date from the RM event
    const stdair::Date_T lDate = iRMEventTime.date();

    const stdair::InventoryList_T lInventoryList =
      stdair::BomManager::getList<stdair::Inventory> (lBomRoot);
    assert (!lInventoryList.empty());
    for (stdair::InventoryList_T::const_iterator itInv = lInventoryList.begin();
         itInv != lInventoryList.end(); ++itInv) {
      const stdair::Inventory* lInventory_ptr = *itInv;
      const stdair::FlightDateList_T lFlightDateList =
        stdair::BomManager::getList<stdair::FlightDate> (*lInventory_ptr);
      assert (!lFlightDateList.empty());
      for (stdair::FlightDateList_T::const_iterator itFD = lFlightDateList.begin();
           itFD != lFlightDateList.end(); ++itFD) {
        const stdair::FlightDate* lFlightDate_ptr = *itFD;
        assert (lFlightDate_ptr != NULL);
        
        const stdair::Date_T& lDepartureDate = lFlightDate_ptr->getDepartureDate();
        stdair::DateOffset_T lDateOffset = lDepartureDate - lDate;
        stdair::DTD_T lDTD = short (lDateOffset.days());
      
        stdair::DCPList_T::const_iterator itDCP =
          std::find (stdair::DEFAULT_DCP_LIST.begin(), stdair::DEFAULT_DCP_LIST.end(), lDTD);
        if (itDCP != stdair::DEFAULT_DCP_LIST.end()) {
          
          const stdair::LegDateList_T lLegDateList =
            stdair::BomManager::getList<stdair::LegDate> (*lFlightDate_ptr);
          assert (!lLegDateList.empty());
          for (stdair::LegDateList_T::const_iterator itLD = lLegDateList.begin();
               itLD != lLegDateList.end(); ++itLD) {
            const stdair::LegDate* lLegDate_ptr = *itLD;
            assert (lLegDate_ptr != NULL);
            const stdair::LegCabinList_T lLegCabinList =
              stdair::BomManager::getList<stdair::LegCabin> (*lLegDate_ptr);
            assert (!lLegCabinList.empty());
            for (stdair::LegCabinList_T::const_iterator itLC = lLegCabinList.begin();
                 itLC != lLegCabinList.end(); ++itLC) {
              stdair::LegCabin* lLegCabin_ptr = *itLC;
              assert (lLegCabin_ptr != NULL);
              lLegCabin_ptr->resetYieldDemandMap();
            }
          }
        }
      }
    }
  }

  // ///////////////////////////////////////////////////////////////////
  void RMOL_Service::projectOnDDemandOntoLegCabins(const stdair::DateTime_T& iRMEventTime) {

    if (_rmolServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The Rmol service "
                                                    "has not been initialised");
    }
    assert (_rmolServiceContext != NULL);
    RMOL_ServiceContext& lRMOL_ServiceContext = *_rmolServiceContext;
    
    // Retrieve the bom root
    stdair::STDAIR_Service& lSTDAIR_Service =
      lRMOL_ServiceContext.getSTDAIR_Service();
    stdair::BomRoot& lBomRoot = lSTDAIR_Service.getBomRoot();

    // Retrieve the date from the RM event
    const stdair::Date_T lDate = iRMEventTime.date();

    const stdair::InventoryList_T lInventoryList =
      stdair::BomManager::getList<stdair::Inventory> (lBomRoot);
    assert (!lInventoryList.empty());
    for (stdair::InventoryList_T::const_iterator itInv = lInventoryList.begin();
         itInv != lInventoryList.end(); ++itInv) {
      const stdair::Inventory* lInventory_ptr = *itInv;
      assert (lInventory_ptr != NULL);
      const stdair::OnDDateList_T lOnDDateList =
        stdair::BomManager::getList<stdair::OnDDate> (*lInventory_ptr);
      assert (!lOnDDateList.empty());
      for (stdair::OnDDateList_T::const_iterator itOD = lOnDDateList.begin();
           itOD != lOnDDateList.end(); ++itOD) {
        stdair::OnDDate* lOnDDate_ptr = *itOD;
        assert (lOnDDate_ptr != NULL);

        const stdair::Date_T& lDepartureDate = lOnDDate_ptr->getDate();
        stdair::DateOffset_T lDateOffset = lDepartureDate - lDate;
        stdair::DTD_T lDTD = short (lDateOffset.days());
      
        stdair::DCPList_T::const_iterator itDCP =
          std::find (stdair::DEFAULT_DCP_LIST.begin(), stdair::DEFAULT_DCP_LIST.end(), lDTD);
        if (itDCP != stdair::DEFAULT_DCP_LIST.end()) {
        
          const stdair::StringDemandStructMap_T& lStringDemandStructMap =
            lOnDDate_ptr->getDemandInfoMap ();
          for (stdair::StringDemandStructMap_T::const_iterator itStrDS = lStringDemandStructMap.begin();
               itStrDS != lStringDemandStructMap.end(); ++itStrDS) {
            std::string lCabinClassPath = itStrDS->first;
            const stdair::DemandStruct& lDemandStruct = itStrDS->second;
            const stdair::CabinClassPairList_T& lCabinClassPairList =
              lOnDDate_ptr->getCabinClassPairList(lCabinClassPath);
            const unsigned int lNbOfSegments = lOnDDate_ptr->getNbOfSegments();
            // Sanity check
            assert (lCabinClassPairList.size() == lNbOfSegments);
            
            const stdair::SegmentDateList_T lOnDSegmentDateList =
              stdair::BomManager::getList<stdair::SegmentDate> (*lOnDDate_ptr);
            // Sanity check
            assert (lOnDSegmentDateList.size() == lNbOfSegments);
            stdair::CabinClassPairList_T::const_iterator itCCP = lCabinClassPairList.begin();
            stdair::SegmentDateList_T::const_iterator itSD = lOnDSegmentDateList.begin();
            for (; itSD != lOnDSegmentDateList.end(); ++itCCP, ++itSD) {
              const stdair::SegmentDate* lSegmentDate_ptr = *itSD;
              assert (lSegmentDate_ptr != NULL);
              if (!lSegmentDate_ptr->isOtherAirlineOperating()) {
                const stdair::CabinCode_T lCabinCode = itCCP->first;
                const stdair::ClassCode_T lClassCode = itCCP->second;
                const stdair::SegmentCabin* lSegmentCabin_ptr =
                  stdair::BomManager::getObjectPtr<stdair::SegmentCabin> (*lSegmentDate_ptr,
                                                                          lCabinCode);
                assert (lSegmentCabin_ptr != NULL);
                const stdair::LegCabinList_T lLegCabinList =
                  stdair::BomManager::getList<stdair::LegCabin> (*lSegmentCabin_ptr);
                assert (!lLegCabinList.empty());
                const int lNbOfLegs = lLegCabinList.size();
                const stdair::Yield_T& lYield = lDemandStruct.getYield()/(lNbOfLegs*lNbOfSegments);
                const stdair::MeanValue_T& lMeanValue = lDemandStruct.getDemandMean();
                const stdair::StdDevValue_T& lStdDevValue = lDemandStruct.getDemandStdDev();
                for (stdair::LegCabinList_T::const_iterator itLC = lLegCabinList.begin();
                     itLC != lLegCabinList.end(); ++itLC) {
                  stdair::LegCabin* lLegCabin_ptr = *itLC;
                  assert (lLegCabin_ptr != NULL);
                  lLegCabin_ptr->addDemandInformation (lYield, lMeanValue, lStdDevValue);
                }
              }
            }
          }
        }
      }
    }
  }

  // ///////////////////////////////////////////////////////////////////
  void RMOL_Service::
  optimiseUsingOnDForecast (const stdair::DateTime_T& iRMEventTime) {

    if (_rmolServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The Rmol service "
                                                    "has not been initialised");
    }
    assert (_rmolServiceContext != NULL);
    RMOL_ServiceContext& lRMOL_ServiceContext = *_rmolServiceContext;

    // Retrieve the bom root
    stdair::STDAIR_Service& lSTDAIR_Service =
      lRMOL_ServiceContext.getSTDAIR_Service();
    stdair::BomRoot& lBomRoot = lSTDAIR_Service.getBomRoot();

    // Retrieve the date from the RM event
    const stdair::Date_T lDate = iRMEventTime.date();

    const stdair::InventoryList_T& lInvList =
      stdair::BomManager::getList<stdair::Inventory> (lBomRoot);
    for (stdair::InventoryList_T::const_iterator itInv = lInvList.begin();
         itInv != lInvList.end(); ++itInv) {
      stdair::Inventory* lCurrentInv_ptr = *itInv;
      assert (lCurrentInv_ptr != NULL);

      const stdair::FlightDateList_T& lFlightDateList =
	stdair::BomManager::getList<stdair::FlightDate> (*lCurrentInv_ptr);
      for (stdair::FlightDateList_T::const_iterator itFlightDate =
	     lFlightDateList.begin();
	   itFlightDate != lFlightDateList.end(); ++itFlightDate) {
	stdair::FlightDate* lCurrentFlightDate_ptr = *itFlightDate;
	assert (lCurrentFlightDate_ptr != NULL);

        const stdair::Date_T& lCurrentDepartureDate = lCurrentFlightDate_ptr->getDepartureDate();
        stdair::DateOffset_T lDateOffset = lCurrentDepartureDate - lDate;
        stdair::DTD_T lDTD = short (lDateOffset.days());
      
        stdair::DCPList_T::const_iterator itDCP =
          std::find (stdair::DEFAULT_DCP_LIST.begin(), stdair::DEFAULT_DCP_LIST.end(), lDTD);
        if (itDCP != stdair::DEFAULT_DCP_LIST.end()) {
          STDAIR_LOG_DEBUG ("Optimisation using O&D forecast: " << lCurrentInv_ptr->getAirlineCode()
                            << " Departure " << lCurrentDepartureDate << " DTD " << lDTD);
          Optimiser::optimiseUsingOnDForecast (*lCurrentFlightDate_ptr);
        }
      }
    }    
  }

  // ///////////////////////////////////////////////////////////////////
  void RMOL_Service::communicateBidPrice (const stdair::DateTime_T& iRMEventTime) {

        if (_rmolServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The Rmol service "
                                                    "has not been initialised");
    }
    assert (_rmolServiceContext != NULL);
    RMOL_ServiceContext& lRMOL_ServiceContext = *_rmolServiceContext;

    // Retrieve the bom root
    stdair::STDAIR_Service& lSTDAIR_Service =
      lRMOL_ServiceContext.getSTDAIR_Service();
    stdair::BomRoot& lBomRoot = lSTDAIR_Service.getBomRoot();

    // Retrieve the date from the RM event
    const stdair::Date_T lDate = iRMEventTime.date();

    const stdair::InventoryList_T& lInvList =
      stdair::BomManager::getList<stdair::Inventory> (lBomRoot);

    for (stdair::InventoryList_T::const_iterator itInv = lInvList.begin();
         itInv != lInvList.end(); ++itInv) {
      stdair::Inventory* lCurrentInv_ptr = *itInv;
      assert (lCurrentInv_ptr != NULL);
      
      const stdair::FlightDateList_T& lFlightDateList =
	stdair::BomManager::getList<stdair::FlightDate> (*lCurrentInv_ptr);
      for (stdair::FlightDateList_T::const_iterator itFlightDate =
	     lFlightDateList.begin();
	   itFlightDate != lFlightDateList.end(); ++itFlightDate) {
	stdair::FlightDate* lCurrentFlightDate_ptr = *itFlightDate;
	assert (lCurrentFlightDate_ptr != NULL);

        const stdair::Date_T& lCurrentDepartureDate = lCurrentFlightDate_ptr->getDepartureDate();
        stdair::DateOffset_T lDateOffset = lCurrentDepartureDate - lDate;
        stdair::DTD_T lDTD = short (lDateOffset.days());
      
        stdair::DCPList_T::const_iterator itDCP =
          std::find (stdair::DEFAULT_DCP_LIST.begin(), stdair::DEFAULT_DCP_LIST.end(), lDTD);
        if (itDCP != stdair::DEFAULT_DCP_LIST.end()) {
          communicateBidPrice (*lCurrentFlightDate_ptr, lBomRoot);
        }
      }
    }
  }

  // ///////////////////////////////////////////////////////////////////
  void RMOL_Service::communicateBidPrice (const stdair::FlightDate& iFlightDate,
                                          stdair::BomRoot& iBomRoot) {
    const stdair::SegmentDateList_T& lSegmentDateList =
      stdair::BomManager::getList<stdair::SegmentDate> (iFlightDate);
    const stdair::AirlineCode_T& lOptAC = iFlightDate.getAirlineCode();
    const std::string lFDKeyStr = iFlightDate.describeKey();
    
    for (stdair::SegmentDateList_T::const_iterator itSegmentDate = lSegmentDateList.begin();
         itSegmentDate != lSegmentDateList.end(); ++itSegmentDate) {
      stdair::SegmentDate* lSegmentDate_ptr = *itSegmentDate;
      assert (lSegmentDate_ptr != NULL);
      if (stdair::BomManager::hasList<stdair::SegmentDate>(*lSegmentDate_ptr)) {
        const stdair::LegDateList_T& lLegDateList =
          stdair::BomManager::getList<stdair::LegDate>(*lSegmentDate_ptr);
        const stdair::SegmentDateList_T& lMktSegmentDateList =
          stdair::BomManager::getList<stdair::SegmentDate>(*lSegmentDate_ptr);
        for (stdair::SegmentDateList_T::const_iterator itMktSD = lMktSegmentDateList.begin();
             itMktSD != lMktSegmentDateList.end(); ++itMktSD) {
          stdair::SegmentDate* lMktSD_ptr = *itMktSD;
          assert (lMktSD_ptr != NULL);
          stdair::FlightDate* lMktFD_ptr =
            stdair::BomManager::getParentPtr<stdair::FlightDate>(*lMktSD_ptr);
          assert (lMktFD_ptr != NULL);
          const stdair::AirlineCode_T& lMktAC = lMktFD_ptr->getAirlineCode();
          const stdair::Inventory* lMktInv_ptr =
            stdair::BomManager::getObjectPtr<stdair::Inventory>(iBomRoot,lMktAC);
          assert (lMktInv_ptr != NULL);
          const stdair::Inventory* lOptInv_ptr =
            stdair::BomManager::getObjectPtr<stdair::Inventory>(*lMktInv_ptr,lOptAC);
          assert (lOptInv_ptr != NULL);
          const stdair::FlightDate* lOptFD_ptr =
            stdair::BomManager::getObjectPtr<stdair::FlightDate>(*lOptInv_ptr,lFDKeyStr);
          assert (lOptFD_ptr != NULL);
          for (stdair::LegDateList_T::const_iterator itLD = lLegDateList.begin();
               itLD != lLegDateList.end(); ++itLD) {
            const stdair::LegDate* lLD_ptr = *itLD;
            assert (lLD_ptr != NULL);
            const std::string lLDKeyStr = lLD_ptr->describeKey();
            stdair::LegDate* lOptLD_ptr =
              stdair::BomManager::getObjectPtr<stdair::LegDate>(*lOptFD_ptr,lLDKeyStr);
            assert (lOptLD_ptr != NULL);
            const stdair::LegCabinList_T& lLegCabinList_T =
              stdair::BomManager::getList<stdair::LegCabin>(*lLD_ptr);
            for (stdair::LegCabinList_T::const_iterator itLC = lLegCabinList_T.begin();
                 itLC != lLegCabinList_T.end(); ++itLC) {
              const stdair::LegCabin* lLC_ptr = *itLC;
              assert (lLC_ptr != NULL);
              const std::string lLCKeyStr = lLC_ptr->describeKey();
              stdair::LegCabin* lOptLC_ptr =
                stdair::BomManager::getObjectPtr<stdair::LegCabin>(*lOptLD_ptr, lLCKeyStr);
              assert (lOptLC_ptr != NULL);
              lOptLC_ptr->updatePreviousBidPrice();
              lOptLC_ptr->setCurrentBidPrice (lLC_ptr->getCurrentBidPrice());
            }
          }
        }
      }
    }
  }

  // ///////////////////////////////////////////////////////////////////
  void RMOL_Service::projectOnDDemandOntoLegCabinsUsingDA(const stdair::DateTime_T& iRMEventTime) {

    if (_rmolServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The Rmol service "
                                                    "has not been initialised");
    }
    assert (_rmolServiceContext != NULL);
    RMOL_ServiceContext& lRMOL_ServiceContext = *_rmolServiceContext;
    
    // Retrieve the bom root
    stdair::STDAIR_Service& lSTDAIR_Service =
      lRMOL_ServiceContext.getSTDAIR_Service();
    stdair::BomRoot& lBomRoot = lSTDAIR_Service.getBomRoot();

    // Retrieve the date from the RM event
    const stdair::Date_T lDate = iRMEventTime.date();

    const stdair::InventoryList_T lInventoryList =
      stdair::BomManager::getList<stdair::Inventory> (lBomRoot);
    assert (!lInventoryList.empty());
    for (stdair::InventoryList_T::const_iterator itInv = lInventoryList.begin();
         itInv != lInventoryList.end(); ++itInv) {
      const stdair::Inventory* lInventory_ptr = *itInv;
      assert (lInventory_ptr != NULL);
      const stdair::OnDDateList_T lOnDDateList =
        stdair::BomManager::getList<stdair::OnDDate> (*lInventory_ptr);
      assert (!lOnDDateList.empty());
      for (stdair::OnDDateList_T::const_iterator itOD = lOnDDateList.begin();
           itOD != lOnDDateList.end(); ++itOD) {
        stdair::OnDDate* lOnDDate_ptr = *itOD;
        assert (lOnDDate_ptr != NULL);

        const stdair::Date_T& lDepartureDate = lOnDDate_ptr->getDate();
        stdair::DateOffset_T lDateOffset = lDepartureDate - lDate;
        stdair::DTD_T lDTD = short (lDateOffset.days());
      
        stdair::DCPList_T::const_iterator itDCP =
          std::find (stdair::DEFAULT_DCP_LIST.begin(), stdair::DEFAULT_DCP_LIST.end(), lDTD);
        if (itDCP != stdair::DEFAULT_DCP_LIST.end()) {
        
          const stdair::StringDemandStructMap_T& lStringDemandStructMap =
            lOnDDate_ptr->getDemandInfoMap ();
          for (stdair::StringDemandStructMap_T::const_iterator itStrDS = lStringDemandStructMap.begin();
               itStrDS != lStringDemandStructMap.end(); ++itStrDS) {
            std::string lCabinClassPath = itStrDS->first;
            const stdair::DemandStruct& lDemandStruct = itStrDS->second;
            const stdair::CabinClassPairList_T& lCabinClassPairList =
              lOnDDate_ptr->getCabinClassPairList(lCabinClassPath);
            const unsigned int lNbOfSegments = lOnDDate_ptr->getNbOfSegments();
            // Sanity check
            assert (lCabinClassPairList.size() == lNbOfSegments);

            //
            const stdair::SegmentDateList_T lOnDSegmentDateList =
              stdair::BomManager::getList<stdair::SegmentDate> (*lOnDDate_ptr);
            // Sanity check
            assert (lOnDSegmentDateList.size() == lNbOfSegments);
            stdair::CabinClassPairList_T::const_iterator itCCP = lCabinClassPairList.begin();
            stdair::SegmentDateList_T::const_iterator itSD = lOnDSegmentDateList.begin();
            std::list<stdair::BidPrice_T> lBidPriceList;
            stdair::BidPrice_T lTotalBidPrice = 0;
            for (; itSD != lOnDSegmentDateList.end(); ++itCCP, ++itSD) {
              stdair::SegmentDate* lSegmentDate_ptr = *itSD;
              assert (lSegmentDate_ptr != NULL);
              if (!lSegmentDate_ptr->isOtherAirlineOperating()) {
                lSegmentDate_ptr = lSegmentDate_ptr->getOperatingSegmentDate ();
              }
              const stdair::CabinCode_T lCabinCode = itCCP->first;
              const stdair::SegmentCabin* lSegmentCabin_ptr =
                stdair::BomManager::getObjectPtr<stdair::SegmentCabin> (*lSegmentDate_ptr,
                                                                        lCabinCode);
              assert (lSegmentCabin_ptr != NULL);
              stdair::BidPrice_T lBidPrice = 0;
              const stdair::LegCabinList_T lLegCabinList =
                stdair::BomManager::getList<stdair::LegCabin>(*lSegmentCabin_ptr);
              for (stdair::LegCabinList_T::const_iterator itLC = lLegCabinList.begin();
                   itLC != lLegCabinList.end(); ++itLC) {
                const stdair::LegCabin* lLegCabin_ptr = *itLC;
                assert (lLegCabin_ptr != NULL);
                lBidPrice += lLegCabin_ptr->getCurrentBidPrice();
              }
              lBidPriceList.push_back (lBidPrice);
              lTotalBidPrice += lBidPrice;
            }            
            
            
            itCCP = lCabinClassPairList.begin();
            itSD = lOnDSegmentDateList.begin();
            std::list<stdair::BidPrice_T>::const_iterator itBP = lBidPriceList.begin();
            for (; itSD != lOnDSegmentDateList.end(); ++itCCP, ++itSD, ++itBP) {
              stdair::BidPrice_T lBidPrice = *itBP;
              stdair::BidPrice_T lComplementaryBidPrice = lTotalBidPrice - lBidPrice;
              const stdair::SegmentDate* lSegmentDate_ptr = *itSD;
              assert (lSegmentDate_ptr != NULL);
              if (!lSegmentDate_ptr->isOtherAirlineOperating()) {
                const stdair::CabinCode_T lCabinCode = itCCP->first;
                const stdair::ClassCode_T lClassCode = itCCP->second;
                const stdair::SegmentCabin* lSegmentCabin_ptr =
                  stdair::BomManager::getObjectPtr<stdair::SegmentCabin> (*lSegmentDate_ptr,
                                                                          lCabinCode);
                assert (lSegmentCabin_ptr != NULL);
                const stdair::LegCabinList_T lLegCabinList =
                  stdair::BomManager::getList<stdair::LegCabin> (*lSegmentCabin_ptr);
                assert (!lLegCabinList.empty());
                const stdair::Yield_T& lYield = lDemandStruct.getYield() - lComplementaryBidPrice;
                const stdair::MeanValue_T& lMeanValue = lDemandStruct.getDemandMean();
                const stdair::StdDevValue_T& lStdDevValue = lDemandStruct.getDemandStdDev();
                for (stdair::LegCabinList_T::const_iterator itLC = lLegCabinList.begin();
                     itLC != lLegCabinList.end(); ++itLC) {
                  stdair::LegCabin* lLegCabin_ptr = *itLC;
                  assert (lLegCabin_ptr != NULL);
                  lLegCabin_ptr->addDemandInformation (lYield, lMeanValue, lStdDevValue);
                }
              }
            }
          }
        }
      }
    }
  }
  
}
