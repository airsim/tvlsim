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
#include <simfqt/bom/SegmentFeatures.hpp>
#include <simfqt/command/FareQuoter.hpp>

namespace SIMFQT {  

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

    // Get the first segment-date key of the traval solution.
    stdair::SegmentPath_T lSegmentPath =
      ioTravelSolution.getSegmentPath();
    for (stdair::KeyList_T::const_iterator itSegmentPath =
           lSegmentPath.begin ();
         itSegmentPath != lSegmentPath.end (); ++itSegmentPath) {

      const std::string lSegmentDateKey = *itSegmentPath;

      // Parse the segment-date key into a vector of strings.  
      std::vector<std::string> lResultParsing;
      const bool hasParsingBeenSuccesful = 
        parseSegmentDateKey(lSegmentDateKey.begin(), 
                            lSegmentDateKey.end(),
                            lResultParsing);

      assert (hasParsingBeenSuccesful == true);
      assert (lResultParsing.size() == 5);

      priceQuote (iBookingRequest,
                  ioTravelSolution,
                  iBomRoot,
                  lResultParsing);
    }
  }

  // //////////////////////////////////////////////////////////////////////
  void FareQuoter::
  priceQuote (const stdair::BookingRequestStruct& iBookingRequest,
              stdair::TravelSolutionStruct& ioTravelSolution,
              const stdair::BomRoot& iBomRoot,
              const std::vector<std::string>& iResultParsing) {
    
    try {

      // Get the Airport pair stream of the segment path.
      std::ostringstream lAirportPairStr; 
      lAirportPairStr << iResultParsing.back() << std::endl;

      // Search for the fare rules having the same origin and
      // destination airport as the travel solution
      const AirportPair* lAirportPair_ptr = stdair::BomManager::
        getObjectPtr<AirportPair> (iBomRoot, lAirportPairStr.str());  

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
              const AirportPair& iAirportPair,
              const std::vector<std::string>& iResultParsing) {
    
    try {

      // Get the position of the booking request.
      const stdair::CityCode_T& lPosition =
        iBookingRequest.getPOS();

      // Get the booking request channel.
      const stdair::ChannelLabel_T lChannel =
        iBookingRequest.getBookingChannel();

      // Construct the corresponding position-channel primary key.
      const FarePosChannelKey lFarePosChannelKey (lPosition, lChannel);

      // Search for the fare rules having the same position as the travel
      // solution
      const FarePosChannel* lFarePosChannel_ptr =
        stdair::BomManager::
        getObjectPtr<FarePosChannel> (iAirportPair, 
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
              const FarePosChannel& iFarePosChannel,
              const std::vector<std::string>& iResultParsing) {
    
    try {
      
      // Get the date of the segment path.
      const stdair::Date_T lSPDate =
        boost::gregorian::from_simple_string(iResultParsing.at(2));
      bool AtLeastOneAvailableDateRule = false;

      // Get the list of the fare rules.
      const FareDatePeriodList_T& lFareDatePeriodList =
        stdair::BomManager::
        getList<FareDatePeriod>(iFarePosChannel);

      for (FareDatePeriodList_T::const_iterator itDateRange =
             lFareDatePeriodList.begin();
           itDateRange != lFareDatePeriodList.end();
           ++itDateRange) {
        const FareDatePeriod* lcurrentFareDatePeriod_ptr =
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
              const FareDatePeriod& iFareDatePeriod,
              const FarePosChannel& iFarePosChannel,
              const std::vector<std::string>& iResultParsing) {

    try {
      
      // Get the segment boarding time of the segment path.
      const stdair::Duration_T lSPTime = 
        boost::posix_time::duration_from_string(iResultParsing.at(3));
      bool AtLeastOneAvailableTimeRule = false;

      // Get the list of the fare rules.
      const FareTimePeriodList_T& lFareTimePeriodList =
        stdair::BomManager::
        getList<FareTimePeriod>(iFareDatePeriod);

      for (FareTimePeriodList_T::const_iterator itTimeRange =
             lFareTimePeriodList.begin();
           itTimeRange != lFareTimePeriodList.end();
           ++itTimeRange) {
        const FareTimePeriod* lcurrentFareTimePeriod_ptr =
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
              const FareTimePeriod& iFareTimePeriod,
              const FarePosChannel& iFarePosChannel,
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
      
      // Declare fare parameters to determine.
      stdair::PriceValue_T lPrice;
      stdair::SaturdayStay_T lSaturdayStay;
      stdair::ChangeFees_T lChangeFees;
      stdair::NonRefundable_T lNonRefundable;

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
          lPrice = lcurrentFareRuleFeatures_ptr->getFare();
          lSaturdayStay = lcurrentFareRuleFeatures_ptr->getSaturdayStay();
          lChangeFees = lcurrentFareRuleFeatures_ptr->getChangeFees();
          lNonRefundable = lcurrentFareRuleFeatures_ptr->getRefundableOption();

          priceQuote (iBookingRequest,
                      ioTravelSolution,
                      *lcurrentFareRuleFeatures_ptr,
                      iFarePosChannel,
                      iResultParsing,
                      lPrice,
                      lSaturdayStay,
                      lChangeFees,
                      lNonRefundable);
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
              const FarePosChannel& iFarePosChannel,
              const std::vector<std::string>& iResultParsing,
              const stdair::PriceValue_T& iPrice,
              const stdair::SaturdayStay_T& iSaturdayStay,
              const stdair::ChangeFees_T& iChangeFees,
              const stdair::NonRefundable_T& iNonRefundable) {
    
    try {

      // Get the airline code of the segment path.
      const stdair::AirlineCode_T lAirlineCode (iResultParsing.at(0));

      // Get the list of the fare rules.
      const SegmentFeaturesList_T& lSegmentFeaturesList =
        stdair::BomManager::
        getList<SegmentFeatures>(iFareRuleFeatures);

      bool AtLeastOneAvailableAirlineRule = false;

      for (SegmentFeaturesList_T::const_iterator itSegmentFeatures =
             lSegmentFeaturesList.begin();
           itSegmentFeatures != lSegmentFeaturesList.end();
           ++itSegmentFeatures) {
        const SegmentFeatures* lcurrentSegmentFeatures_ptr =
          *itSegmentFeatures;
        assert (lcurrentSegmentFeatures_ptr != NULL);

        // Search for the fare rules having a corresponding airline.
        if (lcurrentSegmentFeatures_ptr->getAirlineCode()
            == lAirlineCode) {

          AtLeastOneAvailableAirlineRule = true;

          // Set the travel fare option.
          stdair::FareOptionStruct lFareOption;
          //lFareOption.addClass (lcurrentSegmentFeatures_ptr->getClassCodeList());
          lFareOption.setFare (iPrice);
          lFareOption.setChangeFees (iChangeFees);
          lFareOption.setNonRefundable (iNonRefundable);
          lFareOption.setSaturdayStay (iSaturdayStay);

          ioTravelSolution.addFareOption(lFareOption);
          
          // DEBUG
          STDAIR_LOG_DEBUG ("Segment path: "
                            << iResultParsing.at(0) << ", "
                            << iResultParsing.at(1) << ", "
                            << iResultParsing.at(2) << ", "
                            << iResultParsing.at(3) << ", "
                            << iResultParsing.at(4) << ", "
                            << "\n   A corresponding fare rule is:"
                            << "\n     Fare:       "
                            << iPrice
                            << "\n     Class:      "
                            //<< lcurrentSegmentFeatures_ptr->getClassCode()
                            << "\n     Conditions: "
                            << iSaturdayStay << ", "
                            << iChangeFees << ", "
                            << iNonRefundable);
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
