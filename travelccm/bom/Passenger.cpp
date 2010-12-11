// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <iostream>
#include <iomanip>
// StdAir
#include <stdair/service/Logger.hpp>
// TRAVELCCM 
#include <travelccm/bom/Passenger.hpp>
#include <travelccm/bom/Request.hpp>
#include <travelccm/bom/DepartureTimePreferencePattern.hpp>
#include <travelccm/bom/RestrictionHolder.hpp>
#include <travelccm/bom/Restriction.hpp>

namespace TRAVELCCM {

  // ////////////////////////////////////////////////////////////////////
  Passenger::Passenger ()
    : _passengerType (stdair::PassengerType::LAST_VALUE), _request (NULL),
      _departureTimePreferencePattern (NULL), _passengerRestrictions (NULL) {
  }

  // ////////////////////////////////////////////////////////////////////
  Passenger::Passenger (const stdair::PassengerType& iPaxType)
    : _passengerType (iPaxType), _request (NULL),
      _departureTimePreferencePattern (NULL), _passengerRestrictions (NULL)  {
    
    /* For the moment, only 2 different types of passengers are implemented;
       business and leisure.
       But we can imagine as many types as possible to reflect the different
       buyer profiles*/
    /* We create a list of restrictions void, and we add the different elements
       of the list when building the different objects, in service or command
    */ 
  }

  // ////////////////////////////////////////////////////////////////////
  Passenger::~Passenger () {
  }

  // //////////////////////////////////////////////////////////////////////
  void Passenger::toStream (std::ostream& ioOut) const {
  }

  // //////////////////////////////////////////////////////////////////////
  void Passenger::fromStream (std::istream& ioIn) {
  }

  // //////////////////////////////////////////////////////////////////////
  std::string Passenger::toString() const {
    std::ostringstream oString;
    oString << "passenger of type: " << _passengerType << ";";
    if (_request != NULL) {
      oString << "passenger's request: " << _request->toString() << ";";
    }
    
    if (_passengerRestrictions != NULL) {
      oString << "passenger's restrictions: "
              << _passengerRestrictions->toString() << ";";
    }
    
    if (_departureTimePreferencePattern != NULL) {
      oString << "passenger's departure time preference pattern: "
              << _departureTimePreferencePattern->toString() << ";";
    }
    return oString.str();
  }
    
  // //////////////////////////////////////////////////////////////////////
  const std::string Passenger::describeKey() const {
    std::string oKey;
    return oKey;
  }

  // //////////////////////////////////////////////////////////////////////
  const std::string Passenger::describeShortKey() const {
    std::string oKey;
    return oKey;
  }

  // //////////////////////////////////////////////////////////////////////
  void Passenger::begin() {
    assert (_passengerRestrictions != NULL);
    _passengerRestrictions->begin();
  }

  // //////////////////////////////////////////////////////////////////////
  const stdair::PassengerType&  Passenger::getPassengerType() const {
    return _passengerType;
  }

  // /////////////////////////////////////////////////////////////////////
  const DateTimePair_T Passenger::getDepartureWindow() const {
    assert (_request != NULL);
    DateTime_T departureTime = _request->getDepartureTime();
    Duration_T lowerBound = getLowerBound(departureTime);
    Duration_T upperBound = getUpperBound(departureTime);
    
    DateTime_T lowerBoundDateTime = departureTime - lowerBound;
    DateTime_T upperBoundDateTime = departureTime + upperBound;
    
    DateTimePair_T oDateTimePair (lowerBoundDateTime, upperBoundDateTime);
    return oDateTimePair;
  }

  // /////////////////////////////////////////////////////////////////////
  const Duration_T Passenger::getLowerBound (DateTime_T departureTime) const {

    // Retrieve the lower time duration, that is up to which amount of
    // time before the passenger is ready to leave
    const DurationPair_T lowerPair = getLowerPair(departureTime);
    const Duration_T lowerDurationTime = lowerPair.first;
    
    // retrieve the upper time duration
    const DurationPair_T upperPair = getUpperPair(departureTime);
    const Duration_T upperDurationTime = upperPair.first;
    
    const Duration_T timeOfDay = departureTime.time_of_day();
    // calculus of the lower bound of the departure window
    // we use a linear regression to determine the lower bound of the departure
    // window.
    // we put all the time durations in seconds to determine the ratio:
    long lowestHourInSeconds = 3600 * timeOfDay.hours();
    long uppestHourInSeconds = 3600 * (timeOfDay.hours() + 1); // +1h
    long timeOfDayInSeconds = timeOfDay.total_seconds();
    double ratio = ((double)timeOfDayInSeconds - (double)lowestHourInSeconds) /
      ((double)uppestHourInSeconds - (double)lowestHourInSeconds);

    const Duration_T oDurationTime =
      computeMiddleDuration(lowerDurationTime.total_seconds(),
                            upperDurationTime.total_seconds(), ratio);

    return oDurationTime;
  }

  // /////////////////////////////////////////////////////////////////////
  const Duration_T Passenger::getUpperBound (DateTime_T departureTime) const {
    
    // Retrieve the lower time duration, that is up to which amount of
    // time after the passenger is ready to leave 
    const DurationPair_T lowerPair = getLowerPair(departureTime);
    const Duration_T lowerDurationTime = lowerPair.second;

    // retrieve the upper time duration
    const DurationPair_T upperPair = getUpperPair(departureTime);
    const Duration_T upperDurationTime = upperPair.second;
    
    const Duration_T timeOfDay = departureTime.time_of_day();

    // calculus of the upper bound of the departure window
    // we use a linear regression to determine the upper boudn of the departure
    // window.
    // we put all the time durations in seconds to determine the ratio:
    long lowestHourInSeconds = 3600 * timeOfDay.hours();
    long uppestHourInSeconds = 3600 * (timeOfDay.hours() + 1); // +1h
    long timeOfDayInSeconds = timeOfDay.total_seconds();
    double ratio = ((double)timeOfDayInSeconds - (double)lowestHourInSeconds) /
      ((double)uppestHourInSeconds - (double)lowestHourInSeconds);
    const Duration_T& oDurationTime =
      computeMiddleDuration(lowerDurationTime.total_seconds(),
                            upperDurationTime.total_seconds(), ratio);
    return oDurationTime;
  }

  // //////////////////////////////////////////////////////////////////////
  const Duration_T Passenger::
  computeMiddleDuration (long firstDurationInSeconds,
                         long secondDurationInSeconds, double ratio) {
    double finalResultInSeconds = (1 - ratio) * firstDurationInSeconds +
      ratio * secondDurationInSeconds;
    const Duration_T oDurationTime =
      boost::posix_time::seconds((int)finalResultInSeconds);
    return oDurationTime;
  }
    
  // //////////////////////////////////////////////////////////////////////
  const DurationPair_T Passenger::getLowerPair(DateTime_T departureTime) const {
    assert (_departureTimePreferencePattern != NULL);
    
    // get the lower and upper bounds of the window
    const Duration_T timeOfDay = departureTime.time_of_day();
    const long nbOfHours = timeOfDay.hours();

    /* from the number of hours, we retrieve the lower bound
       in the map in DepartureTimePreferencePattern */
    const Duration_T lowerBound (nbOfHours, 0, 0);
    
    const DurationPair_T lowerPair =
      _departureTimePreferencePattern->getDurationPair(lowerBound);
    
    return lowerPair;
  }

  // //////////////////////////////////////////////////////////////////////
  const DurationPair_T Passenger::getUpperPair (DateTime_T departureTime) const {
    assert (_departureTimePreferencePattern != NULL);
    
    // get the lower and upper bounds of the window
    const Duration_T timeOfDay = departureTime.time_of_day();
    const long nbOfHours = timeOfDay.hours();

    /* from the number of hours, we retrieve the upper bound
       in the map in DepartureTimePreferencePattern */
    const Duration_T upperBound (nbOfHours + 1, 0, 0);
    const DurationPair_T& upperPair =
      _departureTimePreferencePattern->getDurationPair(upperBound);
    return upperPair;
  }

  // /////////////////////////////////////////////////////////////////////
  RestrictionHolder& Passenger::getPassengerRestrictions() {
    /* the preferrence order, that is the PreferenceList, depends
       on the passenger type;
       See the constructor where the list of restrictions are attributed
       to each type of passenger
    */
    assert (_passengerRestrictions != NULL);
    return *_passengerRestrictions;
  }

  // /////////////////////////////////////////////////////////////////////
  Request& Passenger::getPassengerRequest() {
    assert (_request != NULL);
    return *_request;
  }

  ///////////////////////////////////////////////////////////////////////
  void Passenger::addRestriction (const Restriction& iRestriction) {
    assert (_passengerRestrictions != NULL);
    _passengerRestrictions->addRestriction (iRestriction);
  }
  
}

