#ifndef __AIRSCHED_BOM_ONDPERIODSTRUCT_HPP
#define __AIRSCHED_BOM_ONDPERIODSTRUCT_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
// StdAir
#include <stdair/stdair_inventory_types.hpp>
#include <stdair/basic/StructAbstract.hpp>

namespace AIRSCHED {
  /** Utility Structure for the parsing of FareRule structures. */
  struct OnDPeriodStruct : public stdair::StructAbstract {
  public:
    // /////////// Getters //////////////      
    /** Get the first airline code. */
    const stdair::AirlineCode_T& getFirstAirlineCode () const;

    /** Get the date from the staging details. */
    stdair::Date_T getDate() const;

    /** Get the time from the staging details. */
    stdair::Duration_T getTime() const;
      
    // ///////// Display Methods //////////
    /** Give a description of the structure (for display purposes). */
    const std::string describe() const;

    /** Give a short description of the key required in the
        travel solution object to differentiate fare rule structures. */
    const std::string describeTSKey() const;
      
  public:
    /** Default constructor. */
    OnDPeriodStruct ();
      
  public:
    // Attributes
    stdair::AirportCode_T _origin;
    stdair::AirportCode_T _destination;
    stdair::DatePeriod_T _datePeriod;
    stdair::Duration_T _timeRangeStart;
    stdair::Duration_T _timeRangeEnd;
    stdair::NbOfAirlines_T _nbOfAirlines;
    stdair::AirlineCode_T _airlineCode;
    stdair::ClassCode_T _classCode; 
    stdair::AirlineCodeList_T _airlineCodeList;
    stdair::ClassCodeList_T _classCodeList;

    /** Staging Date. */
    stdair::Date_T _dateRangeStart;
    stdair::Date_T _dateRangeEnd;
    unsigned int _itYear;
    unsigned int _itMonth;
    unsigned int _itDay;

    /** Staging Time. */
    long _itHours;
    long _itMinutes;
    long _itSeconds;
  };
}
#endif // __AIRSCHED_BOM_ONDPERIODSTRUCT_HPP
