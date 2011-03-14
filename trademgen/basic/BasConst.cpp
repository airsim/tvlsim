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
    DefaultMap::create();

  /** Default PoS probability mass. */
  POSProbabilityMassFunction_T DefaultMap::create() {
    POSProbabilityMassFunction_T oMap;
    // oMap["SIN"] = 0.44; oMap["HKG"] = 0.04; oMap["CGK"] = 0.04;
    // oMap["SYD"] = 0.04; oMap["BKK"] = 0.04; oMap["LHR"] = 0.03;
    // oMap["MEL"] = 0.03; oMap["KUL"] = 0.03; oMap["MNL"] = 0.03;
    // oMap["PVG"] = 0.03; oMap["PER"] = 0.02; oMap["BNE"] = 0.02;
    // oMap["NRT"] = 0.02; oMap["DPS"] = 0.02; oMap["SGN"] = 0.02;
    // oMap["PEN"] = 0.02; oMap["FRA"] = 0.02; oMap["PEK"] = 0.02;
    // oMap["HKT"] = 0.02; oMap["AKT"] = 0.02; oMap["SFO"] = 0.01;
    // oMap["ICN"] = 0.01; oMap["TPE"] = 0.01; oMap["row"] = 0.02;
    oMap["LHR"] = 0.60; oMap["BKK"] = 0.05; oMap["SYD"] = 0.10;
    oMap["CDG"] = 0.10; oMap["JFK"] = 0.10;
    oMap["row"] = 0.05;
    return oMap;
  }

  /** Default base generator. */
  stdair::BaseGenerator_T DEFAULT_BASE_GENERATOR (stdair::DEFAULT_RANDOM_SEED);

  /** Default random uniform real distribution. */
  const stdair::UniformDistribution_T DEFAULT_UNIFORM_REAL_DISTRIBUTION;

  /** Default uniform variate generator. */
  stdair::UniformGenerator_T
  DEFAULT_UNIFORM_GENERATOR (DEFAULT_BASE_GENERATOR,
                             DEFAULT_UNIFORM_REAL_DISTRIBUTION);

}
