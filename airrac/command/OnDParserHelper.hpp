#ifndef __AIRSCHED_CMD_ONDPARSERHELPER_HPP
#define __AIRSCHED_CMD_ONDPARSERHELPER_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
// Boost (Extended STL)
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
// StdAir
#include <stdair/command/CmdAbstract.hpp>
// AirSched
#include <airsched/AIRSCHED_Types.hpp>
#include <airsched/basic/BasParserTypes.hpp>
#include <airsched/bom/OnDPeriodStruct.hpp>

// Forward declarations
namespace stdair {
  class BomRoot;
}

namespace AIRSCHED {

  namespace OnDParserHelper {
           
    // ///////////////////////////////////////////////////////////////////////
    //
    //  Semantic actions
    //
    // ///////////////////////////////////////////////////////////////////////
    /** Generic Semantic Action (Actor / Functor) for the Schedule Parser. */
    struct ParserSemanticAction {
      /** Actor Constructor. */
      ParserSemanticAction (OnDPeriodStruct&);
      /** Actor Context. */
      OnDPeriodStruct& _onDPeriod;
    };

    /** Store the parsed origin. */
    struct storeOrigin : public ParserSemanticAction {
      /** Actor Constructor. */
      storeOrigin (OnDPeriodStruct&);
      /** Actor Function (functor). */
      void operator() (iterator_t iStr, iterator_t iStrEnd) const;
    };
  
    /** Store the parsed destination. */
    struct storeDestination : public ParserSemanticAction {
      /** Actor Constructor. */
      storeDestination (OnDPeriodStruct&);
      /** Actor Function (functor). */
      void operator() (iterator_t iStr, iterator_t iStrEnd) const;
    };

    /** Store the start of the date range. */
    struct storeDateRangeStart : public ParserSemanticAction {
      /** Actor Constructor. */
      storeDateRangeStart (OnDPeriodStruct&);
      /** Actor Function (functor). */
      void operator() (iterator_t iStr, iterator_t iStrEnd) const;
    };

    /** Store the end of the date range. */
    struct storeDateRangeEnd : public ParserSemanticAction {
      /** Actor Constructor. */
      storeDateRangeEnd (OnDPeriodStruct&);
      /** Actor Function (functor). */
      void operator() (iterator_t iStr, iterator_t iStrEnd) const;
    };

    /** Store the start range time. */
    struct storeStartRangeTime : public ParserSemanticAction {
      /** Actor Constructor. */
      storeStartRangeTime (OnDPeriodStruct&);
      /** Actor Function (functor). */
      void operator() (iterator_t iStr, iterator_t iStrEnd) const;
    };

    /** Store the end range time. */
    struct storeEndRangeTime : public ParserSemanticAction {
      /** Actor Constructor. */
      storeEndRangeTime (OnDPeriodStruct&);
      /** Actor Function (functor). */
      void operator() (iterator_t iStr, iterator_t iStrEnd) const;
    };

    /** Store the parsed airline code. */
    struct storeAirlineCode : public ParserSemanticAction {
      /** Actor Constructor. */
      storeAirlineCode (OnDPeriodStruct&);
      /** Actor Function (functor). */
      void operator() (iterator_t iStr, iterator_t iStrEnd) const;
    };

    /** Store the parsed class code. */
    struct storeClassCode : public ParserSemanticAction {
      /** Actor Constructor. */
      storeClassCode (OnDPeriodStruct&);
      /** Actor Function (functor). */
      void operator() (char iChar) const;
    };

    /** Mark the end of the O&D parsing. */
    struct doEndOnD : public ParserSemanticAction {
      /** Actor Constructor. */
      doEndOnD (stdair::BomRoot&, OnDPeriodStruct&);
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
       Fares:   AirlineCode; OriginCity; DestinationCity;
       DepartureDate-Range(FirstDate; LastDate); Airline; Class; 
       BA; NCE; LHR; 2007-01-01; 2007-12-31; BA; Y; BA; Y 
       BA; NCE; LHR; 2007-01-01; 2007-12-31; BA; V; BA; H */

    /** Grammar for the FareRule parser. */
    struct OnDParser : 
      public boost::spirit::classic::grammar<OnDParser> {

      OnDParser (stdair::BomRoot&, OnDPeriodStruct&);

      template <typename ScannerT>
      struct definition {
        definition (OnDParser const& self);

        // Instantiation of rules
        boost::spirit::classic::rule<ScannerT> ond_list, ond, segment,
          ond_key, ond_end, date, time;

        /** Entry point of the parser. */
        boost::spirit::classic::rule<ScannerT> const& start() const;
      };

      // Parser Context
      stdair::BomRoot& _bomRoot;
      OnDPeriodStruct& _onDPeriod;
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

  class OnDPeriodFileParser : public stdair::CmdAbstract {
  public:
    /** Constructor. */
    OnDPeriodFileParser (const stdair::Filename_T& iFilename,
                         stdair::BomRoot& ioBomRoot);
      
    /** Parse the input file and generate the O&D-Periods. */
    bool generateOnDPeriods ();
      
  private:
    /** Initialise. */
    void init();
      
  private:
    // Attributes
    /** File-name of the CSV-formatted input file. */
    stdair::Filename_T _filename;
      
    /** Start iterator for the parser. */
    iterator_t _startIterator;
      
    /** End iterator for the parser. */
    iterator_t _endIterator;
      
    /** stdair::BomRoot. */
    stdair::BomRoot& _bomRoot;
      
    /** FareRule Structure. */
    OnDPeriodStruct _onDPeriod;
  };
  
}
#endif // __AIRSCHED_CMD_ONDPARSERHELPER_HPP
