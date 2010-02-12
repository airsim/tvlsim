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
#include <stdair/bom/OutboundPathTypes.hpp>
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

      // Hardcoded section for demand generation.
      // Demand characteristics
      stdair::DemandCharacteristics demandCharacteristics1;
      stdair::DemandCharacteristics demandCharacteristics2;
      // Demand distribution
      stdair::DemandDistribution demandDistribution1;
      stdair::DemandDistribution demandDistribution2;
      
      // distribution of number of requests
      demandDistribution1.setMeanNumberOfRequests (10.0);
      demandDistribution1.setStandardDeviationNumberOfRequests (2.0);
      demandDistribution2.setMeanNumberOfRequests (12.0);
      demandDistribution2.setStandardDeviationNumberOfRequests (1.0);

      // origin
      demandCharacteristics1.setOrigin ("LHR");
      demandCharacteristics2.setOrigin ("LHR");
      // destination
      demandCharacteristics1.setDestination ("JFK");
      demandCharacteristics2.setDestination ("JFK");
      // preferred departure date
      demandCharacteristics1.setPreferredDepartureDate (boost::gregorian::date (2010,1,17));
      demandCharacteristics2.setPreferredDepartureDate (boost::gregorian::date (2010,1,18));
      // Passenger type
      demandCharacteristics1.setPaxType ("L");
      demandCharacteristics2.setPaxType ("B");
  
      // arrival pattern
      std::multimap<stdair::FloatDuration_T, stdair::Probability_T> arrivalPatternCumulativeDistribution1;
      arrivalPatternCumulativeDistribution1.
        insert ( std::pair<stdair::FloatDuration_T, stdair::Probability_T> (-365.0, 0) );
      arrivalPatternCumulativeDistribution1.
        insert ( std::pair<stdair::FloatDuration_T, stdair::Probability_T> (-67.0, 0.2) );
      arrivalPatternCumulativeDistribution1.
        insert ( std::pair<stdair::FloatDuration_T, stdair::Probability_T> (-17.0, 0.5) );
      arrivalPatternCumulativeDistribution1.
        insert ( std::pair<stdair::FloatDuration_T, stdair::Probability_T> (0.0, 1.0) );

      std::multimap<stdair::FloatDuration_T, stdair::Probability_T> arrivalPatternCumulativeDistribution2;
      arrivalPatternCumulativeDistribution2.
        insert ( std::pair<stdair::FloatDuration_T, stdair::Probability_T> (-365.0, 0) );
      arrivalPatternCumulativeDistribution2.
        insert ( std::pair<stdair::FloatDuration_T, stdair::Probability_T> (-300.0, 0.5) );
      arrivalPatternCumulativeDistribution2.
        insert ( std::pair<stdair::FloatDuration_T, stdair::Probability_T> (-200.0, 0.9) );
      arrivalPatternCumulativeDistribution2.
        insert ( std::pair<stdair::FloatDuration_T, stdair::Probability_T> (0.0, 1.0) );

      // When creating the ContinuousAttribute object, the mapping is
      // inverted, i.e., the inverse cumulative distribution can be
      // derived from the cumulative distribution
      const stdair::ContinuousAttribute<stdair::FloatDuration_T> arrivalPattern1 (arrivalPatternCumulativeDistribution1);
      demandCharacteristics1.setArrivalPattern (arrivalPattern1);
      const stdair::ContinuousAttribute<stdair::FloatDuration_T> arrivalPattern2 (arrivalPatternCumulativeDistribution2);
      demandCharacteristics2.setArrivalPattern (arrivalPattern2);
  
      // Display
      STDAIR_LOG_DEBUG ("Demand 1: " << demandCharacteristics1.display()
                        << demandDistribution1.display()
                        << std::endl << std::endl);

      STDAIR_LOG_DEBUG ("Demand 2: " << demandCharacteristics2.display()
                        << demandDistribution2.display()
                        << std::endl << std::endl);

      // Seeds
      stdair::RandomSeed_T seed = 2;
  
      // Key
      stdair::DemandStreamKey_T key1 = 1;
      stdair::DemandStreamKey_T key2 = 2;

      

      // Initialize the demand stream
      ioTRADEMGEN_Service.addDemandStream (key1, demandCharacteristics1,
                                        demandDistribution1, seed, seed, seed);
      ioTRADEMGEN_Service.addDemandStream (key2, demandCharacteristics2,
                                        demandDistribution2, seed, seed, seed);
  
      // Get the total number of requests to be generated
      stdair::Count_T totalNumberOfRequestsToBeGenerated1 =
        ioTRADEMGEN_Service.getTotalNumberOfRequestsToBeGenerated (key1);
      stdair::Count_T totalNumberOfRequestsToBeGenerated2 =
        ioTRADEMGEN_Service.getTotalNumberOfRequestsToBeGenerated (key2);

      STDAIR_LOG_DEBUG ("Number of requests to be generated (demand 1): "
                        << totalNumberOfRequestsToBeGenerated1 << std::endl);
      STDAIR_LOG_DEBUG ("Number of requests to be generated (demand 2): "
                        << totalNumberOfRequestsToBeGenerated2 << std::endl);

      // /////////////////////////////////////////////////////
      // Event queue
      stdair::EventQueue lEventQueue = stdair::EventQueue ();
  
      // Initialize by adding one request of each type
      const bool stillHavingRequestsToBeGenerated1 = 
        ioTRADEMGEN_Service.stillHavingRequestsToBeGenerated (key1);
      if (stillHavingRequestsToBeGenerated1) {
        stdair::BookingRequestPtr_T lRequest1 =
          ioTRADEMGEN_Service.generateNextRequest (key1);
        assert (lRequest1 != NULL);
        stdair::DateTime_T lRequestDateTime = lRequest1->getRequestDateTime ();
        stdair::EventStruct lEventStruct ("Request", lRequestDateTime, key1,
                                          lRequest1);
        lEventQueue.addEvent (lEventStruct);
      }
  
      const bool stillHavingRequestsToBeGenerated2 = 
        ioTRADEMGEN_Service.stillHavingRequestsToBeGenerated (key2);
      if (stillHavingRequestsToBeGenerated2) {
        stdair::BookingRequestPtr_T lRequest2 =
          ioTRADEMGEN_Service.generateNextRequest (key2);
        assert (lRequest2 != NULL);
        stdair::DateTime_T lRequestDateTime = lRequest2->getRequestDateTime ();
        stdair::EventStruct lEventStruct("Request", lRequestDateTime, key2,
                                         lRequest2);
        lEventQueue.addEvent (lEventStruct);
      }
  
      // Pop requests, get type, and generate next request of same type
      int i = 0;
      while (lEventQueue.isQueueDone() == false && i < 20) {
        // DEBUG
        STDAIR_LOG_DEBUG ("Before popping (" << i << ")" );
        STDAIR_LOG_DEBUG ("Queue size: " << lEventQueue.getQueueSize () );
        STDAIR_LOG_DEBUG ("Is queue done? " << lEventQueue.isQueueDone () );
    
        stdair::EventStruct& lEventStruct = lEventQueue.popEvent ();

        // DEBUG
        STDAIR_LOG_DEBUG ("After popping" );
        STDAIR_LOG_DEBUG ("Queue size: " << lEventQueue.getQueueSize ());
        STDAIR_LOG_DEBUG ("Is queue done? " << lEventQueue.isQueueDone ());

        STDAIR_LOG_DEBUG ("Popped request " << i );
    
        const stdair::BookingRequestStruct& lPoppedRequest =
          lEventStruct.getBookingRequest ();
    
        // DEBUG
        STDAIR_LOG_DEBUG (lPoppedRequest.describe());
        
        // Play booking request
        playBookingRequest (ioSIMCRS_Service, lPoppedRequest);
    
        // Retrieve the corresponding demand stream
        const stdair::DemandStreamKey_T& lDemandStreamKey =
          lEventStruct.getDemandStreamKey ();
        // generate next request
        bool stillHavingRequestsToBeGenerated = 
          ioTRADEMGEN_Service.stillHavingRequestsToBeGenerated(lDemandStreamKey);
        STDAIR_LOG_DEBUG ("stillHavingRequestsToBeGenerated: " << stillHavingRequestsToBeGenerated );
        if (stillHavingRequestsToBeGenerated) {
          stdair::BookingRequestPtr_T lNextRequest =
            ioTRADEMGEN_Service.generateNextRequest (lDemandStreamKey);
          assert (lNextRequest != NULL);
          // DEBUG
          STDAIR_LOG_DEBUG ("Added request: " << lNextRequest->describe());
      
          stdair::DateTime_T lNextRequestDateTime =
            lNextRequest->getRequestDateTime ();
          stdair::EventStruct lNextEventStruct ("Request",
                                                lNextRequestDateTime,
                                                lDemandStreamKey,
                                                lNextRequest);
          lEventQueue.eraseLastUsedEvent ();
          lEventQueue.addEvent (lNextEventStruct);
      
          // DEBUG
          STDAIR_LOG_DEBUG ("After adding");
          STDAIR_LOG_DEBUG ("Queue size: " << lEventQueue.getQueueSize ());
          STDAIR_LOG_DEBUG ("Is queue done? " << lEventQueue.isQueueDone ());
    
        }

        // DEBUG
        STDAIR_LOG_DEBUG (std::endl);
    
        // Iterate
        ++i;
      }
      // Play booking request
      //playBookingRequest (ioSIMCRS_Service, lBookingRequest);
            
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

    // Hardcode a travel solution choice.
    if (lTravelSolutionList.empty() == false) {
      // DEBUG
      STDAIR_LOG_DEBUG ("A travel solution is chosen.");
      
      stdair::TravelSolutionStruct lChosenTravelSolution =
        lTravelSolutionList.at(0);
      // Get the number of seats in the request.
      const stdair::NbOfSeats_T& lNbOfSeats = iBookingRequest.getPartySize();
      // Make a sale.
      ioSIMCRS_Service.sell (lChosenTravelSolution, lNbOfSeats);

    } else {
      // DEBUG
      STDAIR_LOG_DEBUG ("No travel solution is chosen.");
    }
  }
  
}

