#ifndef __TRAVELCCM_TRAVELCCM_TYPES_HPP
#define __TRAVELCCM_TRAVELCCM_TYPES_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
#include <map>
// Boost (Extended STL)
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

namespace TRAVELCCM {

   // ///////// Exceptions ///////////
  class RootException : public std::exception {
  };

  class FileNotFoundException : public RootException {
  };
  
  class NonInitialisedServiceException : public RootException {
  };

  class MemoryAllocationException : public RootException {
  };

  class ObjectNotFoundException : public RootException {
  };

  class DocumentNotFoundException : public RootException {
  };


  // /////////////// Log /////////////
  /** Level of logs. */
  namespace LOG {
    typedef enum {
      CRITICAL = 0,
      ERROR,
      NOTIFICATION,
      WARNING,
      DEBUG,
      VERBOSE,
      LAST_VALUE
    } EN_LogLevel;
  }

  // //////// Type definitions /////////
  /** Define the Duration (e.g., elapsed in-flight time). */
  typedef boost::posix_time::time_duration Duration_T;

  /** Define the Date (e.g., flight-date departure date). */
  typedef boost::gregorian::date Date_T;
  
  /** Define an accurate time (date +time). */
  typedef boost::posix_time::ptime DateTime_T;

  /** Define an identity of a pattern. */
  typedef std::string PatternId_T;

  /** Define a pair of Duration_T. */
  typedef std::pair<Duration_T, Duration_T> DurationPair_T;

  /** Define a pair of DateTime_T. */
  typedef std::pair<DateTime_T, DateTime_T> DateTimePair_T;

  /** Define a map between a time of the day and a DurationPair_T. */
  typedef std::map<const Duration_T, const DurationPair_T> DepartureTimePreferencePattern_T;
    
}
#endif // __TRAVELCCM_TRAVELCCM_TYPES_HPP
