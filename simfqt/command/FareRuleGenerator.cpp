// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// StdAir
#include <stdair/bom/BomManager.hpp>
#include <stdair/bom/BomRoot.hpp>
#include <stdair/factory/FacBomManager.hpp>
#include <stdair/service/Logger.hpp>
// SIMFQT
#include <simfqt/bom/FareRuleStruct.hpp>
#include <simfqt/bom/AirportPair.hpp>
#include <simfqt/bom/FarePosChannel.hpp>
#include <simfqt/bom/FareDatePeriod.hpp>
#include <simfqt/bom/FareTimePeriod.hpp>
#include <simfqt/bom/FareRuleFeatures.hpp>
#include <simfqt/bom/SegmentFeatures.hpp>
#include <simfqt/command/FareRuleGenerator.hpp>

namespace SIMFQT {

  // //////////////////////////////////////////////////////////////////////
  void FareRuleGenerator::
  createFareRule (stdair::BomRoot& ioBomRoot,
                  FareRuleStruct& iFareRuleStruct) {

    // Set the airport-pair primary key.
    const stdair::AirportCode_T& lBoardPoint =
      iFareRuleStruct._origin;
    const stdair::AirportCode_T& lOffPoint = 
      iFareRuleStruct._destination;
    const AirportPairKey lAirportPairKey (lBoardPoint, lOffPoint);
  
    // If the AirportPairKey object corresponding to the fare rule set
    // having the same origin and destination airport does not exist, create
    // it and link it to the ioBomRoot object.
    AirportPair* lAirportPair_ptr = stdair::BomManager::
      getObjectPtr<AirportPair> (ioBomRoot, lAirportPairKey.toString());
    if (lAirportPair_ptr == NULL) {
      lAirportPair_ptr =
        &stdair::FacBom<AirportPair>::instance().create (lAirportPairKey);
      stdair::FacBomManager::
        instance().addToListAndMap (ioBomRoot, *lAirportPair_ptr);
      stdair::FacBomManager::
      instance().linkWithParent (ioBomRoot, *lAirportPair_ptr);
    }
    assert (lAirportPair_ptr != NULL);    

    // Set the position-channel primary key.
    const stdair::CityCode_T& lPosition = 
      iFareRuleStruct._pos;
    const stdair::ChannelLabel_T& lChannel = 
      iFareRuleStruct._channel;
    const FarePosChannelKey lFarePosChannelKey (lPosition, lChannel);  

    // If the FarePositionKey object corresponding to the fare rule set
    // having the same city code does not exist, create it and link it
    // to the AirportPair object.     
    FarePosChannel* lFarePosChannel_ptr = stdair::BomManager::
      getObjectPtr<FarePosChannel> (*lAirportPair_ptr, 
                                    lFarePosChannelKey.toString());
    if (lFarePosChannel_ptr == NULL) {
      lFarePosChannel_ptr =
        &stdair::FacBom<FarePosChannel>::instance().create (lFarePosChannelKey);
      stdair::FacBomManager::
	instance().addToListAndMap (*lAirportPair_ptr, *lFarePosChannel_ptr);
      stdair::FacBomManager::
	instance().linkWithParent (*lAirportPair_ptr, *lFarePosChannel_ptr);
    }
    assert (lFarePosChannel_ptr != NULL);   

    // Set the fare date-period primary key.
    const stdair::Date_T& lDateRangeStart = 
      iFareRuleStruct._dateRangeStart;
    const stdair::Date_T& lDateRangeEnd = 
      iFareRuleStruct._dateRangeEnd;
    const stdair::DatePeriod_T lDatePeriod (lDateRangeStart, lDateRangeEnd); 
    const FareDatePeriodKey lFareDatePeriodKey (lDatePeriod);

    // If the FareDatePeriodeKey object corresponding to the fare rule set
    // does not exist, create it and link it to the FarePosChannel object.     
    FareDatePeriod* lFareDatePeriod_ptr = stdair::BomManager::
      getObjectPtr<FareDatePeriod> (*lFarePosChannel_ptr, 
				    lFareDatePeriodKey.toString());
    if (lFareDatePeriod_ptr == NULL) {
      lFareDatePeriod_ptr =
        &stdair::FacBom<FareDatePeriod>::instance().create (lFareDatePeriodKey);
      stdair::FacBomManager::
        instance().addToListAndMap (*lFarePosChannel_ptr, *lFareDatePeriod_ptr);
      stdair::FacBomManager::
      instance().linkWithParent (*lFarePosChannel_ptr, *lFareDatePeriod_ptr);
    }
    assert (lFareDatePeriod_ptr != NULL);
   
    // Set the fare time-period primary key.
    const stdair::Time_T& lTimeRangeStart
      = iFareRuleStruct._timeRangeStart;
    const stdair::Time_T& lTimeRangeEnd
      = iFareRuleStruct._timeRangeEnd;
    const FareTimePeriodKey lFareTimePeriodKey (lTimeRangeStart,
                                                lTimeRangeEnd);

    // If the FareTimePeriodeKey object corresponding to the fare rule set
    // does not exist, create it and link it to the FarePosChannel object.     
    FareTimePeriod* lFareTimePeriod_ptr = stdair::BomManager::
      getObjectPtr<FareTimePeriod> (*lFareDatePeriod_ptr, 
				    lFareTimePeriodKey.toString());
    if (lFareTimePeriod_ptr == NULL) {
      lFareTimePeriod_ptr =
        &stdair::FacBom<FareTimePeriod>::instance().create (lFareTimePeriodKey);
      stdair::FacBomManager::
        instance().addToListAndMap (*lFareDatePeriod_ptr, *lFareTimePeriod_ptr);
      stdair::FacBomManager::
      instance().linkWithParent (*lFareDatePeriod_ptr, *lFareTimePeriod_ptr);
    }
    assert (lFareTimePeriod_ptr != NULL);

    // Generate the FareRule
    const stdair::DayDuration_T& lAdvancePurchase = 
      iFareRuleStruct._advancePurchase;
    const stdair::SaturdayStay_T& lSaturdayStay = 
      iFareRuleStruct._saturdayStay;
    const stdair::ChangeFees_T& lChangeFees =
      iFareRuleStruct._changeFees;
    const stdair::NonRefundable_T& lNonRefundable = 
      iFareRuleStruct._nonRefundable;
    const stdair::DayDuration_T& lMinimumStay = 
      iFareRuleStruct._minimumStay;
    const stdair::Fare_T& lFare = 
      iFareRuleStruct._fare; 
    const FareRuleFeaturesKey lFareRuleFeaturesKey (lAdvancePurchase, lSaturdayStay,
                                                    lChangeFees, lNonRefundable,
                                                    lMinimumStay, lFare);  

    // Create ther fare rule object and link it to the FareDatePeriod object.  
    FareRuleFeatures* lFareRuleFeatures_ptr = stdair::BomManager::
      getObjectPtr<FareRuleFeatures> (*lFareTimePeriod_ptr, 
				      lFareRuleFeaturesKey.toString());
    if (lFareRuleFeatures_ptr == NULL) {
      lFareRuleFeatures_ptr =
	&stdair::FacBom<FareRuleFeatures>::instance().create (lFareRuleFeaturesKey);
      assert(lFareRuleFeatures_ptr != NULL); 
      stdair::FacBomManager::
	instance().addToListAndMap (*lFareTimePeriod_ptr, *lFareRuleFeatures_ptr);
      stdair::FacBomManager::
	instance().linkWithParent (*lFareTimePeriod_ptr, *lFareRuleFeatures_ptr);  
    }
    assert(lFareRuleFeatures_ptr != NULL); 

    // Generate Segment Features and link them to their FareRule
    std::list<std::list<std::string> >::const_iterator lItCurrentClassCodeList =
      iFareRuleStruct._classCodeListOfList.begin();
    const unsigned int lAirlineListSize =
      iFareRuleStruct.getAirlineListSize();
    const unsigned int lClassCodeListOfListSize =
      iFareRuleStruct.getClassCodeListOfListSize();
    assert (lAirlineListSize == lClassCodeListOfListSize);
    iFareRuleStruct.beginClassCodeList();
    for (iFareRuleStruct.beginAirline();
	 iFareRuleStruct.hasNotReachedEndAirline();
	 iFareRuleStruct.iterateAirline()) {
      stdair::AirlineCode_T lAirlineCode =
	iFareRuleStruct.getCurrentAirlineCode();
      std::list<std::string> lClassCodeList = 
	iFareRuleStruct.getCurrentClassCodeList();
      iFareRuleStruct.iterateClassCodeList();
      const SegmentFeaturesKey lSegmentFeaturesKey (lAirlineCode, lClassCodeList); 
        
      SegmentFeatures* lSegmentFeatures_ptr =
         &stdair::FacBom<SegmentFeatures>::instance().create (lSegmentFeaturesKey);
      stdair::FacBomManager::
	instance().addToListAndMap (*lFareRuleFeatures_ptr, *lSegmentFeatures_ptr); 
      stdair::FacBomManager::
	instance().linkWithParent(*lFareRuleFeatures_ptr, *lSegmentFeatures_ptr); 
      }
  }
        
}

