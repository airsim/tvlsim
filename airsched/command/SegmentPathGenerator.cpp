// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <vector>
// STDAIR
#include <stdair/basic/BasConst_Inventory.hpp>
#include <stdair/bom/BomManager.hpp>
#include <stdair/bom/BomRoot.hpp>
#include <stdair/bom/Inventory.hpp>
#include <stdair/bom/FlightPeriod.hpp>
#include <stdair/bom/SegmentPeriod.hpp>
#include <stdair/factory/FacBomManager.hpp>
#include <stdair/service/Logger.hpp>
// AirSched
#include <airsched/bom/ReachableUniverse.hpp>
#include <airsched/bom/OriginDestinationSet.hpp>
#include <airsched/bom/SegmentPathPeriod.hpp>
#include <airsched/command/SegmentPathGenerator.hpp>

namespace AIRSCHED {

  // ////////////////////////////////////////////////////////////////////
  void SegmentPathGenerator::
  createSegmentPathNetwork (const stdair::BomRoot& iBomRoot) {
    // Build the list of single-segment segment path objects.
    const stdair::InventoryList_T& lInventoryList =
      stdair::BomManager::getList<stdair::Inventory> (iBomRoot);
    for (stdair::InventoryList_T::const_iterator itInv = lInventoryList.begin();
         itInv != lInventoryList.end(); ++itInv) {
      const stdair::Inventory* lCurrentInventory_ptr = *itInv;
      assert (lCurrentInventory_ptr != NULL);
      createSinglePaths (*lCurrentInventory_ptr);
    }

    // Build the list of i-fixed-length segment path objects. In other words,
    // build the whole segment path network.
    for (stdair::NbOfSegments_T i = 2;
         i <= stdair::MAXIMAL_NUMBER_OF_SEGMENTS_IN_OND; ++i) {
      buildSegmentPathNetwork (iBomRoot, i);
    }
  }
  
  // ////////////////////////////////////////////////////////////////////
  void SegmentPathGenerator::
  createSinglePaths (const stdair::Inventory& iInventory) {
    const stdair::FlightPeriodList_T& lFlightPeriodList =
      stdair::BomManager::getList<stdair::FlightPeriod> (iInventory);
    for (stdair::FlightPeriodList_T::const_iterator itFlightPeriod =
           lFlightPeriodList.begin();
         itFlightPeriod != lFlightPeriodList.end(); ++itFlightPeriod) {
      const stdair::FlightPeriod* lCurrentFlightPeriod_ptr = *itFlightPeriod;
      assert (lCurrentFlightPeriod_ptr != NULL);
      createSinglePaths (*lCurrentFlightPeriod_ptr);
    }
  }

  // ////////////////////////////////////////////////////////////////////
  void SegmentPathGenerator::
  createSinglePaths (const stdair::FlightPeriod& iFlightPeriod) {
    const stdair::SegmentPeriodList_T& lSegmentPeriodList =
      stdair::BomManager::getList<stdair::SegmentPeriod> (iFlightPeriod);
    for (stdair::SegmentPeriodList_T::const_iterator itSegmentPeriod =
           lSegmentPeriodList.begin();
         itSegmentPeriod != lSegmentPeriodList.end(); ++itSegmentPeriod) {
      stdair::SegmentPeriod* lCurrentSegmentPeriod_ptr = *itSegmentPeriod;
      assert (lCurrentSegmentPeriod_ptr != NULL);
      createSinglePath (*lCurrentSegmentPeriod_ptr);
    }
  }

  // ////////////////////////////////////////////////////////////////////
  void SegmentPathGenerator::
  createSinglePath (stdair::SegmentPeriod& ioSegmentPeriod) {
    const stdair::AirportCode_T& lOrigin = ioSegmentPeriod.getBoardingPoint ();
    const stdair::FlightPeriod& lFlightPeriod =
      stdair::BomManager::getParent<stdair::FlightPeriod> (ioSegmentPeriod);
    const stdair::Inventory& lInventory =
      stdair::BomManager::getParent<stdair::Inventory> (lFlightPeriod);
    stdair::BomRoot& lBomRoot =
      stdair::BomManager::getParent<stdair::BomRoot> (lInventory);      

    // Retrieve the ReachableUniverse (if existed) which corresponds
    // to the origin. If it does not exist, then create one.
    ReachableUniverse* lReachableUniverse_ptr =
      stdair::BomManager::getObjectPtr<ReachableUniverse> (lBomRoot, lOrigin);
    if (lReachableUniverse_ptr == NULL) {
      ReachableUniverseKey lKey (lOrigin);
      lReachableUniverse_ptr =
        &stdair::FacBom<ReachableUniverse>::instance().create (lKey);
      stdair::FacBomManager::
        instance().addToListAndMap (lBomRoot, *lReachableUniverse_ptr);
      stdair::FacBomManager::
        instance().linkWithParent (lBomRoot, *lReachableUniverse_ptr);
    }
    assert (lReachableUniverse_ptr != NULL);
    createSinglePath (*lReachableUniverse_ptr, ioSegmentPeriod);
  }

  // ////////////////////////////////////////////////////////////////////
  void SegmentPathGenerator::
  createSinglePath (ReachableUniverse& ioReachableUniverse,
                    stdair::SegmentPeriod& ioSegmentPeriod) {
    const stdair::AirportCode_T& lDestination = ioSegmentPeriod.getOffPoint ();

    // Retrieve the origin-destination set (if existed) which corresponds
    // to the destination. If it does not exist, then create one.
    OriginDestinationSet* lOriginDestinationSet_ptr =
      stdair::BomManager::getObjectPtr<OriginDestinationSet>(ioReachableUniverse,
                                                             lDestination);
    if (lOriginDestinationSet_ptr == NULL) {
      OriginDestinationSetKey lKey (lDestination);
      lOriginDestinationSet_ptr =
        &stdair::FacBom<OriginDestinationSet>::instance().create (lKey);
      stdair::FacBomManager::instance().addToListAndMap (ioReachableUniverse,
                                              *lOriginDestinationSet_ptr);
      stdair::FacBomManager::instance().linkWithParent (ioReachableUniverse,
                                             *lOriginDestinationSet_ptr);
    }
    assert (lOriginDestinationSet_ptr != NULL);
    
    // Create a segment path period and add it to the corresponding
    // origin-destination set and reachable-universe.
    const stdair::FlightPeriod& lFlightPeriod =
      stdair::BomManager::getParent<stdair::FlightPeriod> (ioSegmentPeriod);
    const stdair::PeriodStruct& lPeriodOfFlight = lFlightPeriod.getPeriod();
    // The departure period of the segment is the departure period of
    // the flight plus the boarding date offset of the segment.
    const stdair::DateOffset_T& lBoardingDateOffset = 
      ioSegmentPeriod.getBoardingDateOffset();
    const stdair::PeriodStruct lPeriodOfSegment =
      lPeriodOfFlight.addDateOffset (lBoardingDateOffset);
    const stdair::Duration_T& lBoardingTime = ioSegmentPeriod.getBoardingTime();
    const stdair::Duration_T& lElapsed = ioSegmentPeriod.getElapsedTime();
    DateOffsetList_T lDateOffsetList;
    const stdair::DateOffset_T lFirstDateOffset (0);
    lDateOffsetList.push_back (lFirstDateOffset);
    const SegmentPathPeriodKey lSegmentPathKey (lPeriodOfSegment,
                                                lBoardingTime, lElapsed,
                                                lDateOffsetList, 1);
    SegmentPathPeriod& lSegmentPathPeriod =
      stdair::FacBom<SegmentPathPeriod>::instance().create (lSegmentPathKey);
    stdair::FacBomManager::
      instance().addToList (*lOriginDestinationSet_ptr, lSegmentPathPeriod);
    stdair::FacBomManager::
      instance().linkWithParent (*lOriginDestinationSet_ptr, lSegmentPathPeriod);
    stdair::FacBomManager::
      instance().addToList (lSegmentPathPeriod, ioSegmentPeriod);
    ioReachableUniverse.addSegmentPathPeriod (lSegmentPathPeriod);
  }

  // ////////////////////////////////////////////////////////////////////
  void SegmentPathGenerator::
  buildSegmentPathNetwork (const stdair::BomRoot& iBomRoot,
                           const stdair::NbOfSegments_T& lNbOfSegments) {
    // Browse the reachable-universe list and create the i-fixed-length
    // segment path period objects corresponding to the boarding point of
    // the current reachable-universe object.
    const ReachableUniverseList_T& lReachableUniverseList =
      stdair::BomManager::getList<ReachableUniverse> (iBomRoot);
    for (ReachableUniverseList_T::const_iterator itReachableUniverse =
           lReachableUniverseList.begin();
         itReachableUniverse != lReachableUniverseList.end();
         ++itReachableUniverse) {
      ReachableUniverse* lReachableUniverse_ptr = *itReachableUniverse;
      assert (lReachableUniverse_ptr != NULL);
      buildSegmentPathNetwork (*lReachableUniverse_ptr, lNbOfSegments);
    }
  }

  // ////////////////////////////////////////////////////////////////////
  void SegmentPathGenerator::
  buildSegmentPathNetwork (ReachableUniverse& ioReachableUniverse,
                           const stdair::NbOfSegments_T& iNbOfSegments) {
    
    // The goal of that method is to build the i-fixed-length
    // segment path period objects, knowing that all the
    // lower-fixed-length segment path period objects have already been
    // built during the previous steps. Once an i-fixed-length
    // segment path period object is created, it is added to the list of
    // the (fixed-length segment path period object) lists.
    
    // Hence, at that iteration, by construction, the list of the
    // (fixed-length segment path period object) lists should already get
    // a size of i-1, if there were such possibilities (in terms of
    // segment path period). In that case, at the end of the method, its
    // size should be of i.
    
    // If the size of the list of the (fixed-length segment path period
    // object) lists is (strictly) less than i-1, it means that that
    // reachable universe has no more possibilities of destinations. We
    // are thus done at that stage.
    const SegmentPathPeriodListList_T& lSegmentPathPeriodListList =
      ioReachableUniverse.getSegmentPathPeriodListList();
    const unsigned short lNbOfSegments_m1 = iNbOfSegments - 1;
    assert (lNbOfSegments_m1 >= 0);
    if (lSegmentPathPeriodListList.size() < lNbOfSegments_m1) {
      return;
    }
      
    // Retrieve the (i-1)-fixed-length segment path period list (i = number of
    // segments).
    
    // Note that a STL vector starts at 0, whereas the number of segments
    // starts at 1. Hence, (i-1) for the length (in number of segments)
    // corresponds to [iNbOfSegments-2] for the STL vector.
    
    // As the lSegmentPathPeriodListList may change during the next loop
    // iterations (as some SegmentPathPeriod objects are created and linked to
    // ReachableUniverse), we need to take the initial copy of that list.
    const SegmentPathPeriodLightList_T lSegmentPathPeriodLightList_im1 =
      lSegmentPathPeriodListList.at (iNbOfSegments-2);

    // Iterate on the (i-1)-fixed-length segment path period objects, in order
    // to build a i-fixed-length segment path period objects. 
    // There are two steps:
    // 1. Retrieve the airport-dates at a (i-1) length (in number of segments)
    //    of the origin airport-date.
    // 2. From each of such (i-1) airport-date, add the single-segment pathes
    //    to the (i-1)-length pathes, so as to make i-length pathes.
    for (SegmentPathPeriodLightList_T::const_iterator itSegmentPathPeriodList =
           lSegmentPathPeriodLightList_im1.begin();
         itSegmentPathPeriodList != lSegmentPathPeriodLightList_im1.end();
         ++itSegmentPathPeriodList) {
      const SegmentPathPeriod* lSegmentPathPeriod_im1_ptr = 
        *itSegmentPathPeriodList;
      assert (lSegmentPathPeriod_im1_ptr != NULL);

      // Get the reachable-universe departing from the destination of
      // the current segment path period.
      const stdair::AirportCode_T& lDestination_im1 =
        lSegmentPathPeriod_im1_ptr->getDestination();
      const stdair::BomRoot& lBomRoot =
        stdair::BomManager::getParent<stdair::BomRoot> (ioReachableUniverse);
      const ReachableUniverse* lReachableUniverseFromDestination_im1_ptr =
        stdair::BomManager::getObjectPtr<ReachableUniverse> (lBomRoot,
                                                            lDestination_im1);

      // If there is no ReachableUniverse corresponding to the destination (off
      // point of the last SegmentDate), it means that the destination is
      // an end point (no other SegmentDate is starting from there).
      // Hence, there is nothing else to do for now for that (final)
      // destination, and we can process the next (i-1)-segment path period.
      if (lReachableUniverseFromDestination_im1_ptr == NULL) {
        continue;
      }
      assert (lReachableUniverseFromDestination_im1_ptr != NULL);
        
      // Retrieve the single-segment segment path period list,
      // so as to make a i-length SegmentPathPeriod.
      const SegmentPathPeriodListList_T& 
        lSegmentPathPeriodListListFromDestination_im1 = 
        lReachableUniverseFromDestination_im1_ptr->
        getSegmentPathPeriodListList();
      assert (lSegmentPathPeriodListListFromDestination_im1.size() >= 1);

      // As the lSegmentPathPeriodListListFromDestination_im1 may change during
      // the next loop iterations (as some SegmentPathPeriod objects are
      // created and linked to ReachableUniverse), we need to take the initial
      // copy of that list.
      const SegmentPathPeriodLightList_T lSingleSegmentPathPeriodLightListFromDestination_im1 =
        lSegmentPathPeriodListListFromDestination_im1.at (0);

      for (SegmentPathPeriodLightList_T::const_iterator
             itSegmentPathPeriodFromDestination_im1 =
             lSingleSegmentPathPeriodLightListFromDestination_im1.begin();
           itSegmentPathPeriodFromDestination_im1
             != lSingleSegmentPathPeriodLightListFromDestination_im1.end();
           ++itSegmentPathPeriodFromDestination_im1) {
        const SegmentPathPeriod* lSingleSegmentPathPeriodFromDestination_im1_ptr=
          *itSegmentPathPeriodFromDestination_im1;
        assert (lSingleSegmentPathPeriodFromDestination_im1_ptr != NULL);

        // Check if the (i-1)-length segment path period can be fused with the
        // single segment segment path period in order to create an i-length
        // segment path period. The function will return a valid or non-valid
        // segment path period key.

        // The two segment path period above can be fused (and will produce a
        // valid new segment path period key) if:
        // 1. A passenger can connect from the last segment of the
        // first segment path and the first segment of the next segment path.
        // These two segments should not create another segment.
        // 2. There is no circle within the new segment path.
        // 3. The intersection of the two periods is non-empty.
        SegmentPathPeriodKey lSegmentPathPeriodKey_i =
          lSegmentPathPeriod_im1_ptr->connectWithAnotherSegment (*lSingleSegmentPathPeriodFromDestination_im1_ptr);

        if (lSegmentPathPeriodKey_i.isValid () == false) {
          continue;
        }
          
        // Get the off point of the single-segment SegmentPathPeriod
        // attached to the intermediate destination (im1). That off point is
        // at a length i of the initial ReachableUniverse: (i-1) + 1.
        const stdair::AirportCode_T& lDestination_i =
          lSingleSegmentPathPeriodFromDestination_im1_ptr->getDestination();

        // Build the i-length SegmentPathPeriod
        // Get the parameters of the last segment
	stdair::SegmentPeriod* lSegmentPeriod_1_ptr =
          lSingleSegmentPathPeriodFromDestination_im1_ptr->getFirstSegmentPeriod();
        assert (lSegmentPeriod_1_ptr != NULL);
          
        // Calculate the number of airlines flown by the i-length
        // segment path period
        const stdair::FlightPeriod& lFlightPeriod = stdair::BomManager::
          getParent<stdair::FlightPeriod> (*lSegmentPeriod_1_ptr);
        const stdair::Inventory& lInventory =
          stdair::BomManager::getParent<stdair::Inventory> (lFlightPeriod);
        const stdair::AirlineCode_T& lAirlineCode_1 =lInventory.getAirlineCode();
        stdair::NbOfAirlines_T lNbOfAirlines_i =
          lSegmentPathPeriod_im1_ptr->getNbOfAirlines();
        if (lSegmentPathPeriod_im1_ptr->isAirlineFlown(lAirlineCode_1) == false){
          ++lNbOfAirlines_i;
        }
        lSegmentPathPeriodKey_i.setNbOfAirlines (lNbOfAirlines_i);

        // Create the new segment path and add it to the dedicated lists.
        OriginDestinationSet* lOriginDestinationSet_ptr = stdair::BomManager::
          getObjectPtr<OriginDestinationSet>(ioReachableUniverse,lDestination_i);
        if (lOriginDestinationSet_ptr == NULL) {
          OriginDestinationSetKey lKey (lDestination_i);
          lOriginDestinationSet_ptr = 
            &stdair::FacBom<OriginDestinationSet>::instance().create (lKey);
          stdair::FacBomManager::instance().addToListAndMap (ioReachableUniverse,
                                                  *lOriginDestinationSet_ptr);
          stdair::FacBomManager::instance().linkWithParent (ioReachableUniverse,
                                                 *lOriginDestinationSet_ptr);
        }
        assert (lOriginDestinationSet_ptr != NULL);
        
        
        SegmentPathPeriod& lSegmentPathPeriod_i = stdair::
          FacBom<SegmentPathPeriod>::instance().create (lSegmentPathPeriodKey_i);
        stdair::FacBomManager::instance().addToList (*lOriginDestinationSet_ptr,
                                                     lSegmentPathPeriod_i);
        stdair::FacBomManager::
          instance().linkWithParent (*lOriginDestinationSet_ptr,
                                     lSegmentPathPeriod_i);

        // Clone the list of SegmentPeriod references of the given
        // SegmentPathPeriod object (passed as the second parameter).
        stdair::FacBomManager::instance().
          cloneHolder<stdair::SegmentPeriod> (lSegmentPathPeriod_i,
                                              *lSegmentPathPeriod_im1_ptr);
       
        
        // Add the SegmentPeriod reference to the dedicated list within
        // the SegmentPathPeriod. Note that this must be done before
        // the link between the SegmentPathPeriod and
        // ReachableUniverse, as that latter method uses the number of
        // segments within the SegmentPathPeriod object.
        stdair::FacBomManager::instance().addToList (lSegmentPathPeriod_i,
                                                     *lSegmentPeriod_1_ptr);
        
        // Link the SegmentPathPeriod to the ReachableUniverse
        ioReachableUniverse.addSegmentPathPeriod (lSegmentPathPeriod_i);
      }
     }
  }
}
