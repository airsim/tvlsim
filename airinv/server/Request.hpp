#ifndef __AIRINV_SVR_REQUEST_HPP
#define __AIRINV_SVR_REQUEST_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
#include <vector>
// StdAir
#include <stdair/stdair_basic_types.hpp>
#include <stdair/stdair_date_time_types.hpp>
// AirInv

namespace AIRINV {

  /** A request received from a client. */
  struct Request {
  public:
    /** Parse the incoming request.
        <br>Expected requested is of the form:
        <airline_code>,<flight_number>,<flight_date>, where date format is
        YYYY-MM-DD. For instance: BA,341,2010-09-20. */
    bool parseFlightDate();
    
  public:
    // /////////// Attributes //////////////
    /** String as it comes from the connected client. */
    std::string _flightDetails;
    /** Parsed airline code. */
    stdair::AirlineCode_T _airlineCode;
    /** Parsed flight number. */
    stdair::FlightNumber_T _flightNumber;
    /** Parsed departure date. */
    stdair::Date_T _departureDate;
  };

}
#endif // __AIRINV_SVR_REQUEST_HPP
