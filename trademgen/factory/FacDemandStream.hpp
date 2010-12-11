#ifndef __TRADEMGEN_FAC_FACDEMANDSTREAM_HPP
#define __TRADEMGEN_FAC_FACDEMANDSTREAM_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <string>
// StdAir
#include <stdair/stdair_basic_types.hpp>
#include <stdair/factory/FacAbstract.hpp>
#include <stdair/service/FacSupervisor.hpp>
#include <stdair/service/Logger.hpp>
// TRADEMGEN
#include <trademgen/bom/DemandStream.hpp>

namespace TRADEMGEN {  

  /** Base class for Factory layer. */
  class FacDemandStream : public stdair::FacAbstract {
    // Internal type definitions.
    typedef std::list<DemandStream*> DemandStreamPool_T;
    typedef DemandStream::Key_T Key_T;

  public:
    // ///////////// Business methods. ////////////
    /** Provide the unique instance.
        <br>The singleton is instantiated when first used.
        @return FacTrademgen& */
    static FacDemandStream& instance();

    /** Create a DemandStream object. */
    DemandStream& create (const Key_T&,
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
                          const stdair::RandomSeed_T& iDemandCharacteristicSeed,
                          stdair::UniformGenerator_T&,
                          const POSProbabilityMass_T&);
    
  protected:
    /** Default Constructor.
        <br>This constructor is protected to ensure the class is . */
    FacDemandStream() {}

  public:
    /** Destructor. */
    ~FacDemandStream();
    /** Destroyed all the object instantiated by this factory. */
    void clean();
    
  private:
    /** The unique instance.*/
    static FacDemandStream* _instance;
    /** List of instantiated Business Objects*/
    DemandStreamPool_T _pool;
  };
}
#endif // __TRADEMGEN_FAC_FACDEMANDSTREAM_HPP
