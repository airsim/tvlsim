// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// Boost
#include <boost/make_shared.hpp>
// STL
#include <fstream>
#include <string>
#include <vector>
#include <exception>
#include <iomanip>
// StdAir
#include <stdair/stdair_demand_types.hpp>
#include <stdair/basic/ProgressStatusSet.hpp>
#include <stdair/basic/BasChronometer.hpp>
#include <stdair/bom/BookingRequestStruct.hpp>
#include <stdair/bom/SnapshotStruct.hpp>
#include <stdair/bom/CancellationStruct.hpp>
#include <stdair/bom/RMEventStruct.hpp>
#include <stdair/bom/BreakPointStruct.hpp>
#include <stdair/bom/EventStruct.hpp>
#include <stdair/bom/TravelSolutionStruct.hpp>
#include <stdair/service/Logger.hpp>
#include <stdair/STDAIR_Service.hpp>
// Distribution
#include <simcrs/SIMCRS_Service.hpp>
// SEVMGR
#include <sevmgr/SEVMGR_Service.hpp>
// TRADEMGEN
#include <trademgen/TRADEMGEN_Service.hpp>
// TRAVELCCM
#include <travelccm/TRAVELCCM_Service.hpp>
// Airsched
#include <airsched/AIRSCHED_Service.hpp>
// Dsim
#include <dsim/DSIM_Types.hpp>
#include <dsim/basic/SimulationMode.hpp>
#include <dsim/bom/SimulationStatus.hpp>
#include <dsim/command/Simulator.hpp>

namespace DSIM {

  // ////////////////////////////////////////////////////////////////////
  void Simulator::
  simulate (SIMCRS::SIMCRS_Service& ioSIMCRS_Service,
	    TRADEMGEN::TRADEMGEN_Service& ioTRADEMGEN_Service,
	    TRAVELCCM::TRAVELCCM_Service& ioTRAVELCCM_Service,
	    stdair::STDAIR_Service& ioSTDAIR_Service,
	    SimulationStatus& ioSimulationStatus,
	    const stdair::DemandGenerationMethod& iDemandGenerationMethod) {

    if (ioSimulationStatus.getMode() == SimulationMode::START ||
	ioSimulationStatus.getMode() == SimulationMode::DONE) {  
 
      /**
	 Initialisation step.
	 <br>Generate the first event for each demand stream.
      */
      // Retrieve the expected (mean value of the) number of events to be
      // generated
      const stdair::Count_T& lExpectedNbOfEventsToBeGenerated =
      ioTRADEMGEN_Service.getExpectedTotalNumberOfRequestsToBeGenerated();
      const stdair::Count_T& lActualNbOfEventsToBeGenerated =
	ioTRADEMGEN_Service.generateFirstRequests(iDemandGenerationMethod);

      // DEBUG  
      STDAIR_LOG_DEBUG ("Expected number of events: "
			<< lExpectedNbOfEventsToBeGenerated << ", actual: "
			<< lActualNbOfEventsToBeGenerated);

    }

    // Change the current mode of the simulation status
    ioSimulationStatus.setMode (SimulationMode::RUNNING);

    // Initialise the (Boost) progress display object
    //boost::progress_display lProgressDisplay(lActualNbOfEventsToBeGenerated);
  
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
      stdair::ProgressStatusSet lPSS = 
	ioTRADEMGEN_Service.popEvent (lEventStruct);

      // Update the simulation status with the current date
      const stdair::DateTime_T& lCurrentEventDateTime = 
	lEventStruct.getEventTime (); 
      const stdair::Date_T& lCurrentEventDate = 
	lCurrentEventDateTime.date();
      ioSimulationStatus.setCurrentDate(lCurrentEventDate);

      // DEBUG
      STDAIR_LOG_DEBUG ("Poped event: '" << lEventStruct.describe() << "'.");

      // Check the event type
      const stdair::EventType::EN_EventType& lEventType =
        lEventStruct.getEventType();

      stdair::BasChronometer lNextEventChronometer;
      lNextEventChronometer.start();

      switch (lEventType) {

      case stdair::EventType::BKG_REQ: 
	playBookingRequest (ioSIMCRS_Service,
			    ioTRADEMGEN_Service,
			    ioTRAVELCCM_Service,
			    lEventStruct,
			    lPSS,
                            ioSimulationStatus,
			    iDemandGenerationMethod); 
	break;
	
      case stdair::EventType::CX: 
	playCancellation (ioSIMCRS_Service,
			  lEventStruct); 	
	break;

      case stdair::EventType::SNAPSHOT: 
      	playSnapshotEvent (ioSIMCRS_Service,
			   lEventStruct); 	
	break;

      case stdair::EventType::RM: 
	playRMEvent (ioSIMCRS_Service,
		     lEventStruct);	
	break; 

      case stdair::EventType::BRK_PT:	
	// Change the current mode of the simulation status
	ioSimulationStatus.setMode (SimulationMode::BREAK);
	updateStatus (ioTRADEMGEN_Service, lEventType, ioSimulationStatus);
	return;
	break;

      case stdair::EventType::OPT_NOT_4_FD: 
      case stdair::EventType::OPT_NOT_4_NET: 
      case stdair::EventType::SKD_CHG:
	break;	
      default: assert (false); 
	break;
      }

      // Update the simulation status
      const double lNextEventMeasure = lNextEventChronometer.elapsed();
      updateStatus (ioTRADEMGEN_Service, lEventType, 
		    ioSimulationStatus, lNextEventMeasure);
      
      // Update the progress display
      //++lProgressDisplay;
      
    }	

    // Change the current mode of the simulation status
    ioSimulationStatus.setMode (SimulationMode::DONE);
  } 

  // ////////////////////////////////////////////////////////////////////
  void Simulator::
  updateStatus (const TRADEMGEN::TRADEMGEN_Service& ioTRADEMGEN_Service,
		const stdair::EventType::EN_EventType& iEN_EventType,
		SimulationStatus& ioSimulationStatus,
		const double& iEventMeasure) { 	
    // Update the global simulation status 
    const stdair::ProgressStatus& lProgressStatus = 
      ioTRADEMGEN_Service.getProgressStatus ();
    ioSimulationStatus.setCurrentProgressStatus (lProgressStatus);  

    // Re-Calculate the progress percentage for the given event type
    const stdair::ProgressStatus& lProgressStatusByType = 
      ioTRADEMGEN_Service.getProgressStatus (iEN_EventType);
    
    // Update the simulation status for the current type
    ioSimulationStatus.updateProgress(iEN_EventType,
				      lProgressStatusByType,
				      iEventMeasure);

  }

  // ////////////////////////////////////////////////////////////////////
  void Simulator::
  playBookingRequest (SIMCRS::SIMCRS_Service& ioSIMCRS_Service,
                      TRADEMGEN::TRADEMGEN_Service& ioTRADEMGEN_Service,
                      TRAVELCCM::TRAVELCCM_Service& ioTRAVELCCM_Service,
                      const stdair::EventStruct& iEventStruct,
                      stdair::ProgressStatusSet& ioPSS,
                      SimulationStatus& ioSimulationStatus,
                      const stdair::DemandGenerationMethod& iDemandGenerationMethod) {
    
    // Extract the corresponding demand/booking request
    const stdair::BookingRequestStruct& lPoppedRequest =
      iEventStruct.getBookingRequest();

    // Check if the request if valid
    const stdair::Date_T& lDepDate = lPoppedRequest.getPreferedDepartureDate();
    const stdair::Duration_T& lDepTime = lPoppedRequest.getPreferredDepartureTime();
    const stdair::DateTime_T& lReqDateTime = lPoppedRequest.getRequestDateTime();
    const stdair::Date_T& lReqDate = lReqDateTime.date();
    const stdair::Duration_T& lReqTime = lReqDateTime.time_of_day();
    const bool isRequestDateValid =
      ((lDepDate > lReqDate) || (lDepDate == lReqDate && lDepTime > lReqTime));
    assert (isRequestDateValid == true);
    
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

      // Get the fare quote for each travel solution.
      ioSIMCRS_Service.fareQuote (lPoppedRequest, lTravelSolutionList);

        // Get the availability for each travel solution.
      ioSIMCRS_Service.calculateAvailability (lTravelSolutionList);
        
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
          ioSimulationStatus.increaseGlobalNumberOfBookings(lPartySize);
          ioTRADEMGEN_Service.generateCancellation (*lChosenTS_ptr,
                                                    lPartySize, lReqDateTime,
                                                    lDepDate);
        }
          
        // LOG
        const stdair::DateTime_T lDepartureDateTime =
          boost::posix_time::ptime (lDepDate, boost::posix_time::hours (0));
        const stdair::Duration_T lDTDRequest = lReqDateTime - lDepartureDateTime;
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
               const stdair::EventStruct& iEventStruct) {
    // Retrieve the RM event struct from the event.
    const stdair::RMEventStruct lRMEvent = iEventStruct.getRMEvent();

    // DEBUG
    STDAIR_LOG_DEBUG ("Running RM system: " << lRMEvent.describe());

    ioSIMCRS_Service.optimise (lRMEvent);   
  } 

  // ////////////////////////////////////////////////////////////////////
  const stdair::Count_T Simulator::
  initialiseBreakPoint (const TRADEMGEN::TRADEMGEN_Service& ioTRADEMGEN_Service,
			SEVMGR::SEVMGR_Service& ioSEVMGR_Service,
			const stdair::BreakPointList_T& iBreakPointList,
			SimulationStatus& ioSimulationStatus) {
    
    // Number of break points actually added to the event queue. 
    stdair::Count_T lBreakPointNumber = 0;

    // Get the start and end date of the simulation 
    const stdair::Date_T& lStartDate = ioSimulationStatus.getStartDate();
    const stdair::Date_T& lEndDate = ioSimulationStatus.getEndDate();

    // Try to add each break point of the list to the event queue.
    for (stdair::BreakPointList_T::const_iterator itBPEvent = 
	   iBreakPointList.begin();
         itBPEvent != iBreakPointList.end(); ++itBPEvent) {
      const stdair::BreakPointStruct& lBPEvent = *itBPEvent;
      const stdair::DateTime_T& lDateTimeBP = 
	lBPEvent.getBreakPointTime();
      const stdair::Date_T lDateBP = lDateTimeBP.date();
      // If the break point is within the simulation period, add it to the 
      // queue.
      if (lDateBP < lEndDate && lDateBP >= lStartDate) {
	stdair::BreakPointPtr_T lBPEventPtr =
	  boost::make_shared<stdair::BreakPointStruct> (lBPEvent);
	// Create an event structure
	stdair::EventStruct lEventStruct (stdair::EventType::BRK_PT,
					  lBPEventPtr);
	ioSEVMGR_Service.addEvent (lEventStruct);
	lBreakPointNumber ++;
      }
    } 

    if (lBreakPointNumber > 0) { 
      // Update the status of break point events within the event queue. 
      const bool hasProgressStatus = 
	ioSEVMGR_Service.hasProgressStatus(stdair::EventType::BRK_PT); 
      if (hasProgressStatus == false) {   
	ioSEVMGR_Service.addStatus (stdair::EventType::BRK_PT, 
				    lBreakPointNumber); 
      } else {   
	stdair::Count_T lCurrentBPNumber = ioSEVMGR_Service.
	  getActualTotalNumberOfEventsToBeGenerated (stdair::EventType::BRK_PT);
	lCurrentBPNumber += lBreakPointNumber;
	ioSEVMGR_Service.updateStatus (stdair::EventType::BRK_PT, 
				       lCurrentBPNumber);   
      }     
      // Update the global Simulation Status
      updateStatus(ioTRADEMGEN_Service, stdair::EventType::BRK_PT,
		 ioSimulationStatus);
    }
    return lBreakPointNumber;
  }
}
