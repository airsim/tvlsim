#ifndef __AIRINV_SVR_BOMPROPERTYTREE_HPP
#define __AIRINV_SVR_BOMPROPERTYTREE_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
#include <set>
// StdAir
#include <stdair/stdair_basic_types.hpp>
#include <stdair/stdair_date_time_types.hpp>

namespace stdair {

  /**
   * Structure representing a list of airports.
   */
  struct BomPropertyTree {
    /**
     * Update the current BOM tree (*this) with the parsed stream,
     * which is JSON formatted.
     */
    void load (const std::string& iBomTree);

    /**
     * Dump the BOM tree (*this) into the stream with a JSON format.
     */
    std::string save() const;

    // ///////////// Attributes ////////////
    /** Airline code. */
    stdair::AirlineCode_T _airlineCode;

    /** Flight number. */
    stdair::FlightNumber_T _flightNumber;

    /** Departure date. */
    stdair::Date_T _departureDate;

    /** Just to have a list, for now. */
    std::set<stdair::AirportCode_T> _airportCodeList;
  };

}
#endif // __AIRINV_SVR_BOMPROPERTYTREE_HPP
