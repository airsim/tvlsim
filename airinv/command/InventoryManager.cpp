// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <exception>
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
        stdair::FacBomManager::
          instance().addToListAndMap (*lCurrentSegmentDate_ptr, lLegDate);
        stdair::FacBomManager::
          instance().addToListAndMap (lLegDate, *lCurrentSegmentDate_ptr);

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
        stdair::FacBomManager::
          instance().addToListAndMap (*lCurrentSegmentCabin_ptr,
                                      lLegCabin, lLegCabin.getFullerKey());

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
          instance().addToListAndMap (lLegCabin, *lCurrentSegmentCabin_ptr,
                                      lCurrentSegmentCabin_ptr->getFullerKey());
      }      
    }
  }
    
  // ////////////////////////////////////////////////////////////////////
  void InventoryManager::
  buildSimilarFlightDateSets (const stdair::BomRoot& iBomRoot) {
    // Browse the list of inventories and create direct accesses
    // within each inventory.
    const stdair::InventoryList_T& lInvList =
      stdair::BomManager::getList<stdair::Inventory> (iBomRoot);
    for (stdair::InventoryList_T::const_iterator itInv = lInvList.begin();
         itInv != lInvList.end(); ++itInv) {
      stdair::Inventory* lCurrentInv_ptr = *itInv;
      assert (lCurrentInv_ptr != NULL);

      buildSimilarFlightDateSets (*lCurrentInv_ptr);
    }
  }
    
  // ////////////////////////////////////////////////////////////////////
  void InventoryManager::
  buildSimilarFlightDateSets (stdair::Inventory& ioInventory) {
    // For instance, we consider two flight-dates are similar if they have the
    // same flight number.

    // By conseconquence, the guillotine blocks will have the same identify
    // number as the flight-dates within it.

    // Browse the flight-date list and build the sets of flight-date
    // which have the same flight number.
    GuillotineNumberSimilarFlightDateSetMap_T lGNSFDSM;
    const stdair::FlightDateList_T& lFlightDateList =
      stdair::BomManager::getList<stdair::FlightDate> (ioInventory);
    for (stdair::FlightDateList_T::const_iterator itFD = lFlightDateList.begin();
         itFD != lFlightDateList.end(); ++itFD) {
      stdair::FlightDate* lFD_ptr = *itFD;
      assert (lFD_ptr != NULL);

      const stdair::FlightNumber_T& lFlightNumber = lFD_ptr->getFlightNumber();
      const stdair::Date_T& lDepartureDate = lFD_ptr->getDepartureDate();

      // Set the guillotine number for the flight-date.
      lFD_ptr->setGuillotineBlockNumber (lFlightNumber);
      
      GuillotineNumberSimilarFlightDateSetMap_T::iterator itGNSFDSM =
        lGNSFDSM.find (lFlightNumber);
      if (itGNSFDSM == lGNSFDSM.end()) {
        DepartureDateFlightDateMap_T lDDFDMap;
        lDDFDMap.insert (DepartureDateFlightDateMap_T::
                         value_type (lDepartureDate, lFD_ptr));
        lGNSFDSM.
          insert (GuillotineNumberSimilarFlightDateSetMap_T::
                  value_type(lFlightNumber, lDDFDMap));
      } else {
        DepartureDateFlightDateMap_T& lDDFDMap = itGNSFDSM->second;
        lDDFDMap.insert (DepartureDateFlightDateMap_T::
                         value_type (lDepartureDate, lFD_ptr));
      }
    }

    // Browse the flight number - departure date set map and initialised the
    // guillotine blocks.
    for (GuillotineNumberSimilarFlightDateSetMap_T::const_iterator itGNSFDSM =
           lGNSFDSM.begin(); itGNSFDSM != lGNSFDSM.end(); ++itGNSFDSM) {
      const stdair::FlightNumber_T& lFlightNumber = itGNSFDSM->first;
      const DepartureDateFlightDateMap_T& lDepartureDateFlightDateMap =
        itGNSFDSM->second;

      buildGuillotineBlock (ioInventory, lFlightNumber,
                            lDepartureDateFlightDateMap);
    }    
  }

  // ////////////////////////////////////////////////////////////////////
  void InventoryManager::
  buildGuillotineBlock (stdair::Inventory& ioInventory,
                        const stdair::GuillotineNumber_T& iGuillotineNumber,
                        const DepartureDateFlightDateMap_T& iDDFDMap) {
    // Build an empty guillotine block.
    const stdair::GuillotineBlockKey lKey (iGuillotineNumber);
    stdair::GuillotineBlock& lGuillotineBlock =
      stdair::FacBom<stdair::GuillotineBlock>::instance().create (lKey);
    stdair::FacBomManager::
      instance().addToListAndMap (ioInventory, lGuillotineBlock);

    // Build the value type index map.
    DepartureDateFlightDateMap_T::const_iterator itDDFD = iDDFDMap.begin();
    assert (itDDFD != iDDFDMap.end());
    const stdair::FlightDate* lFlightDate_ptr = itDDFD->second;
    
    // Browse the fare family list and build the value type for the classes
    // as well as for the family (Q-equivalent).
    stdair::ValueTypeIndexMap_T lValueTypeIndexMap;
    stdair::BlockIndex_T lBlockIndex = 0;

    // Browse the segment-date list
    const stdair::SegmentDateList_T& lSDList =
      stdair::BomManager::getList<stdair::SegmentDate> (*lFlightDate_ptr);
    for (stdair::SegmentDateList_T::const_iterator itSD = lSDList.begin();
         itSD != lSDList.end(); ++itSD) {
      const stdair::SegmentDate* lSegmentDate_ptr = *itSD;
      assert (lSegmentDate_ptr != NULL);

      // Browse the segment-cabin list
      const stdair::SegmentCabinList_T& lSCList =
        stdair::BomManager::getList<stdair::SegmentCabin> (*lSegmentDate_ptr);
      for (stdair::SegmentCabinList_T::const_iterator itSC = lSCList.begin();
         itSC != lSCList.end(); ++itSC) {
        const stdair::SegmentCabin* lSegmentCabin_ptr = *itSC;
        assert (lSegmentCabin_ptr != NULL);

        std::ostringstream lCabinMapKey;
        lCabinMapKey << stdair::DEFAULT_SEGMENT_CABIN_VALUE_TYPE
                     << lSegmentCabin_ptr->describeKey();
        lValueTypeIndexMap.insert (stdair::ValueTypeIndexMap_T::
                                   value_type (lCabinMapKey.str(), lBlockIndex));
        ++lBlockIndex;
        
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
            stdair::BomManager::getList<stdair::BookingClass> (*lFareFamily_ptr);
          for (stdair::BookingClassList_T::const_iterator itBC = lBCList.begin();
               itBC != lBCList.end(); ++itBC) {
            const stdair::BookingClass* lBookingClass_ptr = *itBC;
            assert (lBookingClass_ptr != NULL);
            lValueTypeIndexMap.
              insert (stdair::ValueTypeIndexMap_T::
                      value_type (lBookingClass_ptr->describeKey(),lBlockIndex));
            ++lBlockIndex;
          }
        }
      }
    }

    // Build the flight-date index map
    stdair::FlightDateIndexMap_T lFlightDateIndexMap;
    stdair::BlockNumber_T lBlockNumber = 0;
    for (; itDDFD != iDDFDMap.end(); ++itDDFD, ++lBlockNumber) {
      const stdair::FlightDate* lCurrentFD_ptr = itDDFD->second;
      assert (lCurrentFD_ptr != NULL);
      lFlightDateIndexMap.
        insert (stdair::FlightDateIndexMap_T::value_type (lCurrentFD_ptr,
                                                          lBlockNumber));
    }

    // Initialise the guillotine block.
    lGuillotineBlock.initSnapshotBlocks (lFlightDateIndexMap,lValueTypeIndexMap);
  }
}
