// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <ostream>
#include <sstream>
// STDAIR
#include <simfqt/bom/FareRuleFeaturesKey.hpp>

namespace SIMFQT  {

   // ////////////////////////////////////////////////////////////////////
  FareRuleFeaturesKey::FareRuleFeaturesKey (const stdair::Duration_T& iTimeRangeStart,
			    const stdair::Duration_T& iTimeRangeEnd,
			    const stdair::DayDuration_T& iAdvancePurchase,
			    const stdair::SaturdayStay_T& iSaturdayStay,
			    const stdair::ChangeFees_T& iChangeFees,
			    const stdair::NonRefundable_T& iNonRefundable,
			    const stdair::DayDuration_T& iMinimumStay,
			    const stdair::Fare_T& iFare)
    : _timeRangeStart(iTimeRangeStart), _timeRangeEnd(iTimeRangeEnd),
      _advancePurchase(iAdvancePurchase), _saturdayStay(iSaturdayStay),
      _changeFees(iChangeFees), _nonRefundable(iNonRefundable),
      _minimumStay(iMinimumStay), _fare(iFare) {
  }

  // ////////////////////////////////////////////////////////////////////
  FareRuleFeaturesKey::FareRuleFeaturesKey (const FareRuleFeaturesKey& iKey)
    :  _timeRangeStart(iKey.getTimeRangeStart()), 
       _timeRangeEnd(iKey.getTimeRangeEnd()),
       _advancePurchase(iKey.getAdvancePurchase()), 
       _saturdayStay(iKey.getSaturdayStay()),
       _changeFees(iKey.getChangeFees()), 
       _nonRefundable(iKey.getRefundableOption()),
      _minimumStay(iKey.getMinimumStay()), 
       _fare(iKey.getFare()) {
  }
  
  // ////////////////////////////////////////////////////////////////////
  FareRuleFeaturesKey::~FareRuleFeaturesKey () {
  }

  // ////////////////////////////////////////////////////////////////////
  void FareRuleFeaturesKey::toStream (std::ostream& ioOut) const {
    ioOut << "FareRuleFeaturesKey: " << toString() << std::endl;
  }

  // ////////////////////////////////////////////////////////////////////
  void FareRuleFeaturesKey::fromStream (std::istream& ioIn) {
  }

  // ////////////////////////////////////////////////////////////////////
  const std::string FareRuleFeaturesKey::toString() const {
    std::ostringstream oStr;
    oStr << _timeRangeStart << "-" << _timeRangeEnd << "-"
	 << _advancePurchase << "-" << _saturdayStay << "-"
	 << _changeFees << "-" << _nonRefundable << "-" 
	 << _minimumStay << "-" << _fare << "-";
    return oStr.str();
  }

}
