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
#include <stdair/bom/BookingRequestStruct.hpp>
#include <stdair/bom/TravelSolutionStruct.hpp>
#include <stdair/service/Logger.hpp>
#include <stdair/bom/key_types.hpp>
// SimFQT
#include <simfqt/SIMFQT_Types.hpp>
#include <simfqt/command/FareQuoter.hpp>

namespace SIMFQT {

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
  void FareQuoter::
  priceQuote (const stdair::BookingRequestStruct& iBookingRequest,
              stdair::TravelSolutionList_T& ioTravelSolutionList,
              const stdair::BomRoot& iBomRoot) {

    // Do an independent price quote for each travel solution
    for (stdair::TravelSolutionList_T::iterator itTravelSolution =
           ioTravelSolutionList.begin();
         itTravelSolution != ioTravelSolutionList.end(); ++itTravelSolution) {
      FareQuoter::priceQuote (iBookingRequest, *itTravelSolution, iBomRoot);
    }
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
    
    // Get the first and the last segment of the travel solution.
    const std::string lFirstSegmentDateKey = lSegmentPath.front();
    const std::string lLastSegmentDateKey = lSegmentPath.back();

    // Get the origin and destination of the first and the last segment
    // in order to build the origin and the destination of the solution.
    const stdair::SegmentDateKey& lFirstSegmentKey =
      stdair::BomKeyManager::extractSegmentDateKey (lFirstSegmentDateKey);
    const stdair::AirportCode_T& lOrigin = lFirstSegmentKey.getBoardingPoint();

    const stdair::SegmentDateKey& lLastSegmentKey =
      stdair::BomKeyManager::extractSegmentDateKey (lLastSegmentDateKey);
    const stdair::AirportCode_T& lDestination = lLastSegmentKey.getOffPoint();

    // Get the Airport pair stream of the segment path.
    const stdair::AirportPairKey lAirportPairKey (lOrigin, lDestination);
    
    // Search for the fare rules having the same origin and
    // destination airport as the travel solution
    const stdair::AirportPair* lAirportPair_ptr = stdair::BomManager::
      getObjectPtr<stdair::AirportPair> (iBomRoot, lAirportPairKey.toString());  

    if (lAirportPair_ptr == NULL) { 
      STDAIR_LOG_ERROR ("No available fare rule for the "
                        << "Origin-Destination pair: "
                        << lAirportPairKey.toString());
      throw AirportPairNotFoundException ("No available fare rule for "
                                          "the Origin-Destination pair: "
                                          + lAirportPairKey.toString());
    }
    assert(lAirportPair_ptr != NULL);

    const stdair::ParsedKey& lParsedKey =
      stdair::BomKeyManager::extractKeys (lFirstSegmentDateKey);
    priceQuote(iBookingRequest, ioTravelSolution, *lAirportPair_ptr, lParsedKey);
  }

  // //////////////////////////////////////////////////////////////////////
  void FareQuoter::
  priceQuote (const stdair::BookingRequestStruct& iBookingRequest,
              stdair::TravelSolutionStruct& ioTravelSolution,
              const stdair::AirportPair& iAirportPair,
              const stdair::ParsedKey& iParsedKey) {

    // Get the point_of_sale of the booking request.
    const stdair::CityCode_T& lPointOfSale = iBookingRequest.getPOS();

    // Get the booking request channel.
    const stdair::ChannelLabel_T lChannel = iBookingRequest.getBookingChannel();

    // Construct the corresponding point_of_sale-channel primary key.
    const stdair::PosChannelKey lFarePosChannelKey (lPointOfSale, lChannel);

    // Search for the fare rules having the same point_of_sale as the travel
    // solution
    const stdair::PosChannel* lFarePosChannel_ptr = stdair::BomManager::
      getObjectPtr<stdair::PosChannel> (iAirportPair,
                                        lFarePosChannelKey.toString());

    if (lFarePosChannel_ptr == NULL) {
      STDAIR_LOG_ERROR ("No available fare rule corresponding to the "
                        "point of sale " << lPointOfSale
                        << ", to the channel " << lChannel
                        << " and to the Origin-Destination pair: "
                        << iAirportPair.toString());
      throw PosOrChannelNotFoundException ("No available fare rule for the "
                                           "point of sale " + lPointOfSale
                                           + " and the channel " + lChannel);
    }
    assert(lFarePosChannel_ptr != NULL);

    priceQuote (iBookingRequest, ioTravelSolution, *lFarePosChannel_ptr,
                iParsedKey);
  }

  // //////////////////////////////////////////////////////////////////////
  void FareQuoter::
  priceQuote (const stdair::BookingRequestStruct& iBookingRequest,
              stdair::TravelSolutionStruct& ioTravelSolution,
              const stdair::PosChannel& iFarePosChannel,
              const stdair::ParsedKey& iParsedKey) {
    
    // Get the date of the segment date key.
    const stdair::FlightDateKey lFlightDateKey = iParsedKey.getFlightDateKey();
    const stdair::Date_T& lSPDate = lFlightDateKey.getDepartureDate();

    bool isThereAtLeastOneAvailableDateRule = false;

    // Get the list of the fare rules.
    const stdair::DatePeriodList_T& lFareDatePeriodList =
      stdair::BomManager::getList<stdair::DatePeriod> (iFarePosChannel);

    for (stdair::DatePeriodList_T::const_iterator itDateRange =
           lFareDatePeriodList.begin();
         itDateRange != lFareDatePeriodList.end(); ++itDateRange) {

      const stdair::DatePeriod* lCurrentFareDatePeriod_ptr =  *itDateRange ;
      assert (lCurrentFareDatePeriod_ptr != NULL);

      // Select the fare rules having a corresponding date range
      const bool isDepartureDateValid =
        lCurrentFareDatePeriod_ptr->isDepartureDateValid (lSPDate);

      if (isDepartureDateValid == true) {
        isThereAtLeastOneAvailableDateRule = true;

        priceQuote (iBookingRequest, ioTravelSolution,
                    *lCurrentFareDatePeriod_ptr, iFarePosChannel, iParsedKey);
      }
    }
      
    if (isThereAtLeastOneAvailableDateRule == false) {
      const stdair::SegmentDateKey lSegmentDateKey = iParsedKey.getSegmentKey();
      STDAIR_LOG_ERROR ("No available fare rule corresponding to the "
                        "flight date " << lFlightDateKey.toString()
                        << ", to the point of sale "
                        << iFarePosChannel.getPos()
                        << ", to the channel "
                        << iFarePosChannel.getChannel()
                        << " and to the Origin-Destination pair: "
                        << lSegmentDateKey.toString());
      throw FlightDateNotFoundException ("No available fare rule for the "
                                         "flight date "
                                         + lFlightDateKey.toString());
    }
    
    assert (isThereAtLeastOneAvailableDateRule == true);
  }

  // //////////////////////////////////////////////////////////////////////
  void FareQuoter::
  priceQuote (const stdair::BookingRequestStruct& iBookingRequest,
              stdair::TravelSolutionStruct& ioTravelSolution,
              const stdair::DatePeriod& iFareDatePeriod,
              const stdair::PosChannel& iFarePosChannel,
              const stdair::ParsedKey& iParsedKey) {
      
    // Get the segment boarding time of the segment path.
    const stdair::Duration_T lSPTime = iParsedKey.getBoardingTime();
    bool lAtLeastOneAvailableTimeRule = false;

    // Get the list of the fare rules.
    const stdair::TimePeriodList_T& lFareTimePeriodList =
      stdair::BomManager::getList<stdair::TimePeriod> (iFareDatePeriod);

    for (stdair::TimePeriodList_T::const_iterator itTimeRange =
           lFareTimePeriodList.begin();
         itTimeRange != lFareTimePeriodList.end();
         ++itTimeRange) {
      const stdair::TimePeriod* lCurrentFareTimePeriod_ptr = *itTimeRange ;
      assert (lCurrentFareTimePeriod_ptr != NULL);

      // Select the fare rules having a corresponding time range.
      if (lCurrentFareTimePeriod_ptr->isDepartureTimeValid (lSPTime) == true) {

        lAtLeastOneAvailableTimeRule = true;

        priceQuote (iBookingRequest, ioTravelSolution,
                    *lCurrentFareTimePeriod_ptr, iFarePosChannel, iParsedKey);
      }
    }
      
    if (lAtLeastOneAvailableTimeRule == false) {
      STDAIR_LOG_ERROR ("No available fare rule corresponding to '"
                        << iParsedKey.toString() << "' (parsed key) and to '"
                        << iFarePosChannel.toString() << "' (POS and channel)");
      throw FlightTimeNotFoundException ("No available fare rule corresponding "
                                         "to '" + iParsedKey.toString()
                                         + "' (parsed key) and to '"
                                         + iFarePosChannel.toString()
                                         + "' (POS and channel)");
    }
      
    assert (lAtLeastOneAvailableTimeRule == true);
  }

  // //////////////////////////////////////////////////////////////////////
  void FareQuoter::
  priceQuote (const stdair::BookingRequestStruct& iBookingRequest,
              stdair::TravelSolutionStruct& ioTravelSolution,
              const stdair::TimePeriod& iFareTimePeriod,
              const stdair::PosChannel& iFarePosChannel,
              const stdair::ParsedKey& iParsedKey) {

    // Get the stay duration of the booking request.
    const stdair::DayDuration_T lStayDuration= iBookingRequest.getStayDuration();

    // Get the booking request date time.
    const stdair::DateTime_T lRequestDateTime =
      iBookingRequest.getRequestDateTime();

    // Get the referenced departure date of the segment path.
    const stdair::Date_T lSPDate =
      iParsedKey.getFlightDateKey().getDepartureDate();

    // Get the segment boarding time of the segment path.
    const stdair::Duration_T lSPTime = iParsedKey.getBoardingTime();

    // Construct the date-time type correponding to the flight date
    const stdair::DateTime_T lSPDateTime (lSPDate, lSPTime);

    bool AtLeastOneAvailableFeaturesRule = false;
    bool IsStayDurationValid = false;
    bool IsAdvancePurchaseValid = false;

    // Get the list of the fare rules.
    const FareRuleFeaturesList_T& lFareRuleFeaturesList =
      stdair::BomManager::getList<FareRuleFeatures> (iFareTimePeriod);

    for (FareRuleFeaturesList_T::const_iterator itFareFeatures =
           lFareRuleFeaturesList.begin();
         itFareFeatures != lFareRuleFeaturesList.end();
         ++itFareFeatures) {
      const FareRuleFeatures* lCurrentFareRuleFeatures_ptr =
        *itFareFeatures;
      assert (lCurrentFareRuleFeatures_ptr != NULL);

      IsStayDurationValid =
        lCurrentFareRuleFeatures_ptr->IsStayDurationValid (lStayDuration);
      IsAdvancePurchaseValid = lCurrentFareRuleFeatures_ptr->
        IsAdvancePurchaseValid (lRequestDateTime,
                                lSPDateTime);
        
      // Search for the fare rules having corresponding features.
      if (IsStayDurationValid && IsAdvancePurchaseValid){
        AtLeastOneAvailableFeaturesRule = true;
        // Set the travel fare option.
        stdair::FareOptionStruct lFareOption;
        lFareOption.setFare (lCurrentFareRuleFeatures_ptr->getFare());
        lFareOption.
          setChangeFees (lCurrentFareRuleFeatures_ptr->getChangeFees());
        lFareOption.
          setNonRefundable (lCurrentFareRuleFeatures_ptr->getRefundableOption());
        lFareOption.
          setSaturdayStay (lCurrentFareRuleFeatures_ptr->getSaturdayStay());

        priceQuote (iBookingRequest, ioTravelSolution,
                    *lCurrentFareRuleFeatures_ptr, iFarePosChannel,
                    lFareOption, iParsedKey);
      }
    }
    
    if (AtLeastOneAvailableFeaturesRule == false) {
      std::ostringstream lStayDurationStream;
      lStayDurationStream << lStayDuration;
      const std::string lStayDurationString (lStayDurationStream.str());

      STDAIR_LOG_ERROR ("No available fare rule corresponding to a "
                        "stay duration of " <<  lStayDurationString
                        << ", to a request date time of " << lRequestDateTime
                        << ", to '" << iParsedKey.toString()
                        << "' (parsed key) and to '"
                        << iFarePosChannel.toString() << "' (POS and channel)");
      throw FeaturesNotFoundException ("No available fare rule corresponding to "
                                       "a stay duration of "
                                       + lStayDurationString
                                       + ", to a request date time of "
                                       + boost::posix_time::to_simple_string(lRequestDateTime)
                                       + ", to '" + iParsedKey.toString()
                                       + "' (parsed key) and to '"
                                       + iFarePosChannel.toString()
                                       + "' (POS and channel)");
    }
    assert (AtLeastOneAvailableFeaturesRule == true);
  }

  
  // //////////////////////////////////////////////////////////////////////
  void FareQuoter::
  priceQuote (const stdair::BookingRequestStruct& iBookingRequest,
              stdair::TravelSolutionStruct& ioTravelSolution,
              const FareRuleFeatures& iFareRuleFeatures,
              const stdair::PosChannel& iFarePosChannel,
              stdair::FareOptionStruct& iFareOption,
              const stdair::ParsedKey& iParsedKey) {
    
    // Get the segment-path of the travel solution.
    const stdair::SegmentPath_T lSegmentPath = ioTravelSolution.getSegmentPath();

    // Get the list of the fare rules.
    const stdair::AirlineClassListList_T& lAirlineClassListList =
      stdair::BomManager::getList<stdair::AirlineClassList> (iFareRuleFeatures);

    bool lAtLeastOneAvailableAirlineRule = false;
    bool lCorrectAirlineRule = false;
    bool lAtLeastOneDifferentAirline = false; 

    // Search for the fare rules having a corresponding airline list.
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
        stdair::SegmentPath_T::const_iterator itSegmentPath =
          lSegmentPath.begin();

        stdair::ClassList_StringList_T::const_iterator itClassList_String =
          lClassList_StringList.begin();
        while (itSegmentPath != lSegmentPath.end()
               && lAtLeastOneDifferentAirline == false) {

          // Get the current segment path of the travel solution.
          const std::string lSegmentDateKey = *itSegmentPath;
          const stdair::ParsedKey lParsedKey =
            stdair::BomKeyManager::extractKeys (lSegmentDateKey);
          
          if (lParsedKey.getInventoryKey().getAirlineCode()
              != *itClassList_String) {
            lAtLeastOneDifferentAirline = true;
          }
          itSegmentPath++;
          itClassList_String++;
        }      

      } else {
        lCorrectAirlineRule = false;
      }

      if (lAtLeastOneDifferentAirline == true) {
        lCorrectAirlineRule = false;
      }
          
      if (lCorrectAirlineRule == true) {
        lAtLeastOneAvailableAirlineRule = true;

        // Set the travel fare option.
        const stdair::ClassList_StringList_T& lClassCodeList =
          lCurrentAirlineClassList_ptr->getClassCodeList();
        for (stdair::ClassList_StringList_T::const_iterator itClassCodeList =
               lClassCodeList.begin();
             itClassCodeList != lClassCodeList.end(); ++itClassCodeList ) {
          iFareOption.addClassList (*itClassCodeList);
        }

        //
        ioTravelSolution.addFareOption (iFareOption);
          
        // DEBUG
        STDAIR_LOG_DEBUG ("Segment path: " << iParsedKey.toString()
                          << ". A corresponding fare option for the '"
                          << lCurrentAirlineClassList_ptr->describeKey()
                          << "' class is: " << iFareOption);
      }
    }
    
    if (lAtLeastOneAvailableAirlineRule == false) {

      STDAIR_LOG_ERROR ("No available fare rule corresponding to '"
                        << iParsedKey.toString() << "' (parsed key), to '"
                        << iBookingRequest.describe()
                        << "' (booking request) and to '"
                        << iFarePosChannel.toString() << "' (POS and channel)");
      throw AirlineNotFoundException ("No available fare rule corresponding to '"
                                      + iParsedKey.toString()
                                      + "' (parsed key), to '"
                                      + iBookingRequest.describe()
                                      + "' (booking request) and to '"
                                      + iFarePosChannel.toString()
                                      + "' (POS and channel)");
    }
    assert (lAtLeastOneAvailableAirlineRule == true);
  }

}
