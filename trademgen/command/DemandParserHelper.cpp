// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// StdAir
#include <stdair/basic/BasFileMgr.hpp>
#include <stdair/bom/EventQueue.hpp>
#include <stdair/service/Logger.hpp>
// TraDemGen
#include <trademgen/basic/DemandCharacteristicsTypes.hpp>
#include <trademgen/command/DemandParserHelper.hpp>
#include <trademgen/command/DemandManager.hpp>

namespace TRADEMGEN {

  namespace DemandParserHelper {
      
    // //////////////////////////////////////////////////////////////////
    //  Semantic actions
    // //////////////////////////////////////////////////////////////////

    ParserSemanticAction::ParserSemanticAction (DemandStruct& ioDemand)
      : _demand (ioDemand) {
    }      

    // //////////////////////////////////////////////////////////////////
    storePreferredDepartureDate::
    storePreferredDepartureDate (DemandStruct& ioDemand)
      : ParserSemanticAction (ioDemand) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storePreferredDepartureDate::operator() (iterator_t iStr,
                                                  iterator_t iStrEnd) const {
      _demand._prefDepDate = _demand.getDate();

      // TODO: do not harcode the preferred arrival date
      _demand._prefArrDate = _demand._prefDepDate;
    }
      
    // //////////////////////////////////////////////////////////////////
    storeOrigin::storeOrigin (DemandStruct& ioDemand)
      : ParserSemanticAction (ioDemand) {
    }

    // //////////////////////////////////////////////////////////////////
    void storeOrigin::operator() (iterator_t iStr, iterator_t iStrEnd) const {
      stdair::AirportCode_T lOrigin (iStr, iStrEnd);
      _demand._origin = lOrigin;
    }

    // //////////////////////////////////////////////////////////////////
    storeDestination::storeDestination (DemandStruct& ioDemand)
      : ParserSemanticAction (ioDemand) {
    }

    // //////////////////////////////////////////////////////////////////
    void storeDestination::operator() (iterator_t iStr,
                                       iterator_t iStrEnd) const {
      stdair::AirportCode_T lDestination (iStr, iStrEnd);
      _demand._destination = lDestination;
    }

    // //////////////////////////////////////////////////////////////////
    storePrefCabin::storePrefCabin (DemandStruct& ioDemand)
      : ParserSemanticAction (ioDemand) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storePrefCabin::operator() (iterator_t iStr,
                                     iterator_t iStrEnd) const {
      stdair::CabinCode_T lPrefCabin  (iStr, iStrEnd);
      _demand._prefCabin = lPrefCabin; 
      //STDAIR_LOG_DEBUG ("Preferred cabin: " << lPrefCabin);
    }
    
    // //////////////////////////////////////////////////////////////////
    storeDemandMean::storeDemandMean (DemandStruct& ioDemand)
      : ParserSemanticAction (ioDemand) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeDemandMean::operator() (double iReal) const { 
      _demand._demandMean = iReal; 
      //STDAIR_LOG_DEBUG ("Demand mean: " << iReal);
    }

    // //////////////////////////////////////////////////////////////////
    storeDemandStdDev::storeDemandStdDev (DemandStruct& ioDemand)
      : ParserSemanticAction (ioDemand) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeDemandStdDev::operator() (double iReal) const { 
      _demand._demandStdDev = iReal; 
      //STDAIR_LOG_DEBUG ("Demand stddev: " << iReal);
    }

    // //////////////////////////////////////////////////////////////////
    storePosCode::storePosCode (DemandStruct& ioDemand)
      : ParserSemanticAction (ioDemand) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storePosCode::operator() (iterator_t iStr, iterator_t iStrEnd) const {
      const stdair::AirportCode_T lPosCode (iStr, iStrEnd);
      _demand._itPosCode = lPosCode;
      //STDAIR_LOG_DEBUG ("Pos code: " << lPosCode);
    }

    // //////////////////////////////////////////////////////////////////
    storePosProbMass::storePosProbMass (DemandStruct& ioDemand)
      : ParserSemanticAction (ioDemand) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storePosProbMass::operator() (double iReal) const {
      const bool hasInsertBeenSuccessfull = 
        _demand._posProbDist.
        insert (POSProbabilityMassFunction_T::
                value_type (_demand._itPosCode, iReal)).second;
      if (hasInsertBeenSuccessfull == false) {
        STDAIR_LOG_ERROR ("The same POS code ('" << _demand._itPosCode
                          << "') has probably been given twice");
        throw stdair::CodeDuplicationException ("The same POS code ('"
                                                + _demand._itPosCode
                                                + "') has probably been given twice");
      }
      
      //STDAIR_LOG_DEBUG ("PosProbMass: " << iReal);
    }

    // //////////////////////////////////////////////////////////////////
    storeChannelCode::storeChannelCode (DemandStruct& ioDemand)
      : ParserSemanticAction (ioDemand) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeChannelCode::operator() (iterator_t iStr,
                                       iterator_t iStrEnd) const {
      _demand._itChannelCode = std::string (iStr, iStrEnd);
      //STDAIR_LOG_DEBUG ("Channel code: " << _demand._itChannelCode);
    }

    // //////////////////////////////////////////////////////////////////
    storeChannelProbMass::storeChannelProbMass (DemandStruct& ioDemand)
      : ParserSemanticAction (ioDemand) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeChannelProbMass::operator() (double iReal) const {
      const bool hasInsertBeenSuccessfull = 
        _demand._channelProbDist.
        insert (ChannelProbabilityMassFunction_T::
                value_type (_demand._itChannelCode, iReal)).second;
      if (hasInsertBeenSuccessfull == false) {
        STDAIR_LOG_ERROR ("The same channel type code ('"
                          << _demand._itChannelCode
                          << "') has probably been given twice");
        throw stdair::CodeDuplicationException ("The same channel type code ('"
                                                + _demand._itChannelCode
                                                + "') has probably been given twice");
      }
      
      //STDAIR_LOG_DEBUG ("ChannelProbMass: " << iReal);
    }

    // //////////////////////////////////////////////////////////////////
    storeTripCode::storeTripCode (DemandStruct& ioDemand)
      : ParserSemanticAction (ioDemand) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeTripCode::operator() (iterator_t iStr,
                                       iterator_t iStrEnd) const {
      _demand._itTripCode = std::string (iStr, iStrEnd);
      //STDAIR_LOG_DEBUG ("Trip code: " << _demand._itTripCode);
    }

    // //////////////////////////////////////////////////////////////////
    storeTripProbMass::storeTripProbMass (DemandStruct& ioDemand)
      : ParserSemanticAction (ioDemand) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeTripProbMass::operator() (double iReal) const {
      const bool hasInsertBeenSuccessfull = 
        _demand._tripProbDist.
        insert (TripTypeProbabilityMassFunction_T::
                value_type (_demand._itTripCode, iReal)).second;
      if (hasInsertBeenSuccessfull == false) {
        STDAIR_LOG_ERROR ("The same trip type code ('"
                          << _demand._itTripCode
                          << "') has probably been given twice");
        throw stdair::CodeDuplicationException ("The same trip type code ('"
                                                + _demand._itTripCode
                                                + "') has probably been given twice");
      }
      
      //STDAIR_LOG_DEBUG ("TripProbMass: " << iReal);
    }

    // //////////////////////////////////////////////////////////////////
    storeStayCode::storeStayCode (DemandStruct& ioDemand)
      : ParserSemanticAction (ioDemand) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeStayCode::operator() (unsigned int iInteger) const {
      const stdair::DayDuration_T lStayDuration (iInteger);
      _demand._itStayDuration = lStayDuration;
      // STDAIR_LOG_DEBUG ("Stay duration: " << lStayDuration);
    }

    // //////////////////////////////////////////////////////////////////
    storeStayProbMass::storeStayProbMass (DemandStruct& ioDemand)
      : ParserSemanticAction (ioDemand) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeStayProbMass::operator() (double iReal) const {
      const bool hasInsertBeenSuccessfull = 
        _demand._stayProbDist.
        insert (StayDurationProbabilityMassFunction_T::
                value_type (_demand._itStayDuration, iReal)).second;
      if (hasInsertBeenSuccessfull == false) {
        std::ostringstream oStr;
        oStr << "The same stay duration ('" << _demand._itStayDuration
             << "') has probably been given twice";
        STDAIR_LOG_ERROR (oStr.str());
        throw stdair::CodeDuplicationException (oStr.str());
      }
      
      // STDAIR_LOG_DEBUG ("StayProbMass: " << iReal);
    }

    // //////////////////////////////////////////////////////////////////
    storeFFCode::storeFFCode (DemandStruct& ioDemand)
      : ParserSemanticAction (ioDemand) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeFFCode::operator() (iterator_t iStr, iterator_t iStrEnd) const {
      _demand._itFFCode = std::string (iStr, iStrEnd);
      //STDAIR_LOG_DEBUG ("FF code: " << _demand._itFFCode);
    }

    // //////////////////////////////////////////////////////////////////
    storeFFProbMass::storeFFProbMass (DemandStruct& ioDemand)
      : ParserSemanticAction (ioDemand) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeFFProbMass::operator() (double iReal) const {
      const bool hasInsertBeenSuccessfull = 
        _demand._ffProbDist.
        insert (FrequentFlyerProbabilityMassFunction_T::
                value_type (_demand._itFFCode, iReal)).second;
      if (hasInsertBeenSuccessfull == false) {
        STDAIR_LOG_ERROR ("The same Frequent Flyer code ('"
                          << _demand._itFFCode
                          << "') has probably been given twice");
        throw stdair::CodeDuplicationException("The same Frequent Flyer code ('"
                                               + _demand._itFFCode
                                               + "') has probably been given twice");
      }
      
      //STDAIR_LOG_DEBUG ("FfProbMass: " << iReal);
    }

    // //////////////////////////////////////////////////////////////////
    storePrefDepTime::storePrefDepTime (DemandStruct& ioDemand)
      : ParserSemanticAction (ioDemand) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storePrefDepTime::operator() (iterator_t iStr,
                                       iterator_t iStrEnd) const {
      _demand._itPrefDepTime = _demand.getTime();
        
      // DEBUG
      // STDAIR_LOG_DEBUG ("Pref dep time: " << _demand._itHours << ":"
      //                   << _demand._itMinutes << ":" << _demand._itSeconds
      //                   << " ==> " << _demand._itPrefDepTime);
      
      // Reset the number of minutes and seconds
      _demand._itMinutes = 0;
      _demand._itSeconds = 0;
    }

    // //////////////////////////////////////////////////////////////////
    storePrefDepTimeProbMass::storePrefDepTimeProbMass (DemandStruct& ioDemand)
      : ParserSemanticAction (ioDemand) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storePrefDepTimeProbMass::operator() (double iReal) const {
      const stdair::IntDuration_T lIntDuration =
        _demand._itPrefDepTime.total_seconds();
      
      _demand._prefDepTimeProbDist.
        insert (PreferredDepartureTimeContinuousDistribution_T::
                value_type (lIntDuration, iReal));
      //STDAIR_LOG_DEBUG ("PrefDepTimeProbMass: " << iReal);
    }

    // //////////////////////////////////////////////////////////////////
    storeWTP::storeWTP (DemandStruct& ioDemand)
      : ParserSemanticAction (ioDemand) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeWTP::operator() (double iReal) const {
      _demand._minWTP = iReal;
      //STDAIR_LOG_DEBUG ("WTP: " << iReal);
    }

    // //////////////////////////////////////////////////////////////////
    storeTimeValue::storeTimeValue (DemandStruct& ioDemand)
      : ParserSemanticAction (ioDemand) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeTimeValue::operator() (double iReal) const {
      _demand._itTimeValue = iReal;
      //STDAIR_LOG_DEBUG ("Time value: " << iReal);
    }

    // //////////////////////////////////////////////////////////////////
    storeTimeValueProbMass::storeTimeValueProbMass (DemandStruct& ioDemand)
      : ParserSemanticAction (ioDemand) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeTimeValueProbMass::operator() (double iReal) const {
      _demand._timeValueProbDist.
        insert (ValueOfTimeContinuousDistribution_T::
                value_type (_demand._itTimeValue, iReal));
      //STDAIR_LOG_DEBUG ("TimeValueProbMass: " << iReal);
    }

    // //////////////////////////////////////////////////////////////////
    storeDTD::storeDTD (DemandStruct& ioDemand)
      : ParserSemanticAction (ioDemand) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeDTD::operator() (unsigned int iInteger) const {
      const stdair::DayDuration_T lDTD (iInteger);
      _demand._itDTD = lDTD;
      //STDAIR_LOG_DEBUG ("DTD: " << lDTD);
    }

    // //////////////////////////////////////////////////////////////////
    storeDTDProbMass::storeDTDProbMass (DemandStruct& ioDemand)
      : ParserSemanticAction (ioDemand) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeDTDProbMass::operator() (double iReal) const {
      const stdair::FloatDuration_T lZeroDTDFloat = 0.0;
      stdair::FloatDuration_T lDTDFloat =
        static_cast<stdair::FloatDuration_T> (_demand._itDTD);
      lDTDFloat = lZeroDTDFloat - lDTDFloat;

      _demand._dtdProbDist.insert (ArrivalPatternCumulativeDistribution_T::
                                   value_type (lDTDFloat, iReal));
      //STDAIR_LOG_DEBUG ("DTDProbMass: " << iReal);
    }

    // //////////////////////////////////////////////////////////////////
    doEndDemand::doEndDemand (stdair::EventQueue& ioEventQueue,
                              stdair::UniformGenerator_T& ioSharedGenerator,
                              const POSProbabilityMass_T& iPOSProbMass,
                              DemandStruct& ioDemand)
      : ParserSemanticAction (ioDemand), _eventQueue (ioEventQueue),
        _uniformGenerator (ioSharedGenerator),
        _posProbabilityMass (iPOSProbMass) {
    }
    
    // //////////////////////////////////////////////////////////////////
    // void doEndDemand::operator() (char iChar) const {
    void doEndDemand::operator() (iterator_t iStr, iterator_t iStrEnd) const {
        
      // DEBUG: Display the result
      // STDAIR_LOG_DEBUG ("Demand: " << _demand.describe());

      // Create the Demand BOM objects
      DemandManager::createDemandCharacteristics (_eventQueue, _uniformGenerator,
                                                  _posProbabilityMass, _demand);
                                 
      // Clean the lists
      _demand._posProbDist.clear();
      _demand._channelProbDist.clear();
      _demand._tripProbDist.clear();
      _demand._stayProbDist.clear();
      _demand._ffProbDist.clear();
      _demand._prefDepTimeProbDist.clear(); 
      _demand._timeValueProbDist.clear();
      _demand._dtdProbDist.clear();
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
    bounded1_2_p_t hours_p (uint1_2_p.derived(), 0u, 23u);

    /** Minute Parser: limit_d(0u, 59u)[uint2_p] */
    bounded2_p_t minutes_p (uint2_p.derived(), 0u, 59u);

    /** Second Parser: limit_d(0u, 59u)[uint2_p] */
    bounded2_p_t seconds_p (uint2_p.derived(), 0u, 59u);

    /** Cabin code parser: chset_p("A-Z") */
    chset_t cabin_code_p ("A-Z");

    /** Passenger type parser: chset_p("A-Z") */
    chset_t passenger_type_p ("A-Z");

    /** Frequent flyer type parser: chset_p("A-Z") */
    chset_t ff_type_p ("A-Z");

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
    DemandParser::DemandParser (stdair::EventQueue& ioEventQueue,
                                stdair::UniformGenerator_T& ioSharedGenerator,
                                const POSProbabilityMass_T& iPOSProbMass,
                                DemandStruct& ioDemand) 
      : _eventQueue (ioEventQueue), _uniformGenerator (ioSharedGenerator),
        _posProbabilityMass (iPOSProbMass), _demand (ioDemand) {
    }

    // //////////////////////////////////////////////////////////////////
    template<typename ScannerT>
    DemandParser::definition<ScannerT>::
    definition (DemandParser const& self) {

      demand_list = *( boost::spirit::classic::comment_p("//")
                       | boost::spirit::classic::comment_p("/*", "*/")
                       | demand )
        ;

      demand =
        pref_dep_date
        >> ';' >> origin >> ';' >> destination
        >> ';' >> pref_cabin[storePrefCabin(self._demand)]
        >> ';' >> pos_dist
        >> ';' >> channel_dist
        >> ';' >> trip_dist
        >> ';' >> stay_dist
        >> ';' >> ff_dist
        >> ';' >> pref_dep_time_dist
        >> ';' >> wtp
        >> ';' >> time_value_dist
        >> ';' >> dtd_dist
        >> ';' >> demand_params
        >> demand_end[doEndDemand(self._eventQueue, self._uniformGenerator,
                                  self._posProbabilityMass, self._demand)]
        ;

      demand_end =
        boost::spirit::classic::ch_p(';')
        ;
      
      pref_dep_date =
        date[storePreferredDepartureDate(self._demand)]
        ;

      date =
        boost::spirit::classic::lexeme_d[
         (year_p)[boost::spirit::classic::assign_a(self._demand._itYear)]
         >> '-'
         >> (month_p)[boost::spirit::classic::assign_a(self._demand._itMonth)]
         >> '-'
         >> (day_p)[boost::spirit::classic::assign_a(self._demand._itDay)]
         ]
        ;

      origin =
        (airport_p)[storeOrigin(self._demand)]
        ;

      destination =
        (airport_p)[storeDestination(self._demand)]
        ;

      pref_cabin = cabin_code_p;

      pos_dist =
        pos_pair >> *( ',' >> pos_pair )
        ;

      pos_pair =
        pos_code[storePosCode(self._demand)]
        >> ':' >> pos_share
        ;

      pos_code =
        airport_p
        | boost::spirit::classic::chseq_p("row")
        ;

      pos_share =
        (boost::spirit::classic::ureal_p)[storePosProbMass(self._demand)]
        ;

      channel_dist =
        channel_pair >> *( ',' >> channel_pair )
        ;

      channel_pair =
        channel_code[storeChannelCode(self._demand)]
        >> ':' >> channel_share
        ;

      channel_code =
        boost::spirit::classic::chseq_p("DF")
        | boost::spirit::classic::chseq_p("DN")
        | boost::spirit::classic::chseq_p("IF")
        | boost::spirit::classic::chseq_p("IN")
        ;

      channel_share =
        (boost::spirit::classic::ureal_p)[storeChannelProbMass(self._demand)]
        ;
      
      trip_dist =
        trip_pair >> *( ',' >> trip_pair )
        ;

      trip_pair =
        trip_code[storeTripCode(self._demand)]
        >> ':' >> trip_share
        ;

      trip_code =
        boost::spirit::classic::chseq_p("RO")
        | boost::spirit::classic::chseq_p("RI")
        | boost::spirit::classic::chseq_p("OW")
        ;

      trip_share =
        (boost::spirit::classic::ureal_p)[storeTripProbMass(self._demand)]
        ;
      
      stay_dist =
        stay_pair >> *( ',' >> stay_pair )
        ;

      stay_pair =
        (stay_duration_p)[storeStayCode(self._demand)]
        >> ':' >> stay_share
        ;

      stay_share =
        (boost::spirit::classic::ureal_p)[storeStayProbMass(self._demand)]
        ;

      ff_dist =
        ff_pair >> *( ',' >> ff_pair )
        ;

      ff_pair =
        ff_code[storeFFCode(self._demand)]
        >> ':' >> ff_share
        ;

      ff_code = ff_type_p;

      ff_share =
        (boost::spirit::classic::ureal_p)[storeFFProbMass(self._demand)]
        ;
      
      pref_dep_time_dist =
        pref_dep_time_pair >> *( ',' >> pref_dep_time_pair )
        ;

      pref_dep_time_pair =
        (time)[storePrefDepTime(self._demand)]
        >> ':' >> pref_dep_time_share
        ;

      pref_dep_time_share =
        (boost::spirit::classic::ureal_p)[storePrefDepTimeProbMass(self._demand)]
        ;

      time =
        boost::spirit::classic::lexeme_d[
       (hours_p)[boost::spirit::classic::assign_a(self._demand._itHours)]
       >> !('.' >> (minutes_p)[boost::spirit::classic::assign_a(self._demand._itMinutes)])
       >> !('.' >> (seconds_p)[boost::spirit::classic::assign_a(self._demand._itSeconds)])
       ]
        ;

      wtp =
        (boost::spirit::classic::ureal_p)[storeWTP(self._demand)]
        ;

      time_value_dist =
        time_value_pair >> *( ',' >> time_value_pair )
        ;

      time_value_pair =
        (boost::spirit::classic::ureal_p)[storeTimeValue(self._demand)]
        >> ':' >> time_value_share
        ;

      time_value_share =
        (boost::spirit::classic::ureal_p)[storeTimeValueProbMass(self._demand)]
        ;

      dtd_dist =
        dtd_pair >> *( ',' >> dtd_pair )
        ;

      dtd_pair =
        (boost::spirit::classic::ureal_p)[storeDTD(self._demand)]
        >> ':' >> dtd_share
        ;

      dtd_share =
        (boost::spirit::classic::ureal_p)[storeDTDProbMass(self._demand)]
        ;

      demand_params =
        boost::spirit::classic::ch_p('N')
        >> ','
        >> (boost::spirit::classic::ureal_p)[storeDemandMean(self._demand)]
        >> ','
        >> (boost::spirit::classic::ureal_p)[storeDemandStdDev(self._demand)]
        ;
      
      // BOOST_SPIRIT_DEBUG_NODE (DemandParser);
      BOOST_SPIRIT_DEBUG_NODE (demand_list);
      BOOST_SPIRIT_DEBUG_NODE (demand);
      BOOST_SPIRIT_DEBUG_NODE (demand_end);
      BOOST_SPIRIT_DEBUG_NODE (pref_dep_date);
      BOOST_SPIRIT_DEBUG_NODE (date);
      BOOST_SPIRIT_DEBUG_NODE (origin);
      BOOST_SPIRIT_DEBUG_NODE (destination);
      BOOST_SPIRIT_DEBUG_NODE (pref_cabin);
      BOOST_SPIRIT_DEBUG_NODE (pos_dist);
      BOOST_SPIRIT_DEBUG_NODE (pos_pair);
      BOOST_SPIRIT_DEBUG_NODE (pos_code);
      BOOST_SPIRIT_DEBUG_NODE (pos_share);
      BOOST_SPIRIT_DEBUG_NODE (channel_dist);
      BOOST_SPIRIT_DEBUG_NODE (channel_pair);
      BOOST_SPIRIT_DEBUG_NODE (channel_code);
      BOOST_SPIRIT_DEBUG_NODE (channel_share);
      BOOST_SPIRIT_DEBUG_NODE (trip_dist);
      BOOST_SPIRIT_DEBUG_NODE (trip_pair);
      BOOST_SPIRIT_DEBUG_NODE (trip_code);
      BOOST_SPIRIT_DEBUG_NODE (trip_share);
      BOOST_SPIRIT_DEBUG_NODE (stay_dist);
      BOOST_SPIRIT_DEBUG_NODE (stay_pair);
      BOOST_SPIRIT_DEBUG_NODE (stay_share);
      BOOST_SPIRIT_DEBUG_NODE (ff_dist);
      BOOST_SPIRIT_DEBUG_NODE (ff_pair);
      BOOST_SPIRIT_DEBUG_NODE (ff_code);
      BOOST_SPIRIT_DEBUG_NODE (ff_share);
      BOOST_SPIRIT_DEBUG_NODE (pref_dep_time_dist);
      BOOST_SPIRIT_DEBUG_NODE (pref_dep_time_pair);
      BOOST_SPIRIT_DEBUG_NODE (pref_dep_time_share);
      BOOST_SPIRIT_DEBUG_NODE (time);
      BOOST_SPIRIT_DEBUG_NODE (wtp);
      BOOST_SPIRIT_DEBUG_NODE (time_value_dist);
      BOOST_SPIRIT_DEBUG_NODE (time_value_pair);
      BOOST_SPIRIT_DEBUG_NODE (time_value_share);
      BOOST_SPIRIT_DEBUG_NODE (dtd_dist);
      BOOST_SPIRIT_DEBUG_NODE (dtd_pair);
      BOOST_SPIRIT_DEBUG_NODE (dtd_share);
      BOOST_SPIRIT_DEBUG_NODE (demand_params);
    }

    // //////////////////////////////////////////////////////////////////
    template<typename ScannerT>
    boost::spirit::classic::rule<ScannerT> const&
    DemandParser::definition<ScannerT>::start() const {
      return demand_list;
    }
    
  }


  /////////////////////////////////////////////////////////////////////////
  //
  //  Entry class for the file parser
  //
  /////////////////////////////////////////////////////////////////////////

  // //////////////////////////////////////////////////////////////////////
  DemandFileParser::
  DemandFileParser (stdair::EventQueue& ioEventQueue,
                    stdair::UniformGenerator_T& ioSharedGenerator,
                    const POSProbabilityMass_T& iPOSProbMass,
                    const std::string& iFilename)
    : _filename (iFilename), _eventQueue (ioEventQueue),
      _uniformGenerator (ioSharedGenerator),
      _posProbabilityMass (iPOSProbMass) {
    init();
  }

  // //////////////////////////////////////////////////////////////////////
  void DemandFileParser::init() {
    // Check that the file exists and is readable
    const bool doesExistAndIsReadable =
      stdair::BasFileMgr::doesExistAndIsReadable (_filename);

    if (doesExistAndIsReadable == false) {
      STDAIR_LOG_ERROR ("The demand file " << _filename
                        << " does not exist or can not be read.");

      throw DemandInputFileNotFoundException ("The demand file " + _filename
                                              + " does not exist or can not "
                                              + "be read");
    }
    
    // Open the file
    _startIterator = iterator_t (_filename);

    // Check the filename exists and can be open
    if (!_startIterator) {
      STDAIR_LOG_ERROR ("The demand file " << _filename << " can not be open.");

      throw DemandInputFileNotFoundException ("The demand file " + _filename
                                              + " does not exist or can not "
                                              + "be read");
    }

    // Create an EOF iterator
    _endIterator = _startIterator.make_end();
  }
    
  // //////////////////////////////////////////////////////////////////////
  bool DemandFileParser::generateDemand () {
    bool oResult = false;
      
    STDAIR_LOG_DEBUG ("Parsing demand input file: " << _filename);

    // Initialise the parser (grammar) with the helper/staging structure.
    DemandParserHelper::DemandParser lDemandParser (_eventQueue,
                                                    _uniformGenerator,
                                                    _posProbabilityMass,
                                                    _demand);
      
    // Launch the parsing of the file and, thanks to the doEndDemand
    // call-back structure, the building of the whole EventQueue BOM
    // (i.e., including Inventory, FlightDate, LegDate, SegmentDate, etc.)
    boost::spirit::classic::parse_info<iterator_t> info =
      boost::spirit::classic::parse (_startIterator, _endIterator,
                                     lDemandParser,
                                     boost::spirit::classic::space_p);

    // Retrieves whether or not the parsing was successful
    oResult = info.hit;
      
    const std::string hasBeenFullyReadStr = (info.full == true)?"":"not ";
    if (oResult == true) {
      STDAIR_LOG_DEBUG ("Parsing of demand input file: " << _filename
                       << " succeeded: read " << info.length
                       << " characters. The input file has "
                       << hasBeenFullyReadStr
                       << "been fully read. Stop point: " << info.stop);
        
    } else {
      std::ostringstream oStr;
      oStr << "Parsing of demand input file: " << _filename << " failed: read "
           << info.length << " characters. The input file has "
           << hasBeenFullyReadStr << "been fully read. Stop point: "
           << info.stop;
      STDAIR_LOG_ERROR (oStr.str());
      throw stdair::ParserException (oStr.str());
    }

    return oResult;
  }
    
}
