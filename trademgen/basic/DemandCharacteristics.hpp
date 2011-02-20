#ifndef __TRADEMGEN_BAS_DEMAND_CHARACTERISTICS_HPP
#define __TRADEMGEN_BAS_DEMAND_CHARACTERISTICS_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
// StdAir
#include <stdair/stdair_date_time_types.hpp>
#include <stdair/basic/StructAbstract.hpp>
// TraDemGen
#include <trademgen/basic/DemandCharacteristicsTypes.hpp>

namespace TRADEMGEN {

  /** Class modeling the characteristics of a demand type. */
  struct DemandCharacteristics : public stdair::StructAbstract {
        
  public:
    // ////////////// Display Support Methods //////////
    /** Give a description of the structure (for display purposes). */
    const std::string describe() const;


  public:
    // ////////// Constructors and destructors /////////
    /** Constructor. */
    DemandCharacteristics (const ArrivalPatternCumulativeDistribution_T&,
                           const POSProbabilityMassFunction_T&,
                           const ChannelProbabilityMassFunction_T&,
                           const TripTypeProbabilityMassFunction_T&,
                           const StayDurationProbabilityMassFunction_T&,
                           const FrequentFlyerProbabilityMassFunction_T&,
                           const PreferredDepartureTimeContinuousDistribution_T&,
                           const stdair::WTP_T&,
                           const ValueOfTimeContinuousDistribution_T&);
    
    /** Destructor */
    ~DemandCharacteristics();
  private:
    /** Default constructor. */
    DemandCharacteristics();
    /** Deault copy constructor. */
    DemandCharacteristics (const DemandCharacteristics&);


  public:
    // //////////////////// Attributes /////////////////////
    /** Arrival pattern (cumulative distribution of timing of arrival
        of requests (negative number of days between departure date
        and request date). */
    const ContinuousFloatDuration_T _arrivalPattern;

    /** POS probability mass. */
    const POSProbabilityMass_T _posProbabilityMass;
    
    /** Channel probability mass. */
    const ChannelProbabilityMass_T _channelProbabilityMass;
    
    /** Trip type probability mass */
    const TripTypeProbabilityMass_T _tripTypeProbabilityMass;
    
    /** Stay duration probability mass */
    const StayDurationProbabilityMass_T _stayDurationProbabilityMass;
    
    /** Frequent flyer probability mass */
    const FrequentFlyerProbabilityMass_T _frequentFlyerProbabilityMass;

    /** Preferred departure time cumulative distribution. */
    const PreferredDepartureTimeCumulativeDistribution_T _preferredDepartureTimeCumulativeDistribution;
    
    /** Min Willingness-to-pay, used for the computation of the WTP of
        each request. */
    const stdair::WTP_T _minWTP;

    /** Value of time cumulative distribution. */
    const ValueOfTimeCumulativeDistribution_T _valueOfTimeCumulativeDistribution;
  };

}
#endif // __TRADEMGEN_BAS_DEMAND_CHARACTERISTICS_HPP
