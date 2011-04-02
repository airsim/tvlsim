#ifndef __AIRSCHED_BOM_FLIGHTPERIODSTRUCT_HPP
#define __AIRSCHED_BOM_FLIGHTPERIODSTRUCT_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
// StdAir
#include <stdair/stdair_basic_types.hpp>
#include <stdair/basic/StructAbstract.hpp>
#include <stdair/bom/DoWStruct.hpp>
// AirSched
#include <airsched/bom/LegCabinStruct.hpp>
#include <airsched/bom/LegStruct.hpp>
#include <airsched/bom/SegmentStruct.hpp>
#include <airsched/bom/SegmentCabinStruct.hpp>
#include <airsched/bom/FareFamilyStruct.hpp>
#include <airsched/bom/AirportList.hpp>

namespace AIRSCHED {

  /**
   * Utility Structure for the parsing of Flight-Period structures.
   */
  struct FlightPeriodStruct : public stdair::StructAbstract {
    
    /** Set the date from the staging details. */
    stdair::Date_T getDate() const;

    /** Set the time from the staging details. */
    stdair::Duration_T getTime() const;
  
    /** Give a description of the structure (for display purposes). */
    const std::string describe() const;

    /** Add the given airport to the internal lists (if not already
        existing). */
    void addAirport (const stdair::AirportCode_T&);

    /** Build the list of SegmentStruct objects. */
    void buildSegments();
      
    /** Add, to the Segment structure whose key corresponds to the
        given (board point, off point) pair, the specific segment cabin
        details (mainly, the list of the class codes).
        <br>Note that the Segment structure is retrieved from the internal
        list, already filled by a previous step (the buildSegments()
        method). */
    void addSegmentCabin (const SegmentStruct&,
                          const SegmentCabinStruct&);
      
    /** Add, to all the Segment structures, the general segment cabin details
        (mainly, the list of the class codes).
        <br>Note that the Segment structures are stored within the internal
        list, already filled by a previous step (the buildSegments()
        method). */
    void addSegmentCabin (const SegmentCabinStruct&);

    /** Add, to the SegmentCabin structure whose key corresponds to the
        given cabin code, the specific segment fare family details
        (mainly, the list of the class codes).
        <br>Note that the SegmentCabin structure is retrieved from the internal
        list, already filled by a previous step (the buildSegmentCabins()
        method). */
    void addFareFamily (const SegmentStruct&,
                        const SegmentCabinStruct&,
                        const FareFamilyStruct&);
      
    /** Add, to all the Segment structures, the general fare family sets
        (list of fare families).
        <br>Note that the SegmentCabin structures are stored within the internal
        list, already filled by a previous step (the buildSegmentCabins()
        method). */
    void addFareFamily (const SegmentCabinStruct&,
                        const FareFamilyStruct&);
      
    /**
     * Constructor.
     */
    FlightPeriodStruct();

    // Attributes
    stdair::AirlineCode_T _airlineCode;
    stdair::FlightNumber_T _flightNumber;
    stdair::DatePeriod_T _dateRange;
    stdair::DoWStruct _dow;
    LegStructList_T _legList;
    SegmentStructList_T _segmentList;

    /** Staging Leg (resp. Cabin) structure, gathering the result of
        the iteration on one leg (resp. cabin). */
    bool _legAlreadyDefined;
    LegStruct _itLeg;
    LegCabinStruct _itLegCabin;

    /** Staging Date. */
    stdair::Date_T _dateRangeStart;
    stdair::Date_T _dateRangeEnd;
    unsigned int _itYear;
    unsigned int _itMonth;
    unsigned int _itDay;
    int _dateOffset;
      
    /** Staging Time. */
    long _itHours;
    long _itMinutes;
    long _itSeconds;

    /** Staging Airport List (helper to derive the list of Segment
        structures). */
    AirportList_T _airportList;
    AirportOrderedList_T _airportOrderedList;

    /** Staging Segment-related attributes. */
    bool _areSegmentDefinitionsSpecific;
    SegmentStruct _itSegment;
    SegmentCabinStruct _itSegmentCabin;
  };

}
#endif // __AIRSCHED_BOM_FLIGHTPERIODSTRUCT_HPP
