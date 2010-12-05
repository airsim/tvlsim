// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// StdAir
#include <stdair/stdair_basic_types.hpp>
#include <stdair/basic/BasConst_BookingClass.hpp>
#include <stdair/basic/BasConst_Yield.hpp>
#include <stdair/basic/BasConst_General.hpp>
#include <stdair/bom/BomRoot.hpp>
#include <stdair/command/CmdBomManager.hpp>
#include <stdair/service/Logger.hpp>
// TraDemGen
#include <trademgen/basic/DemandCharacteristics.hpp>
#include <trademgen/basic/DemandDistribution.hpp>
#include <trademgen/bom/DemandStruct.hpp>
#include <trademgen/factory/FacDemandStream.hpp>
#include <trademgen/command/DemandManager.hpp>
#include <trademgen/command/DemandGenerator.hpp>

namespace TRADEMGEN {

  // //////////////////////////////////////////////////////////////////////
  void DemandGenerator::
  createDemandCharacteristics (stdair::BomRoot& ioBomRoot,
                               stdair::UniformGenerator_T& ioSharedGenerator,
                               const POSProbabilityMass_T& iPOSProbMass,
                               const DemandStruct& iDemand) {
    
    const DemandStreamKey lDemandStreamKey (iDemand._origin,
                                            iDemand._destination,
                                            iDemand._prefDepDate,
                                            iDemand._prefCabin);
    // DEBUG
    // STDAIR_LOG_DEBUG ("Demand stream key: " << lDemandStreamKey.describe());
    
    const DemandDistribution lDemandDistribution (iDemand._demandMean,
                                                  iDemand._demandStdDev);
    
    // Seed
    stdair::RandomSeed_T lNumberOfRequestsSeed =
      generateSeed (ioSharedGenerator);
    stdair::RandomSeed_T lRequestDateTimeSeed = generateSeed (ioSharedGenerator);
    stdair::RandomSeed_T lDemandCharacteristicsSeed =
      generateSeed (ioSharedGenerator);
  
    // Delegate the call to the dedicated command
    DemandManager::addDemandStream(ioBomRoot, lDemandStreamKey,
                                   iDemand._dtdProbDist, iDemand._posProbDist,
                                   iDemand._channelProbDist,
                                   iDemand._tripProbDist,
                                   iDemand._stayProbDist, iDemand._ffProbDist,
                                   iDemand._prefDepTimeProbDist,
                                   iDemand._minWTP,
                                   iDemand._timeValueProbDist,
                                   lDemandDistribution,
                                   lNumberOfRequestsSeed, lRequestDateTimeSeed,
                                   lDemandCharacteristicsSeed,
                                   ioSharedGenerator, iPOSProbMass);
  }    

  // ////////////////////////////////////////////////////////////////////
  stdair::RandomSeed_T DemandGenerator::
  generateSeed (stdair::UniformGenerator_T& ioSharedGenerator) {
    stdair::RealNumber_T lVariateUnif = ioSharedGenerator() * 1e9;
    stdair::RandomSeed_T oSeed = static_cast<stdair::RandomSeed_T>(lVariateUnif);
    return oSeed;
  }
  
}
