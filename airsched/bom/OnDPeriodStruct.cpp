// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <iostream>
// StdAir
#include <stdair/basic/BasConst_Period_BOM.hpp>
#include <stdair/basic/BasConst_General.hpp>
#include <stdair/basic/BasConst_Inventory.hpp>
#include <stdair/service/Logger.hpp>
// AIRSCHED
#include <airsched/bom/OnDPeriodStruct.hpp>

namespace AIRSCHED {
  // //////////////////////////////////////////////////////////////////////
  OnDPeriodStruct::OnDPeriodStruct ()
    : _datePeriod (stdair::BOOST_DEFAULT_DATE_PERIOD),
      _timeRangeStart (stdair::NULL_BOOST_TIME_DURATION),
      _timeRangeEnd (stdair::NULL_BOOST_TIME_DURATION),
      _nbOfAirlines(stdair::DEFAULT_NBOFAIRLINES),
      _airlineCode (stdair::DEFAULT_NULL_AIRLINE_CODE),
      _classCode (stdair::DEFAULT_NULL_CLASS_CODE),
      _itSeconds (0) {
  }

  // //////////////////////////////////////////////////////////////////////
  stdair::Date_T OnDPeriodStruct::getDate() const {
    return stdair::Date_T (_itYear, _itMonth, _itDay);
  }

  // //////////////////////////////////////////////////////////////////////
  stdair::Duration_T OnDPeriodStruct::getTime() const {
    return boost::posix_time::hours (_itHours)
      + boost::posix_time::minutes (_itMinutes)
      + boost::posix_time::seconds (_itSeconds);
  }

  // //////////////////////////////////////////////////////////////////////
  const std::string OnDPeriodStruct::describe() const {
    std::ostringstream ostr;
    ostr << _origin << "-" << _destination << ", "
         << _datePeriod << ", between  "
         << boost::posix_time::to_simple_string(_timeRangeStart)
         << " to "
         << boost::posix_time::to_simple_string(_timeRangeEnd) << ", "
         << _classCode << ", "
         << _airlineCode << ", "
         << std::endl; 
      
    return ostr.str();
  }

  // //////////////////////////////////////////////////////////////////////
  const std::string OnDPeriodStruct::describeTSKey() const {
    std::ostringstream ostr;
    ostr << _origin << "-" << _destination << ", "
         << _airlineCode << ", " << _classCode << std::endl; 
      
    return ostr.str();
  }

  // //////////////////////////////////////////////////////////////////////
  const stdair::AirlineCode_T& OnDPeriodStruct::getFirstAirlineCode () const {
    assert (_airlineCodeList.size() > 0);
    stdair::AirlineCodeList_T::const_iterator itFirstAirlineCode =
      _airlineCodeList.begin();
    return *itFirstAirlineCode;
  }

}
