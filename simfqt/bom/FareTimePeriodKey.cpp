// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <ostream>
#include <sstream>
// STDAIR
#include <simfqt/bom/FareTimePeriodKey.hpp>

namespace SIMFQT  {

  // ////////////////////////////////////////////////////////////////////
  FareTimePeriodKey::FareTimePeriodKey (const stdair::Time_T& iTimeRangeStart,
                                        const stdair::Time_T& iTimeRangeEnd)
    : _timeRangeStart(iTimeRangeStart), _timeRangeEnd(iTimeRangeEnd) {
  }

  // ////////////////////////////////////////////////////////////////////
  FareTimePeriodKey::FareTimePeriodKey (const FareTimePeriodKey& iKey)
    : _timeRangeStart(iKey.getTimeRangeStart()),
      _timeRangeEnd(iKey.getTimeRangeEnd())  {
  }

  // ////////////////////////////////////////////////////////////////////
  FareTimePeriodKey::~FareTimePeriodKey () {
  }

  // ////////////////////////////////////////////////////////////////////
  void FareTimePeriodKey::toStream (std::ostream& ioOut) const {
    ioOut << "FareTimePeriodKey: " << toString() << std::endl;
  }

  // ////////////////////////////////////////////////////////////////////
  void FareTimePeriodKey::fromStream (std::istream& ioIn) {
  }

  // ////////////////////////////////////////////////////////////////////
  const std::string FareTimePeriodKey::toString() const {
    std::ostringstream oStr;
    oStr << _timeRangeStart << "-" << _timeRangeEnd << std::endl; 
    return oStr.str();
  }

}
