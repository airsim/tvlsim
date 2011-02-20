#ifndef __AIRSCHED_CMD_SCHEDULEPARSERHELPER_HPP
#define __AIRSCHED_CMD_SCHEDULEPARSERHELPER_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
// #define BOOST_SPIRIT_DEBUG
// StdAir
#include <stdair/command/CmdAbstract.hpp>
// AirSched
#include <airsched/AIRSCHED_Types.hpp>
#include <airsched/basic/BasParserTypes.hpp>
#include <airsched/bom/FlightPeriodStruct.hpp>

// Forward declarations
namespace stdair {
  class BomRoot;
}

namespace AIRSCHED {

  namespace ScheduleParserHelper {
    
    // ///////////////////////////////////////////////////////////////////
    //  Semantic actions
    // ///////////////////////////////////////////////////////////////////
    /** Generic Semantic Action (Actor / Functor) for the Schedule Parser. */
    struct ParserSemanticAction {
      /** Actor Constructor. */
      ParserSemanticAction (FlightPeriodStruct&);
      /** Actor Context. */
      FlightPeriodStruct& _flightPeriod;
    };
      
    /** Store the parsed airline code. */
    struct storeAirlineCode : public ParserSemanticAction {
      /** Actor Constructor. */
      storeAirlineCode (FlightPeriodStruct&);
      /** Actor Function (functor). */
      void operator() (iterator_t iStr, iterator_t iStrEnd) const;
    };
  
    /** Store the parsed flight number. */
    struct storeFlightNumber : public ParserSemanticAction {
      /** Actor Constructor. */
      storeFlightNumber (FlightPeriodStruct&);
      /** Actor Function (functor). */
      void operator() (unsigned int iNumber) const;
    };
  
    /** Store the start of the date range. */
    struct storeDateRangeStart : public ParserSemanticAction {
      /** Actor Constructor. */
      storeDateRangeStart (FlightPeriodStruct&);
      /** Actor Function (functor). */
      void operator() (iterator_t iStr, iterator_t iStrEnd) const;
    };

    /** Store the end of the date range. */
    struct storeDateRangeEnd : public ParserSemanticAction {
      /** Actor Constructor. */
      storeDateRangeEnd (FlightPeriodStruct&);
      /** Actor Function (functor). */
      void operator() (iterator_t iStr, iterator_t iStrEnd) const;
    };

    /** Store the DOW (day of the Week). */
    struct storeDow : public ParserSemanticAction {
      /** Actor Constructor. */
      storeDow (FlightPeriodStruct&);
      /** Actor Function (functor). */
      void operator() (iterator_t iStr, iterator_t iStrEnd) const;
    };

    /** Store the parsed leg boarding point. */
    struct storeLegBoardingPoint : public ParserSemanticAction {
      /** Actor Constructor. */
      storeLegBoardingPoint (FlightPeriodStruct&);
      /** Actor Function (functor). */
      void operator() (iterator_t iStr, iterator_t iStrEnd) const;
    };
  
    /** Store the parsed leg off point. */
    struct storeLegOffPoint : public ParserSemanticAction {
      /** Actor Constructor. */
      storeLegOffPoint (FlightPeriodStruct&);
      /** Actor Function (functor). */
      void operator() (iterator_t iStr, iterator_t iStrEnd) const;
    };

    /** Store the boarding time. */
    struct storeBoardingTime : public ParserSemanticAction {
      /** Actor Constructor. */
      storeBoardingTime (FlightPeriodStruct&);
      /** Actor Function (functor). */
      void operator() (iterator_t iStr, iterator_t iStrEnd) const;
    };

    /** Store the off time. */
    struct storeOffTime : public ParserSemanticAction {
      /** Actor Constructor. */
      storeOffTime (FlightPeriodStruct&);
      /** Actor Function (functor). */
      void operator() (iterator_t iStr, iterator_t iStrEnd) const;
    };

    /** Store the elapsed time. */
    struct storeElapsedTime : public ParserSemanticAction {
      /** Actor Constructor. */
      storeElapsedTime (FlightPeriodStruct&);
      /** Actor Function (functor). */
      void operator() (iterator_t iStr, iterator_t iStrEnd) const;
    };

    /** Store the parsed leg cabin code. */
    struct storeLegCabinCode : public ParserSemanticAction {
      /** Actor Constructor. */
      storeLegCabinCode (FlightPeriodStruct&);
      /** Actor Function (functor). */
      void operator() (char iChar) const;
    };
  
    /** Store the parsed capacity. */
    struct storeCapacity : public ParserSemanticAction {
      /** Actor Constructor. */
      storeCapacity (FlightPeriodStruct&);
      /** Actor Function (functor). */
      void operator() (double iReal) const;
    };
  
    /** Store whether or not the segment definitions are specific.
        Specific means that there is a definition for each segment.
        General (not specific) means that a single definition defines
        all the segments. */
    struct storeSegmentSpecificity : public ParserSemanticAction {
      /** Actor Constructor. */
      storeSegmentSpecificity (FlightPeriodStruct&);
      /** Actor Function (functor). */
      void operator() (char iChar) const;
    };
  
    /** Store the parsed segment boarding point. */
    struct storeSegmentBoardingPoint : public ParserSemanticAction {
      /** Actor Constructor. */
      storeSegmentBoardingPoint (FlightPeriodStruct&);
      /** Actor Function (functor). */
      void operator() (iterator_t iStr, iterator_t iStrEnd) const;
    };
  
    /** Store the parsed segment off point. */
    struct storeSegmentOffPoint : public ParserSemanticAction {
      /** Actor Constructor. */
      storeSegmentOffPoint (FlightPeriodStruct&);
      /** Actor Function (functor). */
      void operator() (iterator_t iStr, iterator_t iStrEnd) const;
    };

    /** Store the parsed segment cabin code. */
    struct storeSegmentCabinCode : public ParserSemanticAction {
      /** Actor Constructor. */
      storeSegmentCabinCode (FlightPeriodStruct&);
      /** Actor Function (functor). */
      void operator() (char iChar) const;
    };
  
    /** Store the parsed list of class codes. */
    struct storeClasses : public ParserSemanticAction {
      /** Actor Constructor. */
      storeClasses (FlightPeriodStruct&);
      /** Actor Function (functor). */
      void operator() (iterator_t iStr, iterator_t iStrEnd) const;
    };

    /** Store the parsed family code. */
    struct storeFamilyCode : public ParserSemanticAction {
      /** Actor Constructor. */
      storeFamilyCode (FlightPeriodStruct&);
      /** Actor Function (functor). */
      void operator() (int iCode) const;
    };

    /** Store the parsed list of class codes (for families). */
    struct storeFClasses : public ParserSemanticAction {
      /** Actor Constructor. */
      storeFClasses (FlightPeriodStruct&);
      /** Actor Function (functor). */
      void operator() (iterator_t iStr, iterator_t iStrEnd) const;
    };
      
    /** Mark the end of the flight-period parsing. */
    struct doEndFlight : public ParserSemanticAction {
      /** Actor Constructor. */
      doEndFlight (stdair::BomRoot&, FlightPeriodStruct&);
      /** Actor Function (functor). */
      void operator() (iterator_t iStr, iterator_t iStrEnd) const;
      /** Actor Specific Context. */
      stdair::BomRoot& _bomRoot;
    };
  

    /////////////////////////////////////////////////////////////////////////
    //
    //  (Boost Spirit) Grammar Definition
    //
    /////////////////////////////////////////////////////////////////////////
    /**
       AirlineCode; FlightNumber; DateRangeStart; DateRangeEnd; DOW;
       (list) BoardingPoint; OffPoint; BoardingTime; DateOffset; OffTime;
       ElapsedTime;
       (list) CabinCode; Capacity;
       SegmentSpecificty (0 or 1);
       (list) (optional BoardingPoint; OffPoint); CabinCode; Classes
       BA; 9; 2007-04-20; 2007-04-30; 0000011;
       LHR; BKK; 22:00; +1; 15:15; 11:15; C; 12; M; 300;
       BKK; SYD; 18:10; +1; 06:05; 08:55; C; 20; M; 250;
       0; C; CDIU; 1; CD; 2; IU; M; YHBKLMNOPQRSTVWX; 3; YHBKLMNOPQRSTVWX
       BA; 9; 2007-04-20; 2007-04-30; 1111100;
       LHR; SIN; 22:00; +1; 15:15; 11:15; C; 15; M; 310;
       SIN; SYD; 18:10; +1; 06:05; 08:55; C; 25; M; 260;
       1; LHR; SIN; C; CDIU; 1; CDIU; M; YHBKLMNOPQRSTVWX; 2;YHBKLMNOPQRSTVWX
       SIN; SYD; C; CDIU; 1; CDIU; M; YHBKLMNOPQRSTVWX; 2;YHBKLMNOPQRSTVWX
       LHR; SYD; C; CDIU; 1; CDIU; M; YHBKLMNOPQRSTVWX; 2;YHBKLMNOPQRSTVWX
         
       Grammar:
       DOW                 ::= int
       FlightKey           ::= AirlineCode ';' FlightNumber
       ';' DateRangeStart ';' DateRangeEnd ';' DOW
       LegKey              ::= BoardingPoint ';' OffPoint
       LegDetails          ::= BoardingTime ['/' BoardingDateOffset]
       ';' OffTime ['/' BoardingDateOffset]
       ';' Elapsed
       LegCabinDetails     ::= CabinCode ';' Capacity
       Leg                 ::= LegKey ';' LegDetails (';' CabinDetails)+
       SegmentKey          ::= BoardingPoint ';' OffPoint
       SegmentCabinDetails ::= CabinCode ';' Classes
       (';' FamilyCabinDetails)*
       FamilyCabinDetails  ::= FamilyCode ';' Classes
       FullSegmentCabinDetails::= (';' SegmentCabinDetails)+
       GenericSegment      ::= '0' (';' SegmentCabinDetails)+
       SpecificSegments    ::= '1' (';' SegmentKey ';' FullSegmentCabinDetails)+
       SegmentSection      ::= GenericSegment | SpecificSegments
       FlightPeriod        ::= FlightKey (';' Leg)+
                               ';' SegmentSection ';' EndOfFlight
       EndOfFlight         ::= ';'
    */

    /** Grammar for the Flight-Period parser. */
    struct FlightPeriodParser : 
      public boost::spirit::classic::grammar<FlightPeriodParser> {

      FlightPeriodParser (stdair::BomRoot&, FlightPeriodStruct&);

      template <typename ScannerT>
      struct definition {
        definition (FlightPeriodParser const& self);

        // Instantiation of rules
        boost::spirit::classic::rule<ScannerT> flight_period_list, flight_period,
          flight_period_end, flight_key, airline_code, flight_number,
          date, dow, time, date_offset,
          leg, leg_key, leg_details, leg_cabin_details,
          segment_section, segment_key, full_segment_cabin_details,
          segment_cabin_details, full_family_cabin_details,
          family_cabin_details, generic_segment, specific_segment_list;

        /** Entry point of the parser. */
        boost::spirit::classic::rule<ScannerT> const& start() const;
      };

      // Parser Context
      stdair::BomRoot& _bomRoot;
      FlightPeriodStruct& _flightPeriod;
    };

  }
  /** Short Description
      <br> Detailed Description.*/
  

  /////////////////////////////////////////////////////////////////////////
  //
  //  Entry class for the file parser
  //
  /////////////////////////////////////////////////////////////////////////
  /** Class wrapping the initialisation and entry point of the parser.
      <br>The seemingly redundancy is used to force the instantiation of
      the actual parser, which is a templatised Boost Spirit grammar.
      Hence, the actual parser is instantiated within that class object
      code. */
  class FlightPeriodFileParser : public stdair::CmdAbstract {
  public:
    /** Constructor. */
    FlightPeriodFileParser (stdair::BomRoot& ioBomRoot,
                            const stdair::Filename_T& iFilename);

    /** Parse the input file and generate the Inventories. */
    bool generateInventories ();
      
  private:
    /** Initialise. */
    void init();
      
  private:
    // Attributes
    /** File-name of the CSV-formatted schedule input file. */
    stdair::Filename_T _filename;

    /** Start iterator for the parser. */
    iterator_t _startIterator;
      
    /** End iterator for the parser. */
    iterator_t _endIterator;
      
    /** stdair::BomRoot. */
    stdair::BomRoot& _bomRoot;

    /** Flight-Period Structure. */
    FlightPeriodStruct _flightPeriod;
  };
    
}
#endif // __AIRSCHED_CMD_SCHEDULEPARSERHELPER_HPP
