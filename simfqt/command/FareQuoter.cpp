// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <sstream>
// StdAir
#include <stdair/basic/BasParserTypes.hpp>
#include <stdair/basic/BasConst_BomDisplay.hpp>
#include <stdair/bom/BomKeyManager.hpp>
#include <stdair/bom/ParsedKey.hpp>
#include <stdair/bom/BomManager.hpp>
#include <stdair/bom/BomRoot.hpp>
#include <stdair/bom/InventoryKey.hpp>
#include <stdair/bom/FlightDateKey.hpp>
#include <stdair/bom/SegmentDateKey.hpp>
#include <stdair/bom/AirlineClassList.hpp>
#include <stdair/bom/AirportPair.hpp>
#include <stdair/bom/PosChannel.hpp>
#include <stdair/bom/DatePeriod.hpp>
#include <stdair/bom/TimePeriod.hpp>
#include <stdair/bom/FareFeatures.hpp>
#include <stdair/bom/BookingRequestStruct.hpp>
#include <stdair/bom/TravelSolutionStruct.hpp>
#include <stdair/service/Logger.hpp>
#include <stdair/bom/key_types.hpp>
// SimFQT
#include <simfqt/SIMFQT_Types.hpp>
#include <simfqt/command/FareQuoter.hpp>

namespace SIMFQT {

  bool FareQuoter::_atLeastOneAvailableDateRule = false;
  bool FareQuoter::_atLeastOneAvailablePosChannel = false;
  bool FareQuoter::_atLeastOneAvailableTimeRule = false;
  bool FareQuoter::_atLeastOneAvailableFeaturesRule = false;
  bool FareQuoter::_atLeastOneAvailableAirlineClassRule= false;

  // //////////////////////////////////////////////////////////////////////
  FareQuoter::FareQuoter() {
    assert (false);
  }

  // //////////////////////////////////////////////////////////////////////
  FareQuoter::FareQuoter(const FareQuoter&) {
    assert (false);
  }

  // //////////////////////////////////////////////////////////////////////
  FareQuoter::~FareQuoter() {
  }
  
  // //////////////////////////////////////////////////////////////////////
  void FareQuoter::reset() {
    _atLeastOneAvailableDateRule = false;
    _atLeastOneAvailablePosChannel = false;
    _atLeastOneAvailableTimeRule = false;
    _atLeastOneAvailableFeaturesRule = false;
    _atLeastOneAvailableAirlineClassRule = false;
  }


  // //////////////////////////////////////////////////////////////////////
  void FareQuoter::
  priceQuote (const stdair::BookingRequestStruct& iBookingRequest,
              stdair::TravelSolutionList_T& ioTravelSolutionList,
              const stdair::BomRoot& iBomRoot) {

    // Do an independent price quote for each travel solution related to the
    // booking request.
    for (stdair::TravelSolutionList_T::iterator itTravelSolution =
           ioTravelSolutionList.begin();
         itTravelSolution != ioTravelSolutionList.end(); ++itTravelSolution) {
      reset();
      // Select a travel solution.
      stdair::TravelSolutionStruct& lTravelSolutionStruct = *itTravelSolution;
      // Price quote the travel solution into question.
      priceQuote (iBookingRequest, lTravelSolutionStruct, iBomRoot);
    }
  }

  // //////////////////////////////////////////////////////////////////////
  void FareQuoter::
  priceQuote (const stdair::BookingRequestStruct& iBookingRequest,
              stdair::TravelSolutionStruct& ioTravelSolution,
              const stdair::BomRoot& iBomRoot) {

    // Get the origin of the first segment in order to get the origin of
    // the solution.
    const stdair::ParsedKey& lFirstSegmentKey =
      getFirstSPParsedKey(ioTravelSolution);
    const stdair::AirportCode_T& lOrigin = lFirstSegmentKey._boardingPoint;

    // Get the destination of the last segment in order to get the
    // destination of the solution.
    const stdair::ParsedKey& lLastSegmentKey =
      getLastSPParsedKey(ioTravelSolution);
    const stdair::AirportCode_T& lDestination = lLastSegmentKey._offPoint;

    // Construct the Airport pair stream of the segment path.
    const stdair::AirportPairKey lAirportPairKey (lOrigin, lDestination);
    
    // Search for the fare rules having the same origin and destination airports
    // as the travel solution
    const stdair::AirportPair* lAirportPair_ptr = stdair::BomManager::
      getObjectPtr<stdair::AirportPair> (iBomRoot, lAirportPairKey.toString());  

    // If no fare rule has the same origin and destination airports, the pricing
    // is not possible, throw an exception.
    if (lAirportPair_ptr == NULL) { 
      STDAIR_LOG_ERROR ("No available fare rule for the "
                        << "Origin-Destination pair: "
                        << lAirportPairKey.toString());
      throw AirportPairNotFoundException ("No available fare rule for "
                                          "the Origin-Destination pair: "
                                          + lAirportPairKey.toString());
    }
    // Sanity check.
    assert(lAirportPair_ptr != NULL);

    // Fare rule(s) with the same origin and destination airports exist(s), now
    // the date range need to be checked.
    const stdair::AirportPair& lAirportPair = *lAirportPair_ptr;
    priceQuote(iBookingRequest, ioTravelSolution, lAirportPair);

    if (_atLeastOneAvailableAirlineClassRule == false) {
      displayMissingFareRuleMessage(iBookingRequest, ioTravelSolution);
    }
  }

  // //////////////////////////////////////////////////////////////////////
  void FareQuoter::
  priceQuote (const stdair::BookingRequestStruct& iBookingRequest,
              stdair::TravelSolutionStruct& ioTravelSolution,
              const stdair::AirportPair& iAirportPair) {

    // Get the first segment path parsed key.
    const stdair::ParsedKey lFirstSPParsedKey =
      getFirstSPParsedKey(ioTravelSolution);

    // Get the date of the first segment date key.
    const stdair::FlightDateKey& lFlightDateKey =
      lFirstSPParsedKey.getFlightDateKey();
    const stdair::Date_T& lSPDate = lFlightDateKey.getDepartureDate();

    // Get the list of the fare date ranges.
    const stdair::DatePeriodList_T& lFareDatePeriodList =
      stdair::BomManager::getList<stdair::DatePeriod> (iAirportPair);

    // Browse the list of the fare rules date range.
    for (stdair::DatePeriodList_T::const_iterator itDateRange =
           lFareDatePeriodList.begin();
         itDateRange != lFareDatePeriodList.end(); ++itDateRange) {

      const stdair::DatePeriod* lCurrentFareDatePeriod_ptr = *itDateRange ;
      assert (lCurrentFareDatePeriod_ptr != NULL);

      // Select the fare rules having a corresponding date range.
      const bool isDepartureDateValid =
        lCurrentFareDatePeriod_ptr->isDepartureDateValid (lSPDate);

      // If a fare rule has a corresponding date range, its channel and position
      // need to be checked.
      if (isDepartureDateValid == true) {
        _atLeastOneAvailableDateRule = true;
        const stdair::DatePeriod& lCurrentFareDatePeriod =
          *lCurrentFareDatePeriod_ptr;
        priceQuote (iBookingRequest, ioTravelSolution,
                    lCurrentFareDatePeriod, iAirportPair);
      }
    }
    
  }

  // //////////////////////////////////////////////////////////////////////
  void FareQuoter::
  priceQuote (const stdair::BookingRequestStruct& iBookingRequest,
              stdair::TravelSolutionStruct& ioTravelSolution,
              const stdair::DatePeriod& iFareDatePeriod,
              const stdair::AirportPair& iAirportPair) {

    // Get the point-of-sale of the booking request.
    const stdair::CityCode_T& lPointOfSale = iBookingRequest.getPOS();

    // Get the booking request channel.
    const stdair::ChannelLabel_T& lChannel =
      iBookingRequest.getBookingChannel();

    // Construct the corresponding POS-channel primary key.
    const stdair::PosChannelKey lFarePosChannelKey (lPointOfSale, lChannel);

    // Search for the fare rules having the same point-of-sale and channel as
    // the travel solution.
    const stdair::PosChannelList_T lFarePosChannelList =
      stdair::BomManager::getList<stdair::PosChannel> (iFareDatePeriod);

    // Browse the list of the fare rules pos channel.
    for (stdair::PosChannelList_T::const_iterator itPosChannel =
           lFarePosChannelList.begin();
         itPosChannel != lFarePosChannelList.end();
         ++itPosChannel) {
      const stdair::PosChannel* lCurrentFarePosChannel_ptr = *itPosChannel;
      assert (lCurrentFarePosChannel_ptr != NULL);

      // Get the point-of-sale and channel of the current fare rule.
      const stdair::CityCode_T& lCurrentPointOfSale =
        lCurrentFarePosChannel_ptr->getPos();
      const stdair::ChannelLabel_T& lCurrentChannel =
        lCurrentFarePosChannel_ptr->getChannel();

      // Select the fare rules having a corresponding pos channel.
      if (lCurrentPointOfSale == lPointOfSale &&
          lCurrentChannel == lChannel) {
        _atLeastOneAvailablePosChannel = true;
        // Fare rule(s) with the same point-of-sale and channel exist(s), now
        // the time range need to be checked.
        const stdair::PosChannel& lFarePosChannel= *lCurrentFarePosChannel_ptr;
        priceQuote (iBookingRequest, ioTravelSolution, lFarePosChannel);
      }
    }

  }

  // //////////////////////////////////////////////////////////////////////
  void FareQuoter::
  priceQuote (const stdair::BookingRequestStruct& iBookingRequest,
              stdair::TravelSolutionStruct& ioTravelSolution,
              const stdair::PosChannel& iFarePosChannel) {

    // Get the first segment path parsed key.
    const stdair::ParsedKey lFirstSPParsedKey =
      getFirstSPParsedKey(ioTravelSolution);
      
    // Get the segment boarding time of the segment path.
    const stdair::Duration_T& lSPTime = lFirstSPParsedKey.getBoardingTime();

    // Get the list of the fare rules time period.
    const stdair::TimePeriodList_T& lFareTimePeriodList =
      stdair::BomManager::getList<stdair::TimePeriod> (iFarePosChannel);

    // Browse the list of the fare rules time range.
    for (stdair::TimePeriodList_T::const_iterator itTimeRange =
           lFareTimePeriodList.begin();
         itTimeRange != lFareTimePeriodList.end();
         ++itTimeRange) {
      const stdair::TimePeriod* lCurrentFareTimePeriod_ptr = *itTimeRange ;
      assert (lCurrentFareTimePeriod_ptr != NULL);

      // Select the fare rules having a corresponding time range.
      const bool isDepartureTimeValid =
        lCurrentFareTimePeriod_ptr->isDepartureTimeValid (lSPTime);

      // If a fare rule has a corresponding time range, its advanced purchase,
      // trip type and minimum stay duration need to be checked.
      if (isDepartureTimeValid) {
        _atLeastOneAvailableTimeRule = true;
        const stdair::TimePeriod& lCurrentFareTimePeriod =
          *lCurrentFareTimePeriod_ptr;
        priceQuote (iBookingRequest, ioTravelSolution,
                    lCurrentFareTimePeriod, iFarePosChannel);
      }
    }

  }

  // //////////////////////////////////////////////////////////////////////
  void FareQuoter::
  priceQuote (const stdair::BookingRequestStruct& iBookingRequest,
              stdair::TravelSolutionStruct& ioTravelSolution,
              const stdair::TimePeriod& iFareTimePeriod,
              const stdair::PosChannel& iFarePosChannel) {

    // Get the stay duration of the booking request.
    const stdair::DayDuration_T& lStayDuration=
      iBookingRequest.getStayDuration();

    // Get the booking request trip type.
    const stdair::TripType_T& lTripType =
      iBookingRequest.getTripType();

    // Get the booking request date time.
    const stdair::DateTime_T& lRequestDateTime =
      iBookingRequest.getRequestDateTime();
    
    // Get the referenced departure date of the segment path.
    const stdair::ParsedKey lFirstSPParsedKey =
      getFirstSPParsedKey(ioTravelSolution);
    const stdair::Date_T& lSPDate =
      lFirstSPParsedKey.getFlightDateKey().getDepartureDate();

    // Get the segment boarding time of the segment path.
    const stdair::Duration_T& lSPTime = lFirstSPParsedKey.getBoardingTime();

    // Construct the date-time type correponding to the flight date
    const stdair::DateTime_T lSPDateTime (lSPDate, lSPTime);

    bool isTripTypeValid = false;
    bool isStayDurationValid = false;
    bool isAdvancePurchaseValid = false;

    // Get the list of the fare features.
    const stdair::FareFeaturesList_T& lFareFeaturesList =
      stdair::BomManager::getList<stdair::FareFeatures> (iFareTimePeriod);

    // Browse the list of the fare rules features.
    for (stdair::FareFeaturesList_T::const_iterator itFareFeatures =
           lFareFeaturesList.begin();
         itFareFeatures != lFareFeaturesList.end();
         ++itFareFeatures) {
      const stdair::FareFeatures* lCurrentFareFeatures_ptr =
        *itFareFeatures;
      assert (lCurrentFareFeatures_ptr != NULL);

      // Does the current fare features correspond to a correct trip
      // type?
      isTripTypeValid =
        lCurrentFareFeatures_ptr->isTripTypeValid (lTripType);
      // Does the current fare features correspond to a correct stay
      // duration?
      isStayDurationValid =
        lCurrentFareFeatures_ptr->isStayDurationValid (lStayDuration);
      // Does the current fare features correspond to a correct advanced
      // purchase?
      isAdvancePurchaseValid = lCurrentFareFeatures_ptr->
        isAdvancePurchaseValid (lRequestDateTime,
                                lSPDateTime);
        
      // Search for the fare rules having corresponding features.
      if (isStayDurationValid && isAdvancePurchaseValid && isTripTypeValid){
        _atLeastOneAvailableFeaturesRule = true;
        // Create a fare structure for the travel solution.
        stdair::FareOptionStruct lFareOption;
        const stdair::ChangeFees_T& lChangeFees =
          lCurrentFareFeatures_ptr->getChangeFees();
        // Set the fare change fees.
        lFareOption.setChangeFees (lChangeFees);
        const stdair::NonRefundable_T& lNonRefundable =
          lCurrentFareFeatures_ptr->getRefundableOption();
        // Set the fare refundable option.
        lFareOption.setNonRefundable (lNonRefundable);
        const stdair::SaturdayStay_T& lSaturdayStay =
          lCurrentFareFeatures_ptr->getSaturdayStay();
        // Set the fare saturday night stay option.
        lFareOption.setSaturdayStay (lSaturdayStay);
        const stdair::FareFeatures& lCurrentFareFeatures =
          *lCurrentFareFeatures_ptr;
        priceQuote (iBookingRequest, ioTravelSolution,
                    lCurrentFareFeatures, iFarePosChannel,
                    lFareOption);
      }
    }
    
  }

  
  // //////////////////////////////////////////////////////////////////////
  void FareQuoter::
  priceQuote (const stdair::BookingRequestStruct& iBookingRequest,
              stdair::TravelSolutionStruct& ioTravelSolution,
              const stdair::FareFeatures& iFareFeatures,
              const stdair::PosChannel& iFarePosChannel,
              stdair::FareOptionStruct& iFareOption) {

    // Get the first segment path parsed key.
    const stdair::ParsedKey lFirstSPParsedKey =
      getFirstSPParsedKey(ioTravelSolution);
    
    // Get the segment-path of the travel solution.
    const stdair::SegmentPath_T& lSegmentPath =
      ioTravelSolution.getSegmentPath();

    // Get the list of the fare rules.
    const stdair::AirlineClassListList_T& lAirlineClassListList =
      stdair::BomManager::getList<stdair::AirlineClassList> (iFareFeatures);

    bool lCorrectAirlineRule = false;
    bool lAtLeastOneDifferentAirline = false; 

    // Browse the list of airline code list and search for the fare rules
    // having a corresponding airline list.
    for (stdair::AirlineClassListList_T::const_iterator itAirlineClassList =
           lAirlineClassListList.begin();
         itAirlineClassList != lAirlineClassListList.end();
         ++itAirlineClassList) {
      const stdair::AirlineClassList* lCurrentAirlineClassList_ptr =
        *itAirlineClassList;
      assert (lCurrentAirlineClassList_ptr != NULL);

      lCorrectAirlineRule = true;
      lAtLeastOneDifferentAirline = false;
        
      const stdair::ClassList_StringList_T lClassList_StringList =
        lCurrentAirlineClassList_ptr->getAirlineCodeList();

      // Compare the segment path airline list with the fare rule airline list.
      if (lClassList_StringList.size() == lSegmentPath.size()) {
        // If the two sizes are equal, we need to compare the airline codes.
        stdair::SegmentPath_T::const_iterator itSegmentPath =
          lSegmentPath.begin();

        stdair::ClassList_StringList_T::const_iterator itClassList_String =
          lClassList_StringList.begin();
        // Browse the segment path airline code list (while the segment path
        // airline list is equal to the fare rule airline list).
        while (itSegmentPath != lSegmentPath.end()
               && lAtLeastOneDifferentAirline == false) {

          // Get the segment airline code.
          const std::string lSegmentDateKey = *itSegmentPath;
          const stdair::ParsedKey& lParsedKey =
            stdair::BomKeyManager::extractKeys (lSegmentDateKey);
          const stdair::InventoryKey& lInventoryKey =
            lParsedKey.getInventoryKey();
          const stdair::AirlineCode_T& lSegmentAirlineCode =
            lInventoryKey.getAirlineCode();

          // Get the fare rule airline code.
          const stdair::AirlineCode_T& lFareRuleAirlineCode =
            *itClassList_String;

          if (lSegmentAirlineCode != lFareRuleAirlineCode) {
            lAtLeastOneDifferentAirline = true;
          }
          itSegmentPath++;
          itClassList_String++;
        }      

      } else {
        // If the two sizes are different, the fare rule does not match the
        // travel solution into question.
        lCorrectAirlineRule = false;
      }
      
      // If one segment airline code and one fare rule airline code are
      // different then the fare rule does not match the travel solution.
      if (lAtLeastOneDifferentAirline == true) {
        lCorrectAirlineRule = false;
      }

      // If the current fare rule is a match, add the fare option structure
      // to the travel solution into question.
      if (lCorrectAirlineRule == true) {
        _atLeastOneAvailableAirlineClassRule = true;
        // Get the booking request trip type.
        const stdair::TripType_T& lTripType =
          iBookingRequest.getTripType();

        // Get the travel fare.
        stdair::Fare_T lFare =
          lCurrentAirlineClassList_ptr->getFare();
        // If the trip into question is the inbound or outbound part of a round trip,
        // the applicable fare is a half RT fare.
        if (lTripType == "RI" || lTripType == "RO") {
          lFare /= 2;
        }
        // Set the travel fare option.
        iFareOption.setFare (lFare);
        // Copy the class path list into the fare option.
        const stdair::ClassList_StringList_T& lClassCodeList =
          lCurrentAirlineClassList_ptr->getClassCodeList();
        for (stdair::ClassList_StringList_T::const_iterator itClassCodeList =
               lClassCodeList.begin();
             itClassCodeList != lClassCodeList.end(); ++itClassCodeList ) {
          const stdair::ClassList_String_T& lClassCodeList = *itClassCodeList;
          iFareOption.addClassList (lClassCodeList);
        }

        // Add the fare option to the travel solution into question.
        ioTravelSolution.addFareOption (iFareOption);
          
        // DEBUG
        STDAIR_LOG_DEBUG ("Segment path: " << lFirstSPParsedKey.toString()
                          << ". A corresponding fare option for the '"
                          << lCurrentAirlineClassList_ptr->describeKey()
                          << "' class is: " << iFareOption);

        iFareOption.emptyClassList();
      }
    }

  }

  // //////////////////////////////////////////////////////////////////////
  stdair::ParsedKey FareQuoter::
  getFirstSPParsedKey (stdair::TravelSolutionStruct& ioTravelSolution) {

    // Get the segment-path of the travel solution.
    const stdair::SegmentPath_T& lSegmentPath =
      ioTravelSolution.getSegmentPath();  

    // Get the number of segments of the travel solution.
    const stdair::NbOfSegments_T& lNbSegments = lSegmentPath.size();

    // Sanity check: there is at least one segment in the travel solution.
    assert (lNbSegments >= 1);
    
    // Get the first segment of the travel solution.
    const std::string& lFirstSegmentDateKey = lSegmentPath.front();

    // Get the parsed key of the first segment of the travel solution.
    const stdair::ParsedKey& lFirstSegmentParsedKey =
      stdair::BomKeyManager::extractKeys (lFirstSegmentDateKey);

    return lFirstSegmentParsedKey;
    
  }

  // //////////////////////////////////////////////////////////////////////
  stdair::ParsedKey FareQuoter::
  getLastSPParsedKey (stdair::TravelSolutionStruct& ioTravelSolution) {

    // Get the segment-path of the travel solution.
    const stdair::SegmentPath_T& lSegmentPath =
      ioTravelSolution.getSegmentPath();  

    // Get the number of segments of the travel solution.
    const stdair::NbOfSegments_T& lNbSegments = lSegmentPath.size();

    // Sanity check: there is at least one segment in the travel solution.
    assert (lNbSegments >= 1);
    
    // Get the last segment of the travel solution.
    const std::string& lLastSegmentDateKey = lSegmentPath.back();

    // Get the parsed key of the last segment of the travel solution.
    const stdair::ParsedKey& lLastSegmentParsedKey =
      stdair::BomKeyManager::extractKeys (lLastSegmentDateKey);

    return lLastSegmentParsedKey;
    
  }

  // //////////////////////////////////////////////////////////////////////
  void FareQuoter::
  displayMissingFareRuleMessage (const stdair::BookingRequestStruct& iBookingRequest,
                                 stdair::TravelSolutionStruct& ioTravelSolution) {

    // Get the origin of the first segment in order to get the origin of
    // the solution.    
    const stdair::ParsedKey lFirstSPParsedKey =
      getFirstSPParsedKey(ioTravelSolution);
    const stdair::AirportCode_T& lOrigin = lFirstSPParsedKey._boardingPoint;

    // Get the destination of the last segment in order to get the
    // destination of the solution.
    const stdair::ParsedKey& lLastSegmentKey =
      getLastSPParsedKey(ioTravelSolution);
    const stdair::AirportCode_T& lDestination = lLastSegmentKey._offPoint;

    // Construct the Airport pair stream of the segment path.
    const stdair::AirportPairKey lAirportPairKey (lOrigin, lDestination);

    // Get the date of the first segment date key.
    const stdair::FlightDateKey& lFlightDateKey =
      lFirstSPParsedKey.getFlightDateKey();

    // Get the point-of-sale of the booking request.
    const stdair::CityCode_T& lPointOfSale = iBookingRequest.getPOS();
    // Get the booking request channel.
    const stdair::ChannelLabel_T& lChannel =
      iBookingRequest.getBookingChannel();
    // Construct the corresponding POS-channel primary key.
    const stdair::PosChannelKey lFarePosChannelKey (lPointOfSale, lChannel);

    // Get the booking request date time.
    const stdair::DateTime_T& lRequestDateTime =
      iBookingRequest.getRequestDateTime();
    
    // If no fare rule has a corresponding date range, the pricing is not
    // possible, throw an exception.
    if (_atLeastOneAvailableDateRule == false) {
      const stdair::SegmentDateKey lSegmentDateKey =
        lFirstSPParsedKey.getSegmentKey();
      STDAIR_LOG_ERROR ("No available fare rule corresponding to the "
                        "flight date " << lFlightDateKey.toString()
                        << " and the Origin-Destination pair: "
                        << lSegmentDateKey.toString());
      throw FlightDateNotFoundException ("No available fare rule for the "
                                         "flight date "
                                         + lFlightDateKey.toString()
                                         + " and the Origin-Destination pair: "
                                         + lSegmentDateKey.toString());
    }
    // If no fare rule has a corresponding pos channel, the pricing is not possible,
    // throw an exception.
    else if (_atLeastOneAvailablePosChannel == false) {
      STDAIR_LOG_ERROR ("No available fare rule corresponding to the "
                        "point of sale " << lPointOfSale
                        << ", to the channel " << lChannel
                        << ", to the flight date "
                        << lFlightDateKey.toString()
                        << " and to the Origin-Destination pair: "
                        << lAirportPairKey.toString());
      throw PosOrChannelNotFoundException ("No available fare rule for the "
                                           "point of sale " + lPointOfSale
                                           + ", the channel " + lChannel
                                           + ", the flight date "
                                           + lFlightDateKey.toString()
                                           + " and the Origin-Destination pair: "
                                           + lAirportPairKey.toString());
    }
    // If no fare rule has a corresponding time range, the pricing is not possible,
    // throw an exception.
    else if (_atLeastOneAvailableTimeRule == false) {
      STDAIR_LOG_ERROR ("No available fare rule corresponding to '"
                        << lFirstSPParsedKey.toString() << "' (parsed key) and to '"
                        << lFarePosChannelKey.toString() << "' (POS and channel)");
      throw FlightTimeNotFoundException ("No available fare rule corresponding "
                                         "to '" + lFirstSPParsedKey.toString()
                                         + "' (parsed key) and to '"
                                         + lFarePosChannelKey.toString()
                                         + "' (POS and channel)");
    }
    // If no fare rule matches the advance purchase, trip type and stay
    // duration criterion, the pricing is not possible, throw an exception.
    else if (_atLeastOneAvailableFeaturesRule == false) {
      // Get the stay duration of the booking request.
      const stdair::DayDuration_T& lStayDuration=
        iBookingRequest.getStayDuration();
      std::ostringstream lStayDurationStream;
      lStayDurationStream << lStayDuration;
      const std::string lStayDurationString (lStayDurationStream.str());

      // Get the booking request trip type.
      const stdair::TripType_T& lTripType =
        iBookingRequest.getTripType();

      STDAIR_LOG_ERROR ("No available fare rule corresponding to a "
                        "trip type " << lTripType
                        << ", to a stay duration of " <<  lStayDurationString
                        << ", to a request date time of " << lRequestDateTime
                        << ", to '" << lFirstSPParsedKey.toString()
                        << "' (parsed key) and to '"
                        << lFarePosChannelKey << "' (POS and channel)");
      throw FeaturesNotFoundException ("No available fare rule corresponding to a "
                                       "trip type " + lTripType
                                       + ", to a stay duration of "
                                       + lStayDurationString
                                       + ", to a request date time of "
                                       + boost::posix_time::to_simple_string(lRequestDateTime)
                                       + ", to '" + lFirstSPParsedKey.toString()
                                       + "' (parsed key) and to '"
                                       + lFarePosChannelKey.toString()
                                       + "' (POS and channel)");
    }
    assert (_atLeastOneAvailableAirlineClassRule == false);
    // If no fare rule matches the airline class path, the pricing is not
    // possible, throw an exception.
    STDAIR_LOG_ERROR ("No available fare rule corresponding to '"
                      << lFirstSPParsedKey .toString() << "' (parsed key), to '"
                      << iBookingRequest.describe()
                      << "' (booking request) and to '"
                      << lFarePosChannelKey.toString() << "' (POS and channel)");
    throw AirlineNotFoundException ("No available fare rule corresponding to '"
                                    + lFirstSPParsedKey .toString()
                                    + "' (parsed key), to '"
                                    + iBookingRequest.describe()
                                    + "' (booking request) and to '"
                                    + lFarePosChannelKey.toString()
                                    + "' (POS and channel)");
  }
}
  
