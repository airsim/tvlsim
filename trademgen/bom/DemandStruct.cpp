// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <sstream>
// StdAir
#include <stdair/basic/BasConst_Inventory.hpp>
#include <stdair/service/Logger.hpp>
// TRADEMGEN
#include <trademgen/TRADEMGEN_Types.hpp>
#include <trademgen/bom/DemandStruct.hpp>

namespace TRADEMGEN {

  // ////////////////////////////////////////////////////////////////////
  DemandStruct::DemandStruct()
    : _prefDepDate (stdair::DEFAULT_DEPARTURE_DATE),
      _prefArrDate (stdair::DEFAULT_DEPARTURE_DATE),
      _prefCabin (stdair::DEFAULT_CABIN_CODE),
      _itHours (0), _itMinutes (0), _itSeconds (0), _itFFCode ("") {
  }

  // ////////////////////////////////////////////////////////////////////
  DemandStruct::DemandStruct (const DemandStruct& iDemandStruct)
    : _prefDepDate (iDemandStruct._prefDepDate),
      _prefArrDate (iDemandStruct._prefArrDate),
      _prefCabin (iDemandStruct._prefCabin),
      _itHours (iDemandStruct._itHours),
      _itMinutes (iDemandStruct._itMinutes),
      _itSeconds (iDemandStruct._itSeconds),
      _itFFCode (iDemandStruct._itFFCode) {
  }

  // ////////////////////////////////////////////////////////////////////
  DemandStruct::~DemandStruct() {
  }

  // ////////////////////////////////////////////////////////////////////
  stdair::Date_T DemandStruct::getDate() const {
    return stdair::Date_T (_itYear, _itMonth, _itDay);
  }

  // ////////////////////////////////////////////////////////////////////
  stdair::Duration_T DemandStruct::getTime() const {
    return boost::posix_time::hours (_itHours)
      + boost::posix_time::minutes (_itMinutes)
      + boost::posix_time::seconds (_itSeconds);
  }
  
  // ////////////////////////////////////////////////////////////////////
  const std::string DemandStruct::describe() const {
    std::ostringstream ostr;
    ostr << _prefDepDate << " -> " << _prefArrDate
         << " " << _origin << "-" << _destination
         << " " << _prefCabin
         << ", N(" << _demandMean << ", " << _demandStdDev << "); ";

    unsigned short idx = 0;
    for (POSProbabilityMassFunction_T::const_iterator it = _posProbDist.begin();
         it != _posProbDist.end(); ++it, ++idx) {
      const stdair::AirportCode_T& lPosCode = it->first;
      const stdair::Probability_T& lPosProbMass = it->second;
      if (idx != 0) {
        ostr << ", ";
      }
      ostr << lPosCode << ":" << lPosProbMass;
    }
    ostr << "; ";
    
    idx = 0;
    for (ChannelProbabilityMassFunction_T::const_iterator it = 
           _channelProbDist.begin();
         it != _channelProbDist.end(); ++it, ++idx) {
      const stdair::ChannelLabel_T lChannelCode = it->first;
      const stdair::Probability_T& lChannelProbMass = it->second;
      if (idx != 0) {
        ostr << ", ";
      }
      ostr << lChannelCode << ":" << lChannelProbMass;
    }
    ostr << "; ";
    
    idx = 0;
    for (TripTypeProbabilityMassFunction_T::const_iterator it = 
           _tripProbDist.begin();
         it != _tripProbDist.end(); ++it, ++idx) {
      const stdair::TripType_T lTripCode = it->first;
      const stdair::Probability_T& lTripProbMass = it->second;
      if (idx != 0) {
        ostr << ", ";
      }
      ostr << lTripCode << ":" << lTripProbMass;
    }
    ostr << "; ";
    
    idx = 0;
    for (StayDurationProbabilityMassFunction_T::const_iterator it = 
           _stayProbDist.begin();
         it != _stayProbDist.end(); ++it, ++idx) {
      const stdair::DayDuration_T& lStayDuration = it->first;
      const stdair::Probability_T& lStayProbMass = it->second;
      if (idx != 0) {
        ostr << ", ";
      }
      ostr << lStayDuration << ":" << lStayProbMass;
    }
    ostr << "; ";
    
    idx = 0;
    for (FrequentFlyerProbabilityMassFunction_T::const_iterator it =
           _ffProbDist.begin();
         it != _ffProbDist.end(); ++it, ++idx) {
      const stdair::FrequentFlyer_T lFFCode = it->first;
      const stdair::Probability_T& lFFProbMass = it->second;
      if (idx != 0) {
        ostr << ", ";
      }
      ostr << lFFCode << ":" << lFFProbMass;
    }
    ostr << "; ";
    
    idx = 0;
    for (PreferredDepartureTimeContinuousDistribution_T::const_iterator it =
           _prefDepTimeProbDist.begin();
         it != _prefDepTimeProbDist.end(); ++it, ++idx) {
      const stdair::IntDuration_T& lPrefDepTime = it->first;
      const stdair::Probability_T& lPrefDepTimeProbMass = it->second;
      if (idx != 0) {
        ostr << ", ";
      }
      ostr << lPrefDepTime << ":" << lPrefDepTimeProbMass;
    }
    ostr << "; ";

    ostr << _minWTP << "; ";
    
    idx = 0;
    for (ValueOfTimeContinuousDistribution_T::const_iterator it =
           _timeValueProbDist.begin();
         it != _timeValueProbDist.end(); ++it, ++idx) {
      const stdair::PriceValue_T& lTimeValue = it->first;
      const stdair::Probability_T& lTimeValueProbMass = it->second;
      if (idx != 0) {
        ostr << ", ";
      }
      ostr << lTimeValue << ":" << lTimeValueProbMass;
    }
    ostr << "; ";
    
    idx = 0;
    for (ArrivalPatternCumulativeDistribution_T::const_iterator it = 
           _dtdProbDist.begin(); it != _dtdProbDist.end(); ++it, ++idx) {
      const stdair::FloatDuration_T& lDTD = it->first;
      const stdair::Probability_T& lDTDProbMass = it->second;
      if (idx != 0) {
        ostr << ", ";
      }
      ostr << lDTD << ":" << lDTDProbMass;
    }
    ostr << "; ";
    
    return ostr.str();
  }

}
