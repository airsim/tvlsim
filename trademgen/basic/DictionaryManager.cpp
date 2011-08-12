// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// TRADEMGEN
#include <trademgen/basic/DictionaryManager.hpp>

namespace TRADEMGEN {
  // ////////////////////////////////////////////////////////////////////
  const stdair::Probability_T DictionaryManager::
  keyToValue (const DictionaryKey_T iKey) {
    //    return static_cast<stdair::Probability_T>(iKey) / 1000;
    return iKey;
  }

  // ////////////////////////////////////////////////////////////////////
  const DictionaryKey_T DictionaryManager::
  valueToKey (const stdair::Probability_T iValue) {
    //    return iValue * 1000;
    return iValue;
  }
}
