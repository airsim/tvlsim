 // //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <string>
#include <vector>
#include <fstream>
// StdAir
#include <stdair/basic/BasFileMgr.hpp>
#include <stdair/bom/BomRoot.hpp>
#include <stdair/service/Logger.hpp>
// AirInv
#include <airinv/command/DCPParserHelper.hpp>
#include <airinv/command/DCPRuleGenerator.hpp>

namespace AIRINV {

  namespace DCPParserHelper {
      
    // //////////////////////////////////////////////////////////////////
    //  Semantic actions
    // //////////////////////////////////////////////////////////////////

    ParserSemanticAction::ParserSemanticAction (DCPRuleStruct& ioDCPRule)
      : _DCPRule (ioDCPRule) {
    }      
   
    // //////////////////////////////////////////////////////////////////
    storeDCPId::storeDCPId (DCPRuleStruct& ioDCPRule)
      : ParserSemanticAction (ioDCPRule) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeDCPId::operator() (unsigned int iDCPId,
                                  boost::spirit::qi::unused_type,
                                  boost::spirit::qi::unused_type) const {
      _DCPRule._DCPId = iDCPId;
      
      // DEBUG
      //STDAIR_LOG_DEBUG ( "DCP Id: " << _DCPRule._DCPId);

      _DCPRule._nbOfAirlines = 0;
      _DCPRule._airlineCode = "";
      _DCPRule._classCode = "";
      _DCPRule._airlineCodeList.clear();
      _DCPRule._classCodeList.clear();
      _DCPRule._classCodeListOfList.clear();
      _DCPRule._itSeconds = 0; 
    }
    
    // //////////////////////////////////////////////////////////////////
    storeOrigin ::
    storeOrigin (DCPRuleStruct& ioDCPRule)
      : ParserSemanticAction (ioDCPRule) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeOrigin::operator() (std::vector<char> iChar,
                                  boost::spirit::qi::unused_type,
                                  boost::spirit::qi::unused_type) const {
       stdair::AirportCode_T lOrigin (iChar.begin(), iChar.end());
       // DEBUG
       //STDAIR_LOG_DEBUG ( "Origin: " << lOrigin);
       _DCPRule._origin = lOrigin;
    }

    // //////////////////////////////////////////////////////////////////
    storeDestination ::
    storeDestination (DCPRuleStruct& ioDCPRule)
      : ParserSemanticAction (ioDCPRule) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeDestination::operator() (std::vector<char> iChar,
                                       boost::spirit::qi::unused_type,
                                       boost::spirit::qi::unused_type) const {
       stdair::AirportCode_T lDestination (iChar.begin(), iChar.end());
       // DEBUG
       //STDAIR_LOG_DEBUG ( "Destination: " << lDestination);
       _DCPRule._destination = lDestination;
    }
    
    // //////////////////////////////////////////////////////////////////
    storeDateRangeStart::
    storeDateRangeStart (DCPRuleStruct& ioDCPRule)
      : ParserSemanticAction (ioDCPRule) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeDateRangeStart::operator() (boost::spirit::qi::unused_type,
                                          boost::spirit::qi::unused_type,
                                          boost::spirit::qi::unused_type) const {
      _DCPRule._dateRangeStart = _DCPRule.getDate();
      // DEBUG
      //STDAIR_LOG_DEBUG ("Date Range Start: "<< _DCPRule._dateRangeStart);
    }

    // //////////////////////////////////////////////////////////////////
    storeDateRangeEnd::
    storeDateRangeEnd(DCPRuleStruct& ioDCPRule)
      : ParserSemanticAction (ioDCPRule) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeDateRangeEnd::operator() (boost::spirit::qi::unused_type,
                                        boost::spirit::qi::unused_type,
                                        boost::spirit::qi::unused_type) const {
       _DCPRule._dateRangeEnd = _DCPRule.getDate();
       // DEBUG
       //STDAIR_LOG_DEBUG ("Date Range End: " << _DCPRule._dateRangeEnd);
    }

    // //////////////////////////////////////////////////////////////////
    storeStartRangeTime::
    storeStartRangeTime (DCPRuleStruct& ioDCPRule)
      : ParserSemanticAction (ioDCPRule) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeStartRangeTime::operator() (boost::spirit::qi::unused_type,
                                          boost::spirit::qi::unused_type,
                                          boost::spirit::qi::unused_type) const {
      _DCPRule._timeRangeStart = _DCPRule.getTime();
      // DEBUG
      //STDAIR_LOG_DEBUG ("Time Range Start: " << _DCPRule._timeRangeStart);
      // Reset the number of seconds
      _DCPRule._itSeconds = 0;
    }

    // //////////////////////////////////////////////////////////////////
    storeEndRangeTime::
    storeEndRangeTime (DCPRuleStruct& ioDCPRule)
      : ParserSemanticAction (ioDCPRule) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeEndRangeTime::operator() (boost::spirit::qi::unused_type,
                                        boost::spirit::qi::unused_type,
                                        boost::spirit::qi::unused_type) const {
      _DCPRule._timeRangeEnd = _DCPRule.getTime();
      // DEBUG
      //STDAIR_LOG_DEBUG ("Time Range End: " << _DCPRule._timeRangeEnd);
      // Reset the number of seconds
      _DCPRule._itSeconds = 0;
    }
   
    // //////////////////////////////////////////////////////////////////
    storePOS ::
    storePOS (DCPRuleStruct& ioDCPRule)
      : ParserSemanticAction (ioDCPRule) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storePOS::operator() (std::vector<char> iChar,
                               boost::spirit::qi::unused_type,
                               boost::spirit::qi::unused_type) const {
      stdair::AirlineCode_T lPOS (iChar.begin(), iChar.end());
      _DCPRule._pos = lPOS;
      // DEBUG
      //STDAIR_LOG_DEBUG ("POS: " << _DCPRule._pos);
    }

    // //////////////////////////////////////////////////////////////////
    storeCabinCode ::
    storeCabinCode (DCPRuleStruct& ioDCPRule)
      : ParserSemanticAction (ioDCPRule) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeCabinCode::operator() (char iChar,
                                     boost::spirit::qi::unused_type,
                                     boost::spirit::qi::unused_type) const {
      std::ostringstream ostr;
      ostr << iChar;
      std::string cabinCodeStr = ostr.str();
      const stdair::CabinCode_T lCabinCode (cabinCodeStr);
      _DCPRule._cabinCode = lCabinCode;
     
      // DEBUG
      //STDAIR_LOG_DEBUG ("Cabin Code: " << lCabinCode);                 
    
    }

    // //////////////////////////////////////////////////////////////////
    storeChannel ::
    storeChannel (DCPRuleStruct& ioDCPRule)
      : ParserSemanticAction (ioDCPRule) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeChannel::operator() (std::vector<char> iChar,
                                   boost::spirit::qi::unused_type,
                                   boost::spirit::qi::unused_type) const {
      stdair::ChannelLabel_T lChannel (iChar.begin(), iChar.end());
      if (lChannel != "IN" && lChannel != "IF"
          && lChannel != "DN" && lChannel != "DF") {
        // DEBUG
        STDAIR_LOG_DEBUG ("Invalid channel " << lChannel);
      }
      _DCPRule._channel = lChannel;
      // DEBUG
      //STDAIR_LOG_DEBUG ("Channel: " << _DCPRule._channel);
    }
    
    // //////////////////////////////////////////////////////////////////
    storeAdvancePurchase ::
    storeAdvancePurchase (DCPRuleStruct& ioDCPRule)
      : ParserSemanticAction (ioDCPRule) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeAdvancePurchase::operator() (unsigned int iAdancePurchase,
                                           boost::spirit::qi::unused_type,
                                           boost::spirit::qi::unused_type) const {
      _DCPRule._advancePurchase = iAdancePurchase;
      // DEBUG
      //STDAIR_LOG_DEBUG ( "Advance Purchase: " << _DCPRule._advancePurchase);
    }
    
    // //////////////////////////////////////////////////////////////////
    storeSaturdayStay ::
    storeSaturdayStay (DCPRuleStruct& ioDCPRule)
      : ParserSemanticAction (ioDCPRule) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeSaturdayStay::operator() (char iSaturdayStay,
                                        boost::spirit::qi::unused_type,
                                        boost::spirit::qi::unused_type) const {
      bool lBool = false;
      if (iSaturdayStay == 'T') {
        lBool = true;
      } else {
        if (iSaturdayStay != 'F') {
          // DEBUG
          STDAIR_LOG_DEBUG ("Invalid saturdayStay char " << iSaturdayStay);
        }
      }
      stdair::SaturdayStay_T lSaturdayStay (lBool);
      _DCPRule._saturdayStay = lSaturdayStay;
      // DEBUG
      //STDAIR_LOG_DEBUG ("Saturday Stay: " << _DCPRule._saturdayStay);
    }

    // //////////////////////////////////////////////////////////////////
    storeChangeFees ::
    storeChangeFees (DCPRuleStruct& ioDCPRule)
      : ParserSemanticAction (ioDCPRule) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeChangeFees::operator() (char iChangefees,
                                      boost::spirit::qi::unused_type,
                                      boost::spirit::qi::unused_type) const {
      
      bool lBool = false;
      if (iChangefees == 'T') {
        lBool = true;
      } else {
        if (iChangefees != 'F') {
          // DEBUG
          STDAIR_LOG_DEBUG ("Invalid change fees char " << iChangefees);
        }
      }
      stdair::ChangeFees_T lChangefees (lBool);
      _DCPRule._changeFees = lChangefees;
      // DEBUG
      //STDAIR_LOG_DEBUG ("Change fees: " << _DCPRule._changeFees);
    }
    
    // //////////////////////////////////////////////////////////////////
    storeNonRefundable ::
    storeNonRefundable (DCPRuleStruct& ioDCPRule)
      : ParserSemanticAction (ioDCPRule) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeNonRefundable::operator() (char iNonRefundable,
                                         boost::spirit::qi::unused_type,
                                         boost::spirit::qi::unused_type) const {
       bool lBool = false;
       if (iNonRefundable == 'T') {
         lBool = true;
       } else {
         if (iNonRefundable != 'F') {
           // DEBUG
           STDAIR_LOG_DEBUG ("Invalid non refundable char " << iNonRefundable);
         }
       }
       stdair::NonRefundable_T lNonRefundable (lBool);
       _DCPRule._nonRefundable = lNonRefundable;
       // DEBUG
       //STDAIR_LOG_DEBUG ("Non refundable: " << _DCPRule._nonRefundable);
    }
    
    // //////////////////////////////////////////////////////////////////
    storeMinimumStay ::
    storeMinimumStay (DCPRuleStruct& ioDCPRule)
      : ParserSemanticAction (ioDCPRule) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeMinimumStay::operator() (unsigned int iMinStay,
                                       boost::spirit::qi::unused_type,
                                       boost::spirit::qi::unused_type) const {
      _DCPRule._minimumStay = iMinStay;
      // DEBUG
      //STDAIR_LOG_DEBUG ("Minimum Stay: " << _DCPRule._minimumStay );
    }

    // //////////////////////////////////////////////////////////////////
    storeDCP ::
    storeDCP (DCPRuleStruct& ioDCPRule)
      : ParserSemanticAction (ioDCPRule) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeDCP::operator() (double iDCP,
                                boost::spirit::qi::unused_type,
                                boost::spirit::qi::unused_type) const {
      _DCPRule._DCP = iDCP;
      // DEBUG
      //STDAIR_LOG_DEBUG ("DCP: " << _DCPRule._DCP);
    }

    // //////////////////////////////////////////////////////////////////
    storeAirlineCode ::
    storeAirlineCode (DCPRuleStruct& ioDCPRule)
      : ParserSemanticAction (ioDCPRule) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeAirlineCode::operator() (std::vector<char> iChar,
                                       boost::spirit::qi::unused_type,
                                       boost::spirit::qi::unused_type) const {

      bool lAlreadyInTheList = false;
      stdair::AirlineCode_T lAirlineCode (iChar.begin(), iChar.end());
      // Update the airline code
      _DCPRule._airlineCode = lAirlineCode;
      // Test if the DCPRule Struct stands for interline products
      if (_DCPRule._airlineCodeList.size() > 0) {
        _DCPRule._classCodeListOfList.push_back(_DCPRule._classCodeList);
        _DCPRule._classCodeList.clear();
        // Update the number of airlines if necessary
        std::vector<stdair::AirlineCode_T>::iterator Airline_iterator;
        for (Airline_iterator = _DCPRule._airlineCodeList.begin();
             Airline_iterator != _DCPRule._airlineCodeList.end();
             ++Airline_iterator) {
          stdair::AirlineCode_T lPreviousAirlineCode =
            *Airline_iterator;
          if (lPreviousAirlineCode == lAirlineCode) {
            lAlreadyInTheList = true;
            /*STDAIR_LOG_DEBUG ("Airline Code Already Existing: "
              << lAirlineCode);*/
          }
        }
        if (lAlreadyInTheList == false) {
          /*STDAIR_LOG_DEBUG ("New Airline Code: "
          << lAirlineCode);*/
          _DCPRule._airlineCodeList.push_back(lAirlineCode);
          _DCPRule._classCodeList.clear();
        }
      } else {
        /*STDAIR_LOG_DEBUG ("First Airline Code: "
          << lAirlineCode);*/
        _DCPRule._airlineCodeList.push_back (lAirlineCode);
      }
      // DEBUG
      //STDAIR_LOG_DEBUG ( "Airline code: " << lAirlineCode);
    }

    // //////////////////////////////////////////////////////////////////
    storeClass ::
    storeClass (DCPRuleStruct& ioDCPRule)
      : ParserSemanticAction (ioDCPRule) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeClass::operator() (std::vector<char> iChar,
                                 boost::spirit::qi::unused_type,
                                 boost::spirit::qi::unused_type) const {
      std::ostringstream ostr;
      for (std::vector<char>::const_iterator lItVector = iChar.begin();
         lItVector != iChar.end();
         lItVector++) {
        ostr << *lItVector;
      }
      std::string classCodeStr = ostr.str();
      // Insertion of this class Code list in the whole classCode name
      _DCPRule._classCodeList.push_back(classCodeStr);
      // DEBUG
      // STDAIR_LOG_DEBUG ("Class Code: " << classCodeStr);
    }
    
    // //////////////////////////////////////////////////////////////////
    doEndDCP::
    doEndDCP (stdair::BomRoot& ioBomRoot,
               DCPRuleStruct& ioDCPRule)
      : ParserSemanticAction (ioDCPRule),
        _bomRoot (ioBomRoot) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void doEndDCP::operator() (boost::spirit::qi::unused_type,
                                boost::spirit::qi::unused_type,
                                boost::spirit::qi::unused_type) const {
      // DEBUG
      // STDAIR_LOG_DEBUG ("Do End");
      // Generation of the DCP rule object.
      _DCPRule._classCodeListOfList.push_back(_DCPRule._classCodeList);
      DCPRuleGenerator::createDCPRule (_bomRoot, _DCPRule);
      STDAIR_LOG_DEBUG(_DCPRule.describe());
    }  

    // ///////////////////////////////////////////////////////////////////
    //
    //  Utility Parsers
    //
    // ///////////////////////////////////////////////////////////////////
    /** Namespaces. */
    namespace bsq = boost::spirit::qi;
    namespace bsa = boost::spirit::ascii;
    
    /** 1-digit-integer parser */
    stdair::int1_p_t int1_p;
    
    /** 2-digit-integer parser */
    stdair::uint2_p_t uint2_p;

    /** 4-digit-integer parser */
    stdair::uint4_p_t uint4_p;
    
    /** Up-to-4-digit-integer parser */
    stdair::uint1_4_p_t uint1_4_p;

    /** Time element parsers. */
    stdair::hour_p_t hour_p;
    stdair::minute_p_t minute_p;
    stdair::second_p_t second_p;

    /** Date element parsers. */
    stdair::year_p_t year_p;
    stdair::month_p_t month_p;
    stdair::day_p_t day_p;
        
    // //////////////////////////////////////////////////////////////////
    //  (Boost Spirit) Grammar Definition
    // //////////////////////////////////////////////////////////////////

    // //////////////////////////////////////////////////////////////////
    DCPRuleParser::DCPRuleParser (stdair::BomRoot& ioBomRoot,
                                    DCPRuleStruct& ioDCPRule) :
      DCPRuleParser::base_type(start),
      _bomRoot(ioBomRoot), _DCPRule(ioDCPRule) {

      start = *(comments | DCP_rule);

      comments = (bsq::lexeme[bsq::repeat(2)[bsa::char_('/')]
                              >> +(bsa::char_ - bsq::eol)
                              >> bsq::eol]
                  | bsq::lexeme[bsa::char_('/') >>bsa::char_('*') 
                                >> +(bsa::char_ - bsa::char_('*')) 
                                >> bsa::char_('*') >> bsa::char_('/')]);

      DCP_rule = DCP_key
        >> +( ';' >> segment )
        >> DCP_rule_end[doEndDCP(_bomRoot, _DCPRule)];

      DCP_rule_end = bsa::char_(';');

      DCP_key = DCP_id
        >> ';' >> origin >> ';' >> destination
        >> ';' >> dateRangeStart >> ';' >> dateRangeEnd
        >> ';' >> timeRangeStart >> ';' >> timeRangeEnd
        >> ';' >> position >>  ';' >> cabinCode >> ';' >> channel
        >> ';' >> advancePurchase >> ';' >> saturdayStay
        >> ';' >> changeFees >> ';' >> nonRefundable
        >> ';' >> minimumStay >> ';' >> DCP;

      DCP_id = uint1_4_p[storeDCPId(_DCPRule)];

      origin = bsq::repeat(3)[bsa::char_("A-Z")][storeOrigin(_DCPRule)];
      
      destination =  
        bsq::repeat(3)[bsa::char_("A-Z")][storeDestination(_DCPRule)];
      
      dateRangeStart = date[storeDateRangeStart(_DCPRule)];

      dateRangeEnd = date[storeDateRangeEnd(_DCPRule)];
      
      date = bsq::lexeme
        [year_p[boost::phoenix::ref(_DCPRule._itYear) = bsq::labels::_1]
        >> '-'
        >> month_p[boost::phoenix::ref(_DCPRule._itMonth) = bsq::labels::_1]
        >> '-'
        >> day_p[boost::phoenix::ref(_DCPRule._itDay) = bsq::labels::_1] ];

      timeRangeStart = time[storeStartRangeTime(_DCPRule)];
      
      timeRangeEnd = time[storeEndRangeTime(_DCPRule)];

      time =  bsq::lexeme
        [hour_p[boost::phoenix::ref(_DCPRule._itHours) = bsq::labels::_1]
        >> ':'
        >> minute_p[boost::phoenix::ref(_DCPRule._itMinutes) = bsq::labels::_1]      
        >> - (':' >> second_p[boost::phoenix::ref(_DCPRule._itSeconds) = bsq::labels::_1]) ];
      
      position = bsq::repeat(3)[bsa::char_("A-Z")][storePOS(_DCPRule)];

      cabinCode = bsa::char_("A-Z")[storeCabinCode(_DCPRule)];
            
      channel = bsq::repeat(2)[bsa::char_("A-Z")][storeChannel(_DCPRule)];
      
      advancePurchase = uint1_4_p[storeAdvancePurchase(_DCPRule)];

      saturdayStay = bsa::char_("A-Z")[storeSaturdayStay(_DCPRule)];

      changeFees = bsa::char_("A-Z")[storeChangeFees(_DCPRule)];

      nonRefundable = bsa::char_("A-Z")[storeNonRefundable(_DCPRule)];
      
      minimumStay = uint1_4_p[storeMinimumStay(_DCPRule)];

      DCP = bsq::double_[storeDCP(_DCPRule)];
      
      segment = bsq::repeat(2)[bsa::char_("A-Z")][storeAirlineCode(_DCPRule)]
        //>> ';'
        //>> bsa::char_("A-Z")[storeClass(_DCPRule)]
        >> +(';' >> list_class);

      list_class = bsq::repeat(1,bsq::inf)[bsa::char_("A-Z")][storeClass(_DCPRule)];

      //BOOST_SPIRIT_DEBUG_NODE (DCPRuleParser);
      BOOST_SPIRIT_DEBUG_NODE (start);
      BOOST_SPIRIT_DEBUG_NODE (comments);
      BOOST_SPIRIT_DEBUG_NODE (DCP_rule);
      BOOST_SPIRIT_DEBUG_NODE (DCP_rule_end);
      BOOST_SPIRIT_DEBUG_NODE (DCP_key);
      BOOST_SPIRIT_DEBUG_NODE (DCP_id);
      BOOST_SPIRIT_DEBUG_NODE (origin);
      BOOST_SPIRIT_DEBUG_NODE (destination);
      BOOST_SPIRIT_DEBUG_NODE (dateRangeStart);
      BOOST_SPIRIT_DEBUG_NODE (dateRangeEnd);
      BOOST_SPIRIT_DEBUG_NODE (date);
      BOOST_SPIRIT_DEBUG_NODE (timeRangeStart);
      BOOST_SPIRIT_DEBUG_NODE (timeRangeEnd);
      BOOST_SPIRIT_DEBUG_NODE (time);
      BOOST_SPIRIT_DEBUG_NODE (position);
      BOOST_SPIRIT_DEBUG_NODE (cabinCode);
      BOOST_SPIRIT_DEBUG_NODE (channel);
      BOOST_SPIRIT_DEBUG_NODE (advancePurchase);
      BOOST_SPIRIT_DEBUG_NODE (saturdayStay);
      BOOST_SPIRIT_DEBUG_NODE (changeFees);
      BOOST_SPIRIT_DEBUG_NODE (nonRefundable);
      BOOST_SPIRIT_DEBUG_NODE (minimumStay);
      BOOST_SPIRIT_DEBUG_NODE (DCP);
      BOOST_SPIRIT_DEBUG_NODE (segment);
      BOOST_SPIRIT_DEBUG_NODE (list_class);
    }
  }

  /////////////////////////////////////////////////////////////////////////
  //
  //  Entry class for the file parser
  //
  /////////////////////////////////////////////////////////////////////////
    
  // //////////////////////////////////////////////////////////////////////
  DCPRuleFileParser::
  DCPRuleFileParser (stdair::BomRoot& ioBomRoot,
                      const stdair::Filename_T& iFilename)
    : _filename (iFilename), _bomRoot (ioBomRoot) {
    init();
  }

  // //////////////////////////////////////////////////////////////////////
  void DCPRuleFileParser::init() {
    // Check that the file exists and is readable
    const bool doesExistAndIsReadable =
      stdair::BasFileMgr::doesExistAndIsReadable (_filename);

    if (doesExistAndIsReadable == false) {
      STDAIR_LOG_ERROR ("The DCP schedule file " << _filename
                        << " does not exist or can not be  read.");
      
      throw DCPInputFileNotFoundException ("The DCP file " + _filename + " does not exist or can not be read");
    }
  }
    
  // //////////////////////////////////////////////////////////////////////
  bool DCPRuleFileParser::generateDCPRules () {
      
    STDAIR_LOG_DEBUG ("Parsing DCP input file: " << _filename);

    // File to be parsed
    const std::string* lFileName = &_filename;
    const char *lChar = (*lFileName).c_str();
    std::ifstream fileToBeParsed(lChar, std::ios_base::in);

    // Check the filename exists and can be open
    if (fileToBeParsed == false) {
      STDAIR_LOG_ERROR ("The DCP file " << _filename << " can not be open."
                          << std::endl);

      throw DCPInputFileNotFoundException ("The file " + _filename + " does not exist or can not be read");
    }
    
    // Create an input iterator
    stdair::base_iterator_t inputBegin (fileToBeParsed);

    // Convert input iterator to an iterator usable by spirit parser  
    stdair::iterator_t 
      start (boost::spirit::make_default_multi_pass (inputBegin));
    stdair::iterator_t end;

    // Initialise the parser (grammar) with the helper/staging structure.
    DCPParserHelper::DCPRuleParser lFPParser(_bomRoot, _DCPRule);
      
    // Launch the parsing of the file and, thanks to the doEndDCP
    // call-back structure, the building of the whole BomRoot BOM

    const bool hasParsingBeenSuccesful = 
       boost::spirit::qi::phrase_parse (start, end, lFPParser,
                                        boost::spirit::ascii::space);
      
    if (hasParsingBeenSuccesful == false) {
      // TODO: decide whether to throw an exceqption
      STDAIR_LOG_ERROR ("Parsing of DCP input file: " << _filename
                        << " failed");
    }
    if  (start != end) {
      // TODO: decide whether to throw an exception
      STDAIR_LOG_ERROR ("Parsing of DCP input file: " << _filename
                        << " failed");
    }
    if (hasParsingBeenSuccesful == true && start == end) {
      STDAIR_LOG_DEBUG ("Parsing of DCP input file: " << _filename
      << " succeeded");
    }
    return hasParsingBeenSuccesful;
  }
    
}
