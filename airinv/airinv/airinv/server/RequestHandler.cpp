// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <string>
#include <fstream>
#include <sstream>
// Boost
#include <boost/lexical_cast.hpp>
// StdAir
// AirInv
#include <airinv/server/Reply.hpp>
#include <airinv/server/Request.hpp>
#include <airinv/server/RequestHandler.hpp>

namespace AIRINV {

  // //////////////////////////////////////////////////////////////////////
  RequestHandler::RequestHandler (const stdair::AirlineCode_T& iAirlineCode)
    : _airlineCode (iAirlineCode) {
  }

  // //////////////////////////////////////////////////////////////////////
  bool RequestHandler::
  handleRequest (Request& ioRequest, Reply& ioReply) const {
    bool hasBeenSuccessfull = false;
    
    // Decode request string to a flight-date details (airline code,
    // flight number and departure date)
    hasBeenSuccessfull = ioRequest.parseFlightDate();

    if (hasBeenSuccessfull == false) {
      ioReply._status = FlightRequestStatus::INTERNAL_ERROR;
      return hasBeenSuccessfull;
    }

    /**
    if (!is) {
      ioReply = Reply::stock_reply (Reply::not_found);
      return;
    }
    */

    // Fill out the reply to be sent to the client.
    ioReply._status = FlightRequestStatus::OK;
    ioReply.content = "Your are looking for: '" + ioRequest._flightDetails + "'. Ok, I have found your flight-date. Be patient until I give you more information about it";
    
    return hasBeenSuccessfull;
  }

}
