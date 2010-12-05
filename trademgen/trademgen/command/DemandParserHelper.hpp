#ifndef __TRADEMGEN_CMD_DEMANDPARSERHELPER_HPP
#define __TRADEMGEN_CMD_DEMANDPARSERHELPER_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
// STDAIR
#include <stdair/command/CmdAbstract.hpp>
// TRADEMGEN
#include <trademgen/TRADEMGEN_Types.hpp>
#include <trademgen/basic/BasParserTypes.hpp>
#include <trademgen/bom/DemandStruct.hpp>

// Forward declarations
namespace stdair {
  class BomRoot;
}

namespace TRADEMGEN {

  namespace DemandParserHelper {
    
    // ///////////////////////////////////////////////////////////////////
    //  Semantic actions
    // ///////////////////////////////////////////////////////////////////
    /** Generic Semantic Action (Actor / Functor) for the Demand Parser. */
    struct ParserSemanticAction {
      /** Actor Constructor. */
      ParserSemanticAction (DemandStruct&);
      /** Actor Context. */
      DemandStruct& _demand;
    };
      
    /** Store the preferred departure date. */
    struct storePreferredDepartureDate : public ParserSemanticAction {
      /** Actor Constructor. */
      storePreferredDepartureDate (DemandStruct&);
      /** Actor Function (functor). */
      void operator() (iterator_t iStr, iterator_t iStrEnd) const;
    };

    /** Store the origin. */
    struct storeOrigin : public ParserSemanticAction {
      /** Actor Constructor. */
      storeOrigin (DemandStruct&);
      /** Actor Function (functor). */
      void operator() (iterator_t iStr, iterator_t iStrEnd) const;
    };
  
    /** Store the destination. */
    struct storeDestination : public ParserSemanticAction {
      /** Actor Constructor. */
      storeDestination (DemandStruct&);
      /** Actor Function (functor). */
      void operator() (iterator_t iStr, iterator_t iStrEnd) const;
    };

    /** Store the preferred cabin. */
    struct storePrefCabin : public ParserSemanticAction {
      /** Actor Constructor. */
      storePrefCabin (DemandStruct&);
      /** Actor Function (functor). */
      void operator() (iterator_t iStr, iterator_t iStrEnd) const;
    };

    /** Store the demand mean value. */
    struct storeDemandMean : public ParserSemanticAction {
      /** Actor Constructor. */
      storeDemandMean (DemandStruct&);
      /** Actor Function (functor). */
      void operator() (double iReal) const;
    };
  
    /** Store the demand stdandard deviation value. */
    struct storeDemandStdDev : public ParserSemanticAction {
      /** Actor Constructor. */
      storeDemandStdDev (DemandStruct&);
      /** Actor Function (functor). */
      void operator() (double iReal) const;
    };
  
    /** Store the pos type code. */
    struct storePosCode : public ParserSemanticAction {
      /** Actor Constructor. */
      storePosCode (DemandStruct&);
      /** Actor Function (functor). */
      void operator() (iterator_t iStr, iterator_t iStrEnd) const;
    };
  
    /** Store the pos type probability mass. */
    struct storePosProbMass : public ParserSemanticAction {
      /** Actor Constructor. */
      storePosProbMass (DemandStruct&);
      /** Actor Function (functor). */
      void operator() (double iReal) const;
    };
  
    /** Store the channel type code. */
    struct storeChannelCode : public ParserSemanticAction {
      /** Actor Constructor. */
      storeChannelCode (DemandStruct&);
      /** Actor Function (functor). */
      void operator() (iterator_t iStr, iterator_t iStrEnd) const;
    };
  
    /** Store the channel type probability mass. */
    struct storeChannelProbMass : public ParserSemanticAction {
      /** Actor Constructor. */
      storeChannelProbMass (DemandStruct&);
      /** Actor Function (functor). */
      void operator() (double iReal) const;
    };
  
    /** Store the trip type code. */
    struct storeTripCode : public ParserSemanticAction {
      /** Actor Constructor. */
      storeTripCode (DemandStruct&);
      /** Actor Function (functor). */
      void operator() (iterator_t iStr, iterator_t iStrEnd) const;
    };
  
    /** Store the trip type probability mass. */
    struct storeTripProbMass : public ParserSemanticAction {
      /** Actor Constructor. */
      storeTripProbMass (DemandStruct&);
      /** Actor Function (functor). */
      void operator() (double iReal) const;
    };
  
    /** Store the stay type code. */
    struct storeStayCode : public ParserSemanticAction {
      /** Actor Constructor. */
      storeStayCode (DemandStruct&);
      /** Actor Function (functor). */
      void operator() (unsigned int iInteger) const;
    };
  
    /** Store the stay type probability mass. */
    struct storeStayProbMass : public ParserSemanticAction {
      /** Actor Constructor. */
      storeStayProbMass (DemandStruct&);
      /** Actor Function (functor). */
      void operator() (double iReal) const;
    };
  
    /** Store the frequent flyer code. */
    struct storeFFCode : public ParserSemanticAction {
      /** Actor Constructor. */
      storeFFCode (DemandStruct&);
      /** Actor Function (functor). */
      void operator() (iterator_t iStr, iterator_t iStrEnd) const;
    };
  
    /** Store the frequent flyer probability mass. */
    struct storeFFProbMass : public ParserSemanticAction {
      /** Actor Constructor. */
      storeFFProbMass (DemandStruct&);
      /** Actor Function (functor). */
      void operator() (double iReal) const;
    };
  
    /** Store the parameters for the preferred departure time
        continuous probability distribution. */
    struct storePrefDepTime : public ParserSemanticAction {
      /** Actor Constructor. */
      storePrefDepTime (DemandStruct&);
      /** Actor Function (functor). */
      void operator() (iterator_t iStr, iterator_t iStrEnd) const;
    };    
    
    /** Store the parameters for the preferred departure time continuous
        probability distribution. */
    struct storePrefDepTimeProbMass : public ParserSemanticAction {
      /** Actor Constructor. */
      storePrefDepTimeProbMass (DemandStruct&);
      /** Actor Function (functor). */
      void operator() (double iReal) const;
    };
  
    /** Store the parameters for the min Willingness-To-Pay (WTP). */
    struct storeWTP : public ParserSemanticAction {
      /** Actor Constructor. */
      storeWTP (DemandStruct&);
      /** Actor Function (functor). */
      void operator() (double iReal) const;
    };
    
    /** Store the time value. */
    struct storeTimeValue : public ParserSemanticAction {
      /** Actor Constructor. */
      storeTimeValue (DemandStruct&);
      /** Actor Function (functor). */
      void operator() (double iReal) const;
    };
  
    /** Store the time value probability mass. */
    struct storeTimeValueProbMass : public ParserSemanticAction {
      /** Actor Constructor. */
      storeTimeValueProbMass (DemandStruct&);
      /** Actor Function (functor). */
      void operator() (double iReal) const;
    };
  
    /** Store the parameters for the arrival pattern (as expressed in DTD)
        continuous probability distribution. */
    struct storeDTD : public ParserSemanticAction {
      /** Actor Constructor. */
      storeDTD (DemandStruct&);
      /** Actor Function (functor). */
      void operator() (unsigned int iInteger) const;
    };    
    
    /** Store the parameters for the arrival pattern (as expressed in DTD)
        continuous probability distribution. */
    struct storeDTDProbMass : public ParserSemanticAction {
      /** Actor Constructor. */
      storeDTDProbMass (DemandStruct&);
      /** Actor Function (functor). */
      void operator() (double iReal) const;
    };
  
    /** Mark the end of the demand parsing. */
    struct doEndDemand : public ParserSemanticAction {
      /** Actor Constructor. */
      doEndDemand (stdair::BomRoot&, stdair::UniformGenerator_T&,
                   const POSProbabilityMass_T&, DemandStruct&);
      /** Actor Function (functor). */
      void operator() (iterator_t iStr, iterator_t iStrEnd) const;
      /** Actor Specific Context. */
      stdair::BomRoot& _bomRoot;
      stdair::UniformGenerator_T& _uniformGenerator;
      const POSProbabilityMass_T& _posProbabilityMass;
    };
  

    /////////////////////////////////////////////////////////////////////////
    //
    //  (Boost Spirit) Grammar Definition
    //
    /////////////////////////////////////////////////////////////////////////
    /**
       PrefDepDate; Origin; Destination; PassengerType; Mean; StdDev;
       PosDist; ChannelDist; TripTypeDist; StayDurationDist;
       FrequentFlyerDist; PrefDepTimeDist; min WTP;
       (PrefArrivalDate; PrefArrivalTime;) TimeValueDist; 
       ValueOfTimeDist; ArrivalPatternDist;
       2010-02-08; SIN; BKK; L; 10.0; 1.0;
       SIN:0.7, BKK:0.2, row:0.1; DF:0.1, DN:0.3, IF:0.4, IN:0.2;
       RO:0.6, RI:0.2, OW:0.2;
       0:0.1, 1:0.1, 2:0.15, 3:0.15, 4:0.15, 5:0.35;
       P:0.01, G:0.05, S:0.15, M:0.3, N:0.49;
       06:0, 07:0.1, 09:0.3, 17:0.4, 19:0.8, 20:0.95, 22:1;
       100:0, 500:0.8, 2000:1;
       15:0, 60:1;
       330:0, 40:0.2, 20:0.6, 1:1;
       
      Fixed:
        Prefered departure date (yyyy-mm-dd)
        Origin (3-char airport code)
        Destination (3-char airport code)
        PassengerType (1-char, e.g., 'L' for Leisure, 'B' for Business)
      Observable:
        Mean
        StdDev
      Distribution with Probability Masses:
        POS
        Channel (D=direct, I=indirect, N=oNline, F=oFfline)
        Trip type(RO=outbound of round-trip,RI=inbound of round-trip,OW=one way)
        Stay duration (number of days)
        Frequent flyer (P=Platinum, G=Gold, S=Silver, M=Member, N=None)
      Continuous cumulative distribution:
        Preferred departure time (hh:mm:ss)
        Preferred arrival date (equal to prefered departure date)
        Preferred arrival time (equal to prefered departure time)
        Value of time
        Arrival pattern (DTD as a positive value)
    The main fields are separated by ';'
    Probability mass distributions are defined by comma-separated
      'value:probability' pairs
    Continuous cumulative distribution are defined by comma-separated
      'value:probability' pairs, sorted in increasing order of values.
    The meaning of probability is P(random variable <= value) = probability.

    Grammar:
      Demand ::= PrefDepDate ';' Origin ';' Destination ';' PassengerType
         ';' DemandParams ';' PosDist ';' ChannelDist ';'  TripDist
         ';' StayDist ';' FfDist ';'  PrefDepTimeDist
         ';' minWTP ';' TimeValueDist ';'  DtdDist
         EndOfDemand
      PrefDepDate ::= date
      PassengerType ::= 'L' | 'B' | 'F'
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

    /** Grammar for the demand parser. */
    struct DemandParser : 
      public boost::spirit::classic::grammar<DemandParser> {

      DemandParser (stdair::BomRoot&, stdair::UniformGenerator_T&,
                    const POSProbabilityMass_T&, DemandStruct&);

      template <typename ScannerT>
      struct definition {
        definition (DemandParser const& self);
        
        // Instantiation of rules
        boost::spirit::classic::rule<ScannerT> demand_list, demand, demand_end,
          pref_dep_date, date, origin, destination, pref_cabin,
          demand_params,
          pos_dist, pos_pair, pos_code, pos_share,
          channel_dist, channel_pair, channel_code, channel_share,
          trip_dist, trip_pair, trip_code, trip_share,
          stay_dist, stay_pair, stay_share,
          ff_dist, ff_pair, ff_code, ff_share,
          pref_dep_time_dist, pref_dep_time_pair, pref_dep_time_share, time,
          wtp,
          time_value_dist, time_value_pair, time_value_share,
          dtd_dist, dtd_pair, dtd_share;

        /** Entry point of the parser. */
        boost::spirit::classic::rule<ScannerT> const& start() const;
      };

      // Parser Context
      stdair::BomRoot& _bomRoot;
      stdair::UniformGenerator_T& _uniformGenerator;
      const POSProbabilityMass_T& _posProbabilityMass;
      DemandStruct& _demand;
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
  class DemandFileParser : public stdair::CmdAbstract {
  public:
    /** Constructor. */
    DemandFileParser (stdair::BomRoot&, stdair::UniformGenerator_T&,
                      const POSProbabilityMass_T&,
                      const stdair::Filename_T& iDemandInputFilename);

    /** Parse the demand input file. */
    bool generateDemand ();
      
  private:
    /** Initialise. */
    void init();
      
  private:
    // Attributes
    /** File-name of the CSV-formatted demand input file. */
    stdair::Filename_T _filename;

    /** Start iterator for the parser. */
    iterator_t _startIterator;
      
    /** End iterator for the parser. */
    iterator_t _endIterator;
      
    /** Root of the BOM tree. */
    stdair::BomRoot& _bomRoot;

    /** Shared uniform generator. */
    stdair::UniformGenerator_T& _uniformGenerator;

    /** Default POS distribution. */
    const POSProbabilityMass_T& _posProbabilityMass;

    /** Demand Structure. */
    DemandStruct _demand;
  };
    
}
#endif // __TRADEMGEN_CMD_DEMANDPARSERHELPER_HPP
