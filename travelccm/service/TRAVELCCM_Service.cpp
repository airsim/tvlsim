// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// StdAir
#include <stdair/basic/BasChronometer.hpp>
#include <stdair/bom/BomManager.hpp> // for display()
#include <stdair/service/Logger.hpp>
// TravelCCM
#include <travelccm/basic/BasConst_TRAVELCCM_Service.hpp>
#include <travelccm/bom/TravelSolutionHolder.hpp>
#include <travelccm/bom/RestrictionHolder.hpp>
#include <travelccm/factory/FacTRAVELCCMServiceContext.hpp>
#include <travelccm/command/Simulator.hpp>
#include <travelccm/service/TRAVELCCM_ServiceContext.hpp>
#include <travelccm/TRAVELCCM_Service.hpp>

namespace TRAVELCCM {

  // //////////////////////////////////////////////////////////////////////
  TRAVELCCM_Service::TRAVELCCM_Service () 
    : _travelccmServiceContext (NULL) {
    assert (false);
  }

  // //////////////////////////////////////////////////////////////////////
  TRAVELCCM_Service::TRAVELCCM_Service (const TRAVELCCM_Service& iService)
    : _travelccmServiceContext (NULL) {
    assert (false);
  }

  // //////////////////////////////////////////////////////////////////////
  TRAVELCCM_Service::TRAVELCCM_Service (const TravelCCMType& iCCMType) 
    : _travelccmServiceContext (NULL) {

    // Initialise the context
    init (iCCMType);
  }

  // //////////////////////////////////////////////////////////////////////
  TRAVELCCM_Service::TRAVELCCM_Service (const stdair::BasLogParams& iLogParams,
                                        const TravelCCMType& iCCMType) 
    : _travelccmServiceContext (NULL) {
    
    // Set the log file
    logInit (iLogParams);

    // Initialise the (remaining of the) context
    init (iCCMType);
  }

  // //////////////////////////////////////////////////////////////////////
  TRAVELCCM_Service::~TRAVELCCM_Service () {
  }

  // //////////////////////////////////////////////////////////////////////
  void TRAVELCCM_Service::logInit (const stdair::BasLogParams& iLogParams) {
    stdair::Logger::init (iLogParams);
  }

  // //////////////////////////////////////////////////////////////////////
  void TRAVELCCM_Service::init (const TravelCCMType& iCCMType) {
    // Initialise the context
    TRAVELCCM_ServiceContext& lTRAVELCCM_ServiceContext = 
      FacTRAVELCCMServiceContext::instance().create (iCCMType);
    _travelccmServiceContext = &lTRAVELCCM_ServiceContext;
  }
  
  // //////////////////////////////////////////////////////////////////////
  void TRAVELCCM_Service::createPassenger(const stdair::PassengerType& iPaxType){
    assert (_travelccmServiceContext != NULL);
    _travelccmServiceContext->createPassenger (iPaxType);
  }

  // //////////////////////////////////////////////////////////////////////
  void TRAVELCCM_Service::initializePassenger() {
    assert (_travelccmServiceContext != NULL);
    _travelccmServiceContext->intializePassenger();
  }

  // //////////////////////////////////////////////////////////////////////
  void TRAVELCCM_Service::
  addTravelSolution  (const stdair::AirportCode_T& iDepartureAirport,
                      const stdair::AirportCode_T& iArrivalAirport,
                      const stdair::Date_T& iDepartureDate,
                      const stdair::Duration_T& iDepartureTime,
                      const stdair::Duration_T& iArrivalTime,
                      const stdair::Duration_T& iDuration,
                      const bool iRefundability,
                      const stdair::AirlineCode_T& iAirlineCode,
                      const stdair::CabinCode_T& iCabinCode,
                      const int iFlightNumber, double iFare,
                      int iStopsNumber,  bool iSNS, bool iChangeability,
                      const std::string& id) {
    assert (_travelccmServiceContext != NULL);

    _travelccmServiceContext->addTravelSolution (iDepartureAirport,
                                                 iArrivalAirport, iDepartureDate,
                                                 iDepartureTime, iArrivalTime,
                                                 iDuration, iRefundability,
                                                 iAirlineCode, iCabinCode,
                                                 iFlightNumber,
                                                 iFare, iStopsNumber, iSNS,
                                                 iChangeability, id);
  }
  
  // //////////////////////////////////////////////////////////////////////
  void TRAVELCCM_Service::addRestriction (const std::string& iRestrictionType) {
    assert (_travelccmServiceContext != NULL);
    _travelccmServiceContext->addRestriction (iRestrictionType);
  }

  // //////////////////////////////////////////////////////////////////////
  void TRAVELCCM_Service::addRestriction (const std::string& iRestrictionType,
                                          const std::string& iNamePreference) {
    assert (_travelccmServiceContext != NULL);
    _travelccmServiceContext->addRestriction (iRestrictionType, iNamePreference);
  }

  // //////////////////////////////////////////////////////////////////////
  void TRAVELCCM_Service::
  addRequest (bool refundability, bool changeability, bool saturdayNightStay,
              const stdair::AirlineCode_T& preferredAirline,
              const stdair::CabinCode_T& preferredCabin,
              const stdair::DateTime_T& departureTime) {
    assert (_travelccmServiceContext != NULL);
    _travelccmServiceContext->addAndLinkRequest (refundability, changeability,
                                                 saturdayNightStay,
                                                 preferredAirline,
                                                 preferredCabin, departureTime);
  }

  // //////////////////////////////////////////////////////////////////////
  TravelSolutionHolder& TRAVELCCM_Service::getChoosenTravelSolutions() {
    assert (_travelccmServiceContext != NULL);

    // Retrieve the travel solution holder in the service context.
    TravelSolutionHolder& travelSolutionHolder =
      _travelccmServiceContext->getTravelSolutionHolder();

    // Retrieve the passenger
    Passenger& passenger = _travelccmServiceContext->getPassenger();
    
    // Initialise the different pointers at the beginning of the different lists
    passenger.begin();
    travelSolutionHolder.begin();

    // launch the algorithm of preferred choices
    Simulator::simulate (passenger, travelSolutionHolder);
    
    return travelSolutionHolder;
  }

  // //////////////////////////////////////////////////////////////////////
  const TravelSolution* TRAVELCCM_Service::
  getBestTravelSolution (TravelSolutionHolder& ioTravelSolutionHolder) {

    if (ioTravelSolutionHolder.isVoid()) {
      return NULL;
      
    } else {
      ioTravelSolutionHolder.begin();
      const TravelSolution* oBestTravelSolution_ptr =
        &ioTravelSolutionHolder.getCurrentTravelSolution();
      
      while (ioTravelSolutionHolder.hasNotReachedEnd()) {
        const TravelSolution& lCurrentTravelSolution =
          ioTravelSolutionHolder.getCurrentTravelSolution();

        const bool isCheaper =
          lCurrentTravelSolution.isCheaper (*oBestTravelSolution_ptr);
        if (isCheaper == true) {
          
          oBestTravelSolution_ptr = &lCurrentTravelSolution ;
        }
        
        const bool hasTheSamePrice =
          lCurrentTravelSolution.hasTheSamePrice (*oBestTravelSolution_ptr);
        if (hasTheSamePrice == true) {
          
          const int randomIndicator = rand () % 2;
          // we change only when we cast a 0, if more than two travel solutions
          // have the same price, they do not have the same probability!!
          if (randomIndicator == 0)
            oBestTravelSolution_ptr = &lCurrentTravelSolution ;
        }
        
        ioTravelSolutionHolder.iterate();
      }
      
      return oBestTravelSolution_ptr;
    }
  }

  // //////////////////////////////////////////////////////////////////////
  const TravelSolution* TRAVELCCM_Service::
  getBestTravelSolutionByMatchingIndicator () {
    assert (_travelccmServiceContext != NULL);
    
    return _travelccmServiceContext->getBestAndCheapestTravelSolutionByMatchingIndicator();
  }

  // ///////////////////////////////////////////////////////////////////////
  void TRAVELCCM_Service::addRestrictionsFromRequest () {
    assert (_travelccmServiceContext != NULL);
    
    _travelccmServiceContext->addAndOrderRestrictionsFromRequest ();
  }

  // //////////////////////////////////////////////////////////////////////
  std::string TRAVELCCM_Service::getBestTravelSolutionId() {
    TravelSolutionHolder& lTravelSolutionHolder = getChoosenTravelSolutions();
    
    const TravelSolution* lBestTravelSolution_ptr =
      getBestTravelSolution (lTravelSolutionHolder);

    if (lBestTravelSolution_ptr == NULL) {
      return "";
      
    } else {
      const std::string id = lBestTravelSolution_ptr->getId();
      return id;
    }
  }
    
  // //////////////////////////////////////////////////////////////////////
  bool TRAVELCCM_Service::simulate()  {
    assert (_travelccmServiceContext != NULL);

    // add travel solutions to the travelsolution holder
    // AF404, NCE-LHR, 01-JUN-09 12:00 -> 14:00 (02:00), Eco
    addTravelSolution ("NCE","LHR", stdair::Date_T(2009,05,1),
                       stdair::Duration_T(12,00,00),
                       stdair::Duration_T(14,00,00),
                       stdair::Duration_T(02,00,00), false,
                       "AF", "ECO", 404, 200, 0, false, false, "T1");
    
    // AF404, NCE-LHR, 01-JUN-09 12:00 -> 14:00 (02:00), Eco
    addTravelSolution ("NCE","LHR", stdair::Date_T(2009,05,1),
                       stdair::Duration_T(12,00,00),
                       stdair::Duration_T(14,00,00),
                       stdair::Duration_T(02,00,00), true, "AF",
                       "ECO", 404, 200, 0, false, false, "T2");
    
    // BA404, NCE-LHR, 01-JUN-09 12:00 -> 14:00 (02:00), Eco
    addTravelSolution ("NCE","LHR", stdair::Date_T(2009,06,1),
                       stdair::Duration_T(12,00,00),
                       stdair::Duration_T(14,00,00),
                       stdair::Duration_T(02,00,00), false, "BA",
                       "ECO", 404, 200, 0, true, false, "T3");
    
    // BA404, NCE-LHR, 01-JUN-09 12:00 -> 14:00 (02:00), Eco
    addTravelSolution ("NCE","LHR", stdair::Date_T(2009,06,1),
                       stdair::Duration_T(12,00,00),
                       stdair::Duration_T(14,00,00),
                       stdair::Duration_T(02,00,00), true, "BA",
                       "ECO", 404, 200, 0, true, false, "T4");

    _travelccmServiceContext->createPassenger (stdair::PassengerType::LEISURE);
    _travelccmServiceContext->intializePassenger();

    /** Add a request for the passenger */
    stdair::Date_T date(2009, 6, 1);
    stdair::Duration_T duration(8, 30, 0);
    stdair::DateTime_T dateTime(date, duration);
    addRequest (false, true, false, "NONE", "NONE", dateTime);

    /** Add the restrictions stem from the previous request */
    _travelccmServiceContext->addAndOrderRestrictionsFromRequest();

    // Retrieve the travel solution holder in the service context.
    TravelSolutionHolder& travelSolutionHolder =
      _travelccmServiceContext->getTravelSolutionHolder();

    // Retrieve the restriction holder in the service context.
    Passenger& passenger = _travelccmServiceContext->getPassenger();

    // Initialise the different pointers at the beginning of the different lists
    passenger.begin();
    travelSolutionHolder.begin();

    // DEBUG
    STDAIR_LOG_DEBUG (travelSolutionHolder.numberOfTravelSolutions());
    STDAIR_LOG_DEBUG ("TravelSolutionHolder: "
                      << travelSolutionHolder.toString());
    
    RestrictionHolder& passengerRestrictions =
      passenger.getPassengerRestrictions();

    // DEBUG
    STDAIR_LOG_DEBUG ("RestrictionHolder: "
                      << passengerRestrictions.toString());

    // Call the underlying Use Case (command)
    stdair::BasChronometer lSimulateChronometer;
    lSimulateChronometer.start();
    const bool isNotVoid =
      Simulator::simulate (passenger, travelSolutionHolder);

    const double lSimulateMeasure = lSimulateChronometer.elapsed();
    
    /* We will need the different restrictions and their order so the first
       argument of the function orderedPreference will probably change
    */
    // DEBUG
    STDAIR_LOG_DEBUG ("Simulation done in " << lSimulateMeasure);
    STDAIR_LOG_DEBUG ("TravelSolutionHolder: "
                      << travelSolutionHolder.toString());

    return isNotVoid;
  }

}
