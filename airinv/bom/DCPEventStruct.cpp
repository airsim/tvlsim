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
// AirInv
#include <airinv/AIRINV_Types.hpp>
#include <airinv/bom/DCPEventStruct.hpp>

namespace AIRINV {

  // ////////////////////////////////////////////////////////////////////
  DCPEventStruct::DCPEventStruct ()
    : _origin(""), 
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
      _DCP(0),
      _airlineCode(""), 
      _classCode("") {
  }

  // ////////////////////////////////////////////////////////////////////
  stdair::Date_T DCPEventStruct::getDate() const {
    _itYear.check(); _itMonth.check(); _itDay.check();
    return stdair::Date_T (_itYear._value, _itMonth._value, _itDay._value);
  }

  // ////////////////////////////////////////////////////////////////////
  stdair::Duration_T DCPEventStruct::getTime() const {
    _itHours.check(); _itMinutes.check(); _itSeconds.check();
    return boost::posix_time::hours (_itHours._value)
      + boost::posix_time::minutes (_itMinutes._value)
      + boost::posix_time::seconds (_itSeconds._value);
  }
  
 
  // ////////////////////////////////////////////////////////////////////
  const std::string DCPEventStruct::describe () const {
    std::ostringstream ostr; 
    ostr << "DCPEvent: "
         << _origin << "-" << _destination
         << ", POS(" << _pos << "), ["
         << _dateRangeStart << "/" << _dateRangeEnd << "] - ["
         << boost::posix_time::to_simple_string(_timeRangeStart) << "/"
         << boost::posix_time::to_simple_string(_timeRangeEnd) << "]\n    "
         << "-Cabin code- " << _cabinCode << "\n    "
         << "-Channel-    " << _channel << "\n    "
         << "-Conditions- " << _saturdayStay << ", " <<  _changeFees << ", "
         << _nonRefundable << ", " << _advancePurchase << ", "
         << _minimumStay << "\n    "
         << "-DCP-       " << _DCP << "\n           ";
    assert (_airlineCodeList.size() == _classCodeList.size());
    stdair::ClassList_StringList_T::const_iterator lItCurrentClassCode =
      _classCodeList.begin();
    stdair::AirlineCode_T lAirlineCode; 
    std::string lClassCode;
    for (stdair::AirlineCodeList_T::const_iterator lItCurrentAirlineCode =
           _airlineCodeList.begin();
         lItCurrentAirlineCode != _airlineCodeList.end();
         lItCurrentAirlineCode++) {
      lAirlineCode = *lItCurrentAirlineCode;
      lClassCode = *lItCurrentClassCode;
      ostr << lAirlineCode << ", " << lClassCode;
      ostr << "        ";
      lItCurrentClassCode++;
    }
    ostr << std::endl;
    return ostr.str();
  }

 // //////////////////////////////////////////////////////////////////////
  const stdair::AirlineCode_T& DCPEventStruct::getFirstAirlineCode () const {
    assert (_airlineCodeList.size() > 0);
    stdair::AirlineCodeList_T::const_iterator itFirstAirlineCode =
      _airlineCodeList.begin();
    return *itFirstAirlineCode;
  }

  // //////////////////////////////////////////////////////////////////////
  void DCPEventStruct::beginAirline () {
    _itCurrentAirlineCode = _airlineCodeList.begin();
  }

  // //////////////////////////////////////////////////////////////////////
  bool DCPEventStruct::hasNotReachedEndAirline () const {
    bool result = (_itCurrentAirlineCode != _airlineCodeList.end());
    return result;
  }
  
  // //////////////////////////////////////////////////////////////////////
  stdair::AirlineCode_T DCPEventStruct::getCurrentAirlineCode () const {
    assert (_itCurrentAirlineCode != _airlineCodeList.end());    
      return (*_itCurrentAirlineCode);
  }

  // //////////////////////////////////////////////////////////////////////
  void DCPEventStruct::iterateAirline () {
    if (_itCurrentAirlineCode != _classCodeList.end()) {
      _itCurrentAirlineCode++;
    }
  }

  // //////////////////////////////////////////////////////////////////////
  const std::string& DCPEventStruct::getFirstClassCode () const {
    assert (_classCodeList.size() > 0);
    stdair::ClassList_StringList_T::const_iterator itFirstClassCode =
      _classCodeList.begin();
    return *itFirstClassCode;
  }

  // //////////////////////////////////////////////////////////////////////
  void DCPEventStruct::beginClassCode () {
    _itCurrentClassCode = _classCodeList.begin();
  }

  // //////////////////////////////////////////////////////////////////////
  bool DCPEventStruct::hasNotReachedEndClassCode () const {
    bool result = (_itCurrentClassCode != _classCodeList.end());
    return result;
  }
  
  // //////////////////////////////////////////////////////////////////////
  std::string DCPEventStruct::getCurrentClassCode () const {
    assert (_itCurrentClassCode != _classCodeList.end());    
    return (*_itCurrentClassCode);
  }


  // //////////////////////////////////////////////////////////////////////
  void DCPEventStruct::iterateClassCode () {
    if (_itCurrentClassCode != _classCodeList.end()) {
      _itCurrentClassCode++;
    }
  }

}

