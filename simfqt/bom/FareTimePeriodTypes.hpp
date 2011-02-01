// //////////////////////////////////////////////////////////////////////
#ifndef __SIMFQT_BOM_FARETIMEPERIODTYPES_HPP
#define __SIMFQT_BOM_FARETIMEPERIODTYPES_HPP

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
  class FareTimePeriod;
  
  /** Define the date-period list. */
  typedef std::list<FareTimePeriod*> FareTimePeriodList_T;

  /** Define the date-period  map. */
  typedef std::map<const stdair::MapKey_T, FareTimePeriod*> FareTimePeriodMap_T;

  /** Define the list of pair<MapKey_T, FareTimePeriod>. */
  typedef std::pair<stdair::MapKey_T, FareTimePeriod*> FareTimePeriodWithKey_T;
  typedef std::list<FareTimePeriodWithKey_T> FareTimePeriodDetailedList_T;
}
#endif // __SIMFQT_BOM_FARETIMEPERIODTYPES_HPP

