// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// StdAir
#include <stdair/stdair_exceptions.hpp>
#include <stdair/bom/BomRoot.hpp>
#include <stdair/service/Logger.hpp>
// Airrac
#include <airrac/command/YieldParserHelper.hpp>

namespace AIRRAC {

  namespace YieldParserHelper {
      
    // //////////////////////////////////////////////////////////////////
    //  Semantic actions
    // //////////////////////////////////////////////////////////////////

    ParserSemanticAction::ParserSemanticAction (YieldStruct& ioYield)
      : _yield (ioYield) {
    }      

    // //////////////////////////////////////////////////////////////////
    storeSnapshotDate::storeSnapshotDate (YieldStruct& ioYield)
      : ParserSemanticAction (ioYield) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeSnapshotDate::operator() (iterator_t iStr,
                                        iterator_t iStrEnd) const {
      _yield._yield = _yield.getDate();
    }
      
    // //////////////////////////////////////////////////////////////////
    storeAirlineCode::storeAirlineCode (YieldStruct& ioYield)
      : ParserSemanticAction (ioYield) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeAirlineCode::operator() (iterator_t iStr,
                                       iterator_t iStrEnd) const {
      const stdair::AirlineCode_T lAirlineCode (iStr, iStrEnd);
      _yield._airlineCode = lAirlineCode;
                
      // As that's the beginning of a new yield rule, the list of legs
      // must be reset
      //_yield._legList.clear();
    }
      
    // //////////////////////////////////////////////////////////////////
    doEndYield::doEndYield (stdair::BomRoot& ioBomRoot, YieldStruct& ioYield)
      : ParserSemanticAction (ioYield), _bomRoot (ioBomRoot) {
    }
    
    // //////////////////////////////////////////////////////////////////
    // void doEndYield::operator() (char iChar) const {
    void doEndYield::operator() (iterator_t iStr, iterator_t iStrEnd) const {
        
      // DEBUG: Display the result
      // STDAIR_LOG_DEBUG ("Yield: " << _yield.describe());

      // Create the Yield BOM objects
      // YieldGenerator::createYield (_bomRoot, _yield);
    }

      
    // ///////////////////////////////////////////////////////////////////
    //
    //  Utility Parsers
    //
    // ///////////////////////////////////////////////////////////////////
    /** 1-digit-integer parser */
    int1_p_t int1_p;
    
    /** 2-digit-integer parser */
    uint2_p_t uint2_p;
    
    /** Up-to-2-digit-integer parser */
    uint1_2_p_t uint1_2_p;

    /** Up-to-3-digit-integer parser */
    uint1_3_p_t uint1_3_p;

    /** 4-digit-integer parser */
    uint4_p_t uint4_p;
    
    /** Up-to-4-digit-integer parser */
    uint1_4_p_t uint1_4_p;

    /** Airline Code Parser: repeat_p(2,3)[chset_p("0-9A-Z")] */
    repeat_p_t airline_code_p (chset_t("0-9A-Z").derived(), 2, 3);
      
    /** Flight Number Parser: limit_d(0u, 9999u)[uint1_4_p] */
    bounded1_4_p_t flight_number_p (uint1_4_p.derived(), 0u, 9999u);

    /** Year Parser: limit_d(00u, 99u)[uint4_p] */
    bounded2_p_t year_p (uint2_p.derived(), 0u, 99u);
      
    /** Month Parser: limit_d(1u, 12u)[uint2_p] */
    bounded2_p_t month_p (uint2_p.derived(), 1u, 12u);

    /** Day Parser: limit_d(1u, 31u)[uint2_p] */
    bounded2_p_t day_p (uint2_p.derived(), 1u, 31u);
     
    /** DOW (Day-Of-the-Week) Parser: repeat_p(7)[chset_p("0-1")] */
    repeat_p_t dow_p (chset_t("0-1").derived().derived(), 7, 7);

    /** Airport Parser: repeat_p(3)[chset_p("0-9A-Z")] */
    repeat_p_t airport_p (chset_t("0-9A-Z").derived(), 3, 3);
      
    /** Hour Parser: limit_d(0u, 24u)[uint2_p] */
    bounded1_2_p_t hours_p (uint1_2_p.derived(), 0u, 24u);

    /** Minute Parser: limit_d(0u, 59u)[uint2_p] */
    bounded2_p_t minutes_p (uint2_p.derived(), 0u, 59u);

    /** Second Parser: limit_d(0u, 59u)[uint2_p] */
    bounded2_p_t seconds_p (uint2_p.derived(), 0u, 59u);

    /** Cabin code parser: chset_p("A-Z") */
    chset_t cabin_code_p ("A-Z");

    /** Booking class code parser: chset_p("A-Z") */
    chset_t class_code_p ("A-Z");

    /** Passenger type parser: chset_p("A-Z") */
    chset_t passenger_type_p ("A-Z");

    /** Family code parser */
    int1_p_t family_code_p;
      
    /** Class Code List Parser: repeat_p(1,26)[chset_p("A-Z")] */
    repeat_p_t class_code_list_p (chset_t("A-Z").derived(), 1, 26);

    /** Stay duration Parser: limit_d(0u, 999u)[uint3_p] */
    bounded1_3_p_t stay_duration_p (uint1_3_p.derived(), 0u, 999u);


    // //////////////////////////////////////////////////////////////////
    //  (Boost Spirit) Grammar Definition
    // //////////////////////////////////////////////////////////////////

    // //////////////////////////////////////////////////////////////////
    YieldParser::YieldParser (stdair::BomRoot& ioBomRoot, YieldStruct& ioYield) 
      : _bomRoot (ioBomRoot), _yield (ioYield) {
    }

    // //////////////////////////////////////////////////////////////////
    template<typename ScannerT>
    YieldParser::definition<ScannerT>::definition (YieldParser const& self) {

      yield_list = *( boost::spirit::classic::comment_p("//")
                      | boost::spirit::classic::comment_p("/*", "*/")
                      | yield )
        ;
      
      yield = date[storeSnapshotDate(self._yield)]
        >> '/' >> airline_code
        >> yield_end[doEndYield (self._bomRoot, self._yield)]
        ;

      yield_end =
        boost::spirit::classic::ch_p(';')
        ;
      
      airline_code =
        boost::spirit::classic::lexeme_d[
                                (airline_code_p)[storeAirlineCode(self._yield)]
                                ]
        ;
        
      date =
        boost::spirit::classic::lexeme_d[
                                (day_p)[boost::spirit::classic::assign_a(self._yield._itDay)]
                                >> (month_p)[boost::spirit::classic::assign_a(self._yield._itMonth)]
                                >> (year_p)[boost::spirit::classic::assign_a(self._yield._itYear)]
                                ]
        ;
	 
      time =
        boost::spirit::classic::lexeme_d[
                                (hours_p)[boost::spirit::classic::assign_a(self._yield._itHours)]
                                >> (minutes_p)[boost::spirit::classic::assign_a(self._yield._itMinutes)]
                                >> !((seconds_p)[boost::spirit::classic::assign_a(self._yield._itSeconds)])
                                ]
        ;
      

      // BOOST_SPIRIT_DEBUG_NODE (YieldParser);
      BOOST_SPIRIT_DEBUG_NODE (yield_list);
      BOOST_SPIRIT_DEBUG_NODE (yield);
      BOOST_SPIRIT_DEBUG_NODE (yield_end); 
      BOOST_SPIRIT_DEBUG_NODE (airline_code);
      BOOST_SPIRIT_DEBUG_NODE (date);
      BOOST_SPIRIT_DEBUG_NODE (time);
   }

    // //////////////////////////////////////////////////////////////////
    template<typename ScannerT>
    boost::spirit::classic::rule<ScannerT> const&
    YieldParser::definition<ScannerT>::start() const {
      return yield_list;
    }
    
  }


  /////////////////////////////////////////////////////////////////////////
  //
  //  Entry class for the file parser
  //
  /////////////////////////////////////////////////////////////////////////

  // //////////////////////////////////////////////////////////////////////
  YieldFileParser::YieldFileParser (stdair::BomRoot& ioBomRoot,
                                    const std::string& iFilename)
    : _filename (iFilename), _bomRoot (ioBomRoot) {
    init();
  }

  // //////////////////////////////////////////////////////////////////////
  void YieldFileParser::init() {
    // Open the file
    _startIterator = iterator_t (_filename);

    // Check the filename exists and can be open
    if (!_startIterator) {
      std::ostringstream oMessage;
      oMessage << "The file " << _filename << " can not be open.";
      throw stdair::FileNotFoundException (oMessage.str());
    }

    // Create an EOF iterator
    _endIterator = _startIterator.make_end();
  }
    
  // //////////////////////////////////////////////////////////////////////
  bool YieldFileParser::generateYieldStore () {
    bool oResult = false;
      
    STDAIR_LOG_DEBUG ("Parsing yield store input file: " << _filename);

    // Initialise the parser (grammar) with the helper/staging structure.
    YieldParserHelper::YieldParser lYieldParser (_bomRoot, _yield);
      
    // Launch the parsing of the file and, thanks to the doEndYield
    // call-back structure, the building of the whole BomRoot BOM
    // (i.e., including Yield, Yield, LegDate, SegmentDate, etc.)
    boost::spirit::classic::parse_info<iterator_t> info =
      boost::spirit::classic::parse (_startIterator, _endIterator, lYieldParser,
                                     boost::spirit::classic::space_p);

    // Retrieves whether or not the parsing was successful
    oResult = info.hit;
      
    const std::string hasBeenFullyReadStr = (info.full == true)?"":"not ";
    if (oResult == true) {
      STDAIR_LOG_DEBUG ("Parsing of yield store input file: " << _filename
                        << " succeeded: read " << info.length
                        << " characters. The input file has "
                        << hasBeenFullyReadStr
                        << "been fully read. Stop point: " << info.stop);
        
    } else {
      STDAIR_LOG_ERROR ("Parsing of yield store input file: " << _filename
                        << " failed: read " << info.length
                        << " characters. The input file has "
                        << hasBeenFullyReadStr
                        << "been fully read. Stop point: " << info.stop);
      throw stdair::ParserException ("Parsing of yield store input file: "
                                     + _filename + " failed");
    }

    return oResult;
  }
    
}
