// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <fstream>
#include <string>
#include <vector>
#include <exception>
// StdAir
#include <stdair/stdair_demand_types.hpp>
#include <stdair/bom/EventStruct.hpp>
#include <stdair/bom/EventQueue.hpp>
#include <stdair/bom/BookingRequestStruct.hpp>
#include <stdair/bom/TravelSolutionStruct.hpp>
#include <stdair/service/Logger.hpp>
// Distribution
#include <simcrs/SIMCRS_Service.hpp>
// TRADEMGEN
#include <trademgen/TRADEMGEN_Service.hpp>
// Airsched
#include <airsched/AIRSCHED_Service.hpp>
// Dsim
#include <dsim/DSIM_Types.hpp>
#include <dsim/command/Simulator.hpp>

namespace DSIM {

  // ////////////////////////////////////////////////////////////////////
  void Simulator::simulate (SIMCRS::SIMCRS_Service& ioSIMCRS_Service,
                            TRADEMGEN::TRADEMGEN_Service& ioTRADEMGEN_Service) {

    try {

      // DEBUG
      STDAIR_LOG_DEBUG ("The simulation is starting");

      // /////////////////////////////////////////////////////
      // Event queue
      stdair::EventQueue lEventQueue;

      /**
         Initialisation step.
         <br>Generate the first event for each demand stream.
      */
      ioTRADEMGEN_Service.generateFirstRequests (lEventQueue);
  
      /** (Boost) progress display (current number of events, total
          number of events) for every demand stream. */
      stdair::ProgressDisplayMap_T lProgressDisplays;
      lEventQueue.initProgressDisplays (lProgressDisplays);

      /**
         Main loop.
         <ul>
         <li>Pop a request and get its associated type/demand stream.</li>
         <li>Generate the next request for the same type/demand stream.</li>
         </ul>
      */
      stdair::Count_T eventIdx = 1;
      while (lEventQueue.isQueueDone() == false) {

        // Get the next event from the event queue
        stdair::EventStruct& lEventStruct = lEventQueue.popEvent ();

        // Extract the corresponding demand/booking request
        const stdair::BookingRequestStruct& lPoppedRequest =
          lEventStruct.getBookingRequest ();

        // DEBUG
        STDAIR_LOG_DEBUG ("[" << eventIdx << "] Poped booking request: '"
                          << lPoppedRequest.describe() << "'.");
    
        // Play booking request
        playBookingRequest (ioSIMCRS_Service, lPoppedRequest);
    
        // Retrieve the corresponding demand stream
        const stdair::DemandStreamKeyStr_T& lDemandStreamKey =
          lEventStruct.getDemandStreamKey ();

        // Assess whether more events should be generated for that
        // demand stream
        bool stillHavingRequestsToBeGenerated = ioTRADEMGEN_Service.
          stillHavingRequestsToBeGenerated (lDemandStreamKey);
        
        // Retrieve, from the demand stream, the total number of events
        // to be generated
        const stdair::NbOfRequests_T& lNbOfRequests = ioTRADEMGEN_Service.
          getTotalNumberOfRequestsToBeGenerated (lDemandStreamKey);
      
        // DEBUG
        STDAIR_LOG_DEBUG ("=> [" << lDemandStreamKey << "] is now processed. "
                          << "Still generate events for that demand stream? "
                          << stillHavingRequestsToBeGenerated);
    
        // If there are still events to be generated for that demand
        // stream, generate and add them to the event queue
        if (stillHavingRequestsToBeGenerated) {
          stdair::BookingRequestPtr_T lNextRequest_ptr =
            ioTRADEMGEN_Service.generateNextRequest (lDemandStreamKey);
          assert (lNextRequest_ptr != NULL);
          
          // Sanity check
          const stdair::Duration_T lDuration =
            lNextRequest_ptr->getRequestDateTime()
            - lPoppedRequest.getRequestDateTime();
          if (lDuration.total_milliseconds() < 0) {
            STDAIR_LOG_ERROR ("[" << lDemandStreamKey
                              << "] The date-time of the generated event ("
                              << lNextRequest_ptr->getRequestDateTime()
                              << ") is lower than the date-time "
                              << "of the current event ("
                              << lPoppedRequest.getRequestDateTime() << ")");
            assert (false);
          }

          //
          stdair::EventStruct lNextEventStruct (stdair::EventType::BKG_REQ,
                                                lDemandStreamKey,
                                                lNextRequest_ptr);

          /**
             Note that when adding an event in the event queue, the
             former can be altered. It happends when an event already
             exists in the event queue with exactly the same date-time
             stamp. In that case, the date-time stamp is altered for the
             newly added event, so that the unicity on the date-time
             stamp can be guaranteed.
          */
          lEventQueue.addEvent (lNextEventStruct);

          // DEBUG
          STDAIR_LOG_DEBUG ("[" << lDemandStreamKey << "] Added request: '"
                            << lNextRequest_ptr->describe()
                            << "'. Is queue done? "
                            << lEventQueue.isQueueDone());
        }
        
        /**
           Remove the last used event, so that, at any given moment, the
           queue keeps only the active events.
        */
        lEventQueue.eraseLastUsedEvent ();

        // Iterate
        ++eventIdx;
      }
            
      // DEBUG
      STDAIR_LOG_DEBUG ("The simulation has ended");

    } catch (const std::exception& lStdError) {
      STDAIR_LOG_ERROR ("Error: " << lStdError.what());
      throw SimulationException();
    }
  }

  // ////////////////////////////////////////////////////////////////////
  void Simulator::
  playBookingRequest (SIMCRS::SIMCRS_Service& ioSIMCRS_Service,
                      const stdair::BookingRequestStruct& iBookingRequest) {
    // Retrieve a list of travel solutions corresponding the given
    // booking request.
    const stdair::SegmentPathList_T& lSegmentPathList =
      ioSIMCRS_Service.calculateSegmentPathList (iBookingRequest);
    if (lSegmentPathList.empty() == false) {
      // Get the fare quote for each travel solution.
      stdair::TravelSolutionList_T lTravelSolutionList = 
        ioSIMCRS_Service.fareQuote (iBookingRequest, lSegmentPathList);
      
      // Get the availability for each travel solution.
      ioSIMCRS_Service.calculateAvailability (lTravelSolutionList);

      // Hardcode a travel solution choice.
      stdair::TravelSolutionList_T::const_iterator itTS =
        lTravelSolutionList.begin();
      const stdair::TravelSolutionStruct& lChosenTS = *itTS;

      // DEBUG
      STDAIR_LOG_DEBUG ("Chosen TS: " << lChosenTS << "\n");
      
      // Make a sale.
      const stdair::NbOfSeats_T& lPartySize = iBookingRequest.getPartySize();
      ioSIMCRS_Service.sell (lChosenTS, lPartySize);

    } else {
      // DEBUG
      STDAIR_LOG_DEBUG ("No travel solution has been found/chosen.");
    }
  }
  
}
