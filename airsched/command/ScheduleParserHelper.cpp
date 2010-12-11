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
#include <airsched/command/ScheduleParserHelper.hpp>
#include <airsched/command/InventoryGenerator.hpp>

namespace AIRSCHED {

  namespace ScheduleParserHelper {
      
    // //////////////////////////////////////////////////////////////////
    //  Semantic actions
    // //////////////////////////////////////////////////////////////////

    ParserSemanticAction::
    ParserSemanticAction (FlightPeriodStruct& ioFlightPeriod)
      : _flightPeriod (ioFlightPeriod) {
    }      

    // //////////////////////////////////////////////////////////////////
    storeAirlineCode::
    storeAirlineCode (FlightPeriodStruct& ioFlightPeriod)
      : ParserSemanticAction (ioFlightPeriod) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeAirlineCode::operator() (iterator_t iStr,
                                       iterator_t iStrEnd) const { 
      const stdair::AirlineCode_T lAirlineCode (iStr, iStrEnd);
      _flightPeriod._airlineCode = lAirlineCode;
                
      // As that's the beginning of a new flight, the list of legs
      // must be reset
      _flightPeriod._legList.clear();
    }

    // //////////////////////////////////////////////////////////////////
    storeFlightNumber::
    storeFlightNumber (FlightPeriodStruct& ioFlightPeriod)
      : ParserSemanticAction (ioFlightPeriod) {
    }

    // //////////////////////////////////////////////////////////////////
    void storeFlightNumber::operator() (unsigned int iNumber) const { 
      _flightPeriod._flightNumber = iNumber;
    }

    // //////////////////////////////////////////////////////////////////
    storeDateRangeStart::
    storeDateRangeStart (FlightPeriodStruct& ioFlightPeriod)
      : ParserSemanticAction (ioFlightPeriod) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeDateRangeStart::operator() (iterator_t iStr,
                                          iterator_t iStrEnd) const {
      _flightPeriod._dateRangeStart = _flightPeriod.getDate();
        
      // Reset the number of seconds
      _flightPeriod._itSeconds = 0;
    }
      
    // //////////////////////////////////////////////////////////////////
    storeDateRangeEnd::
    storeDateRangeEnd (FlightPeriodStruct& ioFlightPeriod)
      : ParserSemanticAction (ioFlightPeriod) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeDateRangeEnd::operator() (iterator_t iStr,
                                        iterator_t iStrEnd) const {
      // As a Boost date period (DatePeriod_T) defines the last day of
      // the period to be end-date - one day, we have to add one day to that
      // end date before.
      const stdair::DateOffset_T oneDay (1);
      _flightPeriod._dateRangeEnd = _flightPeriod.getDate() + oneDay;

      // Transform the date pair (i.e., the date range) into a date period
      _flightPeriod._dateRange =
        stdair::DatePeriod_T (_flightPeriod._dateRangeStart,
                              _flightPeriod._dateRangeEnd);
        
      // Reset the number of seconds
      _flightPeriod._itSeconds = 0;
    }

    // //////////////////////////////////////////////////////////////////
    storeDow::storeDow (FlightPeriodStruct& ioFlightPeriod)
      : ParserSemanticAction (ioFlightPeriod) {
    }

    // //////////////////////////////////////////////////////////////////
    void storeDow::operator() (iterator_t iStr, iterator_t iStrEnd) const {
      stdair::DOW_String_T lDow (iStr, iStrEnd);
      _flightPeriod._dow = lDow;
    }
      
    // //////////////////////////////////////////////////////////////////
    storeLegBoardingPoint::
    storeLegBoardingPoint (FlightPeriodStruct& ioFlightPeriod)
      : ParserSemanticAction (ioFlightPeriod) {
    }

    // //////////////////////////////////////////////////////////////////
    void storeLegBoardingPoint::operator() (iterator_t iStr,
                                            iterator_t iStrEnd) const {
      stdair::AirportCode_T lBoardingPoint (iStr, iStrEnd);

      // If a leg has already been parsed, add it to the FlightPeriod
      if (_flightPeriod._legAlreadyDefined == true) {
        _flightPeriod._legList.push_back (_flightPeriod._itLeg);
      } else {
        _flightPeriod._legAlreadyDefined = true;
      }
        
      // Set the (new) boarding point
      _flightPeriod._itLeg._boardingPoint = lBoardingPoint;
      
      // As that's the beginning of a new leg, the list of cabins
      // must be reset
      _flightPeriod._itLeg._cabinList.clear();

      // Add the airport code if it is not already stored in the airport lists
      _flightPeriod.addAirport (lBoardingPoint);
    }

    // //////////////////////////////////////////////////////////////////
    storeLegOffPoint::
    storeLegOffPoint (FlightPeriodStruct& ioFlightPeriod)
      : ParserSemanticAction (ioFlightPeriod) {
    }

    // //////////////////////////////////////////////////////////////////
    void storeLegOffPoint::operator() (iterator_t iStr,
                                       iterator_t iStrEnd) const {
      stdair::AirportCode_T lOffPoint (iStr, iStrEnd);
      _flightPeriod._itLeg._offPoint = lOffPoint;

      // Add the airport code if it is not already stored in the airport lists
      _flightPeriod.addAirport (lOffPoint);
    }

    // //////////////////////////////////////////////////////////////////
    storeBoardingTime::
    storeBoardingTime (FlightPeriodStruct& ioFlightPeriod)
      : ParserSemanticAction (ioFlightPeriod) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeBoardingTime::operator() (iterator_t iStr,
                                        iterator_t iStrEnd) const {
      _flightPeriod._itLeg._boardingTime = _flightPeriod.getTime();
        
      // Reset the number of seconds
      _flightPeriod._itSeconds = 0;

      // Reset the date off-set
      _flightPeriod._dateOffset = 0;
    }

    // //////////////////////////////////////////////////////////////////
    storeOffTime::
    storeOffTime (FlightPeriodStruct& ioFlightPeriod)
      : ParserSemanticAction (ioFlightPeriod) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeOffTime::operator() (iterator_t iStr,
                                   iterator_t iStrEnd) const {
      _flightPeriod._itLeg._offTime = _flightPeriod.getTime();
        
      // Reset the number of seconds
      _flightPeriod._itSeconds = 0;

      // As the boarding date off set is optional, it can be set only
      // afterwards, based on the staging date off-set value
      // (_flightPeriod._dateOffset).
      const stdair::DateOffset_T lDateOffset (_flightPeriod._dateOffset);
      _flightPeriod._itLeg._boardingDateOffset = lDateOffset;
    }

    // //////////////////////////////////////////////////////////////////
    storeElapsedTime::
    storeElapsedTime (FlightPeriodStruct& ioFlightPeriod)
      : ParserSemanticAction (ioFlightPeriod) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeElapsedTime::operator() (iterator_t iStr,
                                       iterator_t iStrEnd) const {
      _flightPeriod._itLeg._elapsed = _flightPeriod.getTime();
        
      // Reset the number of seconds
      _flightPeriod._itSeconds = 0;

      // As the boarding date off set is optional, it can be set only
      // afterwards, based on the staging date off-set value
      // (_flightPeriod._dateOffset).
      const stdair::DateOffset_T lDateOffset (_flightPeriod._dateOffset);
      _flightPeriod._itLeg._offDateOffset = lDateOffset;
    }

    // //////////////////////////////////////////////////////////////////
    storeLegCabinCode::
    storeLegCabinCode (FlightPeriodStruct& ioFlightPeriod)
      : ParserSemanticAction (ioFlightPeriod) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeLegCabinCode::operator() (char iChar) const { 
      _flightPeriod._itLegCabin._cabinCode = iChar; 
      //std::cout << "Cabin code: " << iChar << std::endl;
    }

    // //////////////////////////////////////////////////////////////////
    storeCapacity::
    storeCapacity (FlightPeriodStruct& ioFlightPeriod)
      : ParserSemanticAction (ioFlightPeriod) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeCapacity::operator() (double iReal) const { 
      _flightPeriod._itLegCabin._capacity = iReal; 
      //std::cout << "Capacity: " << iReal << std::endl;

      // The capacity is the last (according to the arrival order
      // within the schedule input file) detail of the leg cabin. Hence,
      // when a capacity is parsed, it means that the full cabin
      // details have already been parsed as well: the cabin can
      // thus be added to the leg.
      _flightPeriod._itLeg._cabinList.push_back (_flightPeriod._itLegCabin);
    }

    // //////////////////////////////////////////////////////////////////
    storeSegmentSpecificity::
    storeSegmentSpecificity (FlightPeriodStruct& ioFlightPeriod)
      : ParserSemanticAction (ioFlightPeriod) {
    }

    // //////////////////////////////////////////////////////////////////
    void storeSegmentSpecificity::operator() (char iChar) const {
      if (iChar == '0') {
        _flightPeriod._areSegmentDefinitionsSpecific = false;
      } else {
        _flightPeriod._areSegmentDefinitionsSpecific = true;
      }

      // Do a few sanity checks: the two lists should get exactly the same
      // content (in terms of airport codes). The only difference is that one
      // is a STL set, and the other a STL vector.
      assert (_flightPeriod._airportList.size()
              == _flightPeriod._airportOrderedList.size());
      assert (_flightPeriod._airportList.size() >= 2);
        
      // Since all the legs have now been parsed, we get all the airports
      // and the segments may be built.
      _flightPeriod.buildSegments();
    }
      
    // //////////////////////////////////////////////////////////////////
    storeSegmentBoardingPoint::
    storeSegmentBoardingPoint (FlightPeriodStruct& ioFlightPeriod)
      : ParserSemanticAction (ioFlightPeriod) {
    }

    // //////////////////////////////////////////////////////////////////
    void storeSegmentBoardingPoint::operator() (iterator_t iStr,
                                             iterator_t iStrEnd) const {
      stdair::AirportCode_T lBoardingPoint (iStr, iStrEnd);
      _flightPeriod._itSegment._boardingPoint = lBoardingPoint;
    }

    // //////////////////////////////////////////////////////////////////
    storeSegmentOffPoint::
    storeSegmentOffPoint (FlightPeriodStruct& ioFlightPeriod)
      : ParserSemanticAction (ioFlightPeriod) {
    }

    // //////////////////////////////////////////////////////////////////
    void storeSegmentOffPoint::operator() (iterator_t iStr,
                                           iterator_t iStrEnd) const {
      stdair::AirportCode_T lOffPoint (iStr, iStrEnd);
      _flightPeriod._itSegment._offPoint = lOffPoint;
    }

    // //////////////////////////////////////////////////////////////////
    storeSegmentCabinCode::
    storeSegmentCabinCode (FlightPeriodStruct& ioFlightPeriod)
      : ParserSemanticAction (ioFlightPeriod) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeSegmentCabinCode::operator() (char iChar) const { 
      _flightPeriod._itSegmentCabin._cabinCode = iChar; 
    }

    // //////////////////////////////////////////////////////////////////
    storeClasses::
    storeClasses (FlightPeriodStruct& ioFlightPeriod)
      : ParserSemanticAction (ioFlightPeriod) {
    }

    // //////////////////////////////////////////////////////////////////
    void storeClasses::operator() (iterator_t iStr,
                                   iterator_t iStrEnd) const {
      std::string lClasses (iStr, iStrEnd);
      _flightPeriod._itSegmentCabin._classes = lClasses;

      // The list of classes is the last (according to the arrival order
      // within the schedule input file) detail of the segment cabin. Hence,
      // when a list of classes is parsed, it means that the full segment
      // cabin details have already been parsed as well: the segment cabin
      // can thus be added to the segment.
      if (_flightPeriod._areSegmentDefinitionsSpecific == true) {
        _flightPeriod.addSegmentCabin (_flightPeriod._itSegment,
                                       _flightPeriod._itSegmentCabin);
      } else {
        _flightPeriod.addSegmentCabin (_flightPeriod._itSegmentCabin);
      }
    }

    // //////////////////////////////////////////////////////////////////
    storeFamilyCode::
    storeFamilyCode (FlightPeriodStruct& ioFlightPeriod)
      : ParserSemanticAction (ioFlightPeriod) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeFamilyCode::operator() (int iCode) const {
      std::ostringstream ostr;
      ostr << iCode;
      _flightPeriod._itSegmentCabin._itFamilyCode = ostr.str(); 
    }

    // //////////////////////////////////////////////////////////////////
    storeFClasses::
    storeFClasses (FlightPeriodStruct& ioFlightPeriod)
      : ParserSemanticAction (ioFlightPeriod) {
    }

    // //////////////////////////////////////////////////////////////////
    void storeFClasses::operator() (iterator_t iStr,
                                    iterator_t iStrEnd) const {
      std::string lClasses (iStr, iStrEnd);
      FareFamilyStruct lFareFamily(_flightPeriod._itSegmentCabin._itFamilyCode,
                                     lClasses);

      // The list of classes is the last (according to the arrival order
      // within the schedule input file) detail of the segment cabin. Hence,
      // when a list of classes is parsed, it means that the full segment
      // cabin details have already been parsed as well: the segment cabin
      // can thus be added to the segment.
      if (_flightPeriod._areSegmentDefinitionsSpecific == true) {
        _flightPeriod.addFareFamily (_flightPeriod._itSegment,
                                     _flightPeriod._itSegmentCabin,
                                     lFareFamily);
      } else {
        _flightPeriod.addFareFamily (_flightPeriod._itSegmentCabin,
                                     lFareFamily);
      }
    }

    // //////////////////////////////////////////////////////////////////
    doEndFlight::
    doEndFlight (stdair::BomRoot& ioBomRoot,
                 FlightPeriodStruct& ioFlightPeriod)
      : ParserSemanticAction (ioFlightPeriod),
        _bomRoot (ioBomRoot) {
    }
    
    // //////////////////////////////////////////////////////////////////
    // void doEndFlight::operator() (char iChar) const {
    void doEndFlight::operator() (iterator_t iStr,
                                  iterator_t iStrEnd) const {

      assert (_flightPeriod._legAlreadyDefined == true);
      _flightPeriod._legList.push_back (_flightPeriod._itLeg);
        
      // The lists of legs and cabins must be reset
      _flightPeriod._legAlreadyDefined = false;
      _flightPeriod._itLeg._cabinList.clear();
        
      // DEBUG: Display the result
      STDAIR_LOG_DEBUG ("FlightPeriod: " << _flightPeriod.describe());

      // Create the FlightPeriod BOM objects, and potentially the intermediary
      // objects (e.g., Inventory).
      InventoryGenerator::createFlightPeriod (_bomRoot, _flightPeriod);
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
    
    /** 4-digit-integer parser */
    uint4_p_t uint4_p;
    
    /** Up-to-4-digit-integer parser */
    uint1_4_p_t uint1_4_p;

    /** Airline Code Parser: repeat_p(2,3)[chset_p("0-9A-Z")] */
    repeat_p_t airline_code_p (chset_t("0-9A-Z").derived(), 2, 3);
      
    /** Flight Number Parser: limit_d(0u, 9999u)[uint1_4_p] */
    bounded1_4_p_t flight_number_p (uint1_4_p.derived(), 0u, 9999u);

    /** Year Parser: limit_d(2000u, 2099u)[uint4_p] */
    bounded4_p_t year_p (uint4_p.derived(), 2000u, 2099u);
      
    /** Month Parser: limit_d(1u, 12u)[uint2_p] */
    bounded2_p_t month_p (uint2_p.derived(), 1u, 12u);

    /** Day Parser: limit_d(1u, 31u)[uint2_p] */
    bounded2_p_t day_p (uint2_p.derived(), 1u, 31u);
     
    /** DOW (Day-Of-the-Week) Parser: repeat_p(7)[chset_p("0-1")] */
    repeat_p_t dow_p (chset_t("0-1").derived().derived(), 7, 7);

    /** Airport Parser: repeat_p(3)[chset_p("0-9A-Z")] */
    repeat_p_t airport_p (chset_t("0-9A-Z").derived(), 3, 3);
      
    /** Hour Parser: limit_d(0u, 23u)[uint2_p] */
    bounded2_p_t hours_p (uint2_p.derived(), 0u, 23u);

    /** Minute Parser: limit_d(0u, 59u)[uint2_p] */
    bounded2_p_t minutes_p (uint2_p.derived(), 0u, 59u);

    /** Second Parser: limit_d(0u, 59u)[uint2_p] */
    bounded2_p_t seconds_p (uint2_p.derived(), 0u, 59u);

    /** Cabin Code Parser: chset_p("A-Z") */
    chset_t cabin_code_p ("A-Z");

    /** Family code parser */
    int1_p_t family_code_p;
      
    /** Class Code List Parser: repeat_p(1,26)[chset_p("A-Z")] */
    repeat_p_t class_code_list_p (chset_t("A-Z").derived(), 1, 26);


    // //////////////////////////////////////////////////////////////////
    //  (Boost Spirit) Grammar Definition
    // //////////////////////////////////////////////////////////////////

    // //////////////////////////////////////////////////////////////////
    FlightPeriodParser::
    FlightPeriodParser (stdair::BomRoot& ioBomRoot,
                        FlightPeriodStruct& ioFlightPeriod) 
      : _bomRoot (ioBomRoot),
        _flightPeriod (ioFlightPeriod) {
    }

    // //////////////////////////////////////////////////////////////////
    template<typename ScannerT>
    FlightPeriodParser::definition<ScannerT>::
    definition (FlightPeriodParser const& self) {

      flight_period_list = *( boost::spirit::classic::comment_p("//")
                              | boost::spirit::classic::comment_p("/*", "*/")
                              | flight_period )
        ;
      
      flight_period = flight_key
        >> +( ';' >> leg )
        >> ';' >> segment_section
        >> flight_period_end[doEndFlight (self._bomRoot, self._flightPeriod)]
        ;

      flight_period_end =
        boost::spirit::classic::ch_p(';')
        ;
      
      flight_key = airline_code
        >> ';' >> flight_number
        >> ';' >> date[storeDateRangeStart(self._flightPeriod)]
        >> ';' >> date[storeDateRangeEnd(self._flightPeriod)]
        >> ';' >> dow[storeDow(self._flightPeriod)]
        ;

      airline_code = boost::spirit::classic::
        lexeme_d[(airline_code_p)[storeAirlineCode(self._flightPeriod)] ]
        ;
        
      flight_number = boost::spirit::classic::
        lexeme_d[(flight_number_p)[storeFlightNumber(self._flightPeriod)] ]
        ;

      date = boost::spirit::classic::
        lexeme_d[(year_p)[boost::spirit::classic::
                          assign_a(self._flightPeriod._itYear)]
                 >> '-'
                 >> (month_p)[boost::spirit::classic::
                              assign_a(self._flightPeriod._itMonth)]
                 >> '-'
                 >> (day_p)[boost::spirit::classic::
                            assign_a(self._flightPeriod._itDay)]
                 ]
        ;

      dow = boost::spirit::classic::lexeme_d[ dow_p ]
        ;
      
      leg = leg_key >> ';' >> leg_details >> +( ';' >> leg_cabin_details )
        ;
	 
      leg_key =
        (airport_p)[storeLegBoardingPoint(self._flightPeriod)]
        >> ';'
        >> (airport_p)[storeLegOffPoint(self._flightPeriod)]
        ;
	 
      leg_details =
        time[storeBoardingTime(self._flightPeriod)]
        >> !(date_offset)
        >> ';'
        >> time[storeOffTime(self._flightPeriod)]
        >> !(date_offset)
        >> ';'
        >> time[storeElapsedTime(self._flightPeriod)]
        ;
        
      time = boost::spirit::classic::
        lexeme_d[(hours_p)[boost::spirit::classic::
                           assign_a(self._flightPeriod._itHours)]
                 >> ':'
                 >> (minutes_p)[boost::spirit::classic::
                                assign_a(self._flightPeriod._itMinutes)]
                 >> !(':' 
                      >> (seconds_p)[boost::spirit::classic::
                                     assign_a(self._flightPeriod._itSeconds)])
                 ]
        ;

      date_offset = boost::spirit::classic::ch_p('/')
        >> (int1_p)[boost::spirit::classic::
                    assign_a(self._flightPeriod._dateOffset)]
        ;          
        
      leg_cabin_details = (cabin_code_p)[storeLegCabinCode(self._flightPeriod)]
        >> ';' >> (boost::spirit::classic::
                   ureal_p)[storeCapacity(self._flightPeriod)]
        ;
        
      segment_key =
        (airport_p)[storeSegmentBoardingPoint(self._flightPeriod)]
        >> ';'
        >> (airport_p)[storeSegmentOffPoint(self._flightPeriod)]
        ;
	 
      segment_section =
        generic_segment | specific_segment_list
        ;

      generic_segment = boost::spirit::classic::
        ch_p('0')[storeSegmentSpecificity(self._flightPeriod)]
        >> +(';' >> segment_cabin_details)
        ;

      specific_segment_list = boost::spirit::classic::
        ch_p('1')[storeSegmentSpecificity(self._flightPeriod)]
        >> +(';' >> segment_key >> full_segment_cabin_details)
        ;

      full_segment_cabin_details =
        +(';' >> segment_cabin_details)
        ;

      segment_cabin_details =
        (cabin_code_p)[storeSegmentCabinCode(self._flightPeriod)]
        >> ';' >> (class_code_list_p)[storeClasses(self._flightPeriod)]
        >> *(';' >> family_cabin_details)
        ;

      family_cabin_details =
        (family_code_p)[storeFamilyCode(self._flightPeriod)]
        >> ';'
        >> (class_code_list_p)[storeFClasses(self._flightPeriod)]
        ;
        
      // BOOST_SPIRIT_DEBUG_NODE (FlightPeriodParser);
      BOOST_SPIRIT_DEBUG_NODE (flight_period_list);
      BOOST_SPIRIT_DEBUG_NODE (flight_period);
      BOOST_SPIRIT_DEBUG_NODE (flight_period_end);
      BOOST_SPIRIT_DEBUG_NODE (flight_key);
      BOOST_SPIRIT_DEBUG_NODE (airline_code);
      BOOST_SPIRIT_DEBUG_NODE (flight_number);
      BOOST_SPIRIT_DEBUG_NODE (date);
      BOOST_SPIRIT_DEBUG_NODE (dow);
      BOOST_SPIRIT_DEBUG_NODE (leg);
      BOOST_SPIRIT_DEBUG_NODE (leg_key);
      BOOST_SPIRIT_DEBUG_NODE (leg_details);
      BOOST_SPIRIT_DEBUG_NODE (time);
      BOOST_SPIRIT_DEBUG_NODE (date_offset);
      BOOST_SPIRIT_DEBUG_NODE (leg_cabin_details);
      BOOST_SPIRIT_DEBUG_NODE (segment_section);
      BOOST_SPIRIT_DEBUG_NODE (segment_key);
      BOOST_SPIRIT_DEBUG_NODE (generic_segment);
      BOOST_SPIRIT_DEBUG_NODE (specific_segment_list);
      BOOST_SPIRIT_DEBUG_NODE (full_segment_cabin_details);
      BOOST_SPIRIT_DEBUG_NODE (segment_cabin_details);
      BOOST_SPIRIT_DEBUG_NODE (family_cabin_details);
    }

    // //////////////////////////////////////////////////////////////////
    template<typename ScannerT>
    boost::spirit::classic::rule<ScannerT> const&
    FlightPeriodParser::definition<ScannerT>::start() const {
      return flight_period_list;
    }
    
  }


  /////////////////////////////////////////////////////////////////////////
  //
  //  Entry class for the file parser
  //
  /////////////////////////////////////////////////////////////////////////

  // //////////////////////////////////////////////////////////////////////
  FlightPeriodFileParser::
  FlightPeriodFileParser (stdair::BomRoot& ioBomRoot,
                          const stdair::Filename_T& iFilename)
    : _filename (iFilename), _bomRoot (ioBomRoot) {
    init();
  }

  // //////////////////////////////////////////////////////////////////////
  void FlightPeriodFileParser::init() {
    // Check that the file exists and is readable
    const bool doesExistAndIsReadable =
      stdair::BasFileMgr::doesExistAndIsReadable (_filename);

    if (doesExistAndIsReadable == false) {
      STDAIR_LOG_ERROR ("The schedule file " << _filename
                        << " does not exist or can not be read.");
      
      throw ScheduleInputFileNotFoundException ("The schedule file " + _filename
                                                + " does not exist or can not be read");
    }
    
    // Open the file
    _startIterator = iterator_t (_filename);

    // Check the filename exists and can be open
    if (!_startIterator) {
      STDAIR_LOG_ERROR ("The schedule file " << _filename << " can not be open."
                          << std::endl);

      throw ScheduleInputFileNotFoundException ("The file " + _filename
                                                + " does not exist or can not be read");
    }

    // Create an EOF iterator
    _endIterator = _startIterator.make_end();
  }
    
  // //////////////////////////////////////////////////////////////////////
  bool FlightPeriodFileParser::generateInventories () {
    bool oResult = false;
      
    STDAIR_LOG_DEBUG ("Parsing schedule input file: " << _filename);

    // Initialise the parser (grammar) with the helper/staging structure.
    ScheduleParserHelper::FlightPeriodParser lFPParser (_bomRoot, 
                                                        _flightPeriod);
      
    // Launch the parsing of the file and, thanks to the doEndFlight
    // call-back structure, the building of the whole BomRoot BOM
    // (i.e., including Inventory, FlightDate, LegDate, SegmentDate, etc.)
    boost::spirit::classic::parse_info<iterator_t> info =
      boost::spirit::classic::parse (_startIterator, _endIterator, lFPParser, 
                            boost::spirit::classic::space_p);

    // Retrieves whether or not the parsing was successful
    oResult = info.hit;
      
    const std::string hasBeenFullyReadStr = (info.full == true)?"":"not ";
    if (oResult == true) {
      STDAIR_LOG_DEBUG ("Parsing of schedule input file: " << _filename
                       << " succeeded: read " << info.length
                       << " characters. The input file has "
                       << hasBeenFullyReadStr
                       << "been fully read. Stop point: " << info.stop);
        
    } else {
      // TODO: decide whether to throw an exception
      STDAIR_LOG_ERROR ("Parsing of schedule input file: " << _filename
                       << " failed: read " << info.length
                       << " characters. The input file has "
                       << hasBeenFullyReadStr
                       << "been fully read. Stop point: " << info.stop);
    }

    return oResult;
  }
    
}
