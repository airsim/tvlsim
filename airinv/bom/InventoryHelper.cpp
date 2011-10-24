// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// StdAir
#include <stdair/bom/BomRetriever.hpp>
#include <stdair/bom/BomManager.hpp>
#include <stdair/bom/Inventory.hpp>
#include <stdair/bom/FlightDate.hpp>
#include <stdair/bom/SegmentDate.hpp>
#include <stdair/bom/SegmentCabin.hpp>
#include <stdair/bom/FareFamily.hpp>
#include <stdair/bom/BookingClass.hpp>
#include <stdair/bom/GuillotineBlock.hpp>
#include <stdair/bom/TravelSolutionStruct.hpp>
#include <stdair/service/Logger.hpp>
#include <stdair/bom/LegCabin.hpp>
// AirInv
#include <airinv/bom/InventoryHelper.hpp>
#include <airinv/bom/FlightDateHelper.hpp>
#include <airinv/bom/GuillotineBlockHelper.hpp>
#include <airinv/bom/SegmentCabinHelper.hpp>

namespace AIRINV {

  // ////////////////////////////////////////////////////////////////////
  void InventoryHelper::fillFromRouting (const stdair::Inventory& iInventory) {
    const stdair::FlightDateList_T& lFlightDateList =
      stdair::BomManager::getList<stdair::FlightDate> (iInventory);

    // Browse the list of flight-dates and update each flight-date.
    for (stdair::FlightDateList_T::const_iterator itFlightDate =
           lFlightDateList.begin();
         itFlightDate != lFlightDateList.end(); ++itFlightDate) {
      const stdair::FlightDate* lCurrentFlightDate_ptr = *itFlightDate;
      assert (lCurrentFlightDate_ptr != NULL);
      FlightDateHelper::fillFromRouting (*lCurrentFlightDate_ptr);
    }
  }

  // ////////////////////////////////////////////////////////////////////
  void InventoryHelper::
  calculateAvailability (const stdair::Inventory& iInventory, 
                         const std::string& iFullSegmentDateKey,
                         stdair::TravelSolutionStruct& ioTravelSolution) {

    // Create the map of class/availability for the given segment date.
    stdair::ClassAvailabilityMap_T lClassAvailabilityMap;
 
    // DEBUG
    STDAIR_LOG_DEBUG (iFullSegmentDateKey);
    //
    stdair::SegmentDate* lSegmentDate_ptr =
      stdair::BomRetriever::retrieveSegmentDateFromLongKey(iInventory,
                                                           iFullSegmentDateKey);
    assert (lSegmentDate_ptr != NULL);
    
    // Browse the segment-cabins and fill the map with the availability of
    // each booking class.
    const stdair::SegmentCabinList_T& lSegmentCabinList =
      stdair::BomManager::getList<stdair::SegmentCabin> (*lSegmentDate_ptr);
    for (stdair::SegmentCabinList_T::const_iterator itCabin =
           lSegmentCabinList.begin();
         itCabin != lSegmentCabinList.end(); ++itCabin) {
      stdair::SegmentCabin* lSegmentCabin_ptr = *itCabin;
      assert (lSegmentCabin_ptr != NULL);


      // Compute the availability using the AU and the cumulative
      // booking counter.
      SegmentCabinHelper::updateAvailabilities (*lSegmentCabin_ptr);
      const stdair::BookingClassList_T& lBCList =
        stdair::BomManager::getList<stdair::BookingClass> (*lSegmentCabin_ptr);
      for (stdair::BookingClassList_T::const_reverse_iterator itBC =
           lBCList.rbegin(); itBC != lBCList.rend(); ++itBC) {
        stdair::BookingClass* lBC_ptr = *itBC;
        assert (lBC_ptr != NULL);
        
        const stdair::Availability_T lAvl = lBC_ptr->getSegmentAvailability();
        
        const stdair::ClassCode_T& lClassCode = lBC_ptr->getClassCode();        
        const bool insertSuccessful = lClassAvailabilityMap.
          insert (stdair::ClassAvailabilityMap_T::value_type (lClassCode,
                                                              lAvl)).second;
        assert (insertSuccessful == true);
      }
    }

    //
    ioTravelSolution.addClassAvailabilityMap (lClassAvailabilityMap);
  }

  
  // ////////////////////////////////////////////////////////////////////
  void InventoryHelper::
  getYieldAndBidPrice (const stdair::Inventory& iInventory, 
                         const std::string& iFullSegmentDateKey,
                         stdair::TravelSolutionStruct& ioTravelSolution) {

    // Create the map of class/availability for the given segment date.
    // stdair::ClassAvailabilityMap_T lClassAvailabilityMap;
    
    stdair::ClassYieldMap_T lClassYieldMap; 

    stdair::ClassBpvMap_T lClassBpvMap; 
 
    // DEBUG
    STDAIR_LOG_DEBUG (iFullSegmentDateKey);
    //
    stdair::SegmentDate* lSegmentDate_ptr =
      stdair::BomRetriever::retrieveSegmentDateFromLongKey (iInventory,
                                                            iFullSegmentDateKey);
    assert (lSegmentDate_ptr != NULL);
    
    // Browse the segment-cabins and fill the maps with the bid price vector reference
    // and yield of each booking class.
    const stdair::SegmentCabinList_T& lSegmentCabinList =
      stdair::BomManager::getList<stdair::SegmentCabin> (*lSegmentDate_ptr);
    for (stdair::SegmentCabinList_T::const_iterator itCabin =
           lSegmentCabinList.begin();
         itCabin != lSegmentCabinList.end(); ++itCabin) {
      stdair::SegmentCabin* lSegmentCabin_ptr = *itCabin;
      assert (lSegmentCabin_ptr != NULL);

      stdair::BidPriceVector_T lBPV;
      
      
      //stdair::BidPriceVector_T lBPV;
      stdair::LegCabinList_T lLegCabinList =
        stdair::BomManager::getList<stdair::LegCabin> (*lSegmentCabin_ptr);
      assert (!lLegCabinList.empty());
      if (lLegCabinList.size() > 1) {
        // Compute the sum of bid prices and return a vector containing that value.
        stdair::BidPrice_T lBidPriceValue = 0;
        for (stdair::LegCabinList_T::const_iterator itLC = lLegCabinList.begin();
             itLC != lLegCabinList.end(); ++itLC) {
          const stdair::LegCabin* lLegCabin_ptr = *itLC;
          const stdair::BidPriceVector_T& lLegCabinBPV = lLegCabin_ptr->getBidPriceVector();
          if (!lLegCabinBPV.empty()) {
            lBidPriceValue += lLegCabinBPV.back();
          } else {
            // If the remaining capacity is zero (empty bid price vector) on one of the legs,
            // then the remaining capacity of the segment is also zero (return an empty bid price).
            lBidPriceValue = std::numeric_limits<stdair::BidPrice_T>::max();
            break;
          }
        }
        if (lBidPriceValue < std::numeric_limits<stdair::BidPrice_T>::max()) {
          lBPV.push_back(lBidPriceValue);
        }

      } else {
        const stdair::LegCabin* lLegCabin_ptr = lLegCabinList.front();
        lBPV = lLegCabin_ptr->getBidPriceVector();
      }
      
      
      // const stdair::CabinCapacity_T& lCabinCapacity = lSegmentCabin_ptr->getCapacity();
      // const stdair::CommittedSpace_T& lCommittedSpace = lSegmentCabin_ptr->getCommittedSpace();
      // assert (lCabinCapacity - lCommittedSpace > 0);
      // lBPV.resize(lCabinCapacity - lCommittedSpace);

      const stdair::Availability_T& lAvailabilityPool =
        lSegmentCabin_ptr->getAvailabilityPool();
      //assert (lAvailabilityPool > 0);

      if (lAvailabilityPool < lBPV.size()) {
        lBPV.resize(lAvailabilityPool);
      }

      
      //
      ioTravelSolution.addBidPriceVector (lBPV);

      const stdair::BidPriceVectorHolder_T& lBidPriceVectorHolder =
        ioTravelSolution.getBidPriceVectorHolder();
      const stdair::BidPriceVectorHolder_T::const_reverse_iterator itBPV =
        lBidPriceVectorHolder.rbegin();
      const stdair::BidPriceVector_T& lBpvRef = *itBPV;
 
      const stdair::FareFamilyList_T& lFFList =
        stdair::BomManager::getList<stdair::FareFamily> (*lSegmentCabin_ptr);
      for (stdair::FareFamilyList_T::const_iterator itFF = lFFList.begin();
           itFF != lFFList.end(); ++itFF) {
        const stdair::FareFamily* lFareFamily_ptr = *itFF;
        assert (lFareFamily_ptr != NULL);

        const stdair::BookingClassList_T& lBCList =
          stdair::BomManager::getList<stdair::BookingClass> (*lFareFamily_ptr);
        for (stdair::BookingClassList_T::const_iterator itBC = lBCList.begin();
             itBC != lBCList.end(); ++itBC) {
          const stdair::BookingClass* lBC_ptr = *itBC;
          assert (lBC_ptr != NULL);
          
          const stdair::ClassCode_T& lClassCode = lBC_ptr->getClassCode();

	  const stdair::YieldValue_T lYld = lBC_ptr->getYield() ;  
	  const bool insertYieldMapSuccessful = lClassYieldMap.
            insert (stdair::ClassYieldMap_T::value_type (lClassCode,
                                                         lYld)).second;
	  assert (insertYieldMapSuccessful == true);

	  const bool insertBpvMapSuccessful = lClassBpvMap.
            insert (stdair::ClassBpvMap_T::value_type (lClassCode,
                                                       &lBpvRef)).second;
	  assert (insertBpvMapSuccessful == true);
          
          // DEBUG
          // STDAIR_LOG_DEBUG ("Class: " << lClassCode
          //                   << ", " << "Yield: " << lYld << ", "
          //                   << "Bid price: " << lBpvRef.back() << ", "
	  // 		       << "Remaining capacity: "
          //                   << lCabinCapacity - lCommittedSpace);
  
          //
	  stdair::BidPrice_T lBpvVal = std::numeric_limits<double>::max();
	  if (lBpvRef.empty() == false) {
	    lBpvVal = lBpvRef.back();
	  }

	  //lBpvVal = boost::lexical_cast<std::string> (lBpvRef.back());
          STDAIR_LOG_DEBUG ("Class: " << lClassCode
                            << ", " << "Yield: " << lYld << ", "
                            << "Bid price: " << lBpvVal << ", "
	  		    << "Remaining capacity: " << lAvailabilityPool
			    << " Segment date: " << iFullSegmentDateKey);
        }
      }
    }

    //
    ioTravelSolution.addClassYieldMap (lClassYieldMap);
    ioTravelSolution.addClassBpvMap (lClassBpvMap);
  }
  

  // ////////////////////////////////////////////////////////////////////
  bool InventoryHelper::sell (stdair::Inventory& ioInventory, 
                              const std::string& iFullSegmentDateKey,
                              const stdair::ClassCode_T& iClassCode,
                              const stdair::PartySize_T& iPartySize) {
    bool hasSaleBeenSuccessful = false;

    // DEBUG
    STDAIR_LOG_DEBUG ("Full key: '" << iFullSegmentDateKey
                      << "', " << iClassCode);

    //
    stdair::BookingClass* lBookingClass_ptr =
      stdair::BomRetriever::retrieveBookingClassFromLongKey(ioInventory,
                                                            iFullSegmentDateKey,
                                                            iClassCode);

    // DEBUG
    const std::string hasFoundBookingClassStr =
      (lBookingClass_ptr != NULL)?"Yes":"No";
    STDAIR_LOG_DEBUG ("Found booking class? " << hasFoundBookingClassStr);

    if (lBookingClass_ptr != NULL) {
      // Register the sale in the class.
      lBookingClass_ptr->sell (iPartySize);

      //
      stdair::FareFamily& lFareFamily =
        stdair::BomManager::getParent<stdair::FareFamily> (*lBookingClass_ptr);

      //
      stdair::SegmentCabin& lSegmentCabin =
        stdair::BomManager::getParent<stdair::SegmentCabin> (lFareFamily);

      //
      stdair::SegmentDate& lSegmentDate =
        stdair::BomManager::getParent<stdair::SegmentDate,
                                      stdair::SegmentCabin> (lSegmentCabin);

      //
      stdair::FlightDate& lFlightDate =
        stdair::BomManager::getParent<stdair::FlightDate,
                                      stdair::SegmentDate> (lSegmentDate);
      
      // Update the commited space of the segment-cabins and the leg-cabins.
      SegmentCabinHelper::updateFromReservation (lFlightDate, lSegmentCabin,
                                                 iPartySize);
      
      // STDAIR_LOG_NOTIFICATION (lFlightDate.getDepartureDate()
      //                          << ";" << iClassCode);
      hasSaleBeenSuccessful = true;
    }

    return hasSaleBeenSuccessful;
  }  

  // ////////////////////////////////////////////////////////////////////
  bool InventoryHelper::cancel (stdair::Inventory& ioInventory, 
                              const std::string& iFullSegmentDateKey,
                              const stdair::ClassCode_T& iClassCode,
                              const stdair::PartySize_T& iPartySize) {
    bool hasCancellationBeenSuccessful = false;

    // DEBUG
    STDAIR_LOG_DEBUG ("Full key: '" << iFullSegmentDateKey
                      << "', " << iClassCode);

    //
    stdair::BookingClass* lBookingClass_ptr =
      stdair::BomRetriever::retrieveBookingClassFromLongKey(ioInventory,
                                                            iFullSegmentDateKey,
                                                            iClassCode);

    // DEBUG
    const std::string hasFoundBookingClassStr =
      (lBookingClass_ptr != NULL)?"Yes":"No";
    STDAIR_LOG_DEBUG ("Found booking class? " << hasFoundBookingClassStr);

    if (lBookingClass_ptr != NULL) {
      // Register the cancellation in the class.
      lBookingClass_ptr->cancel (iPartySize);

      //
      stdair::FareFamily& lFareFamily =
        stdair::BomManager::getParent<stdair::FareFamily> (*lBookingClass_ptr);

      //
      stdair::SegmentCabin& lSegmentCabin =
        stdair::BomManager::getParent<stdair::SegmentCabin> (lFareFamily);

      //
      stdair::SegmentDate& lSegmentDate =
        stdair::BomManager::getParent<stdair::SegmentDate,
                                      stdair::SegmentCabin> (lSegmentCabin);

      //
      stdair::FlightDate& lFlightDate =
        stdair::BomManager::getParent<stdair::FlightDate,
                                      stdair::SegmentDate> (lSegmentDate);
      
      // Update the commited space of the segment-cabins and the leg-cabins.
      SegmentCabinHelper::updateFromReservation (lFlightDate, lSegmentCabin,
                                                 -iPartySize);
      
      // STDAIR_LOG_NOTIFICATION (lFlightDate.getDepartureDate()
      //                          << ";" << iClassCode);
      hasCancellationBeenSuccessful = true;
    }

    return hasCancellationBeenSuccessful;
  }

  // ////////////////////////////////////////////////////////////////////
  void InventoryHelper::takeSnapshots(const stdair::Inventory& iInventory,
                                      const stdair::DateTime_T& iSnapshotTime) {
    // Browse the guillotine block list and take the snapshots for
    // each guillotine.
    const stdair::GuillotineBlockList_T& lGuillotineBlockList =
      stdair::BomManager::getList<stdair::GuillotineBlock> (iInventory);
    for (stdair::GuillotineBlockList_T::const_iterator itGB =
           lGuillotineBlockList.begin();
         itGB != lGuillotineBlockList.end(); ++itGB) {
      stdair::GuillotineBlock* lGuillotineBlock_ptr = *itGB;

      GuillotineBlockHelper::takeSnapshots(*lGuillotineBlock_ptr,iSnapshotTime);
    }    
  }
}
