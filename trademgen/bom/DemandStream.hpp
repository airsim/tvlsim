#ifndef __TRADEMGEN_BOM_DEMANDSTREAM_HPP
#define __TRADEMGEN_BOM_DEMANDSTREAM_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// StdAir
#include <stdair/bom/BomAbstract.hpp>
#include <stdair/bom/BookingRequestTypes.hpp>
#include <stdair/basic/RandomGeneration.hpp>
// TraDemGen
#include <trademgen/basic/DemandCharacteristics.hpp>
#include <trademgen/basic/DemandDistribution.hpp>
#include <trademgen/basic/RandomGenerationContext.hpp>
#include <trademgen/bom/DemandStreamKey.hpp>
#include <trademgen/bom/DemandStreamTypes.hpp>

/// Forward declarations
namespace stdair {
  class FacBomManager;
  template <typename BOM> class FacBom;
}

namespace TRADEMGEN {
  
  /**
   * @brief Class modeling a demand stream.
   */
  class DemandStream : public stdair::BomAbstract {
    template <typename BOM> friend class stdair::FacBom;
    friend class stdair::FacBomManager;

  public:
    // ////////// Type definitions ////////////
    /**
     * Definition allowing to retrieve the associated BOM key type.
     */
    typedef DemandStreamKey Key_T;


  public:
    // ///////////// Getters ///////////
    /** Get the key */
    const Key_T& getKey() const {
      return _key;
    }
    
    /** Get the parent object (EventQueue). */
    BomAbstract* const getParent() const {
      return _parent;
    }
    
    /** Get the origin (part of the primary key). */
    const stdair::AirportCode_T& getOrigin() const {
      return _key.getOrigin();
    }

    /** Get the destination (part of the primary key). */
    const stdair::AirportCode_T& getDestination() const {
      return _key.getDestination();
    }

    /** Get the preferred departure date (part of the primary key). */
    const stdair::Date_T& getPreferredDepartureDate() const {
      return _key.getPreferredDepartureDate();
    }
    
    /** Get the preferred cabin (part of the primary key). */
    const stdair::CabinCode_T& getPreferredCabin() const {
      return _key.getPreferredCabin();
    }
    
    /** Get the map of children holders. */
    const stdair::HolderMap_T& getHolderMap() const {
      return _holderMap;
    }
    
    /** Get the demand characteristics. */
    const DemandCharacteristics& getDemandCharacteristics() const {
      return _demandCharacteristics;
    }

    /** Get the demand distribution. */
    const DemandDistribution& getDemandDistribution() const {
      return _demandDistribution;
    }

    /** Get the total number of requests to be generated. */
    const stdair::NbOfRequests_T& getTotalNumberOfRequestsToBeGenerated() const{
      return _totalNumberOfRequestsToBeGenerated;
    }

    /** Get the mean (expected) number of requests. */
    const stdair::NbOfRequests_T& getMeanNumberOfRequests() const {
      return _demandDistribution._meanNumberOfRequests;
    }
    
    /** Get the standard deviation of number of requests. */
    const stdair::StdDevValue_T& getStdDevNumberOfRequests() const {
      return _demandDistribution._stdDevNumberOfRequests;
    }
    
    /** Get the number of requests generated so far. */
    const stdair::Count_T& getNumberOfRequestsGeneratedSoFar() const {
      return _randomGenerationContext.getNumberOfRequestsGeneratedSoFar();
    }

    /**
     * Get the default POS probablity mass, used when "row" (rest of
     * the world) is drawn.
     */
    const POSProbabilityMass_T& getPOSProbabilityMass() const {
      return _posProMass;
    }


  public:
    // //////////////// Setters //////////////////    
    /** Set the number of requests generated so far. */
    void setNumberOfRequestsGeneratedSoFar (const stdair:: Count_T& iCount) {
      _randomGenerationContext.setNumberOfRequestsGeneratedSoFar (iCount);
    }

    /** Set the demand distribution. */
    void setDemandDistribution (const DemandDistribution& iDemandDistribution) {
      _demandDistribution = iDemandDistribution;
    }

    /** Set the demand characteristics. */
    void
    setDemandCharacteristics (const ArrivalPatternCumulativeDistribution_T& iArrivalPattern,
                              const POSProbabilityMassFunction_T& iPOSProbMass,
                              const ChannelProbabilityMassFunction_T& iChannelProbMass,
                              const TripTypeProbabilityMassFunction_T& iTripTypeProbMass,
                              const StayDurationProbabilityMassFunction_T& iStayDurationProbMass,
                              const FrequentFlyerProbabilityMassFunction_T& iFrequentFlyerProbMass,
                              const PreferredDepartureTimeContinuousDistribution_T& iPreferredDepartureTimeContinuousDistribution,
                              const stdair::WTP_T& iMinWTP,
                              const ValueOfTimeContinuousDistribution_T& iValueOfTimeContinuousDistribution) {
      _demandCharacteristics =
        DemandCharacteristics (iArrivalPattern, iPOSProbMass,
                               iChannelProbMass, iTripTypeProbMass,
                               iStayDurationProbMass, iFrequentFlyerProbMass,
                               iPreferredDepartureTimeContinuousDistribution,
                               iMinWTP, iValueOfTimeContinuousDistribution);
    }

    /** Set the total number of requests to be generated. */
    void setTotalNumberOfRequestsToBeGenerated (const stdair::NbOfRequests_T& iNbOfRequests) {
      _totalNumberOfRequestsToBeGenerated = iNbOfRequests;
    }

    /** Set the seed of the random generator for the request datetime. */
    void setRequestDateTimeRandomGeneratorSeed (const stdair::RandomSeed_T& iSeed) {
      _requestDateTimeRandomGenerator.init (iSeed);
    }

    /** Set the seed of the random generator for the demand characteristics. */
    void setDemandCharacteristicsRandomGeneratorSeed (const stdair::RandomSeed_T& iSeed) {
      _demandCharacteristicsRandomGenerator.init (iSeed);
    }

    /**
     * Set the default POS probablity mass, used when "row" (rest of
     * the world) is drawn.
     */
    void setPOSProbabilityMass (const POSProbabilityMass_T& iProbMass) {
      _posProMass = iProbMass;
    }

    /**
     * Initialisation.
     */
    void setAll (const ArrivalPatternCumulativeDistribution_T&,
                 const POSProbabilityMassFunction_T&,
                 const ChannelProbabilityMassFunction_T&,
                 const TripTypeProbabilityMassFunction_T&,
                 const StayDurationProbabilityMassFunction_T&,
                 const FrequentFlyerProbabilityMassFunction_T&,
                 const PreferredDepartureTimeContinuousDistribution_T&,
                 const stdair::WTP_T&,
                 const ValueOfTimeContinuousDistribution_T&,
                 const DemandDistribution&,
                 stdair::BaseGenerator_T& ioSharedGenerator,
                 const stdair::RandomSeed_T& iRequestDateTimeSeed,
                 const stdair::RandomSeed_T& iDemandCharacteristicsSeed,
                 const POSProbabilityMass_T&);

    /**
     * Set the boolean describing if it is the first time we generate a
     * request for a demand stream.
     */
    void setBoolFirstDateTimeRequest (const bool& iFirstDateTimeRequest) {
      _firstDateTimeRequest = iFirstDateTimeRequest;
    }
    

  public:
    // /////////////////// Business Methods ///////////////////
    /** Increment counter of requests generated so far */
    void incrementGeneratedRequestsCounter() {
      _randomGenerationContext.incrementGeneratedRequestsCounter();
    }
    
    /** Check whether enough requests have already been generated. */
    const bool stillHavingRequestsToBeGenerated (const bool) const;

    /** Generate the time of the next request with exponential law. */
    const stdair::DateTime_T generateTimeOfRequestExponentialLaw();

    /** Generate the time of the next request with statistic order */
    const stdair::DateTime_T generateTimeOfRequestStatisticOrder();

    /** Generate the POS. */
    const stdair::AirportCode_T generatePOS();

    /** Generate the reservation channel. */
    const stdair::ChannelLabel_T generateChannel();

    /** Generate the trip type. */
    const stdair::TripType_T generateTripType();

    /** Generate the stay duration. */
    const stdair::DayDuration_T generateStayDuration();

    /** Generate the frequent flyer type. */
    const stdair::FrequentFlyer_T generateFrequentFlyer();

    /** Generate the preferred departure time. */
    const stdair::Duration_T generatePreferredDepartureTime();
    
    /** Generate the WTP. */
    const stdair::WTP_T generateWTP (stdair::RandomGeneration&,
                                     const stdair::Date_T&,
                                     const stdair::DateTime_T&,
                                     const stdair::DayDuration_T&);

    /** Generate the value of time. */
    const stdair::PriceValue_T generateValueOfTime();
    
    /**
     * Generate the next request.
     *
     * @param stdair::RandomGeneration Random generator.
     * @param const bool Boolean describing the method used to generate the
     * date time of the next booking request: statistic order or exponenetial
     * law.
     * @return stdair::BookingRequestPtr_T Next request to be simulate.
     *
     */
    stdair::BookingRequestPtr_T generateNextRequest (stdair::RandomGeneration&,
                                                     const bool);

    /** Reset all the contexts of the demand stream. */
    void reset (stdair::BaseGenerator_T& ioSharedGenerator);
       

  public:
    // ////////////////// Display support methods //////////////
    /**
     * Dump a Business Object into an output stream.
     * @param ostream& the output stream.
     */
    void toStream (std::ostream& ioOut) const {
      ioOut << toString();
    }

    /**
     * Read a Business Object from an input stream.
     * @param istream& the input stream.
     */
    void fromStream (std::istream& ioIn) {
    }

    /**
     * Get the serialised version of the Business Object.
     */
    std::string toString() const;
    
    /**
     * Get a string describing the  key.
     */
    const std::string describeKey() const {
      return _key.toString();
    }

    /**
     * Dump recursively the content of the DemandStream object.
     */
    std::string display() const;

    const stdair::Duration_T convertFloatIntoDuration (const stdair::FloatDuration_T);
    
  protected:
    // ////////// Constructors and destructors /////////
    /**
     * Main constructor.
     */
    DemandStream (const Key_T&);
    /**
     * Destructor.
     */
    virtual ~DemandStream();

  private:
    /** Default constructor. */
    DemandStream();
    /** Copy constructor. */
    DemandStream (const DemandStream&);
    /** Initialisation. */
    void init (stdair::BaseGenerator_T& ioSharedGenerator);

    
  protected:
    // ////////// Attributes //////////
    /**
     * Primary key (string gathering the origin, destination, POS and date).
     */
    Key_T _key;
    
    /**
     * Pointer on the parent class (EventQueue).
     */
    BomAbstract* _parent;
    
    /**
     * Map holding the children (not used for now).
     */
    stdair::HolderMap_T _holderMap;
    
    /**
     * Demand characteristics.
     */
    DemandCharacteristics _demandCharacteristics;

    /**
     * Demand distribution.
     */
    DemandDistribution _demandDistribution;
    
    /**
     * Total number of requests to be generated.
     */
    stdair::NbOfRequests_T _totalNumberOfRequestsToBeGenerated;

    /**
     * Random generation context.
     */
    RandomGenerationContext _randomGenerationContext;
    
    /**
     * Random generator for request date-time.
     */
    stdair::RandomGeneration _requestDateTimeRandomGenerator;
    
    /**
     * Random generator for demand characteristics.
     */
    stdair::RandomGeneration _demandCharacteristicsRandomGenerator;

    /**
     * Defaut POS probablity mass, used when "row" (rest of the world)
     * is drawn.
     */
    POSProbabilityMass_T _posProMass;

  private:
    bool _stillHavingRequestsToBeGenerated;

    bool _firstDateTimeRequest;

    stdair::FloatDuration_T _dateTimeLastRequest;
  };

}
#endif // __TRADEMGEN_BOM_DEMANDSTREAM_HPP
