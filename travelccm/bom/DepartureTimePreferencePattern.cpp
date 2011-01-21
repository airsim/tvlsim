// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <sstream>
#include <iomanip>
// StdAir
#include <stdair/basic/PassengerType.hpp>
#include <stdair/service/Logger.hpp>
// TravelCCM
#include <travelccm/TRAVELCCM_Types.hpp>
#include <travelccm/basic/BasConst_TimePattern.hpp>
#include <travelccm/bom/DepartureTimePreferencePattern.hpp>

namespace TRAVELCCM {

  // //////////////////////////////////////////////////////////////////////
  DepartureTimePreferencePattern::
  DepartureTimePreferencePattern (const PatternId_T& iPaxType)
    : _departureTimePreferencePatternId (iPaxType) {
    
    if (_departureTimePreferencePatternId[0]
        == stdair::PassengerType::getTypeLabel(stdair::PassengerType::BUSINESS)){
      /* For the moment, we consider that a business passenger is
        ready to leave up to 10h sooner or later his preferred
        departure time, and 1h for a business passenger. */
      stdair::Duration_T lowerBound(1,0,0);
      stdair::Duration_T upperBound(1,0,0);
      DurationPair_T timeInterval (lowerBound, upperBound);
      int i = 0;
      // we add the pair (lower bound, upper bound) for each hour of the day
      // that is 0h, 1h, 2h... (5h,(2h,3h)) means that the passenger is ready to
      // leave between 3h and 8h!
      // (i goes from 0 to 24 to facilitate the calculus of the decision window
      // in the passenger class)
      for (i = 0; i != 25 ; i++) {
        stdair::Duration_T timeOfDay (i,0,0);
        std::pair<stdair::Duration_T,
                  DurationPair_T> patternElement (timeOfDay, timeInterval);
        _departureTimePreferencePattern.insert (patternElement);
      }
      
    } else if (_departureTimePreferencePatternId[0]
               == stdair::PassengerType::getTypeLabel (stdair::PassengerType::LEISURE)) {
      stdair::Duration_T lowerBound (10,0,0);
      stdair::Duration_T upperBound (10,0,0);
      DurationPair_T timeInterval (lowerBound, upperBound);
      int i = 0;
      for (i = 0; i < 25 ; i++) {
        stdair::Duration_T timeOfDay(i,0,0);
        std::pair<stdair::Duration_T,
                  DurationPair_T> patternElement (timeOfDay, timeInterval);
        _departureTimePreferencePattern.insert (patternElement);
      }
    }
  }

  // //////////////////////////////////////////////////////////////////////
  DepartureTimePreferencePattern::~DepartureTimePreferencePattern () {
  }
  
  // //////////////////////////////////////////////////////////////////////
  void DepartureTimePreferencePattern::toStream (std::ostream& ioOut) const {
  }
  
  // //////////////////////////////////////////////////////////////////////
  void DepartureTimePreferencePattern::fromStream (std::istream& ioIn) {
  }

  // //////////////////////////////////////////////////////////////////////
  std::string DepartureTimePreferencePattern::toString() const {
    std::ostringstream oString;
    oString << "departure time preference pattern for passenger of type "
            << _departureTimePreferencePatternId;
      return oString.str();
  }

  // //////////////////////////////////////////////////////////////////////
  const std::string DepartureTimePreferencePattern::describeShortKey() const {
    std::string oKey;

    return oKey;
  }
  
  // //////////////////////////////////////////////////////////////////////
  const std::string DepartureTimePreferencePattern::describeKey() const {
    // Store current formatting flags of std::cout
    std::ios::fmtflags oldFlags = std::cout.flags();

    std::ostringstream ostr;

    ostr << _departureTimePreferencePatternId << " ";
      
    // Reset formatting flags of std::cout
    std::cout.flags (oldFlags);

    return ostr.str();
  }

  // //////////////////////////////////////////////////////////////////////
  const std::string DepartureTimePreferencePattern::display() const {

    // Store current formatting flags of std::cout
    std::ios::fmtflags oldFlags = std::cout.flags();

    std::ostringstream ostr;

    ostr << "Departure Time Preference ID: " << describeKey() << ": ";
    /*
    for (DepartureTimePreferencePattern_T::const_iterator
           itTimePreference = _departureTimePreference.begin();
         itTimePreference != _departureTimePreference.end();
         itTimePreference++) {
      const stdair::Duration_T& lTimeOfDay = itTimePreference->first;
      const DurationPair_T& lDurationPair = itTimePreference->second;
      const stdair::Duration_T& lLowerBound = lDurationPair.first;
      const stdair::Duration_T& lUpperBound = lDurationPair.second;
       
      ostr << boost::posix_time::time_duration::to_simple_string (lTimeOfDay)
           << " "
           << lCancellationRate << "% "
           << lNoShowRate <<  "%; ";       
    }
    */
    ostr << std::endl;

    // Reset formatting flags of std::cout
    std::cout.flags (oldFlags);

    return ostr.str();
  }

  // ///////////////////////////////////////////////////////////////////////
  bool DepartureTimePreferencePattern::
  isBetweenDateTheDepartureWindow (stdair::DateTime_T iDateTime,
                                   DateTimePair_T iDateTimePair) {
    stdair::DateTime_T lowerBound = iDateTimePair.first;
    stdair::DateTime_T upperBound = iDateTimePair.second;
    return iDateTime >= lowerBound && iDateTime <= upperBound;
  }

  // ///////////////////////////////////////////////////////////////////////
  const DurationPair_T DepartureTimePreferencePattern::
  getDurationPair (const stdair::Duration_T& ioDuration) const {
    DepartureTimePreferencePattern_T::const_iterator itDepartureTime =
      _departureTimePreferencePattern.find (ioDuration);
    assert (itDepartureTime != _departureTimePreferencePattern.end());
    const DurationPair_T oDurationPair = itDepartureTime->second;
    return oDurationPair;
  }

}
