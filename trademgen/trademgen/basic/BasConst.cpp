// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
#include <trademgen/basic/BasConst_TRADEMGEN_Service.hpp>
#include <trademgen/basic/BasConst_DemandGeneration.hpp>

namespace TRADEMGEN {

  /** Default name for the TRADEMGEN_Service. */
  // const std::string DEFAULT_TRADEMGEN_SERVICE_NAME = "myname";
  
  /** Default PoS probability mass. */
  const POSProbabilityMassFunction_T DEFAULT_POS_PROBALILITY_MASS =
    DefaultMap::create();

}
