// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// TRADEMGEN
#include <trademgen/factory/FacDemandStream.hpp>

namespace TRADEMGEN {

  FacDemandStream* FacDemandStream::_instance = NULL;
  
  // //////////////////////////////////////////////////////////////////////
  FacDemandStream::~FacDemandStream() {
    clean ();
  }

  // ////////////////////////////////////////////////////////////////////
  void FacDemandStream::clean () {
    // Destroy all the objects
    for (DemandStreamPool_T::iterator itDemandStream = _pool.begin();
         itDemandStream != _pool.end(); ++itDemandStream) {
      DemandStream* currentDemandStream_ptr = *itDemandStream;
      assert (currentDemandStream_ptr != NULL);
      delete currentDemandStream_ptr; currentDemandStream_ptr = NULL;
    }

    // Empty the pool.
    _pool.clear();

    // Reset the static instance.
    _instance = NULL;
  }
  
  // ////////////////////////////////////////////////////////////////////
  FacDemandStream& FacDemandStream::instance () {
    if (_instance == NULL) {
      _instance = new FacDemandStream ();
      assert (_instance != NULL);

      stdair::FacSupervisor::instance().registerBomFactory (_instance);
    }
    return *_instance;
  }
  
  // ////////////////////////////////////////////////////////////////////
  DemandStream& FacDemandStream::
  create (const Key_T& iKey,
          const ArrivalPatternCumulativeDistribution_T& iArrivalPattern,
          const POSProbabilityMassFunction_T& iPOSProbMass,
          const ChannelProbabilityMassFunction_T& iChannelProbMass,
          const TripTypeProbabilityMassFunction_T& iTripTypeProbMass,
          const StayDurationProbabilityMassFunction_T& iStayDurationProbMass,
          const FrequentFlyerProbabilityMassFunction_T& iFrequentFlyerProbMass,
          const PreferredDepartureTimeContinuousDistribution_T& iPreferredDepartureTimeContinuousDistribution,
          const stdair::WTP_T& iMinWTP,
          const ValueOfTimeContinuousDistribution_T& iValueOfTimeContinuousDistribution,
          const DemandDistribution& iDemandDistribution,
          const stdair::RandomSeed_T& iNumberOfRequestsSeed,
          const stdair::RandomSeed_T& iRequestDateTimeSeed,
          const stdair::RandomSeed_T& iDemandCharacteristicsSeed,
          stdair::UniformGenerator_T& ioSharedGenerator,
          const POSProbabilityMass_T& iDefaultPOSProbablityMass) {
    DemandStream* oDemandStream_ptr =
      new DemandStream (iKey, iArrivalPattern, iPOSProbMass, iChannelProbMass,
                        iTripTypeProbMass, iStayDurationProbMass,
                        iFrequentFlyerProbMass,
                        iPreferredDepartureTimeContinuousDistribution,
                        iMinWTP, iValueOfTimeContinuousDistribution,
                        iDemandDistribution, iNumberOfRequestsSeed,
                        iRequestDateTimeSeed, iDemandCharacteristicsSeed,
                        ioSharedGenerator, iDefaultPOSProbablityMass);
    assert (oDemandStream_ptr != NULL);
    _pool.push_back (oDemandStream_ptr);
    return *oDemandStream_ptr;
  }
  
}
