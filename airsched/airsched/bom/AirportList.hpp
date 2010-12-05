#ifndef __AIRSCHED_BOM_AIRPORTLIST_HPP
#define __AIRSCHED_BOM_AIRPORTLIST_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <set>
#include <vector>
// StdAir
#include <stdair/stdair_basic_types.hpp>

namespace AIRSCHED {

  /** Define lists of Airport Codes. */
  typedef std::set<stdair::AirportCode_T> AirportList_T;
  typedef std::vector<stdair::AirportCode_T> AirportOrderedList_T;

}
#endif // __AIRSCHED_BOM_AIRPORTLIST_HPP
