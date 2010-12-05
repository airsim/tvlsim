// //////////////////////////////////////////////////////////////////////
#ifndef __TRADEMGEN_BOM_DICTIONARYMANAGER_HPP
#define __TRADEMGEN_BOM_DICTIONARYMANAGER_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// StdAir
#include <stdair/stdair_maths_types.hpp>

namespace TRADEMGEN {
    /** Dictionary key. */
  typedef unsigned short DictionaryKey_T;
  
  /** Class wrapper of dictionary business methods. */
  class DictionaryManager {
  public:
    // //////////// Business methods /////////////////
    /** Convert from key to value. */
    static const stdair::Probability_T keyToValue (const DictionaryKey_T);
    
    /** Convert from value to key. */
    static const DictionaryKey_T valueToKey (const stdair::Probability_T);
  };
}
#endif // __TRADEMGEN_BOM_DICTIONARYMANAGER_HPP
