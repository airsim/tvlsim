#ifndef __AIRINV_BOM_FLIGHTDATESTRUCT_HPP
#define __AIRINV_BOM_FLIGHTDATESTRUCT_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
// StdAir
#include <stdair/stdair_inventory_types.hpp>
#include <stdair/basic/StructAbstract.hpp>
#include <stdair/bom/DoWStruct.hpp>
// AirInv
#include <airinv/basic/FlightTypeCode.hpp>
#include <airinv/basic/FlightVisibilityCode.hpp>
#include <airinv/bom/LegStruct.hpp>
#include <airinv/bom/LegCabinStruct.hpp>
#include <airinv/bom/BucketStruct.hpp>
#include <airinv/bom/SegmentStruct.hpp>
#include <airinv/bom/SegmentCabinStruct.hpp>
#include <airinv/bom/FareFamilyStruct.hpp>
#include <airinv/bom/AirportList.hpp>

namespace AIRINV {

  /** Utility Structure for the parsing of Flight-Date structures. */
  struct FlightDateStruct : public stdair::StructAbstract {
    
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
    void buildSegments ();
      
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
    void addFareFamily (const SegmentStruct&, const SegmentCabinStruct&,
                        const FareFamilyStruct&);
      
    /** Add, to all the Segment structures, the general fare family sets
        (list of fare families).
        <br>Note that the SegmentCabin structures are stored within the internal
        list, already filled by a previous step (the buildSegmentCabins()
        method). */
    void addFareFamily (const SegmentCabinStruct&, const FareFamilyStruct&);
      
    /** Constructor. */
    FlightDateStruct ();

    // Attributes
    stdair::AirlineCode_T _airlineCode;
    stdair::FlightNumber_T _flightNumber;
    stdair::Date_T _flightDate;
    FlightTypeCode _flightTypeCode;
    FlightVisibilityCode _flightVisibilityCode;
    LegStructList_T _legList;
    SegmentStructList_T _segmentList;

    /** Staging Date. */
    unsigned int _itYear;
    unsigned int _itMonth;
    unsigned int _itDay;
    int _dateOffSet;
      
    /** Staging Time. */
    long _itHours;
    long _itMinutes;
    long _itSeconds;

    /** Staging Airport List (helper to derive the list of Segment
        structures). */
    AirportList_T _airportList;
    AirportOrderedList_T _airportOrderedList;

    /** Staging Leg (resp. Cabin) structure, gathering the result of
        the iteration on one leg (resp. cabin). */
    bool _legAlreadyDefined;
    LegStruct _itLeg;
    LegCabinStruct _itLegCabin;
    BucketStruct _itBucket;

    /** Staging Segment-related attributes. */
    bool _areSegmentDefinitionsSpecific;
    SegmentStruct _itSegment;
    SegmentCabinStruct _itSegmentCabin;
    BookingClassStruct _itBookingClass;
  };

}
#endif // __AIRINV_BOM_FLIGHTDATESTRUCT_HPP
