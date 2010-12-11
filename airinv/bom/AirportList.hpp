#ifndef __AIRINV_BOM_AIRPORTLIST_HPP
#define __AIRINV_BOM_AIRPORTLIST_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <set>
#include <vector>
// STDAIR
#include <stdair/stdair_basic_types.hpp>

namespace AIRINV {

  /** Define lists of Airport Codes. */
  typedef std::set<stdair::AirportCode_T> AirportList_T;
  typedef std::vector<stdair::AirportCode_T> AirportOrderedList_T;

}
#endif // __AIRINV_BOM_AIRPORTLIST_HPP
