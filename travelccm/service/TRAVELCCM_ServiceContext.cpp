 // //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// StdAir
#include <stdair/basic/PassengerType.hpp>
#include <stdair/service/Logger.hpp>
// TravelCCM Basic
#include <travelccm/basic/BasConst_TRAVELCCM_Service.hpp>
// TravelCCM Bom
#include <travelccm/bom/TravelSolution.hpp>
#include <travelccm/bom/TravelSolutionHolder.hpp>
#include <travelccm/bom/Restriction.hpp>
#include <travelccm/bom/RestrictionHolder.hpp>
#include <travelccm/bom/Passenger.hpp>
#include <travelccm/bom/CCM.hpp>
// TravelCCM Factory
#include <travelccm/factory/FacTravelSolution.hpp>
#include <travelccm/factory/FacTravelSolutionHolder.hpp>
#include <travelccm/factory/FacRestriction.hpp>
#include <travelccm/factory/FacRestrictionHolder.hpp>
#include <travelccm/factory/FacPassenger.hpp>
#include <travelccm/factory/FacRequest.hpp>
#include <travelccm/factory/FacDepartureTimePreferencePattern.hpp>
#include <travelccm/factory/FacSupervisor.hpp>
// TravelCCM Command
#include <travelccm/command/FileMgr.hpp>
// TravelCCM Service
#include <travelccm/service/TRAVELCCM_ServiceContext.hpp>

namespace TRAVELCCM {
  
  // //////////////////////////////////////////////////////////////////////
  TRAVELCCM_ServiceContext::TRAVELCCM_ServiceContext() :
    _passenger (NULL), _travelSolutionHolder (NULL) {
    assert (false);
  }
  
  // //////////////////////////////////////////////////////////////////////
  TRAVELCCM_ServiceContext::
  TRAVELCCM_ServiceContext (const TRAVELCCM_ServiceContext&) {
    assert (false);
  }

  // //////////////////////////////////////////////////////////////////////
  TRAVELCCM_ServiceContext::
  TRAVELCCM_ServiceContext (const TravelCCMType& iCCMType) :
    _passenger (NULL), _travelSolutionHolder (NULL), _ccmType (iCCMType) {

    // Set the travel solution holder
    _travelSolutionHolder = &FacTravelSolutionHolder::instance().create();
  }
  
  // //////////////////////////////////////////////////////////////////////
  TRAVELCCM_ServiceContext::~TRAVELCCM_ServiceContext() {
  }
  
  // //////////////////////////////////////////////////////////////////////
  const std::string TRAVELCCM_ServiceContext::shortDisplay() const {
    std::ostringstream oStr;
    oStr << "TRAVELCCM_ServiceContext: " << std::endl
         << "Customer-choice model type: "
         << _ccmType << std::endl;
    return oStr.str();
  }

  // //////////////////////////////////////////////////////////////////////
  const std::string TRAVELCCM_ServiceContext::display() const {
    std::ostringstream oStr;
    oStr << shortDisplay();
    return oStr.str();
  }

  // //////////////////////////////////////////////////////////////////////
  void TRAVELCCM_ServiceContext::
  createPassenger (const stdair::PassengerType& iPaxType) {
    _passenger = &FacPassenger::instance().create (iPaxType);
  }

  // //////////////////////////////////////////////////////////////////////
  void TRAVELCCM_ServiceContext::intializePassenger() {

    // Set the restriction holder of the passenger
    RestrictionHolder& lRestrictionHolder =
      FacRestrictionHolder::instance().create();
    
    assert (_passenger != NULL);
    FacPassenger::instance().
      linkPassengerWithRestrictionHolder (*_passenger, lRestrictionHolder);
    
    // Set the departure time preference pattern of the passenger
    const stdair::PassengerType& lPaxType = _passenger->getPassengerType();
    
    DepartureTimePreferencePattern& lDepartureTimePreferencePattern =
      FacDepartureTimePreferencePattern::instance().
      create (lPaxType.getTypeAsString());
    
    FacPassenger::instance().
      linkPassengerWithDepartureTimePreferencePattern (*_passenger,
                                                       lDepartureTimePreferencePattern);
  }

  // //////////////////////////////////////////////////////////////////////
  void TRAVELCCM_ServiceContext::
  addTravelSolution (const std::string& iDepartureAirport,
                     const std::string& iArrivalAirport,
                     const stdair::Date_T& iDepartureDate,
                     const stdair::Duration_T& iDepartureTime,
                     const stdair::Duration_T& iArrivalTime,
                     const stdair::Duration_T& iDuration,
                     const bool iRefundability,
                     const std::string& iAirlineCode,
                     const std::string& iCabinCode,
                     const int iFlightNumber, const double iFare,
                     int iStopsNumber, bool iSNS, bool iChangeability,
                     const std::string& id) {
    TravelSolution& aTravelSolution =
      FacTravelSolution::instance().create(iDepartureAirport, iArrivalAirport,
                                           iDepartureDate, iDepartureTime,
                                           iArrivalTime, iDuration,
                                           iRefundability, iAirlineCode,
                                           iCabinCode, iFlightNumber, iFare,
                                           iStopsNumber, iSNS, iChangeability,
                                           id);

    assert (_travelSolutionHolder != NULL);
    FacTravelSolution::instance().addTravelSolution (*_travelSolutionHolder,
                                                     aTravelSolution);
  }

  // //////////////////////////////////////////////////////////////////////
  void TRAVELCCM_ServiceContext::
  addRestriction (const std::string& iRestrictionType) {
    Restriction& aRestriction =
      FacRestriction::instance().create (iRestrictionType);

    assert (_passenger != NULL);
    _passenger->addRestriction(aRestriction);
  }

  // //////////////////////////////////////////////////////////////////////
  void TRAVELCCM_ServiceContext::
  addRestriction (const std::string& iRestrictionType,
                  const std::string& iNamePreference) {
    Restriction& aRestriction =
      FacRestriction::instance().create (iRestrictionType, iNamePreference);

    assert (_passenger != NULL);
    _passenger->addRestriction(aRestriction);
  }

  // //////////////////////////////////////////////////////////////////////
  void TRAVELCCM_ServiceContext::
  addRestriction (const std::string& iRestrictionType,
                  const stdair::DateTime_T iDepartureTime) {
    Restriction& aRestriction =
      FacRestriction::instance().create (iRestrictionType, iDepartureTime);
    
    assert (_passenger != NULL);
    _passenger->addRestriction (aRestriction);
  }

  // //////////////////////////////////////////////////////////////////////
  void TRAVELCCM_ServiceContext::
  addAndLinkRequest (bool refundability, bool changeability,
                     bool saturdayNightStay, std::string preferredAirline,
                     std::string preferredCabin,
                     stdair::DateTime_T departureTime) {
    Request& aRequest =
      FacRequest::instance().create(refundability, changeability,
                                    saturdayNightStay, preferredAirline,
                                    preferredCabin, departureTime);
    FacPassenger::instance().linkPassengerWithRequest(*_passenger, aRequest);
  }

  // //////////////////////////////////////////////////////////////////////
  RestrictionHolder& TRAVELCCM_ServiceContext::getRestrictionHolder() const {
    assert (_passenger != NULL);
    return _passenger->getPassengerRestrictions();
  }
  
  // //////////////////////////////////////////////////////////////////////
  TravelSolutionHolder& TRAVELCCM_ServiceContext::
  getTravelSolutionHolder() const {
    assert (_travelSolutionHolder != NULL);
    return *_travelSolutionHolder;
  }

  // //////////////////////////////////////////////////////////////////////
  Passenger& TRAVELCCM_ServiceContext::getPassenger() const {
    assert (_passenger != NULL);
    return *_passenger;
  }

  // //////////////////////////////////////////////////////////////////////
  void TRAVELCCM_ServiceContext::
  readFromInputFile (const std::string& iInputFileName) {
    assert (_travelSolutionHolder != NULL);
    FileMgr::readAndProcessTravelSolutionInputFile (iInputFileName,
                                                    *_travelSolutionHolder);
  }

  // //////////////////////////////////////////////////////////////////////
  const TravelSolution* TRAVELCCM_ServiceContext::
  getBestAndCheapestTravelSolutionByMatchingIndicator () {
    // passenger type, request and int
    assert (_passenger != NULL);

    const Request& iRequest = _passenger->getPassengerRequest();
    const stdair::PassengerType& iPassengerType = _passenger->getPassengerType();

    // filtrate to keep only the travel solution with the highest matching
    // indicator.
    CCM::getBestTravelSolutionByMatchingIndicator (iPassengerType, iRequest,
                                                   _travelSolutionHolder);

    assert (_travelSolutionHolder != NULL);
    if (_travelSolutionHolder->isVoid()) {
      return NULL;
    }
    
    const TravelSolution* cheapestTravelSolution =
      _travelSolutionHolder->getCheapestTravelSolution();

    return cheapestTravelSolution;
  }
  
  // /////////////////////////////////////////////////////////////////////
  void TRAVELCCM_ServiceContext::addAndOrderRestrictionsFromRequest() {
    /* the order when adding the restrictions is crucial since it reflects
       the importance of the characteristics, compared to the others; more
       precisely the first characteristic added is the most important for the
       passenger and the last one the less important.
       For a business passenger, we have: departureTime >> saturday night stay >>
       refundability >> preferred airline >> preferred cabin >> changeability
       For a leisure passenger, we have: changeability >> departure time >>
       preferred airline >> saturday night stay >> refundability >>
       preferred cabin  */

    assert (_passenger != NULL);
    Request& request = _passenger->getPassengerRequest();

    const stdair::PassengerType& passengerType = _passenger->getPassengerType();
    
    // retrieve the characteristics of the fare in the Request class
    const bool refundability = request.getRefundability();
    const bool changeability = request.getChangeability();
    const bool saturdayNightStay = request.getSaturdayNightStay();
    const std::string& preferredAirline = request.getPreferredAirline();
    const std::string& preferredCabin = request.getPreferredCabin();
    const stdair::DateTime_T& departureTime = request.getDepartureTime();

    if (passengerType.getType() == stdair::PassengerType::BUSINESS) {
      // there is always a departure time request so we always add a time
      // restriction
      addRestriction("timePreference", departureTime);
      
      if (saturdayNightStay) {
        addRestriction("saturdayStay");
      }
      
      if (refundability) {
        addRestriction("refundability");
      }
      
      if (preferredAirline != "NONE") {
        addRestriction ("preferredAirline", preferredAirline);
      }
      
      if (preferredCabin != "NONE") {
        addRestriction ("preferredCabin", preferredCabin);
      }
      
      if (changeability) {
        addRestriction ("changeability");
      }
      
    } else if (passengerType.getType()  == stdair::PassengerType::LEISURE) {
      if (changeability) {
        addRestriction ("changeability");
      }
      
      addRestriction ("timePreference", departureTime);
      if (preferredAirline != "NONE") {
        addRestriction ("preferredAirline", preferredAirline);
      }
      
      if (saturdayNightStay) {
        addRestriction ("saturdayStay");
      }
      
      if (refundability) {
        addRestriction ("refundability");
      }
      
      if (preferredCabin != "NONE") {
        addRestriction ("preferredCabin", preferredCabin);
      }
    }
  }
  
}
