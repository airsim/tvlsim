// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <ostream>
// StdAir
#include <stdair/basic/BasParserTypes.hpp>
#include <stdair/bom/BomManager.hpp>
#include <stdair/bom/BomRoot.hpp>
#include <stdair/bom/BookingRequestStruct.hpp>
#include <stdair/bom/TravelSolutionStruct.hpp>
#include <stdair/factory/FacBomManager.hpp>
#include <stdair/service/Logger.hpp>
#include <stdair/bom/key_types.hpp>
// SimFQT
#include <simfqt/SIMFQT_Types.hpp>
#include <simfqt/command/FareQuoter.hpp>

namespace SIMFQT {

  // //////////////////////////////////////////////////////////////////////  
  template <typename ITERATOR>
  const bool parseAirportPair(ITERATOR iFirst, 
                              ITERATOR iLast,
                              std::vector<std::string>& oResult) {

    const bool hasParsingBeenSuccesful = 
      boost::spirit::qi::phrase_parse(iFirst, iLast,
	  		      
		 // Begin grammar  
		 (
		 +(boost::spirit::ascii::char_ -
                   boost::spirit::ascii::char_('-')) % '-'
		 )
		 ,
		 // End grammar

		 boost::spirit::ascii::space, oResult);

    // Fail if we did not get a full match
    if (iFirst != iLast) 
      return false;

    return hasParsingBeenSuccesful;
  }
  

  // //////////////////////////////////////////////////////////////////////  
  template <typename ITERATOR>
  const bool parseSegmentDateKey(ITERATOR iFirst, 
				 ITERATOR iLast,
				 std::vector<std::string>& oResult) {

    const bool hasParsingBeenSuccesful = 
      boost::spirit::qi::phrase_parse(iFirst, iLast,
	  		      
		 // Begin grammar  
		 (
		 +(boost::spirit::ascii::char_ -
                   boost::spirit::ascii::char_(',')) % ','
		 )
		 ,
		 // End grammar 

		 boost::spirit::ascii::space, oResult);

    // Fail if we did not get a full match
    if (iFirst != iLast) 
      return false;

    return hasParsingBeenSuccesful;
  }
  
  // //////////////////////////////////////////////////////////////////////
  void FareQuoter::
  priceQuote (const stdair::BookingRequestStruct& iBookingRequest,
              stdair::TravelSolutionStruct& ioTravelSolution,
              const stdair::BomRoot& iBomRoot) {

    // Get the segment-path of the travel solution.
    const stdair::SegmentPath_T lSegmentPath =
      ioTravelSolution.getSegmentPath();  
    // Get the number of segments of the travel solution.
    const stdair::NbOfSegments_T lNbSegments = lSegmentPath.size();
    assert (lNbSegments >= 1);
    
    // Get the first segment path of the travel solution.
    const std::string lSegmentDateKeyFirst = lSegmentPath.front();
    // Parse the first segment-date key into a vector of strings.  
    std::vector<std::string> lResultParsingFirst;
    const bool hasFirstParsingBeenSuccesful = 
      parseSegmentDateKey(lSegmentDateKeyFirst.begin(), 
                          lSegmentDateKeyFirst.end(),
                          lResultParsingFirst);
    assert (hasFirstParsingBeenSuccesful == true);
    assert (lResultParsingFirst.size() == 5);
    
    // Get the last segment path of the travel solution.
    const std::string lSegmentDateKeyLast = lSegmentPath.back();
    // Parse the last segment-date key into a vector of strings.  
    std::vector<std::string> lResultParsingLast;
    const bool hasLastParsingBeenSuccesful = 
      parseSegmentDateKey(lSegmentDateKeyLast.begin(), 
                          lSegmentDateKeyLast.end(),
                          lResultParsingLast);
    assert (hasLastParsingBeenSuccesful == true);
    assert (lResultParsingLast.size() == 5);

    priceQuote (iBookingRequest,
                ioTravelSolution,
                iBomRoot,
                lResultParsingFirst,
                lResultParsingLast);
  }

  // //////////////////////////////////////////////////////////////////////
  void FareQuoter::
  priceQuote (const stdair::BookingRequestStruct& iBookingRequest,
              stdair::TravelSolutionStruct& ioTravelSolution,
              const stdair::BomRoot& iBomRoot,
              const std::vector<std::string>& iFirstResultParsing,
              const std::vector<std::string>& iLastResultParsing) {
    
    try {

      // Get the Airport pair stream of the first segment path.
      const std::string lFirstAirportPairStr = iFirstResultParsing.back();
      // Parse the first segment-date key into a vector of strings.  
      std::vector<std::string> lResultParsingOrigin;
      const bool hasOriginParsingBeenSuccesful = 
        parseAirportPair(lFirstAirportPairStr.begin(), 
                         lFirstAirportPairStr.end(),
                         lResultParsingOrigin);
      assert (hasOriginParsingBeenSuccesful == true);
      assert (lResultParsingOrigin.size() == 2);

      // Get the Airport pair stream of the last segment path.
      const std::string lLastAirportPairStr = iLastResultParsing.back();
      // Parse the last segment-date key into a vector of strings.  
      std::vector<std::string> lResultParsingDestination;
      const bool hasDestinationParsingBeenSuccesful = 
      parseAirportPair(lLastAirportPairStr.begin(), 
                       lLastAirportPairStr.end(),
                       lResultParsingDestination);
      assert (hasDestinationParsingBeenSuccesful == true);
      assert (lResultParsingDestination.size() == 2);

      // Get the Airport pair stream of the segment path.
      std::ostringstream lAirportPairStr; 
      lAirportPairStr << lResultParsingOrigin.at(0) << "-"
                      << lResultParsingDestination.at(1) << std::endl;

      // Search for the fare rules having the same origin and
      // destination airport as the travel solution
      const stdair::AirportPair* lAirportPair_ptr = stdair::BomManager::
        getObjectPtr<stdair::AirportPair> (iBomRoot, lAirportPairStr.str());  

      if (lAirportPair_ptr == NULL) { 
        STDAIR_LOG_ERROR ("No available fare rule for the "
                          << "Origin-Destination pair: "
                          << lAirportPairStr.str());
        throw AirportPairNotFoundException ("No available fare rule for "
                                            "the Origin-Destination pair: "
                                            + lAirportPairStr.str());
      }
      assert(lAirportPair_ptr != NULL);

      priceQuote (iBookingRequest,
                  ioTravelSolution,
                  *lAirportPair_ptr,
                  iFirstResultParsing);

    } catch (const std::exception& lStdError) {
      STDAIR_LOG_ERROR ("Error: " << lStdError.what());
      throw QuotingException();
    }
  }

  // //////////////////////////////////////////////////////////////////////
  void FareQuoter::
  priceQuote (const stdair::BookingRequestStruct& iBookingRequest,
              stdair::TravelSolutionStruct& ioTravelSolution,
              const stdair::AirportPair& iAirportPair,
              const std::vector<std::string>& iResultParsing) {
    
    try {

      // Get the position of the booking request.
      const stdair::CityCode_T& lPosition =
        iBookingRequest.getPOS();

      // Get the booking request channel.
      const stdair::ChannelLabel_T lChannel =
        iBookingRequest.getBookingChannel();

      // Construct the corresponding position-channel primary key.
      const stdair::PosChannelKey lFarePosChannelKey (lPosition, lChannel);

      // Search for the fare rules having the same position as the travel
      // solution
      const stdair::PosChannel* lFarePosChannel_ptr =
        stdair::BomManager::
        getObjectPtr<stdair::PosChannel> (iAirportPair, 
                                          lFarePosChannelKey.toString());

      if (lFarePosChannel_ptr == NULL) {
        STDAIR_LOG_ERROR ("No available fare rule corresponding to the "
                          "position " << lPosition
                          << ", to the channel " << lChannel
                          << " and to the Origin-Destination pair: "
                          << iAirportPair.toString());
        throw PosOrChannelNotFoundException ("No available fare rule for the "
                                         "position " + lPosition
                                         + " and the channel " + lChannel);
      }
      assert(lFarePosChannel_ptr != NULL);

      priceQuote (iBookingRequest,
                  ioTravelSolution,
                  *lFarePosChannel_ptr,
                  iResultParsing);

    } catch (const std::exception& lStdError) {
      STDAIR_LOG_ERROR ("Error: " << lStdError.what());
      throw QuotingException();
    }
  }

  // //////////////////////////////////////////////////////////////////////
  void FareQuoter::
  priceQuote (const stdair::BookingRequestStruct& iBookingRequest,
              stdair::TravelSolutionStruct& ioTravelSolution,
              const stdair::PosChannel& iFarePosChannel,
              const std::vector<std::string>& iResultParsing) {
    
    try {
      
      // Get the date of the segment path.
      const stdair::Date_T lSPDate =
        boost::gregorian::from_simple_string(iResultParsing.at(2));
      bool AtLeastOneAvailableDateRule = false;

      // Get the list of the fare rules.
      const stdair::DatePeriodList_T& lFareDatePeriodList =
        stdair::BomManager::
        getList<stdair::DatePeriod>(iFarePosChannel);

      for (stdair::DatePeriodList_T::const_iterator itDateRange =
             lFareDatePeriodList.begin();
           itDateRange != lFareDatePeriodList.end();
           ++itDateRange) {
        const stdair::DatePeriod* lcurrentFareDatePeriod_ptr =
          *itDateRange ;
        assert (lcurrentFareDatePeriod_ptr != NULL);

        // Select the fare rules having a corresponding date range. 
        if (lcurrentFareDatePeriod_ptr->isDepartureDateValid(lSPDate)
            == true) {

          AtLeastOneAvailableDateRule = true;

          priceQuote (iBookingRequest,
                      ioTravelSolution,
                      *lcurrentFareDatePeriod_ptr,
                      iFarePosChannel,
                      iResultParsing);
        }
      }
      
      if (AtLeastOneAvailableDateRule == false) {
        STDAIR_LOG_ERROR ("No available fare rule corresponding to the "
                          "flight date " << iResultParsing.at(2)
                          << ", to the position "
                          << iFarePosChannel.getPosition()
                          << ", to the channel "
                          << iFarePosChannel.getChannel()
                          << " and to the Origin-Destination pair: "
                          << iResultParsing.back());
        throw FlightDateNotFoundException ("No available fare rule for the "
                                         "flight date "
                                         + iResultParsing.at(2));
      }
      
      assert (AtLeastOneAvailableDateRule == true);
    } catch (const std::exception& lStdError) {
      STDAIR_LOG_ERROR ("Error: " << lStdError.what());
      throw QuotingException();
    }
  }

  // //////////////////////////////////////////////////////////////////////
  void FareQuoter::
  priceQuote (const stdair::BookingRequestStruct& iBookingRequest,
              stdair::TravelSolutionStruct& ioTravelSolution,
              const stdair::DatePeriod& iFareDatePeriod,
              const stdair::PosChannel& iFarePosChannel,
              const std::vector<std::string>& iResultParsing) {

    try {
      
      // Get the segment boarding time of the segment path.
      const stdair::Duration_T lSPTime = 
        boost::posix_time::duration_from_string(iResultParsing.at(3));
      bool AtLeastOneAvailableTimeRule = false;

      // Get the list of the fare rules.
      const stdair::TimePeriodList_T& lFareTimePeriodList =
        stdair::BomManager::
        getList<stdair::TimePeriod>(iFareDatePeriod);

      for (stdair::TimePeriodList_T::const_iterator itTimeRange =
             lFareTimePeriodList.begin();
           itTimeRange != lFareTimePeriodList.end();
           ++itTimeRange) {
        const stdair::TimePeriod* lcurrentFareTimePeriod_ptr =
          *itTimeRange ;
        assert (lcurrentFareTimePeriod_ptr != NULL);

        // Select the fare rules having a corresponding time range.
        if (lcurrentFareTimePeriod_ptr->isDepartureTimeValid(lSPTime)
            == true) {

          AtLeastOneAvailableTimeRule = true;

          priceQuote (iBookingRequest,
                      ioTravelSolution,
                      *lcurrentFareTimePeriod_ptr,
                      iFarePosChannel,
                      iResultParsing);
        }
      }
      
      if (AtLeastOneAvailableTimeRule == false) {
        STDAIR_LOG_ERROR ("No available fare rule corresponding to the "
                          "flight boarding time " << iResultParsing.at(3)
                          << ", to the flight date "
                          << iResultParsing.at(2)
                          << ", to the position "
                          << iFarePosChannel.getPosition()
                          << ", to the channel "
                          << iFarePosChannel.getChannel()
                          << " and to the Origin-Destination pair: "
                          << iResultParsing.back());
        throw FlightTimeNotFoundException ("No available fare rule for the "
                                         "flight boarding time "
                                         + iResultParsing.at(3));
      }
      
      assert (AtLeastOneAvailableTimeRule == true);
    } catch (const std::exception& lStdError) {
      STDAIR_LOG_ERROR ("Error: " << lStdError.what());
      throw QuotingException();
    }
  }

  // //////////////////////////////////////////////////////////////////////
  void FareQuoter::
  priceQuote (const stdair::BookingRequestStruct& iBookingRequest,
              stdair::TravelSolutionStruct& ioTravelSolution,
              const stdair::TimePeriod& iFareTimePeriod,
              const stdair::PosChannel& iFarePosChannel,
              const std::vector<std::string>& iResultParsing) {

    try {

      // Get the stay duration of the booking request.
      const stdair::DayDuration_T lStayDuration =
        iBookingRequest.getStayDuration();

      // Get the booking request date time.
      const stdair::DateTime_T lRequestDateTime =
        iBookingRequest.getRequestDateTime();

      // Get the flight date of the segment path.
      const stdair::Date_T lSPDate =
        boost::gregorian::from_simple_string(iResultParsing.at(2));

      // Get the segment boarding time of the segment path.
      const stdair::Duration_T lSPTime = 
        boost::posix_time::duration_from_string(iResultParsing.at(3));

      // Construct the date-time type correponding to the flight date
      const stdair::DateTime_T lSPDateTime (lSPDate, lSPTime);

      bool AtLeastOneAvailableFeaturesRule = false;
      bool IsStayDurationValid = false;
      bool IsAdvancePurchaseValid = false;

      // Get the list of the fare rules.
      const FareRuleFeaturesList_T& lFareRuleFeaturesList =
        stdair::BomManager::
        getList<FareRuleFeatures> (iFareTimePeriod);

      for (FareRuleFeaturesList_T::const_iterator itFareFeatures =
           lFareRuleFeaturesList.begin();
           itFareFeatures != lFareRuleFeaturesList.end();
           ++itFareFeatures) {
        const FareRuleFeatures* lcurrentFareRuleFeatures_ptr =
          *itFareFeatures;
        assert (lcurrentFareRuleFeatures_ptr != NULL);

        IsStayDurationValid =
          lcurrentFareRuleFeatures_ptr->IsStayDurationValid (lStayDuration);
        IsAdvancePurchaseValid = lcurrentFareRuleFeatures_ptr->
          IsAdvancePurchaseValid (lRequestDateTime,
                                  lSPDateTime);
        
        // Search for the fare rules having corresponding features.
        if (IsStayDurationValid && IsAdvancePurchaseValid){
          AtLeastOneAvailableFeaturesRule = true;
          // Set the travel fare option.
          stdair::FareOptionStruct lFareOption;
          lFareOption.setFare (lcurrentFareRuleFeatures_ptr->getFare());
          lFareOption.setChangeFees (lcurrentFareRuleFeatures_ptr->getChangeFees());
          lFareOption.setNonRefundable (lcurrentFareRuleFeatures_ptr->getRefundableOption());
          lFareOption.setSaturdayStay (lcurrentFareRuleFeatures_ptr->getSaturdayStay());

          priceQuote (iBookingRequest,
                      ioTravelSolution,
                      *lcurrentFareRuleFeatures_ptr,
                      iFarePosChannel,
                      lFareOption,
                      iResultParsing);
        } 

      }
      if (AtLeastOneAvailableFeaturesRule == false) {
        std::string lStayDurationString;
        std::stringstream lStayDurationStream;
        lStayDurationStream << lStayDuration;
        lStayDurationString = lStayDurationStream.str();
        STDAIR_LOG_ERROR ("No available fare rule corresponding to the "
                          "stay duration " <<  lStayDurationString
                          << ", to the request date time "
                          << lRequestDateTime
                          << ", to the flight boarding time "
                          << iResultParsing.at(3)
                          << ", to the flight date "
                          << iResultParsing.at(2)
                          << ", to the position "
                          << iFarePosChannel.getPosition()
                          << ", to the channel "
                          << iFarePosChannel.getChannel()
                          << " and to the Origin-Destination pair: "
                          << iResultParsing.back());
        throw FeaturesNotFoundException ("No available fare rule for the "
                                         "stay duration " +
                                         lStayDurationString +
                                         " and the request date time " +
                                         boost::posix_time::to_simple_string(lRequestDateTime));
      }
      assert (AtLeastOneAvailableFeaturesRule == true);
    } catch (const std::exception& lStdError) {
      STDAIR_LOG_ERROR ("Error: " << lStdError.what());
      throw QuotingException();
    }
  }
  // //////////////////////////////////////////////////////////////////////
  void FareQuoter::
  priceQuote (const stdair::BookingRequestStruct& iBookingRequest,
              stdair::TravelSolutionStruct& ioTravelSolution,
              const FareRuleFeatures& iFareRuleFeatures,
              const stdair::PosChannel& iFarePosChannel,
              stdair::FareOptionStruct& iFareOption,
              const std::vector<std::string>& iResultParsing) {
    
    try {

      // Get the segment-path of the travel solution.
      const stdair::SegmentPath_T lSegmentPath =
        ioTravelSolution.getSegmentPath();

      // Get the airline code of the segment path.
      const stdair::AirlineCode_T lAirlineCode (iResultParsing.at(0));

      // Get the list of the fare rules.
      const stdair::AirlineClassListList_T& lAirlineClassListList =
        stdair::BomManager::
        getList<stdair::AirlineClassList>(iFareRuleFeatures);

      bool AtLeastOneAvailableAirlineRule = false;
      bool CorrectAirlineRule = false;
      bool AtLeastOneDifferentAirline = false; 

      // Search for the fare rules having a corresponding airline list.
      for (stdair::AirlineClassListList_T::const_iterator itAirlineClassList =
             lAirlineClassListList.begin();
           itAirlineClassList != lAirlineClassListList.end();
           ++itAirlineClassList) {
        const stdair::AirlineClassList* lcurrentAirlineClassList_ptr =
          *itAirlineClassList;
        assert (lcurrentAirlineClassList_ptr != NULL);

        CorrectAirlineRule = true;
        AtLeastOneDifferentAirline = false;
        
        const stdair::ClassList_StringList_T lClassList_StringList =
          lcurrentAirlineClassList_ptr->getAirlineCodeList();

        // Compare the segment path airline list with the fare rule airline list.
        if (lClassList_StringList.size() == lSegmentPath.size()) {
          stdair::SegmentPath_T::const_iterator lItSegmentPath =
            lSegmentPath.begin();
          stdair::ClassList_StringList_T::const_iterator lItClassList_String =
            lClassList_StringList.begin();
          while (lItSegmentPath != lSegmentPath.end() && AtLeastOneDifferentAirline == false) {
            // Get the current segment path of the travel solution.
            const std::string lSegmentDateKey = *lItSegmentPath;
            // Parse the segment-date key into a vector of strings.  
            std::vector<std::string> lResultParsing;
            const bool hasParsingBeenSuccesful = 
              parseSegmentDateKey(lSegmentDateKey.begin(), 
                          lSegmentDateKey.end(),
                          lResultParsing);
            assert (hasParsingBeenSuccesful == true);
            assert (lResultParsing.size() == 5);
            if (lResultParsing.at(0) != *lItClassList_String) {
              AtLeastOneDifferentAirline = true;
            }
            lItSegmentPath++;
            lItClassList_String++;
          }      
        } else {
          CorrectAirlineRule = false;
        }
        if (AtLeastOneDifferentAirline == true) {
          CorrectAirlineRule = false;
        }
          
        if (CorrectAirlineRule == true) {
          AtLeastOneAvailableAirlineRule = true;
          // Set the travel fare option.
          stdair::ClassList_StringList_T lClassCodeList = lcurrentAirlineClassList_ptr->getClassCodeList();
          for (stdair::ClassList_StringList_T::const_iterator itClassCodeList =
                 lClassCodeList.begin();
               itClassCodeList != lClassCodeList.end();
               ++itClassCodeList ) {
            iFareOption.addClassList (*itClassCodeList);
          }
          ioTravelSolution.addFareOption(iFareOption);
          
          // DEBUG
          STDAIR_LOG_DEBUG ("Segment path: "
                            << iResultParsing.at(0) << ", "
                            << iResultParsing.at(1) << ", "
                            << iResultParsing.at(2) << ", "
                            << iResultParsing.at(3) << ", "
                            << iResultParsing.at(4) 
                            << "\n   A corresponding fare rule is:"
                            << "\n     Class:      "
                            << lcurrentAirlineClassList_ptr->describeKey()
                            << "     Fare:       "
                            << iFareOption.getFare() 
                            << "\n     Conditions: "
                            << iFareOption.getSaturdayStay()  << ", "
                            << iFareOption.getChangeFees() << ", "
                            << iFareOption.getNonRefundable());
        }
      }
      if (AtLeastOneAvailableAirlineRule == false) {
        STDAIR_LOG_ERROR ("No available fare rule corresponding to the "
                          "airline " << iResultParsing.at(0)
                          << ", to the stay duration "
                          << iBookingRequest.getStayDuration()
                          << ", to the request date time "
                          << iBookingRequest.getRequestDateTime()
                          << ", to the flight boarding time "
                          << iResultParsing.at(3)
                          << ", to the flight date "
                          << iResultParsing.at(2)
                          << ", to the position "
                          << iFarePosChannel.getPosition()
                          << ", to the channel "
                          << iFarePosChannel.getChannel()
                          << " and to the Origin-Destination pair: "
                          << iResultParsing.back());
        throw AirlineNotFoundException ("No available fare rule for the "
                                         "airline " +
                                         iResultParsing.at(0));
       }
       assert (AtLeastOneAvailableAirlineRule == true);

    } catch (const std::exception& lStdError) {
      STDAIR_LOG_ERROR ("Error: " << lStdError.what());
      throw QuotingException();
    }
  }

}
