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
#include <stdair/bom/OutboundPathTypes.hpp>
#include <stdair/bom/BookingRequestStruct.hpp>
#include <stdair/bom/TravelSolutionStruct.hpp>
#include <stdair/service/Logger.hpp>
// Distribution
#include <simcrs/SIMCRS_Service.hpp>
// Airsched
#include <airsched/AIRSCHED_Service.hpp>
// Dsim
#include <dsim/DSIM_Types.hpp>
#include <dsim/command/Simulator.hpp>

namespace DSIM {

  // ////////////////////////////////////////////////////////////////////
  void Simulator::simulate (SIMCRS::SIMCRS_Service& ioSIMCRS_Service) {

    try {

      // DEBUG
      STDAIR_LOG_DEBUG ("The simulation is starting");

      // TODO: remove this hardcoded section
      // Hardcode a booking request in order to simulate a sale.
      // Departure airport code
      stdair::AirportCode_T lOrigin ("LHR");
      // Arrival airport code
      stdair::AirportCode_T lDestination ("JFK");
      // Departure date
      stdair::Date_T lDepartureDate (2010, 01, 19);
      // Passenger type
      stdair::PassengerType_T lPaxType ("L");
      // Number of passengers in the travelling group
      stdair::NbOfSeats_T lPartySize = 5;
      // Booking request
      stdair::BookingRequestStruct lBookingRequest (lOrigin, lDestination,
                                                    lDepartureDate,
                                                    lPaxType, lPartySize);

      // Play booking request
      playBookingRequest (ioSIMCRS_Service, lBookingRequest);
            
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

