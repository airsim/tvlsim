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

  /** Default last lower bound of daily rate interval in arrival pattern. */ 
  const stdair::FloatDuration_T DEFAULT_LAST_LOWER_BOUND_ARRIVAL_PATTERN = -1;

  /** Default FRAT5 pattern. */
  const FRAT5Pattern_T DEFAULT_FRAT5_PATTERN = DefaultMap::createFRAT5Pattern();

  /** Default FRAT5 pattern. */
  FRAT5Pattern_T DefaultMap::createFRAT5Pattern() {
    FRAT5Pattern_T oMap;    
    // oMap[1.10] = 0.0;     oMap[1.40] = 0.80909; oMap[1.45] = 0.8303; 
    // oMap[1.50] = 0.85152; oMap[1.55] = 0.87273; oMap[1.60] = 0.89394;
    // oMap[1.70] = 0.90606; oMap[1.80] = 0.91818; oMap[2.00] = 0.9303; 
    // oMap[2.30] = 0.94242; oMap[2.60] = 0.95152; oMap[3.00] = 0.96061;
    // oMap[3.30] = 0.96970; oMap[3.40] = 0.97879; oMap[3.44] = 0.98485; 
    // oMap[3.47] = 0.99091; oMap[3.50] = 0.99697; oMap[3.500000001] = 1.0;
    // oMap[1.10] = -365;oMap[1.40] = -63; oMap[1.45] = -56; 
    // oMap[1.50] = -49; oMap[1.55] = -42; oMap[1.60] = -35;
    // oMap[1.70] = -31; oMap[1.80] = -27; oMap[2.00] = -23; 
    // oMap[2.30] = -19; oMap[2.60] = -16; oMap[3.00] = -13;
    // oMap[3.30] = -10; oMap[3.40] = -7;  oMap[3.44] = -5; 
    // oMap[3.47] = -3;  oMap[3.50] = -1;  oMap[3.500000001] = 0;
    // oMap[1.0] = -365; oMap[1.10] = -63; oMap[1.13] = -56; 
    // oMap[1.17] = -49; oMap[1.22] = -42; oMap[1.28] = -35;
    // oMap[1.32] = -31; oMap[1.37] = -27; oMap[1.43] = -23; 
    // oMap[1.51] = -19; oMap[1.60] = -16; oMap[1.70] = -13;
    // oMap[1.80] = -10; oMap[1.90] = -7;  oMap[1.93] = -5; 
    // oMap[1.96] = -3;  oMap[2.00] = -1;  oMap[2.00000001] = 0;
    // oMap[1.0] = -365; oMap[1.05] = -63; oMap[1.07] = -56; 
    // oMap[1.09] = -49; oMap[1.11] = -42; oMap[1.14] = -35;
    // oMap[1.16] = -31; oMap[1.18] = -27; oMap[1.21] = -23; 
    // oMap[1.24] = -19; oMap[1.27] = -16; oMap[1.30] = -13;
    // oMap[1.33] = -10; oMap[1.37] = -7;  oMap[1.40] = -5; 
    // oMap[1.45] = -3;  oMap[1.50] = -1;  oMap[1.500000001] = 0;
    oMap[1.10] = -365;oMap[1.40] = -63; 
    oMap[1.50] = -49; oMap[1.60] = -35; oMap[2.00] = -23; 
    oMap[2.60] = -16; oMap[3.30] = -10; oMap[3.44] = -5; 
    oMap[3.50] = -1;  oMap[3.500000001] = 0;
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
