// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <sstream>
// StdAir
#include <stdair/basic/BasConst_General.hpp>
#include <stdair/service/Logger.hpp>
// SIMFQT
#include <simfqt/bom/FareTimePeriod.hpp>

namespace SIMFQT {

  // ////////////////////////////////////////////////////////////////////
  FareTimePeriod::FareTimePeriod (const Key_T& iKey)
    : _key (iKey), _parent (NULL)  {
  }

  // ////////////////////////////////////////////////////////////////////
  FareTimePeriod::~FareTimePeriod () {
  }
  
  // ////////////////////////////////////////////////////////////////////
  std::string FareTimePeriod::toString() const {
    std::ostringstream oStr;
    oStr << describeKey();
    return oStr.str();
  }
  
  // ////////////////////////////////////////////////////////////////////
  bool FareTimePeriod::
  isDepartureTimeValid (const stdair::Time_T& iFlightTime) const {

    const stdair::Time_T& lTimeRangeStart = getTimeRangeStart();
    const stdair::Time_T& lTimeRangeEnd = getTimeRangeEnd();
    
    // Check if the departure time is within the time range.
    if (lTimeRangeStart >= iFlightTime) {
      return false;
    }
    if (lTimeRangeEnd <= iFlightTime) {
      return false;
    }

    return true;
  }
  
}

