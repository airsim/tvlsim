// //////////////////////////////////////////////////////////////////////
#ifndef __SIMFQT_BOM_FAREDATEPERIODTYPES_HPP
#define __SIMFQT_BOM_FAREDATEPERIODTYPES_HPP

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
  class FareDatePeriod;
  
  /** Define the date-period list. */
  typedef std::list<FareDatePeriod*> FareDatePeriodList_T;

  /** Define the date-period  map. */
  typedef std::map<const stdair::MapKey_T, FareDatePeriod*> FareDatePeriodMap_T;

  /** Define the list of pair<MapKey_T, FareDatePeriod>. */
  typedef std::pair<stdair::MapKey_T, FareDatePeriod*> FareDatePeriodWithKey_T;
  typedef std::list<FareDatePeriodWithKey_T> FareDatePeriodDetailedList_T;
}
#endif // __SIMFQT_BOM_FAREDATEPERIODTYPES_HPP

