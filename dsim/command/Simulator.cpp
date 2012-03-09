// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <fstream>
#include <string>
#include <vector>
#include <exception>
#include <iomanip>
// StdAir
#include <stdair/stdair_demand_types.hpp>
#include <stdair/basic/ProgressStatusSet.hpp>
#include <stdair/bom/EventStruct.hpp>
#include <stdair/bom/BookingRequestStruct.hpp>
#include <stdair/bom/SnapshotStruct.hpp>
#include <stdair/bom/CancellationStruct.hpp>
#include <stdair/bom/RMEventStruct.hpp>
#include <stdair/bom/TravelSolutionStruct.hpp>
#include <stdair/service/Logger.hpp>
#include <stdair/STDAIR_Service.hpp>
// Distribution
#include <simcrs/SIMCRS_Service.hpp>
// TRADEMGEN
#include <trademgen/TRADEMGEN_Service.hpp>
// TRAVELCCM
#include <travelccm/TRAVELCCM_Service.hpp>
// Airsched
#include <airsched/AIRSCHED_Service.hpp>
// Dsim
#include <dsim/DSIM_Types.hpp>
#include <dsim/bom/SimulationStatus.hpp>
#include <dsim/command/Simulator.hpp>

namespace DSIM {

  // ////////////////////////////////////////////////////////////////////
  void Simulator::simulate (SIMCRS::SIMCRS_Service& ioSIMCRS_Service,
                            TRADEMGEN::TRADEMGEN_Service& ioTRADEMGEN_Service,
                            TRAVELCCM::TRAVELCCM_Service& ioTRAVELCCM_Service,
                            stdair::STDAIR_Service& ioSTDAIR_Service,
                            SimulationStatus& ioSimulationStatus,
                            const stdair::DemandGenerationMethod& iDemandGenerationMethod,
                            const stdair::ForecastingMethod& iForecastingMethod,
                            const stdair::PartnershipTechnique& iPartnershipTechnique) {

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
      ioTRADEMGEN_Service.generateFirstRequests(iDemandGenerationMethod);

    // Initialise the (Boost) progress display object
    //boost::progress_display lProgressDisplay(lActualNbOfEventsToBeGenerated);
  
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
      stdair::EventStruct lEventStruct;
      stdair::ProgressStatusSet lPSS = ioTRADEMGEN_Service.popEvent (lEventStruct);

      // Update the simulation status with the current date
      const stdair::DateTime_T& lCurrentEventDateTime = lEventStruct.getEventTime (); 
      const stdair::Date_T& lCurrentEventDate = lCurrentEventDateTime.date();
      ioSimulationStatus.setCurrentDate(lCurrentEventDate);

      // DEBUG
      STDAIR_LOG_DEBUG ("Poped event: '" << lEventStruct.describe() << "'.");

      // Check the event type
      const stdair::EventType::EN_EventType& lEventType =
        lEventStruct.getEventType(); 

      stdair::ProgressPercentage_T lProgressPercentageByType;

      switch (lEventType) {

      case stdair::EventType::BKG_REQ: 
	playBookingRequest (ioSIMCRS_Service,
			    ioTRADEMGEN_Service,
			    ioTRAVELCCM_Service,
			    lEventStruct,
			    lPSS,
			    iDemandGenerationMethod,
			    iPartnershipTechnique);  
	// Re-Calculate the progress percentage for the booking requests
	lProgressPercentageByType = 
	  ioTRADEMGEN_Service.calculateProgress(stdair::EventType::BKG_REQ);
	break;

      case stdair::EventType::CX: 
	playCancellation (ioSIMCRS_Service,
			  lEventStruct); 	
	// Re-Calculate the progress percentage for cancellations
	lProgressPercentageByType =
	  ioTRADEMGEN_Service.calculateProgress(stdair::EventType::CX);
	break;

      case stdair::EventType::SNAPSHOT: 
	playSnapshotEvent (ioSIMCRS_Service,
			   lEventStruct); 	
	// Re-Calculate the progress percentage for snap shots
	lProgressPercentageByType = 
	  ioTRADEMGEN_Service.calculateProgress(stdair::EventType::SNAPSHOT);
	break;

      case stdair::EventType::RM: 
	playRMEvent (ioSIMCRS_Service,
		     lEventStruct,
		     iForecastingMethod,
		     iPartnershipTechnique);	
	// Re-Calculate the progress percentage for rm events
	lProgressPercentageByType = 
	  ioTRADEMGEN_Service.calculateProgress(stdair::EventType::RM);
	break; 

      case stdair::EventType::BRK_PT:	
	// Re-Calculate the progress percentage for break point events
	lProgressPercentageByType = 
	  ioTRADEMGEN_Service.calculateProgress(stdair::EventType::BRK_PT);
	break;

      default: assert (false); break;
      }

      // Update the progress display
      //++lProgressDisplay;
  
      // Update the simulation status for the current type
      ioSimulationStatus.updateProgress(lEventType,
					lProgressPercentageByType);

      // Update the global simulation status 
      const stdair::ProgressPercentage_T lProgressPercentage = 
	ioTRADEMGEN_Service.calculateProgress();
      ioSimulationStatus.updateProgress(lProgressPercentage);
	
      
    }
       
    // DEBUG
    STDAIR_LOG_DEBUG ("The simulation has ended");
  }

  // ////////////////////////////////////////////////////////////////////
  void Simulator::
  playBookingRequest (SIMCRS::SIMCRS_Service& ioSIMCRS_Service,
                      TRADEMGEN::TRADEMGEN_Service& ioTRADEMGEN_Service,
                      TRAVELCCM::TRAVELCCM_Service& ioTRAVELCCM_Service,
                      const stdair::EventStruct& iEventStruct,
                      stdair::ProgressStatusSet& ioPSS,
                      const stdair::DemandGenerationMethod& iDemandGenerationMethod,
                      const stdair::PartnershipTechnique& iPartnershipTechnique) {
    // Extract the corresponding demand/booking request
    const stdair::BookingRequestStruct& lPoppedRequest =
      iEventStruct.getBookingRequest();

    // Check if the request if valid
    const stdair::Date_T& lDepDate = lPoppedRequest.getPreferedDepartureDate();
    const stdair::DateTime_T& lReqTime = lPoppedRequest.getRequestDateTime();
    const stdair::Date_T& lRegDate = lReqTime.date();
    if (lDepDate > lRegDate) {
      // DEBUG
      STDAIR_LOG_DEBUG ("Poped booking request: '" << lPoppedRequest.describe()
                        << "'.");

      // Retrieve the corresponding demand stream
      const stdair::DemandGeneratorKey_T& lDemandStreamKey =
        lPoppedRequest.getDemandGeneratorKey();
      
      // Assess whether more events should be generated for that demand stream
      const bool stillHavingRequestsToBeGenerated =
        ioTRADEMGEN_Service.stillHavingRequestsToBeGenerated (lDemandStreamKey,
                                                              ioPSS,
                                                              iDemandGenerationMethod);

      // DEBUG
      // STDAIR_LOG_DEBUG ("=> [" << lDemandStreamKey << "] is now processed. "
      //                   << "Still generate events for that demand stream? "
      //                   << stillHavingRequestsToBeGenerated);
      STDAIR_LOG_DEBUG ("Progress status" << ioPSS.describe());
      
      // If there are still events to be generated for that demand stream,
      // generate and add them to the event queue
      if (stillHavingRequestsToBeGenerated) {
      stdair::BookingRequestPtr_T lNextRequest_ptr =
        ioTRADEMGEN_Service.generateNextRequest (lDemandStreamKey,
                                                 iDemandGenerationMethod);
      assert (lNextRequest_ptr != NULL);
  
      // Sanity check
      const stdair::Duration_T lDuration =
        lNextRequest_ptr->getRequestDateTime()
        - lPoppedRequest.getRequestDateTime();
        if (lDuration.total_milliseconds() < 0) {
          STDAIR_LOG_NOTIFICATION ("[" << lDemandStreamKey
                                   << "] The date-time of the generated event ("
                                   << lNextRequest_ptr->getRequestDateTime()
                                   << ") is lower than the date-time "
                                   << "of the current event ("
                                   << lPoppedRequest.getRequestDateTime()
                                   << ")");
          assert (false);
        }

        // DEBUG
        // STDAIR_LOG_DEBUG ("[" << lDemandStreamKey << "] Added request: '"
        //                   << lNextRequest_ptr->describe()
        //                   << "'. Is queue done? "
        //                   << ioTRADEMGEN_Service.isQueueDone());
      }

      // Retrieve a list of travel solutions corresponding the given
      // booking request.
      stdair::TravelSolutionList_T lTravelSolutionList =
        ioSIMCRS_Service.calculateSegmentPathList (lPoppedRequest);
      
      if (lTravelSolutionList.empty() == false) {
        // Get the availability for each travel solution.
        ioSIMCRS_Service.calculateAvailability (lTravelSolutionList, iPartnershipTechnique);
        
        // Get the fare quote for each travel solution.
        ioSIMCRS_Service.fareQuote (lPoppedRequest, lTravelSolutionList);
        
        // Get a travel solution choice.
        const stdair::TravelSolutionStruct* lChosenTS_ptr =
          ioTRAVELCCM_Service.chooseTravelSolution (lTravelSolutionList,
                                                    lPoppedRequest);
        if (lChosenTS_ptr != NULL) {
          // DEBUG
          STDAIR_LOG_DEBUG ("Chosen TS: " << lChosenTS_ptr->describe());
          
          // Retrieve and convert the party size
          const stdair::NbOfSeats_T& lPartySizeDouble =
            lPoppedRequest.getPartySize();
          const stdair::PartySize_T lPartySize = std::floor (lPartySizeDouble);
        
          // Delegate the sell to the corresponding SimCRS service
          bool saleSucceedful =
            ioSIMCRS_Service.sell (*lChosenTS_ptr, lPartySize);
          
          // If the sale succeeded, generate the potential cancellation event.
          if (saleSucceedful == true) {
            ioTRADEMGEN_Service.generateCancellation (*lChosenTS_ptr,
                                                      lPartySize, lReqTime,
                                                      lDepDate);
          }
          
          // LOG
          const stdair::DateTime_T lDepartureDateTime =
            boost::posix_time::ptime (lDepDate, boost::posix_time::hours (0));
          const stdair::Duration_T lDTDRequest = lReqTime - lDepartureDateTime;
          const double lDTD = double(lDTDRequest.total_seconds()) / 86400.0;
          std::ostringstream oStr;
          const stdair::SegmentPath_T& lSegmentPath =
            lChosenTS_ptr->getSegmentPath();
          for (stdair::SegmentPath_T::const_iterator itSegPath =
                 lSegmentPath.begin();
               itSegPath != lSegmentPath.end(); ++itSegPath) {
            oStr << *itSegPath <<  ";";
          }
          STDAIR_LOG_NOTIFICATION (oStr.str() << std::setprecision(10) << lDTD);
        }
        else {
          // DEBUG
          STDAIR_LOG_DEBUG ("There is no chosen travel solution "
                            <<"for this request: "<< lPoppedRequest.describe());
        }
      } else {
        // DEBUG
        STDAIR_LOG_DEBUG ("No travel solution has been found for: "
                          << lPoppedRequest);
      }  
    }
  }
  
  // ////////////////////////////////////////////////////////////////////
  void Simulator::
  playCancellation (SIMCRS::SIMCRS_Service& ioSIMCRS_Service,
                    const stdair::EventStruct& iEventStruct) {
    // Retrieve the cancellation struct from the event.
    const stdair::CancellationStruct lCancellationStruct =
      iEventStruct.getCancellation();

    // DEBUG
    // STDAIR_LOG_DEBUG ("Play cancellation: "<<lCancellationStruct.describe());

    ioSIMCRS_Service.playCancellation (lCancellationStruct); 
  }
  
  
  // ////////////////////////////////////////////////////////////////////
  void Simulator::
  playSnapshotEvent (SIMCRS::SIMCRS_Service& ioSIMCRS_Service,
                     const stdair::EventStruct& iEventStruct) {
    // Retrieve the snapshot struct from the event.
    const stdair::SnapshotStruct lSnapshotStruct =
      iEventStruct.getSnapshotStruct();

    // DEBUG
    // STDAIR_LOG_DEBUG ("Taking snapshots: " << lSnapshotStruct.describe());

    ioSIMCRS_Service.takeSnapshots (lSnapshotStruct);    
  }
  
  // ////////////////////////////////////////////////////////////////////
  void Simulator::
  playRMEvent (SIMCRS::SIMCRS_Service& ioSIMCRS_Service,
               const stdair::EventStruct& iEventStruct,
               const stdair::ForecastingMethod& iForecastingMethod,
               const stdair::PartnershipTechnique& iPartnershipTechnique) {
    // Retrieve the RM event struct from the event.
    const stdair::RMEventStruct lRMEvent = iEventStruct.getRMEvent();

    // DEBUG
    STDAIR_LOG_DEBUG ("Running RM system: " << lRMEvent.describe());

    //ioSIMCRS_Service.optimise (lRMEvent, iForecastingMethod, iPartnershipTechnique);   
  }
}
