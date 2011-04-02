// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// Boost
#include <boost/date_time/date_iterator.hpp>
// StdAir
#include <stdair/stdair_basic_types.hpp>
#include <stdair/basic/BasConst_Inventory.hpp>
#include <stdair/bom/BomManager.hpp>
#include <stdair/bom/BomRoot.hpp>
#include <stdair/bom/Inventory.hpp>
#include <stdair/bom/FlightPeriod.hpp>
#include <stdair/bom/SegmentPeriod.hpp>
#include <stdair/factory/FacBomManager.hpp>
#include <stdair/service/Logger.hpp>
// AirSched
#include <airsched/bom/FlightPeriodStruct.hpp>
#include <airsched/bom/SegmentPeriodHelper.hpp>
#include <airsched/command/InventoryGenerator.hpp>

namespace AIRSCHED {
  
  // ////////////////////////////////////////////////////////////////////
  void InventoryGenerator::
  createFlightPeriod (stdair::BomRoot& ioBomRoot,
                      const FlightPeriodStruct& iFlightPeriodStruct) {
      
    const stdair::AirlineCode_T& lAirlineCode = iFlightPeriodStruct._airlineCode;
    
    // Instantiate an inventory object (if not exist)
    // for the given key (airline code)
    stdair::Inventory* lInventory_ptr = stdair::BomManager::
      getObjectPtr<stdair::Inventory> (ioBomRoot, lAirlineCode);
    if (lInventory_ptr == NULL) {
      stdair::InventoryKey lKey (lAirlineCode);

      lInventory_ptr =
        &stdair::FacBom<stdair::Inventory>::instance().create (lKey);

      //
      stdair::FacBomManager::instance().addToListAndMap (ioBomRoot,
                                                         *lInventory_ptr);
      stdair::FacBomManager::instance().linkWithParent (ioBomRoot,
                                                        *lInventory_ptr);
    }
    assert (lInventory_ptr != NULL);

    // Create the flight-period key.
    const stdair::PeriodStruct lPeriod (iFlightPeriodStruct._dateRange,
                                        iFlightPeriodStruct._dow);
    const stdair::FlightPeriodKey
      lFlightPeriodKey (iFlightPeriodStruct._flightNumber, lPeriod);
      
    // Check that the flight-period object is not already created.
    stdair::FlightPeriod* lFlightPeriod_ptr = stdair::BomManager::
      getObjectPtr<stdair::FlightPeriod> (*lInventory_ptr, 
                                          lFlightPeriodKey.toString());
    if (lFlightPeriod_ptr != NULL) {
      throw stdair::ObjectCreationgDuplicationException ("");
    }
    assert (lFlightPeriod_ptr == NULL);

    // Instantiate a flight-period object with the given key.
    lFlightPeriod_ptr = &stdair::FacBom<stdair::FlightPeriod>::
      instance().create (lFlightPeriodKey);

    //
    stdair::FacBomManager::instance().addToListAndMap (*lInventory_ptr,
                                                       *lFlightPeriod_ptr);
    stdair::FacBomManager::instance().linkWithParent (*lInventory_ptr,
                                                      *lFlightPeriod_ptr);
    
    // Create the segment-periods.
    createSegmentPeriods (*lFlightPeriod_ptr, iFlightPeriodStruct);
  }

  // ////////////////////////////////////////////////////////////////////
  void InventoryGenerator::
  createSegmentPeriods (stdair::FlightPeriod& ioFlightPeriod,
                        const FlightPeriodStruct& iFlightPeriodStruct) {

    // Iterate on the segment strutures.
    const SegmentStructList_T& lSegmentList = iFlightPeriodStruct._segmentList;
    for (SegmentStructList_T::const_iterator itSegment = lSegmentList.begin();
         itSegment != lSegmentList.end(); ++itSegment) {

      const SegmentStruct& lSegment = *itSegment;

      // Set the segment-period primary key.
      const stdair::AirportCode_T& lBoardingPoint = lSegment._boardingPoint;
      const stdair::AirportCode_T& lOffPoint = lSegment._offPoint;
      const stdair::SegmentPeriodKey lSegmentPeriodKey (lBoardingPoint,
                                                          lOffPoint);

      // Instantiate a segment-perioed with the key.
      stdair::SegmentPeriod& lSegmentPeriod = stdair::
        FacBom<stdair::SegmentPeriod>::instance().create (lSegmentPeriodKey);

      //
      stdair::FacBomManager::instance().addToListAndMap (ioFlightPeriod,
                                                         lSegmentPeriod);
      stdair::FacBomManager::instance().linkWithParent (ioFlightPeriod,
                                                        lSegmentPeriod);
      
      // Set the segment-period attributes.
      SegmentPeriodHelper::fill (lSegmentPeriod, lSegment);
      SegmentPeriodHelper::fill (lSegmentPeriod, iFlightPeriodStruct._legList);
    }
  }

}
