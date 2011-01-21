// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <sstream>
#include <iomanip>
// StdAir
#include <stdair/service/Logger.hpp>
// TravelCCM 
#include <travelccm/bom/TravelSolution.hpp>
#include <travelccm/bom/Restriction.hpp>
#include <travelccm/bom/Passenger.hpp>
#include <travelccm/bom/Request.hpp>
#include <travelccm/bom/DepartureTimePreferencePattern.hpp>

namespace TRAVELCCM {

  // ////////////////////////////////////////////////////////////////////
  TravelSolution::
  TravelSolution (const stdair::AirportCode_T& iDepartureAirport,
                  const stdair::AirportCode_T& iArrivalAirport,
                  const stdair::Date_T& iDepartureDate,
                  const stdair::Duration_T& iDepartureTime,
                  const stdair::Duration_T& iArrivalTime,
                  const stdair::Duration_T& iDuration,
                  const bool iRefundability,
                  const stdair::AirlineCode_T& iAirlineCode,
                  const stdair::CabinCode_T& iCabinCode,
                  const stdair::FlightNumber_T& iFlightNumber,
                  const stdair::Fare_T& iFare, int& iStopsNumber,
                  bool iSNS, bool iChangeability,
                  const std::string& iId)
    :  _departureAirport (iDepartureAirport), _arrivalAirport (iArrivalAirport),
       _departureDate (iDepartureDate), _departureTime (iDepartureTime),
       _arrivalTime (iArrivalTime), _duration (iDuration),
       _refundable (iRefundability), _airlineName (iAirlineCode),
       _cabinName (iCabinCode), _flightNumber (iFlightNumber),
       _fare (iFare), _numberOfLags (iStopsNumber),
       _saturdayNightStay (iSNS), _changeable (iChangeability), id (iId) {
     }

  // /////////////////////////////////////////////////////////////////////
  TravelSolution::TravelSolution () {
  }

  // ////////////////////////////////////////////////////////////////////
  TravelSolution::~TravelSolution () {
  }

  // //////////////////////////////////////////////////////////////////////
  void TravelSolution::toStream (std::ostream& ioOut) const {
  }

  // //////////////////////////////////////////////////////////////////////
  void TravelSolution::fromStream (std::istream& ioIn) {
  }

  // //////////////////////////////////////////////////////////////////////
  std::string TravelSolution::toString() const {
    std::ostringstream oString;
    oString << "travel solution: (" << _departureAirport << ", "
            << _arrivalAirport << ") ; departure date: "
            << boost::gregorian::to_simple_string(_departureDate) << "; "
            << _numberOfLags << " stop(s); "
            << "flight " << _airlineName << _flightNumber << "; "
            << "departure time: " << _departureTime << ", arrival time: "
            << _arrivalTime << ", duration: " << _duration << "; " << "cabin: "
            << _cabinName << "; ";
    if (_refundable){
     oString << "refundable fare; ";
    } else {
     oString << "nonrefundable fare; ";
    }
    if (_changeable){
      oString << "changeable fare; ";
    } else {
      oString << "nonchangeable fare; ";
    }
    if (_saturdayNightStay){
      oString << "Saturday Night Stay mandatory; ";
    } else {
      oString << "Saturday Night Stay non mandatory; ";
    }
    oString << "price: " << _fare <<  " \n";
    return oString.str();
  }
    
  // //////////////////////////////////////////////////////////////////////
  const std::string TravelSolution::describeKey() const {
    std::string oKey;
    return oKey;
  }

  // //////////////////////////////////////////////////////////////////////
  const std::string TravelSolution::describeShortKey() const {
    std::string oKey;
    return oKey;
  }

   // //////////////////////////////////////////////////////////////////////
  const std::string TravelSolution::getDepartureAirport() const {
    return _departureAirport;
  }

  // //////////////////////////////////////////////////////////////////////
  const std::string TravelSolution::getArrivalAirport() const {
    return _arrivalAirport;
  }

  // /////////////////////////////////////////////////////////////////////
  const stdair::DateTime_T TravelSolution::getDepartureDateTime() const {
    stdair::Date_T date = getDepartureDate();
    stdair::Duration_T time = getDepartureTime();
    stdair::DateTime_T dateTime (date, time);
    return dateTime;
  }

  // //////////////////////////////////////////////////////////////////////
  const stdair::Date_T TravelSolution::getDepartureDate() const {
    return _departureDate;
  }
  
  // //////////////////////////////////////////////////////////////////////
  const stdair::Duration_T TravelSolution::getDepartureTime() const {
    return _departureTime;
  }

  // //////////////////////////////////////////////////////////////////////
  const stdair::Duration_T TravelSolution::getArrivalTime() const {
    return _arrivalTime;
  }

  // //////////////////////////////////////////////////////////////////////
  const stdair::Duration_T TravelSolution::getDuration() const {
    return _duration;
  }

  // //////////////////////////////////////////////////////////////////////
  const bool TravelSolution::getRefundable() const {
    return _refundable;
  } 

  // //////////////////////////////////////////////////////////////////////
  const stdair::AirlineCode_T TravelSolution::getAirlineName() const {
    return _airlineName;
  }

  // //////////////////////////////////////////////////////////////////////
  const stdair::CabinCode_T TravelSolution::getCabin() const {
    return _cabinName;
  }

  // //////////////////////////////////////////////////////////////////////
  const stdair::FlightNumber_T TravelSolution::getFlightNumber() const {
    return _flightNumber;
  }

  // //////////////////////////////////////////////////////////////////////
  const stdair::Fare_T TravelSolution::getFare() const {
    return _fare;
  }
  
  // //////////////////////////////////////////////////////////////////////
  const int TravelSolution::getNumberOfLags() const {
    return _numberOfLags;
  }

  // //////////////////////////////////////////////////////////////////////
  const bool TravelSolution::getSaturdayNightStay() const {
    return _saturdayNightStay;
  }

  // //////////////////////////////////////////////////////////////////////
  const bool TravelSolution::getChangeable() const {
    return _changeable;
  }

  // //////////////////////////////////////////////////////////////////////
  const std::string TravelSolution::getId() const {
    return id;
  }

  // //////////////////////////////////////////////////////////////////////
  const bool TravelSolution::
  isCheaper (const TravelSolution& iComparedTravelSolution) const {
    const stdair::Fare_T currentFare = getFare();
    const stdair::Fare_T comparedFare = iComparedTravelSolution.getFare();
    return (comparedFare < currentFare);
  }

  // //////////////////////////////////////////////////////////////////////
  const bool TravelSolution::
  hasTheSamePrice (const TravelSolution& iComparedTravelSolution) const {
    const stdair::Fare_T currentFare = getFare();
    const stdair::Fare_T comparedFare = iComparedTravelSolution.getFare();
    return (comparedFare == currentFare);
  }
  
  // /////////////////////////////////////////////////////////////////////
  bool TravelSolution::
  restrictionMeetsTravelSolution (const Restriction& iRestriction,
                                  const Passenger& iPassenger) const {
    /** Need to consider all the different kind of restrictions in a
        separate way
    */
    /** Restrictions which are to be implemented next: time preference? ,
        changeability, number of stops (flight duration), saturday night stay?
    */
    if (iRestriction.getRestrictionType() == "refundability") {
      if (getRefundable()) {
        return true;
      } else {
        return false;
      }
    }

    if (iRestriction.getRestrictionType() == "preferredAirline") {
      // Be careful on how you write the airline (airline code or no?)
      if (getAirlineName() == iRestriction.getPreferredAirline()) {
        return true;
      } else {
        return false;
      }
    }
    
    if (iRestriction.getRestrictionType() == "preferredCabin") {
      /* Today we look for the perfect match. A better solution would be
         to allow the overclassing */
      if (getCabin() == iRestriction.getPreferredCabin()) {
        return true;
      } else {
        return false;
      }
    }

    if (iRestriction.getRestrictionType() == "saturdayStay") {
      if (getSaturdayNightStay()) {
        return true;
      } else {
        return false;
      }
    }

    if (iRestriction.getRestrictionType() == "timePreference") {
      /** The most difficult restriction to implement. Here is only one
          solution, not perfect, to deal with it. */
      stdair::DateTime_T dateTime = getDepartureDateTime();
      DateTimePair_T passengerWindow = iPassenger.getDepartureWindow();
      
      stdair::DateTime_T lowerBound = passengerWindow.first;
      stdair::DateTime_T upperBound = passengerWindow.second;
        
      const bool travelSolutionIsInTheDepartureWindow =
        DepartureTimePreferencePattern::
        isBetweenDateTheDepartureWindow (dateTime, passengerWindow);
      
      if (travelSolutionIsInTheDepartureWindow) {
        return true;
      } else {
        return false;
      }
    }
    
    /** The function return true by default in order not to loose any
        correct travel solution */
    return true;
  }

  // ///////////////////////////////////////////////////////////////////////
  int TravelSolution::
  CalculateMatchingNumber (const stdair::PassengerType& iPaxType,
                           const Request& iRequest,
                           int baseNumber) const {
    int matchingIndicator = 0;
    int numberOfRestrictions = iRequest.getNumberOfRestrictions();
    // the preferred departure time is still to be added
    
    if (iPaxType == stdair::PassengerType::BUSINESS) {
      // if the passenger wants that condition to be respected...
      bool saturdayNightStay = iRequest.getSaturdayNightStay();
      if (saturdayNightStay) {
        // then we look if the travel solution meets this restriction
        bool travelSolutionSaturdayNightStay = getSaturdayNightStay();
        if (travelSolutionSaturdayNightStay) {
          matchingIndicator += baseNumber^(numberOfRestrictions - 1);
        }
      }
      
      bool refundability = iRequest.getRefundability();
      if (refundability) {
        bool travelSolutionRefundability = getRefundable();
        if (travelSolutionRefundability) {
          matchingIndicator += baseNumber^(numberOfRestrictions - 2);
        }
      }
      
      // to be changed if there are serveral preferred airlines
      stdair::AirlineCode_T preferredAirline = iRequest.getPreferredAirline();
      if (preferredAirline != "NONE") {
        std::string travelSolutionPreferredAirline = getAirlineName();
        if (preferredAirline == travelSolutionPreferredAirline) {
          matchingIndicator += baseNumber^(numberOfRestrictions - 3);
        }
      }
      
      stdair::CabinCode_T preferredCabin = iRequest.getPreferredCabin();
      if (preferredCabin != "NONE") {
        std::string travelSolutionPreferredCabin = getCabin();
        if (preferredCabin == travelSolutionPreferredCabin) {
          matchingIndicator += baseNumber^(numberOfRestrictions - 4);
        }
      }
      
      bool changeability = iRequest.getChangeability();
      if (changeability) {
        bool travelSolutionChangeability = getChangeable();
        if (travelSolutionChangeability) {
          matchingIndicator += baseNumber^(numberOfRestrictions - 5);
        }
      } 

    } else if (iPaxType == stdair::PassengerType::LEISURE) {
      bool changeability = iRequest.getChangeability();
      if (changeability) {
        bool travelSolutionChangeability = getChangeable();
        if (travelSolutionChangeability) {
          matchingIndicator += baseNumber^(numberOfRestrictions - 1);
        }
      }
      
      // to be changed if there are serveral preferred airlines
      stdair::AirlineCode_T preferredAirline = iRequest.getPreferredAirline();
      if (preferredAirline != "NONE") {
        std::string travelSolutionPreferredAirline = getAirlineName();
        if (preferredAirline == travelSolutionPreferredAirline) {
          matchingIndicator += baseNumber^(numberOfRestrictions - 2);
        }
      }
      
      bool saturdayNightStay = iRequest.getSaturdayNightStay();
      if (saturdayNightStay) {
        bool travelSolutionSaturdayNightStay = getSaturdayNightStay();
        if (travelSolutionSaturdayNightStay) {
          matchingIndicator += baseNumber^(numberOfRestrictions - 3);
        }
      }
      
      bool refundability = iRequest.getRefundability();
      if (refundability) {
        bool travelSolutionRefundability = getRefundable();
        if (travelSolutionRefundability) {
          matchingIndicator += baseNumber^(numberOfRestrictions - 4);
        }
      }
      
      stdair::CabinCode_T preferredCabin = iRequest.getPreferredCabin();
      if (preferredCabin != "NONE") {
        std::string travelSolutionPreferredCabin = getCabin();
        if (preferredCabin == travelSolutionPreferredCabin) {
          matchingIndicator += baseNumber^(numberOfRestrictions - 5);
        }
      }
    }
    
    return matchingIndicator;
  }
  
}

