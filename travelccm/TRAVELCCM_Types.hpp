#ifndef __TRAVELCCM_TRAVELCCM_TYPES_HPP
#define __TRAVELCCM_TRAVELCCM_TYPES_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
#include <map>
// StdAir
#include <stdair/stdair_exceptions.hpp>
#include <stdair/stdair_date_time_types.hpp>

namespace TRAVELCCM {

  // ///////// Exceptions ///////////
  class CustomerChoiceException : public stdair::RootException {
  };

  // //////// Type definitions specific to SimFQT /////////
  /** Define an identity of a pattern. */
  typedef std::string PatternId_T;

  /** Define a pair of Duration_T. */
  typedef std::pair<stdair::Duration_T, stdair::Duration_T> DurationPair_T;

  /** Define a pair of DateTime_T. */
  typedef std::pair<stdair::DateTime_T, stdair::DateTime_T> DateTimePair_T;

  /** Define a map between a time of the day and a DurationPair_T. */
  typedef std::map<const stdair::Duration_T,
                   const DurationPair_T> DepartureTimePreferencePattern_T;
    
}
#endif // __TRAVELCCM_TRAVELCCM_TYPES_HPP
