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

    // DEBUG
    STDAIR_LOG_DEBUG ("The simulation is starting");

    // Retrieve the expected (mean value of the) number of events to be
    // generated
    const stdair::Count_T& lExpectedNbOfEventsToBeGenerated =
      ioTRADEMGEN_Service.getExpectedTotalNumberOfRequestsToBeGenerated();

    /**
       Initialisation step.
       <br>Generate the first event for each demand stream.
    */
    const stdair::Count_T& lActualNbOfEventsToBeGenerated =
      ioTRADEMGEN_Service.generateFirstRequests();

    // Initialise the (Boost) progress display object
    // boost::progress_display lProgressDisplay(lActualNbOfEventsToBeGenerated);
  
    // DEBUG
    STDAIR_LOG_DEBUG ("Expected number of events: "
                      << lExpectedNbOfEventsToBeGenerated << ", actual: "
                      << lActualNbOfEventsToBeGenerated);
  
    /**
       Main loop.
       <ul>
         <li>Pop a request and get its associated type/demand stream.</li>
         <li>Generate the next request for the same type/demand stream.</li>
       </ul>
    */
    while (ioTRADEMGEN_Service.isQueueDone() == false) {

      // Get the next event from the event queue
      const stdair::EventStruct& lEventStruct = ioTRADEMGEN_Service.popEvent();

      // DEBUG
      STDAIR_LOG_DEBUG ("Poped event: '" << lEventStruct.describe() << "'.");

      // Extract the corresponding demand/booking request
      const stdair::BookingRequestStruct& lPoppedRequest =
        lEventStruct.getBookingRequest();

      // DEBUG
      STDAIR_LOG_DEBUG ("Poped booking request: '" << lPoppedRequest.describe()
                        << "'.");

      // Play booking request
      playBookingRequest (ioSIMCRS_Service, lPoppedRequest);

      // Retrieve the corresponding demand stream
      const stdair::EventContentKey_T& lDemandStreamKey =
        lEventStruct.getEventContentKey();

      // Assess whether more events should be generated for that demand stream
      const bool stillHavingRequestsToBeGenerated =
        ioTRADEMGEN_Service.stillHavingRequestsToBeGenerated (lDemandStreamKey);

      // DEBUG
      STDAIR_LOG_DEBUG ("=> [" << lDemandStreamKey << "] is now processed. "
                        << "Still generate events for that demand stream? "
                        << stillHavingRequestsToBeGenerated);

      // If there are still events to be generated for that demand stream,
      // generate and add them to the event queue
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

        // DEBUG
        STDAIR_LOG_DEBUG ("[" << lDemandStreamKey << "] Added request: '"
                          << lNextRequest_ptr->describe()
                          << "'. Is queue done? "
                          << ioTRADEMGEN_Service.isQueueDone());
      }

      // Update the progress display
      //++lProgressDisplay;
    }
       
    // DEBUG
    STDAIR_LOG_DEBUG ("The simulation has ended");
  }

  // ////////////////////////////////////////////////////////////////////
  void Simulator::
  playBookingRequest (SIMCRS::SIMCRS_Service& ioSIMCRS_Service,
                      const stdair::BookingRequestStruct& iBookingRequest) {

    // Retrieve a list of travel solutions corresponding the given
    // booking request.
    stdair::TravelSolutionList_T lTravelSolutionList =
      ioSIMCRS_Service.calculateSegmentPathList (iBookingRequest);
    
    if (lTravelSolutionList.empty() == false) {
      // Get the fare quote for each travel solution.
      ioSIMCRS_Service.fareQuote (iBookingRequest, lTravelSolutionList);
      
      // Get the availability for each travel solution.
      ioSIMCRS_Service.calculateAvailability (lTravelSolutionList);

      // Hardcode a travel solution choice.
      stdair::TravelSolutionList_T::const_iterator itTS =
        lTravelSolutionList.begin();
      const stdair::TravelSolutionStruct& lChosenTS = *itTS;

      // DEBUG
      STDAIR_LOG_DEBUG ("Chosen TS: " << lChosenTS.describe());
      
      // Retrieve and convert the party size
      const stdair::NbOfSeats_T& lPartySizeDouble =
        iBookingRequest.getPartySize();
      const stdair::PartySize_T lPartySize = std::floor (lPartySizeDouble);

      // Delegate the sell to the corresponding SimCRS service
      ioSIMCRS_Service.sell (lChosenTS, lPartySize);

    } else {
      // DEBUG
      STDAIR_LOG_DEBUG ("No travel solution has been found/chosen for: "
                        << iBookingRequest);
    }
  }
  
}
