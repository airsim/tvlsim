#ifndef __TRADEMGEN_BAS_BASCONST_DEMANDGENERATION_HPP
#define __TRADEMGEN_BAS_BASCONST_DEMANDGENERATION_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
#include <string>
// TRADEMGEN
#include <trademgen/basic/DemandCharacteristicTypes.hpp>

namespace TRADEMGEN {

  /** Default PoS probability mass. */
  extern const POSProbabilityMassFunction_T DEFAULT_POS_PROBALILITY_MASS;
  struct DefaultMap {
    static POSProbabilityMassFunction_T create() {
      POSProbabilityMassFunction_T oMap;
      oMap["SIN"] = 0.44; oMap["HKG"] = 0.04; oMap["CGK"] = 0.04;
      oMap["SYD"] = 0.04; oMap["BKK"] = 0.04; oMap["LHR"] = 0.03;
      oMap["MEL"] = 0.03; oMap["KUL"] = 0.03; oMap["MNL"] = 0.03;
      oMap["PVG"] = 0.03; oMap["PER"] = 0.02; oMap["BNE"] = 0.02;
      oMap["NRT"] = 0.02; oMap["DPS"] = 0.02; oMap["SGN"] = 0.02;
      oMap["PEN"] = 0.02; oMap["FRA"] = 0.02; oMap["PEK"] = 0.02;
      oMap["HKT"] = 0.02; oMap["AKT"] = 0.02; oMap["SFO"] = 0.01;
      oMap["ICN"] = 0.01; oMap["TPE"] = 0.01; oMap["row"] = 0.02;
      return oMap;
    }
  };
  
}
#endif // __TRADEMGEN_BAS_BASCONST_DEMANDGENERATION_HPP
