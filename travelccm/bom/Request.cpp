// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <sstream>
#include <iomanip>
// TravelCCM 
#include <travelccm/bom/Request.hpp>

namespace TRAVELCCM {

  // //////////////////////////////////////////////////////////////////////
  Request::Request (bool refundability, bool changeability,
                    bool saturdayNightStay,
                    stdair::AirlineCode_T preferredAirline,
                    stdair::CabinCode_T preferredCabin,
                    stdair::DateTime_T departureTime)
    : _refundability (refundability), _changeability (changeability),
      _saturdayNightStay (saturdayNightStay),
      _preferredAirline (preferredAirline), _preferredCabin (preferredCabin),
      _departureTime (departureTime) {
  }
  
  // /////////////////////////////////////////////////////////////////////
  Request::Request () {
  }

  // ////////////////////////////////////////////////////////////////////
  Request::Request (const Request&) {
  }
  
  // ////////////////////////////////////////////////////////////////////
  Request::~Request () {
  }

  // //////////////////////////////////////////////////////////////////////
  void Request::toStream (std::ostream& ioOut) const {
  }

  // //////////////////////////////////////////////////////////////////////
  void Request::fromStream (std::istream& ioIn) {
  }

  // //////////////////////////////////////////////////////////////////////
  std::string Request::toString() const {
    std::ostringstream oString;
    oString << "request's characteristics: ";
    oString << "time of departure = "
            << boost::posix_time::to_simple_string(_departureTime);
    if (_refundability) {
      oString << "refundable ; ";
    } else {
      oString << "non refundable ; ";
    }
    if (_changeability) {
      oString << "changeable ; ";
    } else {
      oString << "non changeable ; ";
    }
    if (_saturdayNightStay) {
      oString << "saturday night stay mandatory; ";
    } else {
      oString << "saturday night stay non mandatory ; ";
    }
    oString << "preferred airline (" << _preferredAirline << ") ";
    oString << "preferred cabin (" << _preferredCabin << ") ";

    return oString.str();
  }
    
  // //////////////////////////////////////////////////////////////////////
  const std::string Request::describeKey() const {
    std::string oKey;

    return oKey;
  }

  // //////////////////////////////////////////////////////////////////////
  const std::string Request::describeShortKey() const {
    std::string oKey;

    return oKey;
  }

  // ///////////////////////////////////////////////////////////////////////
  const stdair::DateTime_T Request::getDepartureTime() const {
    return _departureTime;
  }

  // //////////////////////////////////////////////////////////////////////
  const bool Request::getRefundability() const {
    return _refundability;
  }

  // //////////////////////////////////////////////////////////////////////
  const bool Request::getChangeability() const {
    return _changeability;
  }

  // //////////////////////////////////////////////////////////////////////
  const bool Request::getSaturdayNightStay() const {
    return _saturdayNightStay;
  }

  // //////////////////////////////////////////////////////////////////////
  const std::string Request::getPreferredAirline() const {
    return _preferredAirline;
  }

   // /////////////////////////////////////////////////////////////////////
  const std::string Request::getPreferredCabin() const {
    return _preferredCabin;
  }

}
