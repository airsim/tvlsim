// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// BOOST
#include <boost/date_time/date_iterator.hpp>
// StdAir
#include <stdair/stdair_types.hpp>
#include <stdair/basic/BasConst_Inventory.hpp>
#include <stdair/bom/BomManager.hpp>
#include <stdair/bom/BomRoot.hpp>
#include <stdair/bom/Inventory.hpp>
#include <stdair/bom/FlightDate.hpp>
#include <stdair/bom/SegmentDate.hpp>
#include <stdair/bom/SegmentCabin.hpp>
#include <stdair/bom/FareFamily.hpp>
#include <stdair/bom/BookingClass.hpp>
#include <stdair/bom/LegDate.hpp>
#include <stdair/bom/LegCabin.hpp>
#include <stdair/factory/FacBomManager.hpp>
#include <stdair/service/Logger.hpp>
// AIRINV
#include <airinv/bom/FlightPeriodStruct.hpp>
#include <airinv/bom/BomRootHelper.hpp>
#include <airinv/command/InventoryGenerator.hpp>

namespace AIRINV {

  // ////////////////////////////////////////////////////////////////////
  void InventoryGenerator::
  createFlightDate (stdair::BomRoot& ioBomRoot,
                    const FlightPeriodStruct& iFlightPeriod) {
    const stdair::AirlineCode_T& lAirlineCode = iFlightPeriod._airlineCode;
 
    // Instantiate an inventory object (if not exist)
    // for the given key (airline code)
    stdair::Inventory* lInventory_ptr = stdair::BomManager::
      getObjectPtr<stdair::Inventory> (ioBomRoot, lAirlineCode);
    if (lInventory_ptr == NULL) {
      stdair::InventoryKey lKey (lAirlineCode);
      lInventory_ptr =
        &stdair::FacBom<stdair::Inventory>::instance().create (lKey);
      stdair::FacBomManager::
        instance().addToListAndMap (ioBomRoot, *lInventory_ptr);
      stdair::FacBomManager::
        instance().linkWithParent (ioBomRoot, *lInventory_ptr);
    }
    assert (lInventory_ptr != NULL);
    
    // Generate all the dates corresponding to the period
    // and create the corresponding flight-dates.
    const stdair::DatePeriod_T lDateRange = iFlightPeriod._dateRange;

    for (boost::gregorian::day_iterator itDate = lDateRange.begin();
         itDate != lDateRange.end(); ++itDate) {
      const stdair::Date_T& currentDate = *itDate;

      // Retrieve, for the current day, the Day-Of-the-Week (thanks to Boost)
      const unsigned short currentDoW = currentDate.day_of_week().as_number();
        
      // The FlightPeriod structure stores which Days (-Of-the-Week) are
      // active within the week. For each day (Mon., Tue., etc.), a boolean
      // states whether the Flight is active for that day.
      const stdair::DoWStruct& lDoWList = iFlightPeriod._dow;
      const bool isDoWActive = lDoWList.getStandardDayOfWeek (currentDoW);

      if (isDoWActive == true) {
        stdair::FlightDate& lFlightDate = createFlightDate (*lInventory_ptr,
                                                            currentDate,
                                                            iFlightPeriod);
        
        // Create the list of references on previous built similar flight dates
        const stdair::Date_T& lStartDateRange = lDateRange.begin();
        createSimilarFlightDateList (lFlightDate, *lInventory_ptr, currentDate,
                                     lStartDateRange);
      }
    }
  }
  
  // ////////////////////////////////////////////////////////////////////
  stdair::FlightDate& InventoryGenerator::
  createFlightDate (stdair::Inventory& ioInventory,
                    const stdair::Date_T& iFlightDate,
                    const FlightPeriodStruct& iFlightPeriod) {
    // Create the FlightDateKey
    const stdair::FlightNumber_T& lFlightNumber = iFlightPeriod._flightNumber;
    stdair::FlightDateKey lFlightDateKey (lFlightNumber, iFlightDate);

    // Check that the flight-date object is not already existing. If a
    // FlightDate object with the same key has already been created,
    // it means that the schedule input file is invalid (two flight-periods
    // are overlapping).
    stdair::FlightDate* lFlightDate_ptr = stdair::BomManager::
      getObjectPtr<stdair::FlightDate> (ioInventory, lFlightDateKey.toString());
    if (lFlightDate_ptr != NULL) {
      std::ostringstream oMessage;
      oMessage << ioInventory.describeKey() << ", "
               << lFlightDate_ptr->describeKey();
      throw stdair::ObjectCreationgDuplicationException (oMessage.str());
    }

    // Instantiate a fligh-date object with the given key (flight number and
    // flight date)
    lFlightDate_ptr =
      &stdair::FacBom<stdair::FlightDate>::instance().create (lFlightDateKey);
    stdair::FacBomManager::
      instance().addToListAndMap (ioInventory, *lFlightDate_ptr);
    stdair::FacBomManager::
      instance().linkWithParent (ioInventory, *lFlightDate_ptr);
      
    // Iterate on the leg-dates
    stdair::Duration_T currentOffTime (0, 0, 0);
    stdair::AirportCode_T previousOffPoint;
    const LegStructList_T& lLegList = iFlightPeriod._legList;
    for (LegStructList_T::const_iterator itLeg = lLegList.begin();
         itLeg != lLegList.end(); ++itLeg) {
      const LegStruct& lLeg = *itLeg;

      // Create the leg-branch of the flight-date BOM
      stdair::LegDate& lLegDate =
        createLegDate (*lFlightDate_ptr, iFlightDate, lLeg);

      // TODO: Check that the boarding date/time of the next leg is greated
      // than the off date/time of the current leg. Throw an exception
      // otherwise.

      // TODO: specify, in the schedule input file specifications, that the
      // legs should be given in their natural order.
      // Then, replace the assertion by a thrown exception.
      //
      // Check that the legs are given in their natural order. If the schedule
      // input does not respect that assumption, the following assertion will
      // fail.
      if (itLeg != lLegList.begin()) {
        const stdair::AirportCode_T& currentBoardingPoint =
          lLegDate.getBoardingPoint();
        assert (currentBoardingPoint == previousOffPoint);
      }
        
      // Set the local variable for the next iteration
      previousOffPoint = lLegDate.getOffPoint();
    }

    // Iterate on the segment structures
    const SegmentStructList_T& lSegmentList = iFlightPeriod._segmentList;
    for (SegmentStructList_T::const_iterator itSegment = lSegmentList.begin();
         itSegment != lSegmentList.end(); ++itSegment) {
      const SegmentStruct& lSegment = *itSegment;

      createSegmentDate (*lFlightDate_ptr, lSegment);
    }
    assert (lFlightDate_ptr != NULL);

    return *lFlightDate_ptr;
  }

  // ////////////////////////////////////////////////////////////////////
  stdair::LegDate& InventoryGenerator::
  createLegDate (stdair::FlightDate& ioFlightDate,
                 const stdair::Date_T& iReferenceDate,
                 const LegStruct& iLeg) {
    // Create the leg-date corresponding to the boarding point.
    stdair::LegDateKey lKey (iLeg._boardingPoint);
    stdair::LegDate& lLegDate =
      stdair::FacBom<stdair::LegDate>::instance().create (lKey);
    stdair::FacBomManager::instance().addToListAndMap (ioFlightDate, lLegDate);
    stdair::FacBomManager::instance().linkWithParent (ioFlightDate, lLegDate);

    // Set the leg-date attributes
    iLeg.fill (iReferenceDate, lLegDate);
    
    // Iterate on the cabins
    const LegCabinStructList_T& lCabinList = iLeg._cabinList;
    for (LegCabinStructList_T::const_iterator itCabin = lCabinList.begin();
         itCabin != lCabinList.end(); ++itCabin) {
      const LegCabinStruct& lCabin = *itCabin;

      // Create the leg-cabin-branch of the leg-date 
      createLegCabin (lLegDate, lCabin);
    }

    return lLegDate;
  }

  // ////////////////////////////////////////////////////////////////////
  void InventoryGenerator::
  createLegCabin (stdair::LegDate& ioLegDate,
                  const LegCabinStruct& iCabin) {
    // Instantiate an leg-cabin object with the corresponding cabin code
    stdair::LegCabinKey lKey (iCabin._cabinCode);
    stdair::LegCabin& lLegCabin =
      stdair::FacBom<stdair::LegCabin>::instance().create (lKey);
    stdair::FacBomManager::instance().addToListAndMap (ioLegDate, lLegCabin);
    stdair::FacBomManager::instance().linkWithParent (ioLegDate, lLegCabin);
    
    // Set the Leg-Cabin attributes
    iCabin.fill (lLegCabin);
  }
    
  // ////////////////////////////////////////////////////////////////////
  void InventoryGenerator::
  createSegmentDate (stdair::FlightDate& ioFlightDate,
                     const SegmentStruct& iSegment) {
    // Set the segment-date primary key
    const stdair::AirportCode_T& lBoardingPoint = iSegment._boardingPoint;
    const stdair::AirportCode_T& lOffPoint = iSegment._offPoint;
    stdair::SegmentDateKey lSegmentDateKey (lBoardingPoint, lOffPoint);
    // Instantiate an segment-date object with the key.
    stdair::SegmentDate& lSegmentDate =
      stdair::FacBom<stdair::SegmentDate>::instance().create (lSegmentDateKey);
    stdair::FacBomManager::
      instance().addToListAndMap (ioFlightDate, lSegmentDate);
     stdair::FacBomManager::
      instance().linkWithParent (ioFlightDate, lSegmentDate);
    
    // Set the segment-date attributes
    iSegment.fill (lSegmentDate);
      
    // Iterate on the Cabins
    const SegmentCabinStructList_T& lCabinList = iSegment._cabinList;
    for (SegmentCabinStructList_T::const_iterator itCabin =
           lCabinList.begin(); itCabin != lCabinList.end(); ++itCabin) {
      const SegmentCabinStruct& lCabin = *itCabin;

      // Create the segment-cabin-branch of the segment-date BOM
      createSegmentCabin (lSegmentDate, lCabin);
    }
  }
    
  // ////////////////////////////////////////////////////////////////////
  void InventoryGenerator::
  createSegmentCabin (stdair::SegmentDate& ioSegmentDate,
                      const SegmentCabinStruct& iCabin) {
    // Instantiate an segment-cabin object with the corresponding cabin code
    stdair::SegmentCabinKey lKey (iCabin._cabinCode);
    stdair::SegmentCabin& lSegmentCabin =
      stdair::FacBom<stdair::SegmentCabin>::instance().create (lKey);
    stdair::FacBomManager::
      instance().addToListAndMap (ioSegmentDate, lSegmentCabin);
    stdair::FacBomManager::
      instance().linkWithParent (ioSegmentDate, lSegmentCabin);
    
    // Set the cegment-cabin attributes
    iCabin.fill (lSegmentCabin);

    // Iterate on the classes
    const stdair::ClassList_String_T& lClassList = iCabin._classes;
    for (stdair::ClassList_String_T::const_iterator itClass =
           lClassList.begin(); itClass != lClassList.end(); ++itClass) {
      // Transform the single-character class code into a STL string
      std::ostringstream ostr;
      ostr << *itClass;
      const stdair::ClassCode_T lClassCode (ostr.str());
      
      // Create the booking class branch of the segment-cabin BOM
      createClass (lSegmentCabin, lClassCode);
    }

    // Create the list of fare families if they exist
    if (iCabin._fareFamilies.size() > 0) {
      for (FareFamilyStructList_T::const_iterator itFareFamily =
             iCabin._fareFamilies.begin();
           itFareFamily != iCabin._fareFamilies.end(); itFareFamily++) {
         const FareFamilyStruct& lFareFamilyStruct = *itFareFamily;
         stdair::FareFamilyKey FFlKey (lFareFamilyStruct._familyCode);
         stdair::FareFamily& lFareFamily =
           stdair::FacBom<stdair::FareFamily>::instance().create (FFlKey);
         stdair::FacBomManager::
           instance().addToListAndMap (lSegmentCabin, lFareFamily);
         stdair::FacBomManager::
           instance().linkWithParent (lSegmentCabin, lFareFamily);
      }
    } 
  }
    
  // ////////////////////////////////////////////////////////////////////
  void InventoryGenerator::
  createClass (stdair::SegmentCabin& ioSegmentCabin,
               const stdair::ClassCode_T& iClassCode) {
    // Instantiate a booking class object with the given class code
    const stdair::BookingClassKey lClassKey (iClassCode);
    stdair::BookingClass& lClass =
      stdair::FacBom<stdair::BookingClass>::instance().create (lClassKey);
    stdair::FacBomManager::instance().addToListAndMap (ioSegmentCabin,lClass);
    stdair::FacBomManager::instance().linkWithParent (ioSegmentCabin,lClass);
  }

  // ////////////////////////////////////////////////////////////////////
  void InventoryGenerator::
  createSimilarFlightDateList (const stdair::FlightDate& iFlightDate,
                               const stdair::Inventory& iInventory,
                               const stdair::Date_T& iCurrentDate,
                               const stdair::Date_T& iStartDateRange) {

    // const stdair::FlightNumber_T& lFlightNumber = iFlightDate.getFlightNumber();
    // const stdair::DateOffset_T lWeek(7);
    // stdair::Date_T lSimilarDate = iCurrentDate - lWeek;
    
    // while (!(iStartDateRange > lSimilarDate)) {
    //   const stdair::FlightDateKey lFlightDateKey (lFlightNumber, lSimilarDate);
    //   stdair::FlightDate* lFlightDate_ptr = 
    //     iInventory.getFlightDate (lFlightDateKey);
      
    //   if (lFlightDate_ptr != NULL) {
    //     // Link the Flight-Date and the similar flight date
    //     // stdair::FacBomContent::linkSimilarFlightDates (iFlightDate, 
    //     // *lFlightDate_ptr);
    //   }
    //   lSimilarDate = lSimilarDate - lWeek;
    // }
  }
    
  // ////////////////////////////////////////////////////////////////////
  void InventoryGenerator::
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
  void InventoryGenerator::
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
  void InventoryGenerator::
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
          instance().addToListAndMap (*lCurrentSegmentDate_ptr,
                                                lLegDate);
        stdair::FacBomManager::instance().addToListAndMap (lLegDate,
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
  void InventoryGenerator::
  createDirectAccesses (stdair::SegmentDate& ioSegmentDate) {
    // Browse the list of segment-cabins and create direct accesses
    // within each segment-cabin.
    const stdair::SegmentCabinList_T& lSegmentCabinList = 
      stdair::BomManager::getList<stdair::SegmentCabin> (ioSegmentDate);
    for (stdair::SegmentCabinList_T::const_iterator itSegmentCabin = 
           lSegmentCabinList.begin();
         itSegmentCabin != lSegmentCabinList.end(); ++itSegmentCabin) {
      stdair::SegmentCabin* lCurrentSegmentCabin_ptr = *itSegmentCabin;
      assert (lCurrentSegmentCabin_ptr != NULL);
        const stdair::CabinCode_T& lCabinCode =
          lCurrentSegmentCabin_ptr->getCabinCode();
      stdair::MapKey_T lSegmentCabinFullKey =
        ioSegmentDate.describeKey() + ", " + lCabinCode;
      
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
        stdair::MapKey_T lLegCabinFullKey =
          lCurrentLegDate_ptr->describeKey() + ", " + lCabinCode;

        // Link the SegmentCabin and LegCabin together
        stdair::FacBomManager::
          instance().addToListAndMap (*lCurrentSegmentCabin_ptr,
                                                lLegCabin, lLegCabinFullKey);
        stdair::FacBomManager::instance().addToListAndMap (lLegCabin,
                                                *lCurrentSegmentCabin_ptr,
                                                lSegmentCabinFullKey);
      }      
    }
  }

}
