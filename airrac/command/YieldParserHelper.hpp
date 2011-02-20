#ifndef __AIRRAC_CMD_YIELDPARSERHELPER_HPP
#define __AIRRAC_CMD_YIELDPARSERHELPER_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
// StdAir
#include <stdair/command/CmdAbstract.hpp>
// Airrac
#include <airrac/AIRRAC_Types.hpp>
//#define BOOST_SPIRIT_DEBUG
#include <airrac/basic/BasParserTypes.hpp>
#include <airrac/bom/YieldStruct.hpp>

// Forward declarations
namespace stdair {
  class BomRoot;
}

namespace AIRRAC {

  namespace YieldParserHelper {
    
    // ///////////////////////////////////////////////////////////////////
    //  Semantic actions
    // ///////////////////////////////////////////////////////////////////
    /** Generic Semantic Action (Actor / Functor) for the Yield Parser. */
    struct ParserSemanticAction {
      /** Actor Constructor. */
      ParserSemanticAction (YieldStruct&);
      /** Actor Context. */
      YieldStruct& _yield;
    };
      
    /** Store the snapshot date. */
    struct storeSnapshotDate : public ParserSemanticAction {
      /** Actor Constructor. */
      storeSnapshotDate (YieldStruct&);
      /** Actor Function (functor). */
      void operator() (iterator_t iStr, iterator_t iStrEnd) const;
    };

    /** Store the parsed airline code. */
    struct storeAirlineCode : public ParserSemanticAction {
      /** Actor Constructor. */
      storeAirlineCode (YieldStruct&);
      /** Actor Function (functor). */
      void operator() (iterator_t iStr, iterator_t iStrEnd) const;
    };
  
    /** Mark the end of the yield parsing. */
    struct doEndYield : public ParserSemanticAction {
      /** Actor Constructor. */
      doEndYield (stdair::BomRoot&, YieldStruct&);
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
       Yield;
       2010-02-08; SIN; BKK; L; 10.0; 1.0;

    Grammar:
      Yield ::= Origin ';' Destination
         EndOfYield
      ValidityDate ::= date
      EndOfYield ::= ';'
     */

    /** Grammar for the yield parser. */
    struct YieldParser : 
      public boost::spirit::classic::grammar<YieldParser> {

      YieldParser (stdair::BomRoot&, YieldStruct&);

      template <typename ScannerT>
      struct definition {
        definition (YieldParser const& self);
        
        // Instantiation of rules
        boost::spirit::classic::rule<ScannerT> yield_list, yield, yield_end,
          date, time, airline_code;

        /** Entry point of the parser. */
        boost::spirit::classic::rule<ScannerT> const& start() const;
      };

      // Parser Context
      stdair::BomRoot& _bomRoot;
      YieldStruct& _yield;
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
  class YieldFileParser : public stdair::CmdAbstract {
  public:
    /** Constructor. */
    YieldFileParser (stdair::BomRoot&,
                     const stdair::Filename_T& iYieldInputFilename);

    /** Parse the yield store input file. */
    bool generateYieldStore ();
      
  private:
    /** Initialise. */
    void init();
      
  private:
    // Attributes
    /** File-name of the CSV-formatted yield input file. */
    stdair::Filename_T _filename;

    /** Start iterator for the parser. */
    iterator_t _startIterator;
      
    /** End iterator for the parser. */
    iterator_t _endIterator;
      
    /** Root of the BOM tree. */
    stdair::BomRoot& _bomRoot;

    /** Yield Structure. */
    YieldStruct _yield;
  };
    
}
#endif // __AIRRAC_CMD_YIELDPARSERHELPER_HPP
