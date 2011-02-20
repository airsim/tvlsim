// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// StdAir
#include <stdair/bom/BomManager.hpp>
#include <stdair/bom/EventStruct.hpp>
#include <stdair/bom/BomRoot.hpp>
#include <stdair/bom/BookingRequestStruct.hpp>
#include <stdair/bom/EventQueue.hpp>
#include <stdair/factory/FacBomManager.hpp>
#include <stdair/service/Logger.hpp>
// TraDemGen
#include <trademgen/basic/DemandCharacteristics.hpp>
#include <trademgen/basic/DemandDistribution.hpp>
#include <trademgen/bom/DemandStruct.hpp>
#include <trademgen/bom/DemandStream.hpp>
#include <trademgen/factory/FacDemandStream.hpp>
#include <trademgen/command/DemandManager.hpp>

namespace TRADEMGEN {

  // //////////////////////////////////////////////////////////////////////
  stdair::EventQueue& DemandManager::
  initEventQueue (stdair::BomRoot& ioBomRoot) {
    
    // The event queue key is a just a string. For now, it is not used.
    const stdair::EventQueueKey lKey ("EQ01");

    // Create an EventQueue object instance
    stdair::EventQueue& oEventQueue =
      stdair::FacBom<stdair::EventQueue>::instance().create (lKey);

    // Link the EventQueue to its parent (BomRoot)
    stdair::FacBomManager::instance().linkWithParent (ioBomRoot, oEventQueue);
    
    // Add the EventQueue to the dedicated list
    stdair::FacBomManager::instance().addToList (ioBomRoot, oEventQueue);

    //
    return oEventQueue;
  }

  // //////////////////////////////////////////////////////////////////////
  stdair::EventQueue& DemandManager::
  getEventQueue (const stdair::BomRoot& iBomRoot) {

    //
    const stdair::EventQueueList_T& lEventQueueList =
      stdair::BomManager::getList<stdair::EventQueue> (iBomRoot);
    assert (lEventQueueList.empty() == false);

    //
    stdair::EventQueueList_T::const_iterator itEventQueue =
      lEventQueueList.begin();

    //
    stdair::EventQueue* oEventQueue_ptr = *itEventQueue;
    assert (oEventQueue_ptr != NULL);
    
    return *oEventQueue_ptr;
  }
  
  // //////////////////////////////////////////////////////////////////////
  DemandStream& DemandManager::createDemandStream
  (stdair::EventQueue& ioEventQueue,
   const DemandStreamKey& iKey,
   const ArrivalPatternCumulativeDistribution_T& iArrivalPattern,
   const POSProbabilityMassFunction_T& iPOSProbMass,
   const ChannelProbabilityMassFunction_T& iChannelProbMass,
   const TripTypeProbabilityMassFunction_T& iTripTypeProbMass,
   const StayDurationProbabilityMassFunction_T& iStayDurationProbMass,
   const FrequentFlyerProbabilityMassFunction_T& iFrequentFlyerProbMass,
   const PreferredDepartureTimeContinuousDistribution_T& iPreferredDepartureTimeContinuousDistribution,
   const stdair::WTP_T& iMinWTP,
   const ValueOfTimeContinuousDistribution_T& iValueOfTimeContinuousDistribution,
   const DemandDistribution& iDemandDistribution,
   const stdair::RandomSeed_T& iNumberOfRequestsSeed,
   const stdair::RandomSeed_T& iRequestDateTimeSeed,
   const stdair::RandomSeed_T& iDemandCharacteristicsSeed,
   stdair::UniformGenerator_T& ioSharedGenerator,
   const POSProbabilityMass_T& iDefaultPOSProbablityMass) {
    
    DemandStream& oDemandStream = FacDemandStream::
      instance().create (iKey, iArrivalPattern, iPOSProbMass,
                         iChannelProbMass, iTripTypeProbMass,
                         iStayDurationProbMass, iFrequentFlyerProbMass,
                         iPreferredDepartureTimeContinuousDistribution,
                         iMinWTP, iValueOfTimeContinuousDistribution,
                         iDemandDistribution, iNumberOfRequestsSeed,
                         iRequestDateTimeSeed, iDemandCharacteristicsSeed,
                         ioSharedGenerator, iDefaultPOSProbablityMass);
    
    // Link the DemandStream to its parent (EventQueue)
    stdair::FacBomManager::instance().linkWithParent (ioEventQueue,
                                                      oDemandStream);
    
    // Add the DemandStream to the dedicated list and map
    stdair::FacBomManager::instance().addToListAndMap (ioEventQueue,
                                                       oDemandStream);

    return oDemandStream;
  }
    
  // //////////////////////////////////////////////////////////////////////
  void DemandManager::
  createDemandCharacteristics (stdair::EventQueue& ioEventQueue,
                               stdair::UniformGenerator_T& ioSharedGenerator,
                               const POSProbabilityMass_T& iPOSProbMass,
                               const DemandStruct& iDemand) {
    
    //
    const DemandStreamKey lDemandStreamKey (iDemand._origin,
                                            iDemand._destination,
                                            iDemand._prefDepDate,
                                            iDemand._prefCabin);
    // DEBUG
    // STDAIR_LOG_DEBUG ("Demand stream key: " << lDemandStreamKey.describe());
    
    //
    const DemandDistribution lDemandDistribution (iDemand._demandMean,
                                                  iDemand._demandStdDev);
    
    // Seed
    const stdair::RandomSeed_T& lNumberOfRequestsSeed =
      generateSeed (ioSharedGenerator);
    const stdair::RandomSeed_T& lRequestDateTimeSeed =
      generateSeed (ioSharedGenerator);
    const stdair::RandomSeed_T& lDemandCharacteristicsSeed =
      generateSeed (ioSharedGenerator);
  
    // Delegate the call to the dedicated command
    DemandStream& lDemandStream = 
      createDemandStream (ioEventQueue, lDemandStreamKey,
                          iDemand._dtdProbDist, iDemand._posProbDist,
                          iDemand._channelProbDist,
                          iDemand._tripProbDist,
                          iDemand._stayProbDist, iDemand._ffProbDist,
                          iDemand._prefDepTimeProbDist,
                          iDemand._minWTP,
                          iDemand._timeValueProbDist,
                          lDemandDistribution,
                          lNumberOfRequestsSeed, lRequestDateTimeSeed,
                          lDemandCharacteristicsSeed,
                          ioSharedGenerator, iPOSProbMass);

    // Calculate the expected total number of events for the current
    // demand stream
    const stdair::NbOfRequests_T& lExpectedTotalNbOfEvents =
      lDemandStream.getMeanNumberOfRequests();

    /**
     * Initialise the progress status, specific to the demand stream,
     * held by the event queue.
     *
     * The event queue object, which is part of the StdAir
     * library, has no information on the DemandStream objects, which
     * are part of this (TraDemGen) library. As the number of events
     * to be generated are known from the DemandStream objects only,
     * the event queue object can not calculate the progress statuses
     * itself.
     */
    ioEventQueue.addStatus (lDemandStreamKey.toString(),
                            lExpectedTotalNbOfEvents);
  }

  // ////////////////////////////////////////////////////////////////////
  stdair::RandomSeed_T DemandManager::
  generateSeed (stdair::UniformGenerator_T& ioSharedGenerator) {
    stdair::RealNumber_T lVariateUnif = ioSharedGenerator() * 1e9;
    stdair::RandomSeed_T oSeed = static_cast<stdair::RandomSeed_T>(lVariateUnif);
    return oSeed;
  }
  
  // ////////////////////////////////////////////////////////////////////
  const bool DemandManager::
  stillHavingRequestsToBeGenerated (const stdair::EventQueue& iEventQueue,
                                    const stdair::DemandStreamKeyStr_T& iKey) {
    // Retrieve the DemandStream which corresponds to the given key.
    const DemandStream& lDemandStream =
      stdair::BomManager::getObject<DemandStream> (iEventQueue, iKey);
    
    return lDemandStream.stillHavingRequestsToBeGenerated();
  }

  // ////////////////////////////////////////////////////////////////////
  stdair::BookingRequestPtr_T DemandManager::
  generateNextRequest (stdair::EventQueue& ioEventQueue,
                       const stdair::DemandStreamKeyStr_T& iKey) {

    // Retrieve the DemandStream which corresponds to the given key.
    DemandStream& lDemandStream = 
      stdair::BomManager::getObject<DemandStream> (ioEventQueue, iKey);

    // Generate the next booking request
    stdair::BookingRequestPtr_T lBookingRequest =
      lDemandStream.generateNextRequest();

    // Create an event structure
    stdair::EventStruct lEventStruct (stdair::EventType::BKG_REQ, iKey,
                                      lBookingRequest);

    /**
       \note When adding an event in the event queue, the event can be
       altered. That happens when an event already exists, in the
       event queue, with exactly the same date-time stamp. In that
       case, the date-time stamp is altered for the newly added event,
       so that the unicity on the date-time stamp can be guaranteed.
    */
    ioEventQueue.addEvent (lEventStruct);
    
    return lBookingRequest;
  }

  // ////////////////////////////////////////////////////////////////////
  stdair::Count_T DemandManager::
  generateFirstRequests (stdair::EventQueue& ioEventQueue) {

    // Actual total number of events to be generated
    stdair::NbOfRequests_T lActualTotalNbOfEvents = 0.0;

    // Retrieve the DemandStream list
    const DemandStreamList_T& lDemandStreamList =
      stdair::BomManager::getList<DemandStream> (ioEventQueue);

    for (DemandStreamList_T::const_iterator itDemandStream =
           lDemandStreamList.begin();
         itDemandStream != lDemandStreamList.end(); ++itDemandStream) {
      DemandStream* lDemandStream_ptr = *itDemandStream;
      assert (lDemandStream_ptr != NULL);

      // Calculate the expected total number of events for the current
      // demand stream
      const stdair::NbOfRequests_T& lActualNbOfEvents =
        lDemandStream_ptr->getTotalNumberOfRequestsToBeGenerated();
      lActualTotalNbOfEvents += lActualNbOfEvents;

      // Retrieve the key of the demand stream
      const DemandStreamKey& lKey = lDemandStream_ptr->getKey();

      // Update the progress status for the given demand stream
      ioEventQueue.updateStatus (lKey.toString(), lActualNbOfEvents);

      // Check whether there are still booking requests to be generated
      const bool stillHavingRequestsToBeGenerated =
        lDemandStream_ptr->stillHavingRequestsToBeGenerated();
   
      if (stillHavingRequestsToBeGenerated) {
        // Generate the next event (booking request), and insert it
        // into the event queue
        generateNextRequest (ioEventQueue, lKey.toString());
      }
    }

    // Update the actual total number of events to be generated
    ioEventQueue.setActualTotalNbOfEvents (lActualTotalNbOfEvents);

    // Retrieve the actual total number of events to be generated
    const stdair::Count_T oTotalNbOfEvents = std::floor (lActualTotalNbOfEvents);

    //
    return oTotalNbOfEvents;
  }
  
  // ////////////////////////////////////////////////////////////////////
  void DemandManager::reset (stdair::EventQueue& ioEventQueue) {

    // Reset all the DemandStream objects
    const DemandStreamList_T& lDemandStreamList =
      stdair::BomManager::getList<DemandStream> (ioEventQueue);
    for (DemandStreamList_T::const_iterator itDS = lDemandStreamList.begin();
         itDS != lDemandStreamList.end(); ++itDS) {
      DemandStream* lCurrentDS_ptr = *itDS;
      assert (lCurrentDS_ptr != NULL);
      
      lCurrentDS_ptr->reset();
    }
    
    /**
     * Reset the EventQueue object.
     *
     * \note As the DemandStream objects are attached to the EventQueue
     * instance, that latter has to be resetted after the DemandStream
     * objects.
     */
    ioEventQueue.reset();
  }

}
