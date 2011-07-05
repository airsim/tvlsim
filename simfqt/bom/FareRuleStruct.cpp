// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <sstream>
#include <vector>
// StdAir
#include <stdair/basic/BasConst_General.hpp>
#include <stdair/service/Logger.hpp>
// SIMFQT
#include <simfqt/SIMFQT_Types.hpp>
#include <simfqt/bom/FareRuleStruct.hpp>

namespace SIMFQT {

  // ////////////////////////////////////////////////////////////////////
  FareRuleStruct::FareRuleStruct ()
    :_fareId(0), 
     _origin(""), 
     _destination(""),
     _dateRangeStart(stdair::DEFAULT_DATE),
     _dateRangeEnd(stdair::DEFAULT_DATE),
     _timeRangeStart(stdair::DEFAULT_EPSILON_DURATION),
     _timeRangeEnd(stdair::DEFAULT_EPSILON_DURATION),
     _cabinCode (""), 
     _pos (""), 
     _advancePurchase(0), 
     _saturdayStay("T"),
     _changeFees("T"), 
     _nonRefundable("T"),
     _minimumStay(0), 
     _fare(0),
     _airlineCode(""), 
     _classCode("") {

  }

  // ////////////////////////////////////////////////////////////////////
  stdair::Date_T FareRuleStruct::getDate() const {
    _itYear.check(); _itMonth.check(); _itDay.check();
    return stdair::Date_T (_itYear._value, _itMonth._value, _itDay._value);
  }

  // ////////////////////////////////////////////////////////////////////
  stdair::Duration_T FareRuleStruct::getTime() const {
    _itHours.check(); _itMinutes.check(); _itSeconds.check();
    return boost::posix_time::hours (_itHours._value)
      + boost::posix_time::minutes (_itMinutes._value)
      + boost::posix_time::seconds (_itSeconds._value);
  }
  
 
  // ////////////////////////////////////////////////////////////////////
  const std::string FareRuleStruct::describe () const {

    std::ostringstream oStr;
    oStr << "FareRule: " << _fareId << ", ";

    oStr << _origin << "-" << _destination << " ("
         << _pos << "), " << _channel << ", [";
    oStr << _dateRangeStart << "/" << _dateRangeEnd << "] - ["
         << boost::posix_time::to_simple_string (_timeRangeStart) << "/"
         << boost::posix_time::to_simple_string (_timeRangeEnd) << "], ";

    oStr << _cabinCode << ", " << _fare  << " EUR, ";
    oStr << _tripType << ", " << _saturdayStay << ", "
         <<  _changeFees << ", " << _nonRefundable << ", "
         << _advancePurchase << ", " << _minimumStay << ", ";

    // Sanity check
    assert (_airlineCodeList.size() == _classCodeList.size());

    // Browse the airline and class pathes
    unsigned short idx = 0;
    stdair::ClassList_StringList_T::const_iterator itClass =
      _classCodeList.begin();
    for (stdair::AirlineCodeList_T::const_iterator itAirline =
           _airlineCodeList.begin();
         itAirline != _airlineCodeList.end(); ++itAirline, ++itClass, ++idx) {
      if (idx != 0) {
        oStr << " - ";
      }
      const stdair::AirlineCode_T lAirlineCode = *itAirline;
      const stdair::ClassCode_T lClassCode = *itClass;
      oStr << lAirlineCode << " / " << lClassCode;
    }

    return oStr.str();
  }
  
}

