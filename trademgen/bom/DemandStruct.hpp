#ifndef __TRADEMGEN_BOM_DEMANDSTRUCT_HPP
#define __TRADEMGEN_BOM_DEMANDSTRUCT_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
// StdAir
#include <stdair/stdair_basic_types.hpp>
#include <stdair/stdair_date_time_types.hpp>
#include <stdair/basic/StructAbstract.hpp>
// TraDemGen
#include <trademgen/basic/DemandCharacteristicTypes.hpp>

namespace TRADEMGEN {

  /** Utility Structure for the parsing of Demand structures. */
  struct DemandStruct : public stdair::StructAbstract {
    
    /** Get the date from the staging details. */
    stdair::Date_T getDate() const;

    /** Get the time from the staging details. */
    stdair::Duration_T getTime() const;

    /** Give a description of the structure (for display purposes). */
    const std::string describe() const;

    /** Constructor. */
    DemandStruct ();

    // Attributes
    stdair::Date_T _prefDepDate;
    stdair::Date_T _prefArrDate;
    stdair::AirportCode_T _origin;
    stdair::AirportCode_T _destination;
    stdair::CabinCode_T _prefCabin;
    float _demandMean;
    float _demandStdDev;
    POSProbabilityMassFunction_T _posProbDist;
    ChannelProbabilityMassFunction_T _channelProbDist;
    TripTypeProbabilityMassFunction_T _tripProbDist;
    StayDurationProbabilityMassFunction_T _stayProbDist;
    FrequentFlyerProbabilityMassFunction_T _ffProbDist;
    PreferredDepartureTimeContinuousDistribution_T _prefDepTimeProbDist; 
    stdair::WTP_T _minWTP;
    ValueOfTimeContinuousDistribution_T _timeValueProbDist;
    ArrivalPatternCumulativeDistribution_T _dtdProbDist;
    
    /** Staging Date. */
    unsigned int _itYear;
    unsigned int _itMonth;
    unsigned int _itDay;
      
    /** Staging Time. */
    long _itHours;
    long _itMinutes;
    long _itSeconds;

    /** Staging Point-Of-Sale (POS) code. */
    stdair::AirportCode_T _itPosCode;

    /** Staging channel type code. */
    stdair::ChannelLabel_T _itChannelCode;

    /** Staging trip type code. */
    stdair::TripType_T _itTripCode;

    /** Staging stay duration. */
    stdair::DayDuration_T _itStayDuration;
    
    /** Staging Frequent Flyer code. */
    stdair::FrequentFlyer_T _itFFCode;

    /** Staging preferred departure time. */
    stdair::Duration_T _itPrefDepTime;
    
    /** Staging time value. */
    stdair::PriceValue_T _itTimeValue;

    /** Staging DTD (Days-To-Departure). */
    stdair::DayDuration_T _itDTD;
  };

}
#endif // __TRADEMGEN_BOM_DEMANDSTRUCT_HPP
