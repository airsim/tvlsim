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
#include <stdair/bom/AirportPair.hpp>
#include <stdair/bom/PosChannel.hpp>
#include <stdair/bom/DatePeriod.hpp>
#include <stdair/bom/TimePeriod.hpp>
#include <stdair/bom/FareFeatures.hpp>
#include <stdair/bom/AirlineClassList.hpp>
// SimFQT
#include <simfqt/bom/FareRuleStruct.hpp>
#include <simfqt/command/FareRuleGenerator.hpp>

namespace SIMFQT {

  // //////////////////////////////////////////////////////////////////////
  void FareRuleGenerator::
  createAirportPair (stdair::BomRoot& ioBomRoot,
                     const FareRuleStruct& iFareRuleStruct) {

    // Create the airport-pair primary key.
    const stdair::AirportCode_T& lBoardPoint = iFareRuleStruct.getOrigin ();
    const stdair::AirportCode_T& lOffPoint =
      iFareRuleStruct.getDestination ();
    const stdair::AirportPairKey lAirportPairKey (lBoardPoint, lOffPoint);

    // Check that the airport-pair object is not already existing. If an
    // airport-pair object with the same key has not already been created,
    // create it and link it to the ioBomRoot object.
    stdair::AirportPair* lAirportPair_ptr = stdair::BomManager::
      getObjectPtr<stdair::AirportPair> (ioBomRoot, lAirportPairKey.toString());
    if (lAirportPair_ptr == NULL) {
      lAirportPair_ptr =
        &stdair::FacBom<stdair::AirportPair>::instance().
        create (lAirportPairKey);
      stdair::FacBomManager::addToListAndMap (ioBomRoot, *lAirportPair_ptr);
      stdair::FacBomManager::linkWithParent (ioBomRoot, *lAirportPair_ptr);
    }
<<<<<<< HEAD
<<<<<<< HEAD
    assert (lAirportPair_ptr != NULL);

    stdair::AirportPair& lAirportPair = *lAirportPair_ptr;
=======
=======
>>>>>>> trunk
    // Sanity check.
    assert (lAirportPair_ptr != NULL);

    stdair::AirportPair& lAirportPair = *lAirportPair_ptr;
    // Generate the date-period object corresponding to the given
    // fareRule.  
<<<<<<< HEAD
>>>>>>> trunk
=======
>>>>>>> trunk
    createDateRange (lAirportPair, iFareRuleStruct);

  }

  // //////////////////////////////////////////////////////////////////////
  void FareRuleGenerator::
  createDateRange (stdair::AirportPair& iAirportPair,
                   const FareRuleStruct& iFareRuleStruct) {

    // Create the fare date-period primary key.
    const stdair::Date_T& lDateRangeStart =
      iFareRuleStruct.getDateRangeStart ();
    const stdair::Date_T& lDateRangeEnd =
      iFareRuleStruct.getDateRangeEnd ();
    const stdair::DatePeriod_T lDatePeriod (lDateRangeStart, lDateRangeEnd); 
    const stdair::DatePeriodKey lFareDatePeriodKey (lDatePeriod);

    // Check that the date-period object is not already existing.
    // If a date-period object with the same key has not already been
    // created, create it and link it to the airport-pair object.         
    stdair::DatePeriod* lFareDatePeriod_ptr = stdair::BomManager::
      getObjectPtr<stdair::DatePeriod> (iAirportPair, 
                                        lFareDatePeriodKey.toString());
    if (lFareDatePeriod_ptr == NULL) {
      lFareDatePeriod_ptr = &stdair::FacBom<stdair::DatePeriod>::instance().
        create (lFareDatePeriodKey);
      stdair::FacBomManager::addToListAndMap (iAirportPair,
                                              *lFareDatePeriod_ptr);
      stdair::FacBomManager::linkWithParent (iAirportPair,
                                             *lFareDatePeriod_ptr);
    }
<<<<<<< HEAD
<<<<<<< HEAD
    assert (lFareDatePeriod_ptr != NULL);

    stdair::DatePeriod& lDateRange = *lFareDatePeriod_ptr;
=======
=======
>>>>>>> trunk
    // Sanity check.
    assert (lFareDatePeriod_ptr != NULL);

    stdair::DatePeriod& lDateRange = *lFareDatePeriod_ptr;
    // Generate the point_of_sale-channel object corresponding to
    // the given fareRule.
<<<<<<< HEAD
>>>>>>> trunk
=======
>>>>>>> trunk
    createPOSChannel (lDateRange, iFareRuleStruct);

  }

  // //////////////////////////////////////////////////////////////////////
  void FareRuleGenerator::
  createPOSChannel (stdair::DatePeriod& iDatePeriod,
                    const FareRuleStruct& iFareRuleStruct) {

    // Create the point-of-sale-channel primary key.
    const stdair::CityCode_T& lPosition = iFareRuleStruct.getPOS ();
      const stdair::ChannelLabel_T& lChannel =
        iFareRuleStruct.getChannel ();
    const stdair::PosChannelKey lFarePosChannelKey (lPosition, lChannel);

    // Check that the point_of_sale-channel object is not already existing.
    // If a point_of_sale-channel object with the same key has not already
    // been created, create it and link it to the date-period object.     
    stdair::PosChannel* lFarePosChannel_ptr = stdair::BomManager::
      getObjectPtr<stdair::PosChannel> (iDatePeriod,
                                        lFarePosChannelKey.toString());
    if (lFarePosChannel_ptr == NULL) {
      lFarePosChannel_ptr = &stdair::FacBom<stdair::PosChannel>::instance().
        create (lFarePosChannelKey);
      stdair::FacBomManager::addToListAndMap (iDatePeriod,
                                              *lFarePosChannel_ptr);
      stdair::FacBomManager::linkWithParent (iDatePeriod,
                                             *lFarePosChannel_ptr);
    }
<<<<<<< HEAD
<<<<<<< HEAD
    assert (lFarePosChannel_ptr != NULL);

    stdair::PosChannel& lPosChannel = *lFarePosChannel_ptr;
=======
=======
>>>>>>> trunk
    // Sanity check.
    assert (lFarePosChannel_ptr != NULL);

    stdair::PosChannel& lPosChannel = *lFarePosChannel_ptr;
    // Generate the time-period object corresponding to the given
    // fareRule.
<<<<<<< HEAD
>>>>>>> trunk
=======
>>>>>>> trunk
    createTimeRange (lPosChannel, iFareRuleStruct);

  }


  // //////////////////////////////////////////////////////////////////////
  void FareRuleGenerator::
  createTimeRange (stdair::PosChannel& iPosChannel,
                   const FareRuleStruct& iFareRuleStruct) {
<<<<<<< HEAD
    
    // Create the fare time-period primary key.
    const stdair::Time_T& lTimeRangeStart =
      iFareRuleStruct.getTimeRangeStart ();
    const stdair::Time_T& lTimeRangeEnd =
      iFareRuleStruct.getTimeRangeEnd ();
    const stdair::TimePeriodKey lFareTimePeriodKey (lTimeRangeStart,
                                                    lTimeRangeEnd);

    // Check that the time-period object is not already existing.
    // If a time-period object with the same key has not already been
    // created, create it and link it to the point_of_sale-channel object.       
    stdair::TimePeriod* lFareTimePeriod_ptr = stdair::BomManager::
      getObjectPtr<stdair::TimePeriod> (iPosChannel,
                                        lFareTimePeriodKey.toString());
    if (lFareTimePeriod_ptr == NULL) {
      lFareTimePeriod_ptr = &stdair::FacBom<stdair::TimePeriod>::instance().
        create (lFareTimePeriodKey);
      stdair::FacBomManager::addToListAndMap (iPosChannel,
                                              *lFareTimePeriod_ptr);
      stdair::FacBomManager::linkWithParent (iPosChannel,
                                             *lFareTimePeriod_ptr);
    }
<<<<<<< HEAD
    assert (lFareTimePeriod_ptr != NULL);

    stdair::TimePeriod& lTimeRange = *lFareTimePeriod_ptr;
=======
    // Sanity check.
    assert (lFareTimePeriod_ptr != NULL);

    stdair::TimePeriod& lTimeRange = *lFareTimePeriod_ptr;
    // Generate the fare-features object corresponding to the given
    // fareRule.
>>>>>>> trunk
    createFareFeatures (lTimeRange, iFareRuleStruct);

  }

  // //////////////////////////////////////////////////////////////////////
  void FareRuleGenerator::
  createFareFeatures (stdair::TimePeriod& iTimePeriod,
                      const FareRuleStruct& iFareRuleStruct) {

    // Create the fare-features primary key.
    const stdair::TripType_T& lTripType =
      iFareRuleStruct.getTripType ();
    const stdair::DayDuration_T& lAdvancePurchase =
      iFareRuleStruct.getAdvancePurchase ();
    const stdair::SaturdayStay_T& lSaturdayStay =
      iFareRuleStruct.getSaturdayStay ();
    const stdair::ChangeFees_T& lChangeFees =
      iFareRuleStruct.getChangeFees ();
    const stdair::NonRefundable_T& lNonRefundable =
      iFareRuleStruct.getNonRefundable ();
    const stdair::DayDuration_T& lMinimumStay =
      iFareRuleStruct.getMinimumStay ();
    const stdair::FareFeaturesKey
      lFareFeaturesKey (lTripType, lAdvancePurchase, lSaturdayStay,
                        lChangeFees, lNonRefundable, lMinimumStay);

    // Check that the fare features object is not already existing.
    // If a fare features object with the same key has not already been
    // created, create it and link it to the time-period object.        
    stdair::FareFeatures* lFareFeatures_ptr = stdair::BomManager::
      getObjectPtr<stdair::FareFeatures> (iTimePeriod,
                                          lFareFeaturesKey.toString());
    if (lFareFeatures_ptr == NULL) {
      lFareFeatures_ptr = &stdair::FacBom<stdair::FareFeatures>::instance().
        create (lFareFeaturesKey);
      assert(lFareFeatures_ptr != NULL); 
      stdair::FacBomManager::addToListAndMap (iTimePeriod,
                                              *lFareFeatures_ptr);
      stdair::FacBomManager::linkWithParent (iTimePeriod,
                                             *lFareFeatures_ptr);  
    }
<<<<<<< HEAD
    assert(lFareFeatures_ptr != NULL);
    
    stdair::FareFeatures& lFareFeatures = *lFareFeatures_ptr;
=======
    // Sanity check.
    assert(lFareFeatures_ptr != NULL);
    
    stdair::FareFeatures& lFareFeatures = *lFareFeatures_ptr;
    // Generate the airline-class list object corresponding to the
    // given fareRule
>>>>>>> trunk
=======
    
    // Create the fare time-period primary key.
    const stdair::Time_T& lTimeRangeStart =
      iFareRuleStruct.getTimeRangeStart ();
    const stdair::Time_T& lTimeRangeEnd =
      iFareRuleStruct.getTimeRangeEnd ();
    const stdair::TimePeriodKey lFareTimePeriodKey (lTimeRangeStart,
                                                    lTimeRangeEnd);

    // Check that the time-period object is not already existing.
    // If a time-period object with the same key has not already been
    // created, create it and link it to the point_of_sale-channel object.       
    stdair::TimePeriod* lFareTimePeriod_ptr = stdair::BomManager::
      getObjectPtr<stdair::TimePeriod> (iPosChannel,
                                        lFareTimePeriodKey.toString());
    if (lFareTimePeriod_ptr == NULL) {
      lFareTimePeriod_ptr = &stdair::FacBom<stdair::TimePeriod>::instance().
        create (lFareTimePeriodKey);
      stdair::FacBomManager::addToListAndMap (iPosChannel,
                                              *lFareTimePeriod_ptr);
      stdair::FacBomManager::linkWithParent (iPosChannel,
                                             *lFareTimePeriod_ptr);
    }
    // Sanity check.
    assert (lFareTimePeriod_ptr != NULL);

    stdair::TimePeriod& lTimeRange = *lFareTimePeriod_ptr;
    // Generate the fare-features object corresponding to the given
    // fareRule.
    createFareFeatures (lTimeRange, iFareRuleStruct);

  }

  // //////////////////////////////////////////////////////////////////////
  void FareRuleGenerator::
  createFareFeatures (stdair::TimePeriod& iTimePeriod,
                      const FareRuleStruct& iFareRuleStruct) {

    // Create the fare-features primary key.
    const stdair::TripType_T& lTripType =
      iFareRuleStruct.getTripType ();
    const stdair::DayDuration_T& lAdvancePurchase =
      iFareRuleStruct.getAdvancePurchase ();
    const stdair::SaturdayStay_T& lSaturdayStay =
      iFareRuleStruct.getSaturdayStay ();
    const stdair::ChangeFees_T& lChangeFees =
      iFareRuleStruct.getChangeFees ();
    const stdair::NonRefundable_T& lNonRefundable =
      iFareRuleStruct.getNonRefundable ();
    const stdair::DayDuration_T& lMinimumStay =
      iFareRuleStruct.getMinimumStay ();
    const stdair::FareFeaturesKey
      lFareFeaturesKey (lTripType, lAdvancePurchase, lSaturdayStay,
                        lChangeFees, lNonRefundable, lMinimumStay);

    // Check that the fare features object is not already existing.
    // If a fare features object with the same key has not already been
    // created, create it and link it to the time-period object.        
    stdair::FareFeatures* lFareFeatures_ptr = stdair::BomManager::
      getObjectPtr<stdair::FareFeatures> (iTimePeriod,
                                          lFareFeaturesKey.toString());
    if (lFareFeatures_ptr == NULL) {
      lFareFeatures_ptr = &stdair::FacBom<stdair::FareFeatures>::instance().
        create (lFareFeaturesKey);
      assert(lFareFeatures_ptr != NULL); 
      stdair::FacBomManager::addToListAndMap (iTimePeriod,
                                              *lFareFeatures_ptr);
      stdair::FacBomManager::linkWithParent (iTimePeriod,
                                             *lFareFeatures_ptr);  
    }
    // Sanity check.
    assert(lFareFeatures_ptr != NULL);
    
    stdair::FareFeatures& lFareFeatures = *lFareFeatures_ptr;
    // Generate the airline-class list object corresponding to the
    // given fareRule
>>>>>>> trunk
    createAirlineClassList (lFareFeatures, iFareRuleStruct);

  }

  // //////////////////////////////////////////////////////////////////////
  void FareRuleGenerator::
  createAirlineClassList (stdair::FareFeatures& iFareFeatures,
                          const FareRuleStruct& iFareRuleStruct) {

    // Create the AirlineClassList primary key.
    const unsigned int lAirlineListSize =
      iFareRuleStruct.getAirlineListSize();
    const unsigned int lClassCodeListSize =
      iFareRuleStruct.getClassCodeListSize();
    assert (lAirlineListSize == lClassCodeListSize);
    const stdair::AirlineClassListKey
      lAirlineClassListKey (iFareRuleStruct.getAirlineList(),
                            iFareRuleStruct.getClassCodeList());
    const stdair::Fare_T& lFare = iFareRuleStruct.getFare ();
    
    // Create the airline class list object and link it to the fare features
    // object.        
    stdair::AirlineClassList* lAirlineClassList_ptr =
      &stdair::FacBom<stdair::AirlineClassList>::instance().
      create (lAirlineClassListKey);
    lAirlineClassList_ptr->setFare(lFare);
    stdair::FacBomManager::addToListAndMap (iFareFeatures,
                                            *lAirlineClassList_ptr); 
    stdair::FacBomManager::linkWithParent(iFareFeatures,
                                          *lAirlineClassList_ptr);
  }
        
}

