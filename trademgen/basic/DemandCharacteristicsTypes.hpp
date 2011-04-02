#ifndef __TRADEMGEN_BAS_DEMANDCHARACTERISTICSTYPES_HPP
#define __TRADEMGEN_BAS_DEMANDCHARACTERISTICSTYPES_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// StdAir
#include <stdair/stdair_basic_types.hpp>
#include <stdair/stdair_date_time_types.hpp>
#include <stdair/stdair_demand_types.hpp>
// TraDemGen
#include <trademgen/basic/ContinuousAttributeLite.hpp>
#include <trademgen/basic/CategoricalAttributeLite.hpp>

namespace TRADEMGEN {

  /** Type definition for the continuous distribition of the duration
      (as a float number). */
  typedef ContinuousAttributeLite<stdair::FloatDuration_T> ContinuousFloatDuration_T;
   
  /** Type definition for the arrival pattern cumulative distribution. */
  typedef ContinuousFloatDuration_T::ContinuousDistribution_T ArrivalPatternCumulativeDistribution_T;

  /** Define the point-of-sale probablity mass. */
  typedef CategoricalAttributeLite<stdair::AirportCode_T> POSProbabilityMass_T;

  /** Define the probability mass function type of point-of-sale. */
  typedef POSProbabilityMass_T::ProbabilityMassFunction_T POSProbabilityMassFunction_T;
  
  /** Define the booking channel probablity mass. */
  typedef CategoricalAttributeLite<stdair::ChannelLabel_T> ChannelProbabilityMass_T;

  /** Define the probability mass function type of booking channel. */
  typedef ChannelProbabilityMass_T::ProbabilityMassFunction_T ChannelProbabilityMassFunction_T;

  /** Define the trip type probablity mass. */
  typedef CategoricalAttributeLite<stdair::TripType_T> TripTypeProbabilityMass_T;

  /** Define the probability mass function type of trip type. */
  typedef TripTypeProbabilityMass_T::ProbabilityMassFunction_T TripTypeProbabilityMassFunction_T;
  
  /** Define the stay duration probablity mass. */
  typedef CategoricalAttributeLite<stdair::DayDuration_T> StayDurationProbabilityMass_T;

  /** Define the probability mass function type of stay duration. */
  typedef StayDurationProbabilityMass_T::ProbabilityMassFunction_T StayDurationProbabilityMassFunction_T;

  /** Define the frequent flyer probablity mass. */
  typedef CategoricalAttributeLite<stdair::FrequentFlyer_T> FrequentFlyerProbabilityMass_T;

  /** Define the probability mass function type of frequent flyer. */
  typedef FrequentFlyerProbabilityMass_T::ProbabilityMassFunction_T FrequentFlyerProbabilityMassFunction_T;

  /** Define the preferred departure time cumulative distribution. */
  typedef ContinuousAttributeLite<stdair::IntDuration_T> PreferredDepartureTimeCumulativeDistribution_T;

  /** Define the preferred departure time continuous distribution. */
  typedef PreferredDepartureTimeCumulativeDistribution_T::ContinuousDistribution_T PreferredDepartureTimeContinuousDistribution_T;

  /** Define the value of time cumulative distribution. */
  typedef ContinuousAttributeLite<stdair::PriceValue_T> ValueOfTimeCumulativeDistribution_T;

  /** Define the value of time continuous distribution. */
  typedef ValueOfTimeCumulativeDistribution_T::ContinuousDistribution_T ValueOfTimeContinuousDistribution_T;

  /** Define the FRAT5 pattern type. */
  typedef ContinuousAttributeLite<stdair::RealNumber_T> CumulativeDistribution_T;
  typedef CumulativeDistribution_T::ContinuousDistribution_T FRAT5Pattern_T;
}
#endif // __TRADEMGEN_BAS_DEMANDCHARACTERISTICSTYPES_HPP
