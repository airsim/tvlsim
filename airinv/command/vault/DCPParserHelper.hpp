#ifndef __AIRINV_CMD_DCPPARSERHELPER_HPP
#define __AIRINV_CMD_DCPPARSERHELPER_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// StdAir
// The stdair/basic/BasParserTypes.hpp header includes Boost.Spirit headers
//#define BOOST_SPIRIT_DEBUG
#include <stdair/basic/BasParserTypes.hpp>
#include <stdair/command/CmdAbstract.hpp>
// AirInv
#include <airinv/AIRINV_Types.hpp>
#include <airinv/bom/DCPRuleStruct.hpp>

// Forward declarations
namespace stdair {
  class BomRoot;
}

namespace AIRINV {

  namespace DCPParserHelper {
    
    // ///////////////////////////////////////////////////////////////////
    //  Semantic actions
    // ////////////////////////////////////////////////////
    ///////////////
    /** Generic Semantic Action (Actor / Functor) for the DCP Parser. */
    struct ParserSemanticAction {
      /** Actor Constructor. */
      ParserSemanticAction (DCPRuleStruct&);
      /** Actor Context. */
      DCPRuleStruct& _DCPRule;
    };

    /** Store the parsed DCP Id. */
    struct storeDCPId : public ParserSemanticAction {
      /** Actor Constructor. */
      storeDCPId (DCPRuleStruct&);
      /** Actor Function (functor). */
      void operator() (unsigned int,
                       boost::spirit::qi::unused_type,
                       boost::spirit::qi::unused_type) const;
    };

    /** Store the parsed origin. */
    struct storeOrigin : public ParserSemanticAction {
      /** Actor Constructor. */
      storeOrigin (DCPRuleStruct&);
      /** Actor Function (functor). */
      void operator() (std::vector<char>,
                       boost::spirit::qi::unused_type,
                       boost::spirit::qi::unused_type) const;
    };

    /** Store the parsed destination. */
    struct storeDestination : public ParserSemanticAction {
      /** Actor Constructor. */
      storeDestination (DCPRuleStruct&);
      /** Actor Function (functor). */
      void operator() (std::vector<char>,
                       boost::spirit::qi::unused_type,
                       boost::spirit::qi::unused_type) const;
    };

     /** Store the parsed start of the date range. */
    struct storeDateRangeStart : public ParserSemanticAction {
      /** Actor Constructor. */
      storeDateRangeStart (DCPRuleStruct&);
      /** Actor Function (functor). */
      void operator() (boost::spirit::qi::unused_type,
                       boost::spirit::qi::unused_type,
                       boost::spirit::qi::unused_type) const;
    };

    /** Store the parsed end of the date range. */
    struct storeDateRangeEnd : public ParserSemanticAction {
      /** Actor Constructor. */
      storeDateRangeEnd (DCPRuleStruct&);
      /** Actor Function (functor). */
      void operator() (boost::spirit::qi::unused_type,
                       boost::spirit::qi::unused_type,
                       boost::spirit::qi::unused_type) const;
    };

    /** Store the parsed start range time. */
    struct storeStartRangeTime : public ParserSemanticAction {
      /** Actor Constructor. */
      storeStartRangeTime (DCPRuleStruct&);
      /** Actor Function (functor). */
      void operator() (boost::spirit::qi::unused_type,
                       boost::spirit::qi::unused_type,
                       boost::spirit::qi::unused_type) const;
    };

    /** Store the parsed end start range time. */
    struct storeEndRangeTime : public ParserSemanticAction {
      /** Actor Constructor. */
      storeEndRangeTime (DCPRuleStruct&);
      /** Actor Function (functor). */
      void operator() (boost::spirit::qi::unused_type,
                       boost::spirit::qi::unused_type,
                       boost::spirit::qi::unused_type) const;
    };

    /** Store the parsed customer position. */
    struct storePOS : public ParserSemanticAction {
      /** Actor Constructor. */
      storePOS (DCPRuleStruct&);
      /** Actor Function (functor). */
      void operator() (std::vector<char>,
                       boost::spirit::qi::unused_type,
                       boost::spirit::qi::unused_type) const;
    };

    /** Store the cabin code. */
    struct storeCabinCode : public ParserSemanticAction {
      /** Actor Constructor. */
      storeCabinCode  (DCPRuleStruct&);
      /** Actor Function (functor). */
      void operator() (char,
                       boost::spirit::qi::unused_type,
                       boost::spirit::qi::unused_type) const;
    };

    /** Store the channel distribution. */
    struct storeChannel : public ParserSemanticAction {
      /** Actor Constructor. */
      storeChannel (DCPRuleStruct&);
      /** Actor Function (functor). */
      void operator() (std::vector<char>,
                       boost::spirit::qi::unused_type,
                       boost::spirit::qi::unused_type) const;
    };

    /** Store the parsed advance purchase days. */
    struct storeAdvancePurchase : public ParserSemanticAction {
      /** Actor Constructor. */
      storeAdvancePurchase (DCPRuleStruct&);
      /** Actor Function (functor). */
      void operator() (unsigned int,
                       boost::spirit::qi::unused_type,
                       boost::spirit::qi::unused_type) const;
    };

    /** Store the parsed saturday night. */
    struct storeSaturdayStay : public ParserSemanticAction {
      /** Actor Constructor. */
      storeSaturdayStay (DCPRuleStruct&);
      /** Actor Function (functor). */
      void operator() (char,
                       boost::spirit::qi::unused_type,
                       boost::spirit::qi::unused_type) const;
    };
    
    /** Store the parsed change fees. */
    struct storeChangeFees : public ParserSemanticAction {
      /** Actor Constructor. */
      storeChangeFees (DCPRuleStruct&);
      /** Actor Function (functor). */
      void operator() (char,
                       boost::spirit::qi::unused_type,
                       boost::spirit::qi::unused_type) const;
    };

    /** Store the parsed refundable option */
    struct storeNonRefundable : public ParserSemanticAction {
      /** Actor Constructor. */
      storeNonRefundable (DCPRuleStruct&);
      /** Actor Function (functor). */
      void operator() (char,
                       boost::spirit::qi::unused_type,
                       boost::spirit::qi::unused_type) const;
    };
    
    /** Store the parsed minimum stay. */
    struct storeMinimumStay : public ParserSemanticAction {
      /** Actor Constructor. */
      storeMinimumStay (DCPRuleStruct&);
      /** Actor Function (functor). */
      void operator() (unsigned int,
                       boost::spirit::qi::unused_type,
                       boost::spirit::qi::unused_type) const;
    };

     /** Store the parsed DCP value. */
    struct storeDCP : public ParserSemanticAction {
      /** Actor Constructor. */
      storeDCP (DCPRuleStruct&);
      /** Actor Function (functor). */
      void operator() (double,
                       boost::spirit::qi::unused_type,
                       boost::spirit::qi::unused_type) const; 
    };
    
    /** Store the parsed airline code. */
    struct storeAirlineCode : public ParserSemanticAction {
      /** Actor Constructor. */
      storeAirlineCode (DCPRuleStruct&);
      /** Actor Function (functor). */
      void operator() (std::vector<char>,
                       boost::spirit::qi::unused_type,
                       boost::spirit::qi::unused_type) const;
    };

    /** Store the parsed class. */
    struct storeClass : public ParserSemanticAction {
      /** Actor Constructor. */
      storeClass (DCPRuleStruct&);
      /** Actor Function (functor). */
      void operator() (std::vector<char>,
                       boost::spirit::qi::unused_type,
                       boost::spirit::qi::unused_type) const; 
    };
    
    /** Mark the end of the DCP-rule parsing. */
    struct doEndDCP : public ParserSemanticAction {
      /** Actor Constructor. */
      doEndDCP (stdair::BomRoot&, DCPRuleStruct&);
      /** Actor Function (functor). */
      void operator() (boost::spirit::qi::unused_type,
                       boost::spirit::qi::unused_type,
                       boost::spirit::qi::unused_type) const;
      /** Actor Specific Context. */
      stdair::BomRoot& _bomRoot;
    };
  

    /////////////////////////////////////////////////////////////////////////
    //
    //  (Boost Spirit) Grammar Definition
    //
    /////////////////////////////////////////////////////////////////////////
    /**

     // DCP: DCPID; OriginCity; DestinationCity; DateRangeStart;
     DateRangeEnd; DepartureTimeRangeStart; DepartureTimeRangeEnd;
     POS; AdvancePurchase; SaturdayNight; ChangeFees; NonRefundable;
     MinimumStay; Price; AirlineCode; Class;

     DCPID
     OriginCity              (3-char airport code)
     DestinationCity         (3-char airport code)
     DateRangeStart          (yyyy-mm-dd)
     DateRangeEnd            (yyyy-mm-dd)
     DepartureTimeRangeStart (hh:mm)
     DepartureTimeRangeEnd   (hh:mm)
     POS                     (3-char position city)
     Cabin Code              (1-char cabin code)
     Channel                 (D=direct, I=indirect, N=oNline, F=oFfline)
     AdvancePurchase         
     SaturdayNight           (T=True, F=False)
     ChangeFees              (T=True, F=False)
     NonRefundable           (T=True, F=False)
     MinimumStay
     Price
     AirlineCode             (2-char airline code)
     ClassList               (List of 1-char class code)
     
     Grammar:
      Demand ::= PrefDepDate ';' Origin ';' Destination ';' PassengerType
         ';' DemandParams ';' PosDist ';' ChannelDist ';'  TripDist
         ';' StayDist ';' FfDist ';'  PrefDepTimeDist
         ';' minWTP ';' TimeValueDist ';'  DtdDist
         EndOfDemand
      PrefDepDate ::= date
      PassengerType ::= 'T' | 'F'
      DemandParams ::= DemandMean ';' DemandStdDev
      PosDist ::= PosPair (',' PosPair)*
      PosPair ::= PosCode ':' PosShare
      PosCode ::= AirportCode | "row"
      PosShare ::= real
      ChannelDist ::= ChannelPair (',' ChannelPair)*
      ChannelPair ::= Channel_Code ':' ChannelShare
      ChannelCode ::= "DF" | "DN" | "IF" | "IN"
      ChannelShare ::= real
      TripDist ::= TripPair (',' TripPair)*
      TripPair ::= TripCode ':' TripShare
      TripCode ::= "RO" | "RI" | "OW"
      TripShare ::= real
      StayDist ::= StayPair (',' StayPair)*
      StayPair ::= [0;3]-digit-integer ':' stay_share
      StayShare ::= real
      FFDist ::= FF_Pair (',' FF_Pair)*
      FFPair ::= FFCode ':' FFShare
      FFCode ::= 'P' | 'G' | 'S' | 'M' | 'N'
      FFShare ::= real
      PrefDepTimeDist ::= PrefDepTimePair (',' PrefDepTimePair)*
      PrefDepTimePair ::= time ':' PrefDepTimeShare
      PrefDepTimeShare ::= real
      minWTP ::= real
      TimeValueDist ::= TimeValuePair (',' TimeValuePair)*
      TimeValuePair ::= [0;2]-digit-integer ':' TimeValueShare
      TimeValueShare ::= real
      DTDDist ::= DTDPair (',' DTDPair)*
      DTDPair ::= real ':' DTDShare
      DTDShare ::= real
      EndOfDemand ::= ';'
     
    */ 

    /** Grammar for the DCP-Rule parser. */
    struct DCPRuleParser : 
      public boost::spirit::qi::grammar<stdair::iterator_t, 
                                        boost::spirit::ascii::space_type> {

      DCPRuleParser (stdair::BomRoot&, DCPRuleStruct&);

      // Instantiation of rules
      boost::spirit::qi::rule<stdair::iterator_t,
                              boost::spirit::ascii::space_type>
      start, comments, DCP_rule, DCP_rule_end, DCP_key, DCP_id, origin,
        destination, dateRangeStart, dateRangeEnd, date, timeRangeStart,
        timeRangeEnd, time, position, cabinCode, channel, advancePurchase,
        saturdayStay, changeFees, nonRefundable, minimumStay, DCP,
        segment, list_class;
      
      // Parser Context
      stdair::BomRoot& _bomRoot;
      DCPRuleStruct& _DCPRule;
    };

  }
  
  /////////////////////////////////////////////////////////////////////////
  //
  //  Entry class for the file parser
  //
  /////////////////////////////////////////////////////////////////////////
  /** Class wrapping the 
      initialisation and entry point of the parser.
      <br>The seemingly redundancy is used to force the instantiation of
      the actual parser, which is a templatised Boost Spirit grammar.
      Hence, the actual parser is instantiated within that class object
      code. */
  class DCPRuleFileParser : public stdair::CmdAbstract {
  public:
    /** Constructor. */
    DCPRuleFileParser (stdair::BomRoot& ioBomRoot,
                        const stdair::Filename_T& iFilename);

    /** Parse the input file and generate the Inventories. */
    bool generateDCPRules ();
      
  private:
    /** Initialise. */
    void init();
      
  private:
    // Attributes
    /** File-name of the CSV-formatted schedule input file. */
    stdair::Filename_T _filename;

    /** stdair::BomRoot. */
    stdair::BomRoot& _bomRoot;

    /** DCP-Rule Structure. */
    DCPRuleStruct _DCPRule;
  };
    
}
#endif // __AIRINV_CMD_DCPPARSERHELPER_HPP
