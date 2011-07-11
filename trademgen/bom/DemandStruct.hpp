#ifndef __TRADEMGEN_BOM_DEMANDSTRUCT_HPP
#define __TRADEMGEN_BOM_DEMANDSTRUCT_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
// StdAir
#include <stdair/stdair_basic_types.hpp>
#include <stdair/stdair_maths_types.hpp>
#include <stdair/stdair_date_time_types.hpp>
#include <stdair/basic/StructAbstract.hpp>
#include <stdair/bom/DoWStruct.hpp>
// TraDemGen
#include <trademgen/basic/DemandCharacteristicsTypes.hpp>

namespace TRADEMGEN {

  /** Utility Structure for the parsing of Demand structures. */
  struct DemandStruct : public stdair::StructAbstract {

  public:
    // /////////////////// Getters ////////////////////
    /** Get the date from the staging details. */
    stdair::Date_T getDate() const;

    /** Get the time from the staging details. */
    stdair::Duration_T getTime() const;


  public:
    // ////////////////// Display Support Methods ////////////////
    /** Give a description of the structure (for display purposes). */
    const std::string describe() const;


  public:
    // /////////////// Constructors and destructors ///////////////
    /** Default constructor. */
    DemandStruct();
    /** Destructor */
    ~DemandStruct();
  private:
    /** Deault copy constructor. */
    DemandStruct (const DemandStruct&);


  public:
    // ////////////// Attributes ///////////////////
    stdair::DatePeriod_T _dateRange;
    stdair::DoWStruct _dow;
    stdair::AirportCode_T _origin;
    stdair::AirportCode_T _destination;
    stdair::CabinCode_T _prefCabin;
    stdair::MeanValue_T _demandMean;
    stdair::StdDevValue_T _demandStdDev;
    POSProbabilityMassFunction_T _posProbDist;
    ChannelProbabilityMassFunction_T _channelProbDist;
    TripTypeProbabilityMassFunction_T _tripProbDist;
    StayDurationProbabilityMassFunction_T _stayProbDist;
    FrequentFlyerProbabilityMassFunction_T _ffProbDist;
    PreferredDepartureTimeContinuousDistribution_T _prefDepTimeProbDist; 
    stdair::WTP_T _minWTP;
    ValueOfTimeContinuousDistribution_T _timeValueProbDist;
    ArrivalPatternCumulativeDistribution_T _dtdProbDist;
    
  public:
    // ////////////// Staging ///////////////////
    /** Staging Date. */
    stdair::Date_T _prefDepDateStart;
    stdair::Date_T _prefDepDateEnd;
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
