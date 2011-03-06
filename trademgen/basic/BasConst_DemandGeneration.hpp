#ifndef __TRADEMGEN_BAS_BASCONST_DEMANDGENERATION_HPP
#define __TRADEMGEN_BAS_BASCONST_DEMANDGENERATION_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
// StdAir
#include <stdair/stdair_maths_types.hpp>
// TraDemGen
#include <trademgen/basic/DemandCharacteristicsTypes.hpp>

namespace TRADEMGEN {

  /** Default PoS probability mass. */
  extern const POSProbabilityMassFunction_T DEFAULT_POS_PROBALILITY_MASS;

  /** Default PoS probability mass. */
  struct DefaultMap {
    static POSProbabilityMassFunction_T create();
  };

  /** Default uniform generator. */
  extern stdair::UniformGenerator_T DEFAULT_UNIFORM_GENERATOR;

  /** Default base generator. */
  extern stdair::BaseGenerator_T DEFAULT_BASE_GENERATOR;

  /** Default random uniform real distribution. */
  extern const boost::uniform_01<> DEFAULT_UNIFORM_REAL_DISTRIBUTION;

}
#endif // __TRADEMGEN_BAS_BASCONST_DEMANDGENERATION_HPP
