#ifndef __AIRINV_CMD_INVENTORYPARSERHELPER_HPP
#define __AIRINV_CMD_INVENTORYPARSERHELPER_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
// StdAir
#include <stdair/command/CmdAbstract.hpp>
// Airinv
#include <airinv/AIRINV_Types.hpp>
//#define BOOST_SPIRIT_DEBUG
#include <airinv/basic/BasParserTypes.hpp>
#include <airinv/bom/FlightDateStruct.hpp>

// Forward declarations
namespace stdair {
  class BomRoot;
}

namespace AIRINV {

  namespace InventoryParserHelper {
    
    // ///////////////////////////////////////////////////////////////////
    //  Semantic actions
    // ///////////////////////////////////////////////////////////////////
    /** Generic Semantic Action (Actor / Functor) for the Inventory Parser. */
    struct ParserSemanticAction {
      /** Actor Constructor. */
      ParserSemanticAction (FlightDateStruct&);
      /** Actor Context. */
      FlightDateStruct& _flightDate;
    };
      
    /** Store the snapshot date. */
    struct storeSnapshotDate : public ParserSemanticAction {
      /** Actor Constructor. */
      storeSnapshotDate (FlightDateStruct&);
      /** Actor Function (functor). */
      void operator() (iterator_t iStr, iterator_t iStrEnd) const;
    };

    /** Store the parsed airline code. */
    struct storeAirlineCode : public ParserSemanticAction {
      /** Actor Constructor. */
      storeAirlineCode (FlightDateStruct&);
      /** Actor Function (functor). */
      void operator() (iterator_t iStr, iterator_t iStrEnd) const;
    };
  
    /** Store the parsed flight number. */
    struct storeFlightNumber : public ParserSemanticAction {
      /** Actor Constructor. */
      storeFlightNumber (FlightDateStruct&);
      /** Actor Function (functor). */
      void operator() (unsigned int iNumber) const;
    };
  
    /** Store the flight date. */
    struct storeFlightDate : public ParserSemanticAction {
      /** Actor Constructor. */
      storeFlightDate (FlightDateStruct&);
      /** Actor Function (functor). */
      void operator() (iterator_t iStr, iterator_t iStrEnd) const;
    };

    /** Store the flight type code. */
    struct storeFlightTypeCode : public ParserSemanticAction {
      /** Actor Constructor. */
      storeFlightTypeCode (FlightDateStruct&);
      /** Actor Function (functor). */
      void operator() (iterator_t iStr, iterator_t iStrEnd) const;
    };

    /** Store the flight visibility code. */
    struct storeFlightVisibilityCode : public ParserSemanticAction {
      /** Actor Constructor. */
      storeFlightVisibilityCode (FlightDateStruct&);
      /** Actor Function (functor). */
      void operator() (iterator_t iStr, iterator_t iStrEnd) const;
    };

    /** Store the parsed leg boarding point. */
    struct storeLegBoardingPoint : public ParserSemanticAction {
      /** Actor Constructor. */
      storeLegBoardingPoint (FlightDateStruct&);
      /** Actor Function (functor). */
      void operator() (iterator_t iStr, iterator_t iStrEnd) const;
    };
  
    /** Store the parsed leg off point. */
    struct storeLegOffPoint : public ParserSemanticAction {
      /** Actor Constructor. */
      storeLegOffPoint (FlightDateStruct&);
      /** Actor Function (functor). */
      void operator() (iterator_t iStr, iterator_t iStrEnd) const;
    };

    /** Store the boarding date. */
    struct storeBoardingDate : public ParserSemanticAction {
      /** Actor Constructor. */
      storeBoardingDate (FlightDateStruct&);
      /** Actor Function (functor). */
      void operator() (iterator_t iStr, iterator_t iStrEnd) const;
    };

    /** Store the boarding time. */
    struct storeBoardingTime : public ParserSemanticAction {
      /** Actor Constructor. */
      storeBoardingTime (FlightDateStruct&);
      /** Actor Function (functor). */
      void operator() (iterator_t iStr, iterator_t iStrEnd) const;
    };

    /** Store the off date. */
    struct storeOffDate : public ParserSemanticAction {
      /** Actor Constructor. */
      storeOffDate (FlightDateStruct&);
      /** Actor Function (functor). */
      void operator() (iterator_t iStr, iterator_t iStrEnd) const;
    };

    /** Store the off time. */
    struct storeOffTime : public ParserSemanticAction {
      /** Actor Constructor. */
      storeOffTime (FlightDateStruct&);
      /** Actor Function (functor). */
      void operator() (iterator_t iStr, iterator_t iStrEnd) const;
    };

    /** Store the parsed leg cabin code. */
    struct storeLegCabinCode : public ParserSemanticAction {
      /** Actor Constructor. */
      storeLegCabinCode (FlightDateStruct&);
      /** Actor Function (functor). */
      void operator() (char iChar) const;
    };
  
    /** Store the parsed saleable capacity. */
    struct storeSaleableCapacity : public ParserSemanticAction {
      /** Actor Constructor. */
      storeSaleableCapacity (FlightDateStruct&);
      /** Actor Function (functor). */
      void operator() (double iReal) const;
    };
  
    /** Store the parsed Authorisation Level (AU). */
    struct storeAU : public ParserSemanticAction {
      /** Actor Constructor. */
      storeAU (FlightDateStruct&);
      /** Actor Function (functor). */
      void operator() (double iReal) const;
    };
  
    /** Store the parsed Unsold Protected (UPR). */
    struct storeUPR : public ParserSemanticAction {
      /** Actor Constructor. */
      storeUPR (FlightDateStruct&);
      /** Actor Function (functor). */
      void operator() (double iReal) const;
    };
  
    /** Store the parsed booking counter. */
    struct storeBookingCounter : public ParserSemanticAction {
      /** Actor Constructor. */
      storeBookingCounter (FlightDateStruct&);
      /** Actor Function (functor). */
      void operator() (double iReal) const;
    };
  
    /** Store the parsed Net Availability (NAV). */
    struct storeNAV : public ParserSemanticAction {
      /** Actor Constructor. */
      storeNAV (FlightDateStruct&);
      /** Actor Function (functor). */
      void operator() (double iReal) const;
    };
  
    /** Store the parsed Gross Availability (GAV). */
    struct storeGAV : public ParserSemanticAction {
      /** Actor Constructor. */
      storeGAV (FlightDateStruct&);
      /** Actor Function (functor). */
      void operator() (double iReal) const;
    };
  
    /** Store the parsed Average Cancellation Percentage (ACP). */
    struct storeACP : public ParserSemanticAction {
      /** Actor Constructor. */
      storeACP (FlightDateStruct&);
      /** Actor Function (functor). */
      void operator() (double iReal) const;
    };
  
    /** Store the parsed Expected To Board (ETB) number. */
    struct storeETB : public ParserSemanticAction {
      /** Actor Constructor. */
      storeETB (FlightDateStruct&);
      /** Actor Function (functor). */
      void operator() (double iReal) const;
    };

    /** Store the parsed Yield Upper Range value. */
    struct storeYieldUpperRange : public ParserSemanticAction {
      /** Actor Constructor. */
      storeYieldUpperRange (FlightDateStruct&);
      /** Actor Function (functor). */
      void operator() (double iReal) const;
    };
    
    /** Store the parsed bucket availability. */
    struct storeBucketAvaibality : public ParserSemanticAction {
      /** Actor Constructor. */
      storeBucketAvaibality (FlightDateStruct&);
      /** Actor Function (functor). */
      void operator() (double iReal) const;
    };
    
    /** Store the parsed leg-cabin seat index. */
    struct storeSeatIndex : public ParserSemanticAction {
      /** Actor Constructor. */
      storeSeatIndex (FlightDateStruct&);
      /** Actor Function (functor). */
      void operator() (double iReal) const;
    };
    
    /** Store the parsed segment boarding point. */
    struct storeSegmentBoardingPoint : public ParserSemanticAction {
      /** Actor Constructor. */
      storeSegmentBoardingPoint (FlightDateStruct&);
      /** Actor Function (functor). */
      void operator() (iterator_t iStr, iterator_t iStrEnd) const;
    };
  
    /** Store the parsed segment off point. */
    struct storeSegmentOffPoint : public ParserSemanticAction {
      /** Actor Constructor. */
      storeSegmentOffPoint (FlightDateStruct&);
      /** Actor Function (functor). */
      void operator() (iterator_t iStr, iterator_t iStrEnd) const;
    };

    /** Store the parsed segment cabin code. */
    struct storeSegmentCabinCode : public ParserSemanticAction {
      /** Actor Constructor. */
      storeSegmentCabinCode (FlightDateStruct&);
      /** Actor Function (functor). */
      void operator() (char iChar) const;
    };
    
    /** Store the parsed segment cabin number of bookings. */
    struct storeSegmentCabinBookingCounter : public ParserSemanticAction {
      /** Actor Constructor. */
      storeSegmentCabinBookingCounter (FlightDateStruct&);
      /** Actor Function (functor). */
      void operator() (double iReal) const;
    };

    /** Store the parsed booking class code. */
    struct storeClassCode : public ParserSemanticAction {
      /** Actor Constructor. */
      storeClassCode (FlightDateStruct&);
      /** Actor Function (functor). */
      void operator() (char iChar) const;
    };
    
    /** Store the parsed sub-class code. */
    struct storeSubclassCode : public ParserSemanticAction {
      /** Actor Constructor. */
      storeSubclassCode (FlightDateStruct&);
      /** Actor Function (functor). */
      void operator() (unsigned int iNumber) const;
    };
    
    /** Store the parsed class code of the parent sub-class. */
    struct storeParentClassCode : public ParserSemanticAction {
      /** Actor Constructor. */
      storeParentClassCode (FlightDateStruct&);
      /** Actor Function (functor). */
      void operator() (char iChar) const;
    };
    
    /** Store the parsed sub-class code of the parent sub-class. */
    struct storeParentSubclassCode : public ParserSemanticAction {
      /** Actor Constructor. */
      storeParentSubclassCode (FlightDateStruct&);
      /** Actor Function (functor). */
      void operator() (unsigned int iNumber) const;
    };
    
    /** Store the parsed cumulated protection (at booking class level). */
    struct storeCumulatedProtection : public ParserSemanticAction {
      /** Actor Constructor. */
      storeCumulatedProtection (FlightDateStruct&);
      /** Actor Function (functor). */
      void operator() (double iReal) const;
    };

    /** Store the parsed protection (at booking class level). */
    struct storeProtection : public ParserSemanticAction {
      /** Actor Constructor. */
      storeProtection (FlightDateStruct&);
      /** Actor Function (functor). */
      void operator() (double iReal) const;
    };

    /** Store the negotiated allotment (at booking class level). */
    struct storeNego : public ParserSemanticAction {
      /** Actor Constructor. */
      storeNego (FlightDateStruct&);
      /** Actor Function (functor). */
      void operator() (double iReal) const;
    };

    /** Store the parsed No-Show percentage (at booking class level). */
    struct storeNoShow : public ParserSemanticAction {
      /** Actor Constructor. */
      storeNoShow (FlightDateStruct&);
      /** Actor Function (functor). */
      void operator() (double iReal) const;
    };

    /** Store the parsed Overbooking percentage (at booking class level). */
    struct storeOverbooking : public ParserSemanticAction {
      /** Actor Constructor. */
      storeOverbooking (FlightDateStruct&);
      /** Actor Function (functor). */
      void operator() (double iReal) const;
    };

    /** Store the parsed number of bookings (at booking class level). */
    struct storeNbOfBkgs : public ParserSemanticAction {
      /** Actor Constructor. */
      storeNbOfBkgs (FlightDateStruct&);
      /** Actor Function (functor). */
      void operator() (double iReal) const;
    };

    /** Store the parsed number of group bookings (at booking class level). */
    struct storeNbOfGroupBkgs : public ParserSemanticAction {
      /** Actor Constructor. */
      storeNbOfGroupBkgs (FlightDateStruct&);
      /** Actor Function (functor). */
      void operator() (double iReal) const;
    };

    /** Store the parsed number of pending group bookings (at booking class level). */
    struct storeNbOfPendingGroupBkgs : public ParserSemanticAction {
      /** Actor Constructor. */
      storeNbOfPendingGroupBkgs (FlightDateStruct&);
      /** Actor Function (functor). */
      void operator() (double iReal) const;
    };

    /** Store the parsed number of staff bookings (at booking class level). */
    struct storeNbOfStaffBkgs : public ParserSemanticAction {
      /** Actor Constructor. */
      storeNbOfStaffBkgs (FlightDateStruct&);
      /** Actor Function (functor). */
      void operator() (double iReal) const;
    };

    /** Store the parsed number of wait-list bookings (at booking
        class level). */
    struct storeNbOfWLBkgs : public ParserSemanticAction {
      /** Actor Constructor. */
      storeNbOfWLBkgs (FlightDateStruct&);
      /** Actor Function (functor). */
      void operator() (double iReal) const;
    };

    /** Store the parsed expected to board number (at booking class level). */
    struct storeClassETB : public ParserSemanticAction {
      /** Actor Constructor. */
      storeClassETB (FlightDateStruct&);
      /** Actor Function (functor). */
      void operator() (double iReal) const;
    };

    /** Store the parsed number of net class availability (at booking
        class level). */
    struct storeClassAvailability : public ParserSemanticAction {
      /** Actor Constructor. */
      storeClassAvailability (FlightDateStruct&);
      /** Actor Function (functor). */
      void operator() (double iReal) const;
    };

    /** Store the parsed number of segment availability (at booking
        class level). */
    struct storeSegmentAvailability : public ParserSemanticAction {
      /** Actor Constructor. */
      storeSegmentAvailability (FlightDateStruct&);
      /** Actor Function (functor). */
      void operator() (double iReal) const;
    };

    /** Store the parsed number of net revenue availability (at
        booking class level). */
    struct storeRevenueAvailability : public ParserSemanticAction {
      /** Actor Constructor. */
      storeRevenueAvailability (FlightDateStruct&);
      /** Actor Function (functor). */
      void operator() (double iReal) const;
    };

    /** Store the parsed family code. */
    struct storeFamilyCode : public ParserSemanticAction {
      /** Actor Constructor. */
      storeFamilyCode (FlightDateStruct&);
      /** Actor Function (functor). */
      void operator() (int iCode) const;
    };

    /** Store the parsed list of class codes (for families). */
    struct storeFClasses : public ParserSemanticAction {
      /** Actor Constructor. */
      storeFClasses (FlightDateStruct&);
      /** Actor Function (functor). */
      void operator() (iterator_t iStr, iterator_t iStrEnd) const;
    };
      
    /** Mark the end of the inventory parsing. */
    struct doEndFlightDate : public ParserSemanticAction {
      /** Actor Constructor. */
      doEndFlightDate (stdair::BomRoot&, FlightDateStruct&,
                       unsigned int&);
      /** Actor Function (functor). */
      void operator() (iterator_t iStr, iterator_t iStrEnd) const;
      /** Actor Specific Context. */
      stdair::BomRoot& _bomRoot;
      unsigned int& _nbOfFlights;
    };
  

    /////////////////////////////////////////////////////////////////////////
    //
    //  (Boost Spirit) Grammar Definition
    //
    /////////////////////////////////////////////////////////////////////////
    /**
       FlightDepDate;
       2010-02-08; SIN; BKK; L; 10.0; 1.0;

    Grammar:
      FlightDate ::= FlightDepDate ';' Origin ';' Destination
         EndOfFlightDate
      FlightDepDate ::= date
      EndOfFlightDate ::= ';'
     */

    /** Grammar for the inventory parser. */
    struct InventoryParser : 
      public boost::spirit::classic::grammar<InventoryParser> {

      InventoryParser (stdair::BomRoot&, FlightDateStruct&, unsigned int&);

      template <typename ScannerT>
      struct definition {
        definition (InventoryParser const& self);
        
        // Instantiation of rules
        boost::spirit::classic::rule<ScannerT> flight_date_list, flight_date,
          flight_date_end, flight_key, airline_code, flight_number,
          flight_type_code, flight_visibility_code,
          date, leg_list, leg, leg_key, leg_details,
          leg_cabin_list, leg_cabin_details,
          bucket_list, bucket_details,
          time, segment_list, segment, segment_key, full_segment_cabin_details,
          segment_cabin_list, segment_cabin_key, segment_cabin_details,
          class_list, class_key, parent_subclass_code,
          class_protection, class_nego, class_details,
          family_cabin_list, family_cabin_details;

        /** Entry point of the parser. */
        boost::spirit::classic::rule<ScannerT> const& start() const;
      };

      // Parser Context
      stdair::BomRoot& _bomRoot;
      FlightDateStruct& _flightDate;
      unsigned int& _nbOfFlights;
    };

  }


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
  class InventoryFileParser : public stdair::CmdAbstract {
  public:
    /** Constructor. */
    InventoryFileParser (stdair::BomRoot&,
                         const stdair::Filename_T& iInventoryInputFilename);

    /** Parse the inventory input file. */
    bool buildInventory ();
      
  private:
    /** Initialise. */
    void init();
      
  private:
    // Attributes
    /** File-name of the CSV-formatted inventory input file. */
    stdair::Filename_T _filename;

    /** Start iterator for the parser. */
    iterator_t _startIterator;
      
    /** End iterator for the parser. */
    iterator_t _endIterator;
      
    /** Root of the BOM tree. */
    stdair::BomRoot& _bomRoot;

    /** FlightDate Structure. */
    FlightDateStruct _flightDate;
    
    /** Number of already parsed flight-dates. */
    unsigned int _nbOfFlights;
  };
    
}
#endif // __AIRINV_CMD_INVENTORYPARSERHELPER_HPP
