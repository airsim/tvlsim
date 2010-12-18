// //////////////////////////////////////////////////////////////////////
#ifndef __SIMFQT_BOM_FARERULETYPES_HPP
#define __SIMFQT_BOM_FARERULETYPES_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <map>
#include <list>
// STDAIR
#include <stdair/bom/key_types.hpp>

namespace SIMFQT {

  // Forward declarations.
  class FareRule;
  
  /** Define the fare-rule list. */
  typedef std::list<FareRule*> FareRuleList_T;

  /** Define the fare-rule map. */
  typedef std::map<const stdair::MapKey_T, FareRule*> FareRuleMap_T;

  /** Define the list of pair<MapKey_T, FareRule>. */
  typedef std::pair<stdair::MapKey_T, FareRule*> FareRuleWithKey_T;
  typedef std::list<FareRuleWithKey_T> FareRuleDetailedList_T;
}
#endif // __SIMFQT_BOM_FARERULETYPES_HPP
