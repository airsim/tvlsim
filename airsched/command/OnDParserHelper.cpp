// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// StdAir
#include <stdair/basic/BasFileMgr.hpp>
#include <stdair/bom/BomRoot.hpp>
#include <stdair/service/Logger.hpp>
// AIRSCHED
#include <airsched/command/OnDParserHelper.hpp>
#include <airsched/command/OnDPeriodGenerator.hpp>

namespace AIRSCHED {

  namespace OnDParserHelper {

    // //////////////////////////////////////////////////////////////////////
    //
    //  Semantic actions
    //
    // //////////////////////////////////////////////////////////////////////
      
    ParserSemanticAction::
    ParserSemanticAction (OnDPeriodStruct& ioOnDPeriod)
      : _onDPeriod (ioOnDPeriod) {
    }

    // //////////////////////////////////////////////////////////////////////
    storeOrigin::storeOrigin (OnDPeriodStruct& ioOnDPeriod)
      : ParserSemanticAction (ioOnDPeriod) {
    }

    // //////////////////////////////////////////////////////////////////////
    void storeOrigin::operator() (iterator_t iStr,
                                  iterator_t iStrEnd) const {
      std::string lOrigin (iStr, iStrEnd);
      //STDAIR_LOG_DEBUG ( "Origin: " << lOrigin << std::endl);

      // Set the origin
      _onDPeriod._origin = lOrigin;
      _onDPeriod._nbOfAirlines = 0;
      _onDPeriod._airlineCode = "";
      _onDPeriod._classCode = "";
      _onDPeriod._airlineCodeList.clear();
      _onDPeriod._classCodeList.clear(); 
    }

    // //////////////////////////////////////////////////////////////////////
    storeDestination::storeDestination (OnDPeriodStruct& ioOnDPeriod)
      : ParserSemanticAction (ioOnDPeriod) {
    }

    // //////////////////////////////////////////////////////////////////////
    void storeDestination::operator() (iterator_t iStr,
                                       iterator_t iStrEnd) const {
      std::string lDestination (iStr, iStrEnd);
      //STDAIR_LOG_DEBUG ("Destination: " << lDestination << std::endl);

      // Set the destination
      _onDPeriod._destination = lDestination;
    }

    // //////////////////////////////////////////////////////////////////////
    storeDateRangeStart::
    storeDateRangeStart (OnDPeriodStruct& ioOnDPeriod)
      : ParserSemanticAction (ioOnDPeriod) {
    }
    
    // //////////////////////////////////////////////////////////////////////
    void storeDateRangeStart::operator() (iterator_t iStr,
                                          iterator_t iStrEnd) const {
      _onDPeriod._dateRangeStart = _onDPeriod.getDate();
      /*STDAIR_LOG_DEBUG ("Date Range Start: "
        << _onDPeriod._dateRangeStart << std::endl);*/
        
      // Reset the number of seconds
      _onDPeriod._itSeconds = 0;
    }
      
    // //////////////////////////////////////////////////////////////////////
    storeDateRangeEnd::
    storeDateRangeEnd (OnDPeriodStruct& ioOnDPeriod)
      : ParserSemanticAction (ioOnDPeriod) {
    }
    
    // //////////////////////////////////////////////////////////////////////
    void storeDateRangeEnd::operator() (iterator_t iStr,
                                        iterator_t iStrEnd) const {
      // As a Boost date period (COM::DatePeriod_T) defines the last day of
      // the period to be end-date - one day, we have to add one day to that
      // end date before.
      const stdair::DateOffset_T oneDay (1);
      _onDPeriod._dateRangeEnd = _onDPeriod.getDate() + oneDay;
      /*STDAIR_LOG_DEBUG ( "Date Range End: "
        << _onDPeriod._dateRangeEnd << std::endl);*/

      // Transform the date pair (i.e., the date range) into a date period
      _onDPeriod._datePeriod =
        stdair::DatePeriod_T (_onDPeriod._dateRangeStart,
                              _onDPeriod._dateRangeEnd);
        
      // Reset the number of seconds
      _onDPeriod._itSeconds = 0;
    }

    // //////////////////////////////////////////////////////////////////////
    storeStartRangeTime::
    storeStartRangeTime (OnDPeriodStruct& ioOnDPeriod)
      : ParserSemanticAction (ioOnDPeriod) {
    }
    
    // //////////////////////////////////////////////////////////////////////
    void storeStartRangeTime::operator() (iterator_t iStr,
                                          iterator_t iStrEnd) const {
      _onDPeriod._timeRangeStart = _onDPeriod.getTime();
        
      // Reset the number of seconds
      _onDPeriod._itSeconds = 0;
    }

    // //////////////////////////////////////////////////////////////////////
    storeEndRangeTime::
    storeEndRangeTime (OnDPeriodStruct& ioOnDPeriod)
      : ParserSemanticAction (ioOnDPeriod) {
    }

    // //////////////////////////////////////////////////////////////////////
    void storeEndRangeTime::operator() (iterator_t iStr,
                                        iterator_t iStrEnd) const {
      _onDPeriod._timeRangeEnd = _onDPeriod.getTime();
        
      // Reset the number of seconds
      _onDPeriod._itSeconds = 0;
    }

    // //////////////////////////////////////////////////////////////////////
    storeAirlineCode::
    storeAirlineCode (OnDPeriodStruct& ioOnDPeriod)
      : ParserSemanticAction (ioOnDPeriod) {
    }
    
    // //////////////////////////////////////////////////////////////////////
    void storeAirlineCode::operator() (iterator_t iStr,
                                       iterator_t iStrEnd) const { 
      const std::string lAirlineCodeStr (iStr, iStrEnd);
      const stdair::AirlineCode_T lAirlineCode(lAirlineCodeStr);
      // Test if the OnD Period Struct stands for interline products
      if (_onDPeriod._airlineCodeList.size() > 0) {
        // update the airline code
        std::ostringstream ostr;
        ostr << _onDPeriod._airlineCode << lAirlineCode;
        _onDPeriod._airlineCode = ostr.str();
        // Update the number of airlines if necessary
        const stdair::AirlineCode_T lPreviousAirlineCode =
          _onDPeriod._airlineCodeList.back();
        if (lPreviousAirlineCode != lAirlineCode) {
          _onDPeriod._nbOfAirlines = _onDPeriod._nbOfAirlines + 1;
        }
      }
      else {
        _onDPeriod._airlineCode = lAirlineCode; 
        _onDPeriod._nbOfAirlines = 1;
      }
      _onDPeriod._airlineCodeList.push_back (lAirlineCode);
        
      //STDAIR_LOG_DEBUG ( "Airline code: " << lAirlineCode << std::endl);
    }

    // //////////////////////////////////////////////////////////////////////
    storeClassCode::
    storeClassCode (OnDPeriodStruct& ioOnDPeriod)
      : ParserSemanticAction (ioOnDPeriod) {
    }
    
    // //////////////////////////////////////////////////////////////////////
    void storeClassCode::operator() (char iChar) const {
      std::ostringstream ostr;
      ostr << iChar;
      std::string classCodeStr = ostr.str();
      const stdair::ClassCode_T lClassCode (classCodeStr);
      _onDPeriod._classCodeList.push_back(lClassCode);
      /*STDAIR_LOG_DEBUG ("Class Code: "
        << lClassCode << std::endl);*/
      // Insertion of this class Code in the whole classCode name
      std::ostringstream ostrr;
      ostrr << _onDPeriod._classCode << classCodeStr;
      _onDPeriod._classCode = ostrr.str();
        
    }
      
    // //////////////////////////////////////////////////////////////////////
    doEndOnD::doEndOnD (stdair::BomRoot& ioBomRoot, OnDPeriodStruct& ioOnDPeriod)
      : ParserSemanticAction (ioOnDPeriod),
        _bomRoot (ioBomRoot) {
    }
    
    // //////////////////////////////////////////////////////////////////////
    void doEndOnD::operator() (iterator_t iStr, iterator_t iStrEnd) const {

      // DEBUG: Display the result
      // STDAIR_LOG_DEBUG ("FareRule " << _onDPeriod.describe());

      // Generation of the O&D-Period object.
      OnDPeriodGenerator::createOnDPeriod (_bomRoot, _onDPeriod);
    }
      
    // ///////////////////////////////////////////////////////////////////////
    //
    //  Utility Parsers
    //
    // ///////////////////////////////////////////////////////////////////////
      
    /** 2-digit-integer parser */
    uint2_p_t uint2_p;
    
    /** 4-digit-integer parser */
    uint4_p_t uint4_p;
    
    /** Up-to-4-digit-integer parser */
    uint1_4_p_t uint1_4_p;

    /** Sequence of (capital) alphabetic characters: chset_p("A-Z") */
    chset_t alpha_cap_set_p ("A-Z");
            
    /** Airport Parser: repeat_p(3)[chset_p("0-9A-Z")] */
    repeat_p_t airport_p (chset_t("0-9A-Z").derived(), 3, 3);
      
    /** Airline Code Parser: repeat_p(2,3)[chset_p("0-9A-Z")] */
    repeat_p_t airline_code_p (alpha_cap_set_p.derived(), 2, 3);
      
    /** Year Parser: limit_d(2000u, 2099u)[uint4_p] */
    bounded4_p_t year_p (uint4_p.derived(), 2000u, 2099u);
      
    /** Month Parser: limit_d(1u, 12u)[uint2_p] */
    bounded2_p_t month_p (uint2_p.derived(), 1u, 12u);

    /** Day Parser: limit_d(1u, 31u)[uint2_p] */
    bounded2_p_t day_p (uint2_p.derived(), 1u, 31u);
      
    /** Hour Parser: limit_d(0u, 23u)[uint2_p] */
    bounded2_p_t hours_p (uint2_p.derived(), 0u, 23u);

    /** Minute Parser: limit_d(0u, 59u)[uint2_p] */
    bounded2_p_t minutes_p (uint2_p.derived(), 0u, 59u);

    /** Second Parser: limit_d(0u, 59u)[uint2_p] */
    bounded2_p_t seconds_p (uint2_p.derived(), 0u, 59u);

    /** Class Code Parser: chset_p("A-Z") */
    chset_t class_code_p ("A-Z");

    /////////////////////////////////////////////////////////////////////////
    //
    //  (Boost Spirit) Grammar Definition
    //
    /////////////////////////////////////////////////////////////////////////

    // //////////////////////////////////////////////////////////////////////
    OnDParser::
    OnDParser (stdair::BomRoot& ioBomRoot, OnDPeriodStruct& ioOnDPeriod) 
      : _bomRoot (ioBomRoot), _onDPeriod (ioOnDPeriod) {
    }

    // //////////////////////////////////////////////////////////////////////
    template<typename ScannerT>
    OnDParser::definition<ScannerT>::definition (OnDParser const& self) {

      ond_list = *( boost::spirit::classic::comment_p("//")
                    | boost::spirit::classic::comment_p("/*", "*/")
                    | ond )
        ;

      ond = ond_key
        >> +( ';' >> segment )
        >> ond_end[doEndOnD(self._bomRoot, self._onDPeriod)]
        ;

      ond_end = boost::spirit::classic::ch_p(';')
        ;

      ond_key = (airport_p)[storeOrigin(self._onDPeriod)]
        >> ';' >> (airport_p)[storeDestination(self._onDPeriod)]
        >> ';' >> date[storeDateRangeStart(self._onDPeriod)]
        >> ';' >> date[storeDateRangeEnd(self._onDPeriod)]
        >> ';' >> time[storeStartRangeTime(self._onDPeriod)]
        >> ';' >> time[storeEndRangeTime(self._onDPeriod)]
        ;
        
      date = boost::spirit::classic::
        lexeme_d[(year_p)[boost::spirit::classic::
                          assign_a(self._onDPeriod._itYear)]
                 >> '-'
                 >> (month_p)[boost::spirit::classic::
                              assign_a(self._onDPeriod._itMonth)]
                 >> '-'
                 >> (day_p)[boost::spirit::classic::
                            assign_a(self._onDPeriod._itDay)]]
        ;
           
      time = boost::spirit::classic::
        lexeme_d[(hours_p)[boost::spirit::classic::
                           assign_a(self._onDPeriod._itHours)]
                 >> ':'
                 >> (minutes_p)[boost::spirit::classic::
                                assign_a(self._onDPeriod._itMinutes)]
                 >> !(':' >> (seconds_p)[boost::spirit::classic::
                                         assign_a(self._onDPeriod._itSeconds)])]
        ;

      segment = boost::spirit::classic::
        lexeme_d[(airline_code_p)[storeAirlineCode(self._onDPeriod)]]
        >> ';' >> (class_code_p)[storeClassCode(self._onDPeriod)]
        ;

      //BOOST_SPIRIT_DEBUG_NODE (OnDParser);
      BOOST_SPIRIT_DEBUG_NODE (ond_list);
      BOOST_SPIRIT_DEBUG_NODE (ond);
      BOOST_SPIRIT_DEBUG_NODE (segment);
      BOOST_SPIRIT_DEBUG_NODE (ond_key);
      BOOST_SPIRIT_DEBUG_NODE (ond_end);
      BOOST_SPIRIT_DEBUG_NODE (date);
      BOOST_SPIRIT_DEBUG_NODE (time);

    }

    // //////////////////////////////////////////////////////////////////////
    template<typename ScannerT>
    boost::spirit::classic::rule<ScannerT> const&
    OnDParser::definition<ScannerT>::start() const {
      return ond_list;
    }
  }
    
  /////////////////////////////////////////////////////////////////////////
  //
  //  Entry class for the file parser
  //
  /////////////////////////////////////////////////////////////////////////

  // //////////////////////////////////////////////////////////////////////
  OnDPeriodFileParser::OnDPeriodFileParser (const stdair::Filename_T& iFilename,
                                            stdair::BomRoot& ioBomRoot)
    : _filename (iFilename), _bomRoot (ioBomRoot) {
    init();
  }
    
  // //////////////////////////////////////////////////////////////////////
  void OnDPeriodFileParser::init() {
    // Check that the file exists and is readable
    const bool doesExistAndIsReadable =
      stdair::BasFileMgr::doesExistAndIsReadable (_filename);

    if (doesExistAndIsReadable == false) {
      STDAIR_LOG_ERROR ("The O&D file " << _filename
                        << " does not exist or can not be read.");
      
      throw OnDInputFileNotFoundException ("The O&D file " + _filename
                                           + " does not exist or can not be read");
    }
    
    // Open the file
    _startIterator = iterator_t (_filename);

    // Check that the filename exists and can be open
    if (!_startIterator) {
      STDAIR_LOG_DEBUG ("The O&D file " << _filename << " can not be open."
                        << std::endl);
      throw OnDInputFileNotFoundException ("The file " + _filename
                                           + " does not exist or can not be read");
    }
      
    // Create an EOF iterator
    _endIterator = _startIterator.make_end();
  }
    
  // //////////////////////////////////////////////////////////////////////
  bool OnDPeriodFileParser::generateOnDPeriods () {
    bool oResult = false;
      
    STDAIR_LOG_DEBUG ("Parsing O&D input file: " << _filename);

    // Initialise the parser (grammar) with the helper/staging structure.
    OnDParserHelper::OnDParser lODParser (_bomRoot, _onDPeriod);
      
    // Launch the parsing of the file and, thanks to the doEndOnD
    // call-back structure, filling the worldSchedule (Fares)
    boost::spirit::classic::parse_info<iterator_t> info =
      boost::spirit::classic::parse (_startIterator, _endIterator, lODParser, 
                                     boost::spirit::classic::space_p);

    // Retrieves whether or not the parsing was successful
    oResult = info.hit;
      
    const std::string hasBeenFullyReadStr = (info.full == true)?"":"not ";
    if (oResult == true) {
      STDAIR_LOG_DEBUG ("Parsing of O&D input file: " << _filename
                       << " succeeded: read " << info.length
                       << " characters. The input file has "
                       << hasBeenFullyReadStr
                       << "been fully read. Stop point: " << info.stop);
        
    } else {
      // TODO: decide whether to throw an exception
      STDAIR_LOG_ERROR ("Parsing of O&D input file: " << _filename
                       << " failed: read " << info.length
                       << " characters. The input file has "
                       << hasBeenFullyReadStr
                       << "been fully read. Stop point: " << info.stop);
    }

    return oResult;
  }
}
