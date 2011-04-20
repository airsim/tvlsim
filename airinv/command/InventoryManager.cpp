// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <exception>
// Boost
#include <boost/make_shared.hpp>
// StdAir
#include <stdair/basic/BasConst_Inventory.hpp>
#include <stdair/basic/BasConst_BomDisplay.hpp>
#include <stdair/bom/BomManager.hpp>
#include <stdair/bom/BomKeyManager.hpp> 
#include <stdair/bom/BomRoot.hpp>
#include <stdair/bom/Inventory.hpp>
#include <stdair/bom/FlightDate.hpp>
#include <stdair/bom/SegmentDate.hpp>
#include <stdair/bom/SegmentCabin.hpp>
#include <stdair/bom/LegDate.hpp>
#include <stdair/bom/LegCabin.hpp>
#include <stdair/bom/FareFamily.hpp>
#include <stdair/bom/BookingClass.hpp>
#include <stdair/bom/GuillotineBlock.hpp>
#include <stdair/bom/TravelSolutionStruct.hpp>
#include <stdair/bom/FareOptionStruct.hpp>
#include <stdair/bom/EventStruct.hpp>
#include <stdair/bom/EventQueue.hpp>
#include <stdair/bom/SnapshotStruct.hpp>
#include <stdair/factory/FacBomManager.hpp>
#include <stdair/factory/FacBom.hpp>
#include <stdair/service/Logger.hpp>
// AirInv
#include <airinv/AIRINV_Types.hpp>
#include <airinv/bom/BomRootHelper.hpp>
#include <airinv/bom/InventoryHelper.hpp>
#include <airinv/command/InventoryManager.hpp>

namespace AIRINV {

  // ////////////////////////////////////////////////////////////////////
  void InventoryManager::
  calculateAvailability (const stdair::BomRoot& iBomRoot,
                         stdair::TravelSolutionStruct& ioTravelSolution) {
    // Browse the list of segments and get the availability for the
    // children classes.
    const stdair::SegmentPath_T& lSegmentPath =
      ioTravelSolution.getSegmentPath();
    for (stdair::SegmentPath_T::const_iterator itSK = lSegmentPath.begin();
         itSK != lSegmentPath.end(); ++itSK) {
      const std::string& lSegmentKey = *itSK;
      const stdair::InventoryKey lInvKey =
        stdair::BomKeyManager::extractInventoryKey (lSegmentKey);
      stdair::Inventory& lInventory =
        stdair::BomManager::getObject<stdair::Inventory>(iBomRoot,
                                                         lInvKey.toString());

      InventoryHelper::calculateAvailability (lInventory, lSegmentKey,
                                              ioTravelSolution);
    }

    // Compute the availabitliy for each fare option using the AU's.
    calculateAvailabilityByAU (ioTravelSolution);
  }

  // ////////////////////////////////////////////////////////////////////
  void InventoryManager::
  calculateAvailabilityByAU (stdair::TravelSolutionStruct& ioTravelSolution) {
    // Browse the fare options
    stdair::FareOptionList_T& lFOList = ioTravelSolution.getFareOptionListRef();
    for (stdair::FareOptionList_T::iterator itFO = lFOList.begin();
         itFO != lFOList.end(); ++itFO) {

      stdair::FareOptionStruct& lFO = *itFO;
      
      // Check the availability
      const stdair::ClassList_StringList_T& lClassPath = lFO.getClassPath();
      
      const stdair::ClassAvailabilityMapHolder_T& lClassAvailabilityMapHolder =
        ioTravelSolution.getClassAvailabilityMapHolder();
      
      // Initialise the flag stating whether the availability is enough
      stdair::Availability_T lAvl =
        std::numeric_limits<stdair::Availability_T>::max();
      
      // Sanity check: the travel solution must contain two lists,
      // one for the booking class availabilities, the other for the
      // fare options.
      assert (lClassAvailabilityMapHolder.empty() == false
              && lClassPath.empty() == false);
      
      // List of booking class availability maps (one map per segment)
      stdair::ClassAvailabilityMapHolder_T::const_iterator itCAMH =
        lClassAvailabilityMapHolder.begin();
      
      // List of fare options
      stdair::ClassList_StringList_T::const_iterator itClassList =
        lClassPath.begin();
      
      // Browse both lists at the same time, i.e., one element per segment
      for (; itCAMH != lClassAvailabilityMapHolder.end()
             && itClassList != lClassPath.end(); ++itCAMH, ++itClassList) {
        
        // Retrieve the booking class list for the current segment
        const stdair::ClassList_String_T& lCurrentClassList = *itClassList;
        assert (lCurrentClassList.size() > 0);
        
        // TODO: instead of just extracting the first booking class,
        //       perform a choice on the full list of classes.
        // Extract one booking class key (class code)
        stdair::ClassCode_T lFirstClass;
        lFirstClass.append (lCurrentClassList, 0, 1);
        
        // Retrieve the booking class map for the current segment
        const stdair::ClassAvailabilityMap_T& lClassAvlMap = *itCAMH;
        
        // Retrieve the availability of the chosen booking class
        const stdair::ClassAvailabilityMap_T::const_iterator itClassAvl =
          lClassAvlMap.find (lFirstClass);
        
        if (itClassAvl == lClassAvlMap.end()) {
          // DEBUG
          STDAIR_LOG_DEBUG ("No availability has been set up for the class '"
                            << lFirstClass << "'. Travel solution: "
                            << ioTravelSolution.display());
        }
        assert (itClassAvl != lClassAvlMap.end());
        
        const stdair::Availability_T& lCurrentAvl = itClassAvl->second;
        if (lAvl > lCurrentAvl) {
          lAvl = lCurrentAvl;
        }
      }
      
      lFO.setAvailability (lAvl);
    }
  }

  // ////////////////////////////////////////////////////////////////////
  bool InventoryManager::sell (stdair::Inventory& ioInventory,
                               const std::string& iSegmentDateKey,
                               const stdair::ClassCode_T& iClassCode,
                               const stdair::PartySize_T& iPartySize) {

    // Make the sale within the inventory.
    return InventoryHelper::sell (ioInventory, iSegmentDateKey,
                                  iClassCode, iPartySize);
  }

  // ////////////////////////////////////////////////////////////////////
  void InventoryManager::takeSnapshots(const stdair::Inventory& iInventory,
                                       const stdair::DateTime_T& iSnapshotTime){

    // Make the snapshots within the inventory
    InventoryHelper::takeSnapshots (iInventory, iSnapshotTime);
  }
    
  // ////////////////////////////////////////////////////////////////////
  void InventoryManager::
  createDirectAccesses (const stdair::BomRoot& iBomRoot) {

    // Browse the list of inventories and create direct accesses
    // within each inventory.
    const stdair::InventoryList_T& lInvList =
      stdair::BomManager::getList<stdair::Inventory> (iBomRoot);
    for (stdair::InventoryList_T::const_iterator itInv = lInvList.begin();
         itInv != lInvList.end(); ++itInv) {
      stdair::Inventory* lCurrentInv_ptr = *itInv;
      assert (lCurrentInv_ptr != NULL);

      createDirectAccesses (*lCurrentInv_ptr);
    }

    // Fill some attributes of segment-date with the routing legs.
    BomRootHelper::fillFromRouting (iBomRoot);
  }

  // ////////////////////////////////////////////////////////////////////
  void InventoryManager::
  createDirectAccesses (stdair::Inventory& ioInventory) {

    // Browse the list of flight-dates and create direct accesses
    // within each flight-date.
    const stdair::FlightDateList_T& lFlightDateList = 
      stdair::BomManager::getList<stdair::FlightDate> (ioInventory);
    for (stdair::FlightDateList_T::const_iterator itFlightDate = 
           lFlightDateList.begin();
         itFlightDate != lFlightDateList.end(); ++itFlightDate) {
      stdair::FlightDate* lCurrentFlightDate_ptr = *itFlightDate;
      assert (lCurrentFlightDate_ptr != NULL);

      createDirectAccesses (*lCurrentFlightDate_ptr);
    }
  }

  // ////////////////////////////////////////////////////////////////////
  void InventoryManager::
  createDirectAccesses (stdair::FlightDate& ioFlightDate) {

    // Browse the list of segment-dates and create direct accesses
    // within each segment-date.
    const stdair::SegmentDateList_T& lSegmentDateList = 
      stdair::BomManager::getList<stdair::SegmentDate> (ioFlightDate);
    for (stdair::SegmentDateList_T::const_iterator itSegmentDate = 
           lSegmentDateList.begin();
         itSegmentDate != lSegmentDateList.end(); ++itSegmentDate) {

      stdair::SegmentDate* lCurrentSegmentDate_ptr = *itSegmentDate;
      assert (lCurrentSegmentDate_ptr != NULL);

      const stdair::AirportCode_T& lBoardingPoint =
        lCurrentSegmentDate_ptr->getBoardingPoint();

      stdair::AirportCode_T currentBoardingPoint = lBoardingPoint;
      const stdair::AirportCode_T& lOffPoint =
        lCurrentSegmentDate_ptr->getOffPoint();
      
      // Add a sanity check so as to ensure that the loop stops. If
      // there are more than MAXIMAL_NUMBER_OF_LEGS legs, there is
      // an issue somewhere in the code (not in the parser, as the
      // segments are derived from the legs thanks to the
      // FlightPeriodStruct::buildSegments() method).
      unsigned short i = 1;
      while (currentBoardingPoint != lOffPoint
             && i <= stdair::MAXIMAL_NUMBER_OF_LEGS_IN_FLIGHT) {
        // Retrieve the (unique) LegDate getting that Boarding Point
        stdair::LegDate& lLegDate = stdair::BomManager::
          getObject<stdair::LegDate> (ioFlightDate, currentBoardingPoint);

        // Link the SegmentDate and LegDate together
        stdair::FacBomManager::addToListAndMap (*lCurrentSegmentDate_ptr,
                                                lLegDate);
        stdair::FacBomManager::addToListAndMap (lLegDate,
                                                *lCurrentSegmentDate_ptr);

        // Prepare the next iteration
        currentBoardingPoint = lLegDate.getOffPoint();
        ++i;
      }
      assert (i <= stdair::MAXIMAL_NUMBER_OF_LEGS_IN_FLIGHT);
          
      // Create the routing for the leg- and segment-cabins.
      // At the same time, set the SegmentDate attributes derived from
      // its routing legs (e.g., boarding and off dates).
      createDirectAccesses (*lCurrentSegmentDate_ptr);
    }
  }

  // ////////////////////////////////////////////////////////////////////
  void InventoryManager::
  createDirectAccesses (stdair::SegmentDate& ioSegmentDate) {

    // Browse the list of segment-cabins and create direct accesses
    // within each segment-cabin.
    const stdair::SegmentCabinList_T& lSegmentCabinList = 
      stdair::BomManager::getList<stdair::SegmentCabin> (ioSegmentDate);
    for (stdair::SegmentCabinList_T::const_iterator itSegmentCabin = 
           lSegmentCabinList.begin();
         itSegmentCabin != lSegmentCabinList.end(); ++itSegmentCabin) {

      //
      stdair::SegmentCabin* lCurrentSegmentCabin_ptr = *itSegmentCabin;
      assert (lCurrentSegmentCabin_ptr != NULL);

      //
      const stdair::CabinCode_T& lCabinCode =
          lCurrentSegmentCabin_ptr->getCabinCode();
      
      // Iterate on the routing legs
      const stdair::LegDateList_T& lLegDateList =
        stdair::BomManager::getList<stdair::LegDate> (ioSegmentDate);
      for (stdair::LegDateList_T::const_iterator itLegDate =
             lLegDateList.begin();
           itLegDate != lLegDateList.end(); ++itLegDate) {

        const stdair::LegDate* lCurrentLegDate_ptr = *itLegDate;        
        assert (lCurrentLegDate_ptr != NULL);

        // Retrieve the LegCabin getting the same class of service
        // (cabin code) as the SegmentCabin.
        stdair::LegCabin& lLegCabin = stdair::BomManager::
          getObject<stdair::LegCabin> (*lCurrentLegDate_ptr, lCabinCode);

        /**
         * Add the leg-cabin to the segment-cabin routing.
         *
         * As several leg-cabins may compose the segment-cabin routing,
         * and as the leg-cabin key is only made by a cabin code (which
         * is the same as for the segment-cabin), all the leg-cabins
         * composing the routing would have the same key.
         * Hence, the leg-cabins must be differentiated according to their
         * boarding point as well.
         */
        stdair::FacBomManager::addToListAndMap (*lCurrentSegmentCabin_ptr,
                                                lLegCabin,
                                                lLegCabin.getFullerKey());

        /**
         * Add the segment-cabin to the list which the leg-cabin crosses.
         *
         * As several segment-cabins may cross the leg-cabin,
         * and as the segment-cabin key is only made by a cabin code (which
         * is the same as for the leg-cabin), all the segment-cabins
         * crossing the leg-cabin would have the same key.
         * Hence, the segment-cabins must be differentiated according to their
         * boarding and off points as well.
         */
        stdair::FacBomManager::
          addToListAndMap (lLegCabin, *lCurrentSegmentCabin_ptr,
                           lCurrentSegmentCabin_ptr->getFullerKey());
      }      
    }
  }
    
  // ////////////////////////////////////////////////////////////////////
  void InventoryManager::
  buildSimilarSegmentCabinSets (const stdair::BomRoot& iBomRoot) {
    // Browse the list of inventories and create direct accesses
    // within each inventory.
    const stdair::InventoryList_T& lInvList =
      stdair::BomManager::getList<stdair::Inventory> (iBomRoot);
    for (stdair::InventoryList_T::const_iterator itInv = lInvList.begin();
         itInv != lInvList.end(); ++itInv) {
      stdair::Inventory* lCurrentInv_ptr = *itInv;
      assert (lCurrentInv_ptr != NULL);

      buildSimilarSegmentCabinSets (*lCurrentInv_ptr);
    }
  }
    
  // ////////////////////////////////////////////////////////////////////
  void InventoryManager::
  buildSimilarSegmentCabinSets (stdair::Inventory& ioInventory) {
    // For instance, we consider two flight-dates are
    // similar if they have the same flight number and the same
    // day-of-the-week departure.

    // Browse the segment-cabin list and build the sets of segment-cabin
    // which have the same flight number and origin-destination
    SimilarSegmentCabinSetMap_T lSSCSM;

    // Browsing the flight-date list
    const stdair::FlightDateList_T& lFlightDateList =
      stdair::BomManager::getList<stdair::FlightDate> (ioInventory);
    for (stdair::FlightDateList_T::const_iterator itFD= lFlightDateList.begin();
         itFD != lFlightDateList.end(); ++itFD) {
      const stdair::FlightDate* lFD_ptr = *itFD;
      assert (lFD_ptr != NULL);
      const stdair::FlightNumber_T& lFlightNumber = lFD_ptr->getFlightNumber();

      // Browsing the segment-date list and retrieve the departure
      // date, the origine and the destination of the segment
      const stdair::SegmentDateList_T& lSegmentDateList =
        stdair::BomManager::getList<stdair::SegmentDate> (*lFD_ptr);
      for (stdair::SegmentDateList_T::const_iterator itSD =
             lSegmentDateList.begin(); itSD != lSegmentDateList.end(); ++itSD) {
        const stdair::SegmentDate* lSD_ptr = *itSD;
        assert (lSD_ptr != NULL);

        const stdair::Date_T& lDepartureDate = lSD_ptr->getBoardingDate();
        const stdair::AirportCode_T& lOrigin = lSD_ptr->getBoardingPoint();
        const stdair::AirportCode_T& lDestination = lSD_ptr->getOffPoint();

        // Browsing the segment-cabin list and retrieve the cabin code and
        // build the corresponding key map.
        const stdair::SegmentCabinList_T& lSegmentCabinList =
          stdair::BomManager::getList<stdair::SegmentCabin> (*lSD_ptr);
        for (stdair::SegmentCabinList_T::const_iterator itSC =
               lSegmentCabinList.begin();
             itSC != lSegmentCabinList.end(); ++itSC) {
          const stdair::SegmentCabin* lSC_ptr = *itSC;
          assert (lSC_ptr != NULL);

          std::ostringstream oStr;
          oStr << lFlightNumber << lDepartureDate.day_of_week()
               << lOrigin << lDestination << lSC_ptr->getCabinCode();
          const std::string lMapKey = oStr.str();

          // Add the segment cabin to the similar segment cabin set map.
          SimilarSegmentCabinSetMap_T::iterator itSSCS = lSSCSM.find (lMapKey);
          if (itSSCS == lSSCSM.end()) {
            DepartureDateSegmentCabinMap_T lDDSCMap;
            lDDSCMap.insert (DepartureDateSegmentCabinMap_T::
                             value_type (lDepartureDate, lSC_ptr));
            lSSCSM.insert (SimilarSegmentCabinSetMap_T::
                           value_type (lMapKey, lDDSCMap));
          } else {
            DepartureDateSegmentCabinMap_T& lDDSCMap = itSSCS->second;
            lDDSCMap.insert (DepartureDateSegmentCabinMap_T::
                             value_type (lDepartureDate, lSC_ptr));
          }
        }
      }
    }

    // Initialise the guillotine blocks.
    stdair::GuillotineNumber_T lGuillotineNumber = 1;
    for (SimilarSegmentCabinSetMap_T::const_iterator itSSCS = lSSCSM.begin();
         itSSCS != lSSCSM.end(); ++itSSCS, ++lGuillotineNumber) {
      const DepartureDateSegmentCabinMap_T& lDDSCMap = itSSCS->second;

      buildGuillotineBlock (ioInventory, lGuillotineNumber, lDDSCMap);
    }    
  }

  // ////////////////////////////////////////////////////////////////////
  void InventoryManager::
  buildGuillotineBlock (stdair::Inventory& ioInventory,
                        const stdair::GuillotineNumber_T& iGuillotineNumber,
                        const DepartureDateSegmentCabinMap_T& iDDSCMap) {
    // Build an empty guillotine block.
    const stdair::GuillotineBlockKey lKey (iGuillotineNumber);
    stdair::GuillotineBlock& lGuillotineBlock =
      stdair::FacBom<stdair::GuillotineBlock>::instance().create (lKey);
    stdair::FacBomManager::addToListAndMap (ioInventory, lGuillotineBlock);

    // Build the value type index map.
    DepartureDateSegmentCabinMap_T::const_iterator itDDSC = iDDSCMap.begin();
    assert (itDDSC != iDDSCMap.end());
    const stdair::SegmentCabin* lSegmentCabin_ptr = itDDSC->second;
    
    // Browse the fare family list and build the value type for the classes
    // as well as for the family (Q-equivalent).
    stdair::ValueTypeIndexMap_T lValueTypeIndexMap;
    stdair::BlockIndex_T lBlockIndex = 0;

    // Browse the fare family list
    const stdair::FareFamilyList_T& lFFList =
      stdair::BomManager::getList<stdair::FareFamily> (*lSegmentCabin_ptr);
    for (stdair::FareFamilyList_T::const_iterator itFF = lFFList.begin();
         itFF != lFFList.end(); ++itFF) {
      const stdair::FareFamily* lFareFamily_ptr = *itFF;
      assert (lFareFamily_ptr != NULL);
          
      std::ostringstream lFFMapKey;
      lFFMapKey << stdair::DEFAULT_FARE_FAMILY_VALUE_TYPE
                << lFareFamily_ptr->describeKey();
      lValueTypeIndexMap.insert (stdair::ValueTypeIndexMap_T::
                                 value_type (lFFMapKey.str(), lBlockIndex));
      ++lBlockIndex;

      // Browse the booking class list
      const stdair::BookingClassList_T& lBCList =
        stdair::BomManager::getList<stdair::BookingClass>(*lFareFamily_ptr);
      for (stdair::BookingClassList_T::const_iterator itBC= lBCList.begin();
           itBC != lBCList.end(); ++itBC) {
        const stdair::BookingClass* lBookingClass_ptr = *itBC;
        assert (lBookingClass_ptr != NULL);
        lValueTypeIndexMap.
          insert (stdair::ValueTypeIndexMap_T::
                  value_type(lBookingClass_ptr->describeKey(),lBlockIndex));
        ++lBlockIndex;
      }
    }

    // Build the flight-date index map
    stdair::SegmentCabinIndexMap_T lSegmentCabinIndexMap;
    stdair::BlockNumber_T lBlockNumber = 0;
    for (; itDDSC != iDDSCMap.end(); ++itDDSC, ++lBlockNumber) {
      const stdair::SegmentCabin* lCurrentSC_ptr = itDDSC->second;
      assert (lCurrentSC_ptr != NULL);
      lSegmentCabinIndexMap.
        insert (stdair::SegmentCabinIndexMap_T::value_type (lCurrentSC_ptr,
                                                            lBlockNumber));
    }

    // Initialise the guillotine block.
    lGuillotineBlock.initSnapshotBlocks(lSegmentCabinIndexMap,
                                        lValueTypeIndexMap);
  }

  // ////////////////////////////////////////////////////////////////////
  void InventoryManager::initSnapshotEvents (const stdair::Date_T& iStartDate,
                                             const stdair::Date_T& iEndDate,
                                             stdair::EventQueue& ioQueue) {
    const stdair::Duration_T lTimeZero (0, 0, 0);
    const stdair::Duration_T lOneDayDuration (24, 0, 0);
    const stdair::DateTime_T lBeginSnapshotTime (iStartDate, lTimeZero);
    const stdair::DateTime_T lEndSnapshotTime (iEndDate, lTimeZero);

    // TODO: remove the defaut airline code.
    stdair::NbOfEvents_T lNbOfSnapshots = 0.0;
    for (stdair::DateTime_T lSnapshotTime = lBeginSnapshotTime;
         lSnapshotTime < lEndSnapshotTime; lSnapshotTime += lOneDayDuration) {
      // Create the snapshot event structure
      stdair::SnapshotPtr_T lSnapshotStruct =
        boost::make_shared<stdair::SnapshotStruct>(stdair::DEFAULT_AIRLINE_CODE,
                                                   lSnapshotTime);
      // Create the event structure
      stdair::EventStruct lEventStruct (stdair::EventType::SNAPSHOT,
                                        lSnapshotStruct);

    /**
       \note When adding an event in the event queue, the event can be
       altered. That happens when an event already exists, in the
       event queue, with exactly the same date-time stamp. In that
       case, the date-time stamp is altered for the newly added event,
       so that the unicity on the date-time stamp can be guaranteed.
    */
    ioQueue.addEvent (lEventStruct);
    ++lNbOfSnapshots;
    }

    ioQueue.addStatus (stdair::EventType::SNAPSHOT, lNbOfSnapshots);
  }
}
