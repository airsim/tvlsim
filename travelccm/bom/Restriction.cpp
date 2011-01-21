// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <ostream>
#include <istream>
#include <iomanip>
// StdAir
#include <stdair/service/Logger.hpp>
// TRAVELCCM 
#include <travelccm/bom/Restriction.hpp>

namespace TRAVELCCM {

  // //////////////////////////////////////////////////////////////////////
  Restriction::Restriction () {
  }

  // /////////////////////////////////////////////////////////////////////
  Restriction:: Restriction (const std::string& iRestrictionType)
    : _restrictionType (iRestrictionType) {
  }

  // /////////////////////////////////////////////////////////////////////
  Restriction:: Restriction (const std::string& iRestrictionType,
                             const std::string& iNamePreference)
    : _restrictionType (iRestrictionType) {
    
    if (iRestrictionType == "preferredAirline") {
      _preferredAirline = iNamePreference;
    }
    
    if (iRestrictionType == "preferredCabin") {
      _preferredCabin == iNamePreference;
    }
  }

  // /////////////////////////////////////////////////////////////////////
  Restriction:: Restriction (const std::string& iRestrictionType,
                             const stdair::DateTime_T& iDateTime)
    : _restrictionType (iRestrictionType) {

    if (iRestrictionType == "timePreference") {
      _preferredDepartureTime = iDateTime;
    }
  }
  
  // ////////////////////////////////////////////////////////////////////
  Restriction::~Restriction () {
  }

  // //////////////////////////////////////////////////////////////////////
  void Restriction::toStream (std::ostream& ioOut) const {
  }

  // //////////////////////////////////////////////////////////////////////
  void Restriction::fromStream (std::istream& ioIn) {
  }

  // //////////////////////////////////////////////////////////////////////
  std::string Restriction::toString() const {
    std::ostringstream oString;
    
    // Only the type of restriction is printed for the moment
    oString << "restriction type: " << _restrictionType;
    
    if (_restrictionType == "preferredAirline") {
      oString << " (" << _preferredAirline << ") ";
    }
    
    if (_restrictionType == "preferredCabin") {
      oString << " (" << _preferredCabin << ") ";
    }
    
    if (_restrictionType == "timePreference" ) {
      oString << " ( "
              << boost::posix_time::to_simple_string (_preferredDepartureTime)
              << " )";
    }
    
    return oString.str();
  }
    
  // //////////////////////////////////////////////////////////////////////
  const std::string Restriction::describeKey() const {
    std::string oKey;
    return oKey;
  }

  // //////////////////////////////////////////////////////////////////////
  const std::string Restriction::describeShortKey() const {
    std::string oKey;
    return oKey;
  }

  // /////////////////////////////////////////////////////////////////////
  const std::string Restriction::getRestrictionType() const {
    return _restrictionType;
  }

  // /////////////////////////////////////////////////////////////////////
  const std::string Restriction::getPreferredAirline() const {
    return _preferredAirline;
  }

  // /////////////////////////////////////////////////////////////////////
  const std::string Restriction::getPreferredCabin() const {
    return _preferredCabin;
  }

   // /////////////////////////////////////////////////////////////////////
  const stdair::DateTime_T Restriction::getPreferredDateTime() const {
    return  _preferredDepartureTime;
  }
  
  // /////////////////////////////////////////////////////////////////////
  /*bool Restriction::travelSolutionMeetRestriction (TravelSolution& TS) const {
    ** need to consider all the different kind of restrictions in a
        separate way
    *
    if (getRestrictionType() == "refundability")
      {
      if (TS.getRefundable())
        return true;
      else
        return false;
      }
    else if (getRestrictionType() == "preferredAirline")
      {
        // be careful on how you write the airline (airline code or no?)
        if (TS.getAirlineName() == getPreferredAirline() )
          return true;
        else
          return false;
      }
    else if (getRestrictionType() == "preferredCabin")
      {
        * today we look for the perfect match. A better solution would be
           to allow thee overclassing *
        if (TS.getCabin() == getPreferredCabin() )
          return true;
        else
          return false;
      }
    
    ** the function return true by default in order not to loose any
        correct travel solution *
    else return true;
  }
  */

}

