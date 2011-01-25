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
#include <simfqt/bom/AirportPair.hpp>
#include <simfqt/bom/FarePosChannel.hpp>
#include <simfqt/bom/FareDatePeriod.hpp>
#include <simfqt/bom/FareRuleFeatures.hpp>
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
		 +(boost::spirit::ascii::char_ - boost::spirit::ascii::char_(',')) % ','
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
  priceQuote (const stdair::SegmentPath_T& iSegmentPath,
              stdair::TravelSolutionList_T& ioTravelSolutionList,
              const stdair::BookingRequestStruct& iBookingRequest,
              const stdair::BomRoot& iBomRoot) {
    
    try {   

      if (iSegmentPath.empty() == false) { 
	// Get the first segment-date key of the traval solution. 
	const std::string lFirstSegmentDateKey = iSegmentPath.front(); 

	// Parse the first segment-date key into a vector of strings.  
	std::vector<std::string> lResultParsing;
	const bool hasParsingBeenSuccesful = 
	  parseSegmentDateKey(lFirstSegmentDateKey.begin(), 
			      lFirstSegmentDateKey.end(),
			      lResultParsing);
        
	if (hasParsingBeenSuccesful) {

          assert(lResultParsing.size() == 5);
          
          // Create a new travel solution.
          stdair::TravelSolutionStruct lTravelSolution;
          lTravelSolution.addSegmentDateKey (lFirstSegmentDateKey);
          
	  // Get the Airport pair string of the segment path.
	  std::ostringstream lAirportPairStr; 
	  lAirportPairStr << lResultParsing.back() << std::endl;
          std::string lAirportPairString = lResultParsing.back();

	  // Search for the fare rules having the same origin and
          // destination airport as the travel solution
	  const AirportPair* lAirportPair_ptr = stdair::BomManager::
	    getObjectPtr<AirportPair> (iBomRoot, lAirportPairStr.str());  

	  if (lAirportPair_ptr == NULL) {
            STDAIR_LOG_ERROR ("No available fare rule for the "
                              << "Origin-Destination pair: "
                              << lAirportPairString);
            throw AirportPairNotFoundException ("No available fare rule for "
                                                "the Origin-Destination pair: "
                                                + lAirportPairString);
          }
          
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
            getObjectPtr<FarePosChannel> (*lAirportPair_ptr, 
                                          lFarePosChannelKey.toString());
          
          if (lFarePosChannel_ptr == NULL) {
            STDAIR_LOG_ERROR ("No available fare rule corresponding to the "
                              "position " << lPosition
                              << ", to the channel " << lChannel
                              << " and to the Origin-Destination pair: "
                              << lAirportPairString);
            throw PositionNotFoundException ("No available fare rule for the "
                                             "position " + lPosition
                                             + " and the channel " + lChannel);
          }
          
          // Get the date of the segment path.
          const stdair::Date_T lSPDate =
            boost::gregorian::from_simple_string(lResultParsing.at(2));

          // Get the segment boarding time of the segment path.
          const stdair::Duration_T lSPTime = 
            boost::posix_time::duration_from_string(lResultParsing.at(3));

          // Get the airline code of the segment path.
          const stdair::AirlineCode_T lAirlineCode (lResultParsing.at(0));

          // Get the stay duration of the booking request.
          const stdair::DayDuration_T lStayDuration =
            iBookingRequest.getStayDuration();

          // Get the booking request date time.
          const stdair::DateTime_T lRequestDateTime =
            iBookingRequest.getRequestDateTime();

          // Declare fare parameters to determine.
          stdair::PriceValue_T lPrice;
          stdair::SaturdayStay_T lSaturdayStay;
          stdair::ChangeFees_T lChangeFees;
          stdair::NonRefundable_T lNonRefundable;
      
          // Search for the fare rules having a corresponding date range.
          const FareDatePeriodList_T& lFareDatePeriodList =
            stdair::BomManager::
            getList<FareDatePeriod>(*lFarePosChannel_ptr);
          
          for (FareDatePeriodList_T::const_iterator itDateRange =
                 lFareDatePeriodList.begin();
               itDateRange != lFareDatePeriodList.end();
               ++itDateRange) {
            const FareDatePeriod* lcurrentFareDatePeriod_ptr =
              *itDateRange ;
            assert (lcurrentFareDatePeriod_ptr != NULL);
            
            if (lcurrentFareDatePeriod_ptr->isDepartureDateValid(lSPDate)
                == true) {
              // Search for the fare rules having corresponding features.
              const FareRuleFeaturesList_T& lFareRuleFeaturesList =
                stdair::BomManager::
                getList<FareRuleFeatures> (*lcurrentFareDatePeriod_ptr);

              for (FareRuleFeaturesList_T::const_iterator itFareFeatures =
                     lFareRuleFeaturesList.begin();
                   itFareFeatures != lFareRuleFeaturesList.end();
                   ++itFareFeatures) {
                const FareRuleFeatures* lcurrentFareRuleFeatures_ptr =
                  *itFareFeatures;
                assert (lcurrentFareRuleFeatures_ptr != NULL);

                lPrice = lcurrentFareRuleFeatures_ptr->getFare();
                lSaturdayStay = lcurrentFareRuleFeatures_ptr->getSaturdayStay();
                lChangeFees = lcurrentFareRuleFeatures_ptr->getChangeFees();
                lNonRefundable = lcurrentFareRuleFeatures_ptr->getRefundableOption();
                 
                // Search for the fare rules having a corresponding airline.
                const SegmentFeaturesList_T& lSegmentFeaturesList =
                  stdair::BomManager::
                  getList<SegmentFeatures>(*lcurrentFareRuleFeatures_ptr);  
                
                for (SegmentFeaturesList_T::const_iterator itSegmentFeatures =
                       lSegmentFeaturesList.begin();
                     itSegmentFeatures != lSegmentFeaturesList.end();
                     ++itSegmentFeatures) {
                  const SegmentFeatures* lcurrentSegmentFeatures_ptr =
                    *itSegmentFeatures;
                  assert (lcurrentSegmentFeatures_ptr != NULL);
                  
                  if (lcurrentSegmentFeatures_ptr->getAirlineCode()
                      == lAirlineCode) {
                    
                    // Set the travel solution price.
                    lTravelSolution.setFare(lPrice);

                    // DEBUG
                    STDAIR_LOG_DEBUG ("The price is "
                                      << lTravelSolution.getFare()
                                      << " EUR for the travel request for the airline "
                                      << lResultParsing.front());

                    ioTravelSolutionList.push_back (lTravelSolution); 
                  }
                }
              }
            }
          }
        }
      }
     
    } catch (const std::exception& lStdError) {
      STDAIR_LOG_ERROR ("Error: " << lStdError.what());
      throw QuotingException();
    }
  }
}

