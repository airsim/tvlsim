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
#include <simfqt/bom/FarePosition.hpp>
#include <simfqt/bom/FareDatePeriod.hpp>
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

    // Set the fare-position primary key.
    const stdair::CityCode_T& lPosition = 
      iFareRuleStruct._pos; 
    const FarePositionKey lFarePositionKey (lPosition);  

    // If the FarePositionKey object corresponding to the fare rule set
    // having the same city code does not exist, create it and link it
    // to the AirportPair object.     
    FarePosition* lFarePosition_ptr = stdair::BomManager::
      getObjectPtr<FarePosition> (*lAirportPair_ptr, 
				  lFarePositionKey.toString());
    if (lFarePosition_ptr == NULL) {
      lFarePosition_ptr =
        &stdair::FacBom<FarePosition>::instance().create (lFarePositionKey);
      stdair::FacBomManager::
	instance().addToListAndMap (*lAirportPair_ptr, *lFarePosition_ptr);
      stdair::FacBomManager::
	instance().linkWithParent (*lAirportPair_ptr, *lFarePosition_ptr);
    }
    assert (lFarePosition_ptr != NULL);   

    // Set the fare date-period primary key.
    const stdair::Date_T& lDateRangeStart = 
      iFareRuleStruct._dateRangeStart;
    const stdair::Date_T& lDateRangeEnd = 
      iFareRuleStruct._dateRangeEnd;
    const stdair::DatePeriod_T lDatePeriod (lDateRangeStart, lDateRangeEnd); 
    const FareDatePeriodKey lFareDatePeriodKey (lDatePeriod);

    // If the FareDatePeriodeKey object corresponding to the fare rule set
    // having the same city code does not exist, create it and link it
    // to the FarePosition object.     
    FareDatePeriod* lFareDatePeriod_ptr = stdair::BomManager::
      getObjectPtr<FareDatePeriod> (*lFarePosition_ptr, 
				    lFareDatePeriodKey.toString());
    if (lFareDatePeriod_ptr == NULL) {
      lFareDatePeriod_ptr =
        &stdair::FacBom<FareDatePeriod>::instance().create (lFareDatePeriodKey);
      stdair::FacBomManager::
        instance().addToListAndMap (*lFarePosition_ptr, *lFareDatePeriod_ptr);
      stdair::FacBomManager::
      instance().linkWithParent (*lFarePosition_ptr, *lFareDatePeriod_ptr);
    }
    assert (lFareDatePeriod_ptr != NULL);   
    
    // Generate all FareRules
    const stdair::Duration_T& lTimeRangeStart = 
      iFareRuleStruct._timeRangeStart;
    const stdair::Duration_T& lTimeRangeEnd = 
      iFareRuleStruct._timeRangeEnd;
    const stdair::ChannelLabel_T& lChannel = 
      iFareRuleStruct._channel;
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
    const FareRuleFeaturesKey lFareRuleFeaturesKey (lTimeRangeStart, lTimeRangeEnd,
						    lAdvancePurchase, lSaturdayStay,
						    lChangeFees, lNonRefundable,
						    lMinimumStay, lFare);  

    // Create ther fare rule object and link it to the FareDatePeriod object.  
    FareRuleFeatures* lFareRuleFeatures_ptr = stdair::BomManager::
      getObjectPtr<FareRuleFeatures> (*lFareDatePeriod_ptr, 
				      lFareRuleFeaturesKey.toString());
    if (lFareRuleFeatures_ptr == NULL) {
      lFareRuleFeatures_ptr =
	&stdair::FacBom<FareRuleFeatures>::instance().create (lFareRuleFeaturesKey);
      assert(lFareRuleFeatures_ptr != NULL); 
      stdair::FacBomManager::
	instance().addToListAndMap (*lFareDatePeriod_ptr, *lFareRuleFeatures_ptr);
      stdair::FacBomManager::
	instance().linkWithParent (*lFareDatePeriod_ptr, *lFareRuleFeatures_ptr);  
    }
    assert(lFareRuleFeatures_ptr != NULL); 

    // Generate Segment Features and link them to their FareRule
    const unsigned int lAirlineListSize =
      iFareRuleStruct.getAirlineListSize();
    const unsigned int lClassCodeListSize =
      iFareRuleStruct.getClassCodeListSize();
    assert (lAirlineListSize == lClassCodeListSize);
    iFareRuleStruct.beginClassCode();
    for (iFareRuleStruct.beginAirline();
	 iFareRuleStruct.hasNotReachedEndAirline();
	 iFareRuleStruct.iterateAirline()) {
      stdair::AirlineCode_T lAirlineCode =
	iFareRuleStruct.getCurrentAirlineCode();
      stdair::ClassCode_T lClassCode = 
	iFareRuleStruct.getCurrentClassCode();
      iFareRuleStruct.iterateClassCode();
      const SegmentFeaturesKey lSegmentFeaturesKey (lAirlineCode, lClassCode); 
        
      SegmentFeatures* lSegmentFeatures_ptr =
         &stdair::FacBom<SegmentFeatures>::instance().create (lSegmentFeaturesKey); 
      stdair::FacBomManager::
	instance().linkWithParent(*lFareRuleFeatures_ptr, *lSegmentFeatures_ptr); 
      }
 
    //const stdair::ChannelLabel_T& lChannel = iFareRuleStruct._channel;
  }
        
}
