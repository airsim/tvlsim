#ifndef __TRADEMGEN_BAS_DEMAND_CHARACTERISTICS_HPP
#define __TRADEMGEN_BAS_DEMAND_CHARACTERISTICS_HPP

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
#include <trademgen/basic/DemandCharacteristicsTypes.hpp>

namespace TRADEMGEN {

  /**
   * @brief Class modeling the characteristics of a demand type.
   */
  struct DemandCharacteristics : public stdair::StructAbstract {
        
  public:
    // ////////////// Business support methods //////////
    /**
     * Get the POS corresponding to the cumulative probability
     */
    const stdair::AirportCode_T&
    getPOSValue (const stdair::Probability_T& iCumulativeProbability) const;

    /**
     * Check that the POS is within the distribution.
     */
    bool checkPOSValue (const stdair::AirportCode_T& iPOS) const;


  public:
    // ////////////// Display support methods //////////
    /**
     * Give a description of the structure (for display purposes).
     */
    const std::string describe() const;


  public:
    // ////////// Constructors and destructors /////////
    /**
     * Constructor.
     */
    DemandCharacteristics (const ArrivalPatternCumulativeDistribution_T&,
                           const POSProbabilityMassFunction_T&,
                           const ChannelProbabilityMassFunction_T&,
                           const TripTypeProbabilityMassFunction_T&,
                           const StayDurationProbabilityMassFunction_T&,
                           const FrequentFlyerProbabilityMassFunction_T&,
                           const PreferredDepartureTimeContinuousDistribution_T&,
                           const stdair::WTP_T&,
                           const ValueOfTimeContinuousDistribution_T&);

    /**
     * Default constructor.
     */
    DemandCharacteristics();

    /**
     * Copy constructor.
     */
    DemandCharacteristics (const DemandCharacteristics&);

    /**
     * Destructor.
     */
    ~DemandCharacteristics();


  public:
    // //////////////////// Attributes /////////////////////
    /**
     * Arrival pattern (cumulative distribution of timing of arrival
     * of requests (negative number of days between departure date
     * and request date).
     */
    ContinuousFloatDuration_T _arrivalPattern;

    /**
     * POS probability mass.
     */
    POSProbabilityMass_T _posProbabilityMass;
    
    /**
     * Channel probability mass.
     */
    ChannelProbabilityMass_T _channelProbabilityMass;
    
    /**
     * Trip type probability mass.
     */
    TripTypeProbabilityMass_T _tripTypeProbabilityMass;
    
    /**
     * Stay duration probability mass.
     */
    StayDurationProbabilityMass_T _stayDurationProbabilityMass;
    
    /**
     * Frequent flyer probability mass.
     */
    FrequentFlyerProbabilityMass_T _frequentFlyerProbabilityMass;

    /**
     * Preferred departure time cumulative distribution.
     */
    PreferredDepartureTimeCumulativeDistribution_T _preferredDepartureTimeCumulativeDistribution;
    
    /**
     * Min Willingness-to-pay, used for the computation of the WTP of
     * each request.
     */
    stdair::WTP_T _minWTP;

    /**
     * FRAT5 pattern, used for the computation of WTP.
     */
    CumulativeDistribution_T _frat5Pattern;

    /**
     * Value of time cumulative distribution.
     */
    ValueOfTimeCumulativeDistribution_T _valueOfTimeCumulativeDistribution;
  };

}
#endif // __TRADEMGEN_BAS_DEMAND_CHARACTERISTICS_HPP
