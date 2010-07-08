// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <fstream>
#include <string>
#include <vector>
#include <exception>
// StdAir
#include <stdair/STDAIR_Types.hpp>
#include <stdair/basic/DemandCharacteristics.hpp>
#include <stdair/basic/DemandDistribution.hpp>
#include <stdair/basic/CategoricalAttribute.hpp>
#include <stdair/basic/ContinuousAttribute.hpp>
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
      stdair::EventQueue lEventQueue = stdair::EventQueue ();
      // Browse the list of DemandStreams and Generate the first event for each
      // DemandStream.
      ioTRADEMGEN_Service.generateFirstRequests (lEventQueue);
  
  
      // Pop requests, get type, and generate next request of same type
      while (lEventQueue.isQueueDone() == false) {
        stdair::EventStruct& lEventStruct = lEventQueue.popEvent ();

        const stdair::BookingRequestStruct& lPoppedRequest =
          lEventStruct.getBookingRequest ();

        // DEBUG
        STDAIR_LOG_DEBUG ("Poped request: " << lPoppedRequest.describe());
    
        // Play booking request
        playBookingRequest (ioSIMCRS_Service, lPoppedRequest);
    
        // Retrieve the corresponding demand stream
        const stdair::DemandStreamKeyStr_T& lDemandStreamKey =
          lEventStruct.getDemandStreamKey ();
        // generate next request
        bool stillHavingRequestsToBeGenerated = 
          ioTRADEMGEN_Service.stillHavingRequestsToBeGenerated(lDemandStreamKey);
       
        if (stillHavingRequestsToBeGenerated) {
          stdair::BookingRequestPtr_T lNextRequest =
            ioTRADEMGEN_Service.generateNextRequest (lDemandStreamKey);
          assert (lNextRequest != NULL);
      
          stdair::DateTime_T lNextRequestDateTime =
            lNextRequest->getRequestDateTime ();
          stdair::EventStruct lNextEventStruct ("Request",
                                                lNextRequestDateTime,
                                                lDemandStreamKey,
                                                lNextRequest);
          lEventQueue.addEvent (lNextEventStruct);
        }

        lEventQueue.eraseLastUsedEvent ();
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
    stdair::TravelSolutionList_T lTravelSolutionList =
      ioSIMCRS_Service.getTravelSolutions (iBookingRequest);
    if (lTravelSolutionList.empty() == false) {
      // Get the fare quote for each travel solution.
      // Get the availability for each travel solution.
      //ioSIMCRS_Service.getAvailability (lTravelSolutionList);
      // Hardcode a travel solution choice.
      // Make a sale.
      // ioSIMCRS_Service.sell (lChosenTravelSolution, lNbOfSeats);
      stdair::TravelSolutionList_T::iterator itTS = lTravelSolutionList.begin();
      const stdair::TravelSolutionStruct& lChosenTS = *itTS;
      STDAIR_LOG_DEBUG ("Chosen TS: " << lChosenTS);

    } else {
      // DEBUG
      STDAIR_LOG_DEBUG ("No travel solution is chosen.");
    }
  }
  
}

