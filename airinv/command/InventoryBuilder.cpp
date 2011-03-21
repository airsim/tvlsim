// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// Boost
#include <boost/date_time/date_iterator.hpp>
// StdAir
#include <stdair/basic/BasConst_BookingClass.hpp>
#include <stdair/basic/BasConst_Yield.hpp>
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
#include <stdair/factory/FacBom.hpp>
#include <stdair/factory/FacBomManager.hpp>
#include <stdair/service/Logger.hpp>
// AirInv
#include <airinv/bom/FlightDateStruct.hpp>
#include <airinv/command/InventoryBuilder.hpp>

namespace AIRINV {

  // ////////////////////////////////////////////////////////////////////
  void InventoryBuilder::
  buildInventory (stdair::BomRoot& ioBomRoot,
                  const FlightDateStruct& iFlightDateStruct) {
    const stdair::AirlineCode_T& lAirlineCode = iFlightDateStruct._airlineCode;
 
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

    // Build the flight-date within the inventory.
    buildFlightDate (*lInventory_ptr, iFlightDateStruct);
  }

  // ////////////////////////////////////////////////////////////////////
  void InventoryBuilder::
  buildFlightDate (stdair::Inventory& ioInventory,
                   const FlightDateStruct& iFlightDateStruct) {
    // Create the FlightDateKey
    const stdair::FlightDateKey lFlightDateKey (iFlightDateStruct._flightNumber,
                                                iFlightDateStruct._flightDate);

    // Check that the flight-date object is not already existing. If a
    // flight-date object with the same key has already been created,
    // then just update it, ifnot, create a flight-date and update it.
    stdair::FlightDate* lFlightDate_ptr = stdair::BomManager::
      getObjectPtr<stdair::FlightDate> (ioInventory, lFlightDateKey.toString());
    if (lFlightDate_ptr == NULL) {
      // Instantiate a flighy-date object for the given key (flight number and
      // flight date)
      lFlightDate_ptr =
        &stdair::FacBom<stdair::FlightDate>::instance().create (lFlightDateKey);
      stdair::FacBomManager::instance().addToListAndMap (ioInventory, 
                                                         *lFlightDate_ptr);
      stdair::FacBomManager::instance().linkWithParent (ioInventory, 
                                                        *lFlightDate_ptr);
    }
    assert (lFlightDate_ptr != NULL);

    // Update the BOM flight-date with the attributes of the flight-date struct.
    
    // Browse the list of leg-date struct and segment-date struct and
    // create the corresponding BOM.
    for (LegStructList_T::const_iterator itLegDate =
           iFlightDateStruct._legList.begin();
         itLegDate != iFlightDateStruct._legList.end(); ++itLegDate) {
      const LegStruct& lCurrentLegDateStruct = *itLegDate;
      buildLegDate (*lFlightDate_ptr, lCurrentLegDateStruct);
    }

    for (SegmentStructList_T::const_iterator itSegmentDate =
           iFlightDateStruct._segmentList.begin();
         itSegmentDate != iFlightDateStruct._segmentList.end();
         ++itSegmentDate) {
      const SegmentStruct& lCurrentSegmentDateStruct = *itSegmentDate;
      buildSegmentDate (*lFlightDate_ptr, lCurrentSegmentDateStruct);
    }
  }

  // ////////////////////////////////////////////////////////////////////
  void InventoryBuilder::
  buildLegDate (stdair::FlightDate& ioFlightDate,
                const LegStruct& iLegDateStruct) {
    // Check that the leg-date object is not already existing. If a
    // leg-date object with the same key has already been created,
    // then just update it, ifnot, create a leg-date and update it.
    stdair::LegDate* lLegDate_ptr = stdair::BomManager::
      getObjectPtr<stdair::LegDate>(ioFlightDate, iLegDateStruct._boardingPoint);

    if (lLegDate_ptr == NULL) {
      // Instantiate a leg-date object for the given key (boarding point);
      stdair::LegDateKey lKey (iLegDateStruct._boardingPoint);
      lLegDate_ptr = &stdair::FacBom<stdair::LegDate>::instance().create (lKey);
      stdair::FacBomManager::instance().addToListAndMap (ioFlightDate, 
                                                         *lLegDate_ptr);
      stdair::FacBomManager::instance().linkWithParent (ioFlightDate, 
                                                        *lLegDate_ptr);
    }
    assert (lLegDate_ptr != NULL);

    // Update the BOM leg-date with the attributes of the leg-date struct.
    iLegDateStruct.fill (*lLegDate_ptr);

    // Browse the list of leg-cabin structs and create the corresponding BOM.
    for (LegCabinStructList_T::const_iterator itLegCabin =
           iLegDateStruct._cabinList.begin();
         itLegCabin != iLegDateStruct._cabinList.end(); ++itLegCabin) {
      const LegCabinStruct& lCurrentLegCabinStruct = *itLegCabin;
      buildLegCabin (*lLegDate_ptr, lCurrentLegCabinStruct);
    }
  }

  // ////////////////////////////////////////////////////////////////////
  void InventoryBuilder::
  buildLegCabin (stdair::LegDate& ioLegDate,
                 const LegCabinStruct& iLegCabinStruct) {
    // Check that the leg-cabin object is not already existing. If a
    // leg-cabin object with the same key has already been created,
    // then just update it, ifnot, create a leg-cabin and update it.
    stdair::LegCabin* lLegCabin_ptr = stdair::BomManager::
      getObjectPtr<stdair::LegCabin> (ioLegDate, iLegCabinStruct._cabinCode);
    if (lLegCabin_ptr == NULL) {
      // Instantiate a leg-cabin object for the given key (cabin code);
      stdair::LegCabinKey lKey (iLegCabinStruct._cabinCode);
      lLegCabin_ptr = &stdair::FacBom<stdair::LegCabin>::instance().create(lKey);
      stdair::FacBomManager::instance().addToListAndMap (ioLegDate, 
                                                         *lLegCabin_ptr);
      stdair::FacBomManager::instance().linkWithParent (ioLegDate, 
                                                        *lLegCabin_ptr);
    }
    assert (lLegCabin_ptr != NULL);

    // TODO: Update the BOM leg-cabin with the attributes of the
    // leg-cabin struct.
    iLegCabinStruct.fill (*lLegCabin_ptr);

    // Browse the list of bucket structs and create the corresponding BOM.
    for (BucketStructList_T::const_iterator itBucket =
           iLegCabinStruct._bucketList.begin();
         itBucket != iLegCabinStruct._bucketList.end(); ++itBucket) {
      const BucketStruct& lCurrentBucketStruct = *itBucket;
      buildBucket (*lLegCabin_ptr, lCurrentBucketStruct);
    }
  }

  // ////////////////////////////////////////////////////////////////////
  void InventoryBuilder::buildBucket (stdair::LegCabin& ioLegCabin,
                                      const BucketStruct& iBucketStruct) {
    // Create the BucketKey
    const stdair::BucketKey lBucketKey (iBucketStruct._seatIndex);

    // Check that the bucket object is not already existing. If a
    // bucket object with the same key has already been created,
    // then just update it, ifnot, create a bucket and update it.
    stdair::Bucket* lBucket_ptr = stdair::BomManager::
      getObjectPtr<stdair::Bucket> (ioLegCabin, lBucketKey.toString());
    if (lBucket_ptr == NULL) {
      // Instantiate a bucket object for the given key (seat index);
      stdair::BucketKey lKey (iBucketStruct._seatIndex);
      lBucket_ptr = &stdair::FacBom<stdair::Bucket>::instance().create (lKey);
      stdair::FacBomManager::instance().addToListAndMap (ioLegCabin,
                                                         *lBucket_ptr);
      stdair::FacBomManager::instance().linkWithParent (ioLegCabin, 
                                                        *lBucket_ptr);
    }
    assert (lBucket_ptr != NULL);

    //
    iBucketStruct.fill (*lBucket_ptr);
  }

  // ////////////////////////////////////////////////////////////////////
  void InventoryBuilder::
  buildSegmentDate (stdair::FlightDate& ioFlightDate,
                    const SegmentStruct& iSegmentDateStruct) {
    // Check that the segment-date object is not already existing. If a
    // segment-date object with the same key has already been created,
    // then just update it, ifnot, create a segment-date and update it.
    const stdair::SegmentDateKey
      lSegmentDateKey (iSegmentDateStruct._boardingPoint,
                       iSegmentDateStruct._offPoint);
    stdair::SegmentDate* lSegmentDate_ptr = stdair::BomManager::
      getObjectPtr<stdair::SegmentDate>(ioFlightDate,lSegmentDateKey.toString());
    if (lSegmentDate_ptr == NULL) {
      // Instantiate a segment-date object for the given key (boarding
      // and off points);
      lSegmentDate_ptr =  &stdair::FacBom<stdair::SegmentDate>::
        instance().create (lSegmentDateKey);
      stdair::FacBomManager::instance().addToListAndMap (ioFlightDate, 
                                                         *lSegmentDate_ptr);
      stdair::FacBomManager::instance().linkWithParent (ioFlightDate,
                                                        *lSegmentDate_ptr);
    }
    assert (lSegmentDate_ptr != NULL);

    // Update the BOM segment-date with the attributes of the
    // segment-date struct.
    iSegmentDateStruct.fill (*lSegmentDate_ptr);

    // Browse the list of segment-cabin struct and create the corresponding BOM.
    for (SegmentCabinStructList_T::const_iterator itSegmentCabin =
           iSegmentDateStruct._cabinList.begin();
         itSegmentCabin != iSegmentDateStruct._cabinList.end(); 
         ++itSegmentCabin) {
      const SegmentCabinStruct& lCurrentSegmentCabinStruct = *itSegmentCabin;
      buildSegmentCabin (*lSegmentDate_ptr, lCurrentSegmentCabinStruct);
    }
  }

  // ////////////////////////////////////////////////////////////////////
  void InventoryBuilder::
  buildSegmentCabin (stdair::SegmentDate& ioSegmentDate,
                     const SegmentCabinStruct& iSegmentCabinStruct) {
    // Check that the segment-cabin object is not already existing. If a
    // segment-cabin object with the same key has already been created,
    // then just update it, ifnot, create a segment-cabin and update it.
    stdair::SegmentCabin* lSegmentCabin_ptr = stdair::BomManager::
      getObjectPtr<stdair::SegmentCabin> (ioSegmentDate,
                                          iSegmentCabinStruct._cabinCode);
    if (lSegmentCabin_ptr == NULL) {
      // Instantiate a segment-cabin object for the given key (cabin code);
      stdair::SegmentCabinKey lKey (iSegmentCabinStruct._cabinCode);
      lSegmentCabin_ptr = 
        &stdair::FacBom<stdair::SegmentCabin>::instance().create (lKey);

      // Link the segment-cabin to the segment-date
      stdair::FacBomManager::instance().addToListAndMap (ioSegmentDate, 
                                                         *lSegmentCabin_ptr);
      stdair::FacBomManager::instance().linkWithParent (ioSegmentDate, 
                                                        *lSegmentCabin_ptr);
    }
    assert (lSegmentCabin_ptr != NULL);

    // TODO: Update the BOM segment-cabin with the attributes of the
    // segment-cabin struct.
    iSegmentCabinStruct.fill (*lSegmentCabin_ptr);

    // Browse the list of fare family struct and create the corresponding BOM.
    for (FareFamilyStructList_T::const_iterator itFareFamily =
           iSegmentCabinStruct._fareFamilies.begin();
         itFareFamily != iSegmentCabinStruct._fareFamilies.end(); 
         ++itFareFamily) {
      const FareFamilyStruct& lCurrentFareFamilyStruct = *itFareFamily;
      buildFareFamily (*lSegmentCabin_ptr, lCurrentFareFamilyStruct);
    }
  }

  // ////////////////////////////////////////////////////////////////////
  void InventoryBuilder::
  buildFareFamily (stdair::SegmentCabin& ioSegmentCabin,
                   const FareFamilyStruct& iFareFamilyStruct) {

    // Check that the fare family object is not already existing. If a
    // fare family object with the same key has already been created,
    // then just update it. If not, create a fare family and update it.
    stdair::FareFamily* lFareFamily_ptr = stdair::BomManager::
      getObjectPtr<stdair::FareFamily> (ioSegmentCabin,
                                        iFareFamilyStruct._familyCode);
    if (lFareFamily_ptr == NULL) {
      // Instantiate a fare family object for the given key (fare family code);
      const stdair::FareFamilyKey lFFKey (iFareFamilyStruct._familyCode);
      lFareFamily_ptr = 
        &stdair::FacBom<stdair::FareFamily>::instance().create (lFFKey);

      // Link the fare family to the segment-cabin
      stdair::FacBomManager::instance().addToListAndMap (ioSegmentCabin,
                                                         *lFareFamily_ptr);
      stdair::FacBomManager::instance().linkWithParent (ioSegmentCabin,
                                                        *lFareFamily_ptr);
    }
    assert (lFareFamily_ptr != NULL);

    // TODO: Upcabin the BOM fare family with the attributes of the
    // fare family struct.
    iFareFamilyStruct.fill (*lFareFamily_ptr);

    // Browse the list of booking-class struct and create the corresponding BOM.
    for (BookingClassStructList_T::const_iterator itBookingClass =
           iFareFamilyStruct._classList.begin();
         itBookingClass != iFareFamilyStruct._classList.end(); 
         ++itBookingClass) {
      const BookingClassStruct& lCurrentBookingClassStruct = *itBookingClass;
      buildBookingClass (*lFareFamily_ptr, lCurrentBookingClassStruct);
    }
  }

  // ////////////////////////////////////////////////////////////////////
  void InventoryBuilder::
  buildBookingClass (stdair::FareFamily& ioFareFamily,
                     const BookingClassStruct& iBookingClassStruct) {

    // Check that the booking class object is not already existing. If a
    // booking-class object with the same key has already been created,
    // then just update it. If not, create a booking-class and update it.
    stdair::BookingClass* lBookingClass_ptr = stdair::BomManager::
      getObjectPtr<stdair::BookingClass> (ioFareFamily,
                                          iBookingClassStruct._classCode);
    if (lBookingClass_ptr == NULL) {
      // Instantiate a booking class object for the given key (class code);
      const stdair::BookingClassKey lClassKey (iBookingClassStruct._classCode);
      lBookingClass_ptr = 
        &stdair::FacBom<stdair::BookingClass>::instance().create (lClassKey);

      // Link the booking-class to the fare family
      stdair::FacBomManager::instance().addToListAndMap (ioFareFamily,
                                                         *lBookingClass_ptr);
      stdair::FacBomManager::instance().linkWithParent (ioFareFamily,
                                                        *lBookingClass_ptr);

      // Link the booking-class to the segment-cabin
      stdair::SegmentCabin& lSegmentCabin =
        stdair::BomManager::getParent<stdair::SegmentCabin> (ioFareFamily);
      stdair::FacBomManager::instance().addToListAndMap (lSegmentCabin,
                                                         *lBookingClass_ptr);

      // Link the booking-class to the segment-date
      stdair::SegmentDate& lSegmentDate =
        stdair::BomManager::getParent<stdair::SegmentDate> (lSegmentCabin);
      stdair::FacBomManager::instance().addToListAndMap (lSegmentDate,
                                                         *lBookingClass_ptr);
    }
    assert (lBookingClass_ptr != NULL);

    // TODO: Upcabin the BOM booking-class with the attributes of the
    // booking-class struct.
    iBookingClassStruct.fill (*lBookingClass_ptr);
  }
  
}
