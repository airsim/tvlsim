#ifndef __TRADEMGEN_BOM_DEMANDSTREAM_HPP
#define __TRADEMGEN_BOM_DEMANDSTREAM_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STDAIR
#include <stdair/bom/BomAbstract.hpp>
#include <stdair/bom/BookingRequestTypes.hpp>
// TRADEMGEN
#include <trademgen/basic/DemandCharacteristics.hpp>
#include <trademgen/basic/DemandDistribution.hpp>
#include <trademgen/basic/RandomGeneration.hpp>
#include <trademgen/basic/RandomGenerationContext.hpp>
#include <trademgen/bom/DemandStreamKey.hpp>
#include <trademgen/bom/DemandStreamTypes.hpp>

namespace TRADEMGEN {
  
  /** Class modeling a demand stream. */
  class DemandStream  : public stdair::BomAbstract {
    friend class FacDemandStream;

  public:
    // Type definitions.
    /** Definition allowing to retrieve the associated BOM key type. */
    typedef DemandStreamKey Key_T;

  public:
    // ///////////// Getters ///////////
    /** Get the key */
    const Key_T& getKey () const { return _key; }
    
    /** Get the total number of requests to be generated. */
    const stdair:: NbOfRequests_T& getTotalNumberOfRequestsToBeGenerated() const{
      return _totalNumberOfRequestsToBeGenerated;
    }

    /** Get the mean number of requests. */
    const stdair::NbOfRequests_T& getMeanNumberOfRequests() const {
      return _demandDistribution._meanNumberOfRequests;
    }
    
    /** Get the standard deviation of number of requests. */
    const stdair::StdDevValue_T& getStdDevNumberOfRequests() const {
      return _demandDistribution._stdDevNumberOfRequests;
    }
    
    /** Get the number of requests generated so far. */
    const stdair::Count_T& getNumberOfRequestsGeneratedSoFar() const {
      return _randomGenerationContext._numberOfRequestsGeneratedSoFar;
    }
    
    /** Get the seed of the random generator for the number of requests. */
    const stdair:: RandomSeed_T& getNumberOfRequestsRandomGeneratorSeed () const{
      return _numberOfRequestsRandomGenerator._seed;
    }

    /** Get the seed of the random generator for the request datetime. */
    const stdair:: RandomSeed_T& getRequestDateTimeRandomGeneratorSeed () const {
      return _requestDateTimeRandomGenerator._seed;
    }

    /** Get the seed of the random generator for the demand characteristics. */
    const stdair:: RandomSeed_T& getDemandCharacteristicsRandomGeneratorSeed () const {
      return _demandCharacteristicsRandomGenerator._seed;
    }

    /** Get the demand characteristics. */
    const DemandCharacteristics& getDemandCharacteristics () const {
      return _demandCharacteristics;
    }

    /** Get the demand distribution. */
    const DemandDistribution& getDemandDistribution () const {
      return _demandDistribution;
    }

  public:
    // //////////////// Setters //////////////////    
    /** Set the number of requests generated so far. */
    void setNumberOfRequestsGeneratedSoFar (const stdair:: Count_T& iNumberOfRequests) {
      _randomGenerationContext._numberOfRequestsGeneratedSoFar =
        iNumberOfRequests;
    }
    
  public:
    // /////////// Display support methods /////////
    /** Dump a Business Object into an output stream.
        @param ostream& the output stream. */
    void toStream (std::ostream& ioOut) const { ioOut << toString(); }

    /** Read a Business Object from an input stream.
        @param istream& the input stream. */
    void fromStream (std::istream& ioIn) { }

   /** Get the serialised version of the Business Object. */
    std::string toString() const;
    
    /** Get a string describing the  key. */
    const std::string describeKey() const { return _key.toString(); }

  public:
    // /////////////// Business Methods //////////
    /** Increment counter of requests generated so far */
    void incrementGeneratedRequestsCounter () {
      _randomGenerationContext.incrementGeneratedRequestsCounter();
    }
    
    /** Check whether enough requests have already been generated. */
    const bool stillHavingRequestsToBeGenerated () const;

    /** Generate the time of the next request. */
    const stdair::DateTime_T generateTimeOfRequest ();

    /** Generate the POS. */
    const stdair::AirportCode_T generatePOS ();

    /** Generate the reservation channel. */
    const stdair::ChannelLabel_T generateChannel ();

    /** Generate the trip type. */
    const stdair::TripType_T generateTripType ();

    /** Generate the stay duration. */
    const stdair::DayDuration_T generateStayDuration ();

    /** Generate the frequent flyer type. */
    const stdair::FrequentFlyer_T generateFrequentFlyer ();

    /** Generate the preferred departure time. */
    const stdair::Duration_T generatePreferredDepartureTime ();
    
    /** Generate the WTP. */
    const stdair::WTP_T generateWTP (const stdair::Date_T&,
                                     const stdair::DateTime_T&,
                                     const stdair::DayDuration_T&);

    /** Generate the value of time. */
    const stdair::PriceValue_T generateValueOfTime ();
    
    /** Generate the next request. */
    stdair::BookingRequestPtr_T generateNextRequest ();

    /** Reset all the contexts of the demand stream. */
    void reset ();
       
  protected:
    // ////////// Constructors and destructors /////////
    /** Constructor by default */
    DemandStream (const Key_T&,
                  const ArrivalPatternCumulativeDistribution_T&,
                  const POSProbabilityMassFunction_T&,
                  const ChannelProbabilityMassFunction_T&,
                  const TripTypeProbabilityMassFunction_T&,
                  const StayDurationProbabilityMassFunction_T&,
                  const FrequentFlyerProbabilityMassFunction_T&,
                  const PreferredDepartureTimeContinuousDistribution_T&,
                  const stdair::WTP_T&,
                  const ValueOfTimeContinuousDistribution_T&,
                  const DemandDistribution&,
                  const stdair::RandomSeed_T& iNumberOfRequestsSeed,
                  const stdair::RandomSeed_T& iRequestDateTimeSeed,
                  const stdair::RandomSeed_T& iDemandCharacteristicsSeed,
                  stdair::UniformGenerator_T&, const POSProbabilityMass_T&);
    /** Default constructors. */
    DemandStream ();
    DemandStream (const DemandStream&);
    /** Initialization. */
    void init();
    /** Destructor. */
    ~DemandStream ();

    
  protected:
    // ////////// Attributes //////////
    /** Key */
    Key_T _key;
    
    /** Demand characteristics */
    DemandCharacteristics _demandCharacteristics;

    /** Demand distribution */
    DemandDistribution _demandDistribution;
    
    /** Total number of requests to be generated*/
    stdair::NbOfRequests_T _totalNumberOfRequestsToBeGenerated;

    /** Random generation context */
    RandomGenerationContext _randomGenerationContext;
  
    /** Random generator for number of requests */
    RandomGeneration _numberOfRequestsRandomGenerator;
    
    /** Random generator for request datetime */
    RandomGeneration _requestDateTimeRandomGenerator;
    
    /** Random generator for demand characteristics */
    RandomGeneration _demandCharacteristicsRandomGenerator;

    /** Shared random uniform generator for seeds and some other values. */
    stdair::UniformGenerator_T& _uniformGenerator;

    /** Defaut POS probablity mass, used when "row" is drawn. */
    const POSProbabilityMass_T& _posProMass;
  };

}
#endif // __TRADEMGEN_BOM_DEMANDSTREAM_HPP
