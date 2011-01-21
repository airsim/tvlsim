// //////////////////////////////////////////////////////////////////////
#ifndef __SIMFQT_BOM_FARERULEFEATURESTYPES_HPP
#define __SIMFQT_BOM_FARERULEFEATURESTYPES_HPP

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
  class FareRuleFeatures;
  
  /** Define the fare-rule list. */
  typedef std::list<FareRuleFeatures*> FareRuleFeaturesList_T;

  /** Define the fare-rule map. */
  typedef std::map<const stdair::MapKey_T, FareRuleFeatures*> FareRuleFeaturesMap_T;

  /** Define the list of pair<MapKey_T, FareRule>. */
  typedef std::pair<stdair::MapKey_T, FareRuleFeatures*> FareRuleFeaturesWithKey_T;
  typedef std::list<FareRuleFeaturesWithKey_T> FareRuleFeaturesDetailedList_T;
}
#endif // __SIMFQT_BOM_FARERULEFEATURESTYPES_HPP

