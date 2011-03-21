// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// Boost
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
#include <stdair/bom/Bucket.hpp>
#include <stdair/factory/FacBomManager.hpp>
#include <stdair/service/Logger.hpp>
// AirInv
#include <airinv/bom/FlightPeriodStruct.hpp>
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

    // DEBUG
    // STDAIR_LOG_DEBUG ("Creating flight-date: " << lFlightDateKey.toString());
    
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
    const stdair::LegCabinKey lKey (iCabin._cabinCode);
    stdair::LegCabin& lLegCabin =
      stdair::FacBom<stdair::LegCabin>::instance().create (lKey);
    stdair::FacBomManager::instance().addToListAndMap (ioLegDate, lLegCabin);
    stdair::FacBomManager::instance().linkWithParent (ioLegDate, lLegCabin);

    // Set the Leg-Cabin attributes
    iCabin.fill (lLegCabin);

    // Iterate on the bucket
    const BucketStructList_T& lBucketList = iCabin._bucketList;
    for (BucketStructList_T::const_iterator itBucket = lBucketList.begin();
         itBucket != lBucketList.end(); ++itBucket) {
      const BucketStruct& lBucket = *itBucket;

      // Create the bucket of the leg-cabin
      createBucket (lLegCabin, lBucket);
    }
  }
    
  // ////////////////////////////////////////////////////////////////////
  void InventoryGenerator::createBucket (stdair::LegCabin& ioLegCabin,
                                         const BucketStruct& iBucket) {
    // Instantiate a bucket object with the corresponding seat index
    const stdair::BucketKey lKey (iBucket._seatIndex);
    stdair::Bucket& lBucket =
      stdair::FacBom<stdair::Bucket>::instance().create (lKey);
    stdair::FacBomManager::instance().addToListAndMap (ioLegCabin, lBucket);
    stdair::FacBomManager::instance().linkWithParent (ioLegCabin, lBucket);

    // Set the Bucket attributes
    iBucket.fill (lBucket);
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

    // Link the segment-cabin to its parent, the segment-date
    stdair::FacBomManager::instance().addToListAndMap (ioSegmentDate,
                                                       lSegmentCabin);
    stdair::FacBomManager::instance().linkWithParent (ioSegmentDate,
                                                      lSegmentCabin);
    
    // Set the segment-cabin attributes
    iCabin.fill (lSegmentCabin);

    // Create the list of fare families
    for (FareFamilyStructList_T::const_iterator itFareFamily =
           iCabin._fareFamilies.begin();
         itFareFamily != iCabin._fareFamilies.end(); itFareFamily++) {
      const FareFamilyStruct& lFareFamilyStruct = *itFareFamily;

      //
      createFareFamily (lSegmentCabin, lFareFamilyStruct);
    } 
  }
    
  // ////////////////////////////////////////////////////////////////////
  void InventoryGenerator::
  createFareFamily (stdair::SegmentCabin& ioSegmentCabin,
                    const FareFamilyStruct& iFF) {
    // Instantiate an segment-cabin object with the corresponding cabin code
    stdair::FareFamilyKey lKey (iFF._familyCode);
    stdair::FareFamily& lFareFamily =
      stdair::FacBom<stdair::FareFamily>::instance().create (lKey);

    // Link the fare family to its parent, the segment-cabin
    stdair::FacBomManager::instance().addToListAndMap (ioSegmentCabin,
                                                       lFareFamily);
    stdair::FacBomManager::instance().linkWithParent (ioSegmentCabin,
                                                      lFareFamily);
    
    // Set the fare family attributes
    iFF.fill (lFareFamily);

    // Iterate on the classes
    const stdair::ClassList_String_T& lClassList = iFF._classes;
    for (stdair::ClassList_String_T::const_iterator itClass =
           lClassList.begin(); itClass != lClassList.end(); ++itClass) {
      // Transform the single-character class code into a STL string
      std::ostringstream ostr;
      ostr << *itClass;
      const stdair::ClassCode_T lClassCode (ostr.str());
      
      // Create the booking class branch of the segment-cabin BOM
      createClass (lFareFamily, lClassCode);
    }
  }

  // ////////////////////////////////////////////////////////////////////
  void InventoryGenerator::createClass (stdair::FareFamily& ioFareFamily,
                                        const stdair::ClassCode_T& iClassCode) {

    // Instantiate a booking class object with the given class code
    const stdair::BookingClassKey lClassKey (iClassCode);
    stdair::BookingClass& lClass =
      stdair::FacBom<stdair::BookingClass>::instance().create (lClassKey);

    // Link the booking-class to the fare family
    stdair::FacBomManager::instance().addToListAndMap (ioFareFamily, lClass);
    stdair::FacBomManager::instance().linkWithParent (ioFareFamily, lClass);

    // Link the booking-class to the segment-cabin
    stdair::SegmentCabin& lSegmentCabin =
      stdair::BomManager::getParent<stdair::SegmentCabin> (ioFareFamily);
    stdair::FacBomManager::instance().addToListAndMap (lSegmentCabin, lClass);

    // Link the booking-class to the segment-date
    stdair::SegmentDate& lSegmentDate =
      stdair::BomManager::getParent<stdair::SegmentDate> (lSegmentCabin);
    stdair::FacBomManager::instance().addToListAndMap (lSegmentDate, lClass);
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
}
