// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// StdAir
#include <stdair/basic/BasConst_General.hpp>
// TraDemGen
#include <trademgen/basic/BasConst_TRADEMGEN_Service.hpp>
#include <trademgen/basic/BasConst_DemandGeneration.hpp>

namespace TRADEMGEN {

  /** Default name for the TRADEMGEN_Service. */
  // const std::string DEFAULT_TRADEMGEN_SERVICE_NAME = "trademgen";
  
  /** Default PoS probability mass. */
  const POSProbabilityMassFunction_T DEFAULT_POS_PROBALILITY_MASS =
    DefaultMap::createPOSProbMass();

  /** Default PoS probability mass. */
  POSProbabilityMassFunction_T DefaultMap::createPOSProbMass() {
    POSProbabilityMassFunction_T oMap;
    // oMap["SIN"] = 0.44; oMap["HKG"] = 0.04; oMap["CGK"] = 0.04;
    // oMap["SYD"] = 0.04; oMap["BKK"] = 0.04; oMap["LHR"] = 0.03;
    // oMap["MEL"] = 0.03; oMap["KUL"] = 0.03; oMap["MNL"] = 0.03;
    // oMap["PVG"] = 0.03; oMap["PER"] = 0.02; oMap["BNE"] = 0.02;
    // oMap["NRT"] = 0.02; oMap["DPS"] = 0.02; oMap["SGN"] = 0.02;
    // oMap["PEN"] = 0.02; oMap["FRA"] = 0.02; oMap["PEK"] = 0.02;
    // oMap["HKT"] = 0.02; oMap["AKT"] = 0.02; oMap["SFO"] = 0.01;
    // oMap["ICN"] = 0.01; oMap["TPE"] = 0.01; oMap["row"] = 0.02;
    oMap["row"] = 1.0;
    return oMap;
  }

  /** Default FRAT5 pattern. */
  const FRAT5Pattern_T DEFAULT_FRAT5_PATTERN = DefaultMap::createFRAT5Pattern();

  /** Default FRAT5 pattern. */
  FRAT5Pattern_T DefaultMap::createFRAT5Pattern() {
    FRAT5Pattern_T oMap;    
    oMap[1.10] = 0.0;   oMap[1.40] = 0.812; oMap[1.45] = 0.833; 
    oMap[1.50] = 0.854; oMap[1.55] = 0.876; oMap[1.60] = 0.897;
    oMap[1.70] = 0.909; oMap[1.80] = 0.918; oMap[2.00] = 0.930; 
    oMap[2.30] = 0.939; oMap[2.60] = 0.952; oMap[3.00] = 0.961;
    oMap[3.30] = 0.973; oMap[3.40] = 0.982; oMap[3.44] = 0.988; 
    oMap[3.47] = 0.994; oMap[3.50] = 1.0; 
    return oMap;
  }

  /** Default MAX Advance Purchase. */
  const double DEFAULT_MAX_ADVANCE_PURCHASE = 330.0;

  /** Default base generator. */
  stdair::BaseGenerator_T DEFAULT_BASE_GENERATOR (stdair::DEFAULT_RANDOM_SEED);

  /** Default random uniform real distribution. */
  const stdair::UniformDistribution_T DEFAULT_UNIFORM_REAL_DISTRIBUTION;

  /** Default uniform variate generator. */
  stdair::UniformGenerator_T
  DEFAULT_UNIFORM_GENERATOR (DEFAULT_BASE_GENERATOR,
                             DEFAULT_UNIFORM_REAL_DISTRIBUTION);

}
