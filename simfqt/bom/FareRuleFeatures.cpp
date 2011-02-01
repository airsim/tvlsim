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
#include <simfqt/bom/FareRuleFeatures.hpp>

namespace SIMFQT {

  // ////////////////////////////////////////////////////////////////////
  FareRuleFeatures::FareRuleFeatures (const Key_T& iKey)
    : _key (iKey), _parent (NULL)  {
  }

  // ////////////////////////////////////////////////////////////////////
  FareRuleFeatures::~FareRuleFeatures () {
  }
  
  // ////////////////////////////////////////////////////////////////////
  std::string FareRuleFeatures::toString() const {
    std::ostringstream oStr;
    oStr << describeKey();
    return oStr.str();
  }

  // ////////////////////////////////////////////////////////////////////
  bool FareRuleFeatures::
  IsStayDurationValid (const stdair::DayDuration_T& iStayDuration) const {

    // Check if the stay duration is lower or equal to the minimum one.
    const stdair::DayDuration_T& lMinimumDayDuration = getMinimumStay ();
    if (lMinimumDayDuration > iStayDuration) {
      return false;
    }

    return true;
  }

  // ////////////////////////////////////////////////////////////////////
  bool FareRuleFeatures::
  IsAdvancePurchaseValid (const stdair::DateTime_T& iBookingRequestDateTime,
                          const stdair::DateTime_T& iFlightDateTime) const {

    // Check if the departure date is within the date range.
    long lAdvancedPurchase = getAdvancePurchase ();
    const stdair::DateOffset_T lMinimumAdvancedPurchase(lAdvancedPurchase);
    const stdair::DateTime_T& lCriticalDate = iFlightDateTime - lMinimumAdvancedPurchase;
      
    if (lCriticalDate < iBookingRequestDateTime) {
       return false;
     }

    return true;
  }
}

