// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// Boost Property Tree
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
// Boost ForEach
#include <boost/foreach.hpp>
// AirInvServer
#include <airinv/server/BomPropertyTree.hpp>

namespace bpt = boost::property_tree;

namespace stdair {

  // Loads BomPropertyTree structure from the specified JSON file
  void BomPropertyTree::load (const std::string& iBomTree) {
    // Create an empty property tree object
    bpt::ptree pt;

    // Load the JSON formatted string into the property tree. If reading fails
    // (cannot open stream, parse error), an exception is thrown.
    std::istringstream iStr (iBomTree);
    read_json (iStr, pt);

    // Get the airline_code and store it in the _airlineCode variable.
    // Note that we construct the path to the value by separating
    // the individual keys with dots. If dots appear in the keys,
    // a path type with a different separator can be used.
    // If the flight_date.airline_code key is not found, an exception is thrown.
    _airlineCode = pt.get<stdair::AirlineCode_T> ("flight_date.airline_code");

    // Get the departure_date and store it in the _departureDate variable.
    // This is another version of the get method: if the value is
    // not found, the default value (specified by the second
    // parameter) is returned instead. The type of the value
    // extracted is determined by the type of the second parameter,
    // so we can simply write get(...) instead of get<int>(...).
    _flightNumber =
      pt.get<stdair::FlightNumber_T> ("flight_date.flight_number", 100);

    const std::string& lDepartureDateStr =
      pt.get<std::string> ("flight_date.departure_date");
    _departureDate = boost::gregorian::from_simple_string (lDepartureDateStr);

    // Iterate over the flight_date.airport_codes section and store all found
    // codes in the _airportCodeList set. The get_child() function
    // returns a reference to the child at the specified path; if
    // there is no such child, it throws. Property tree iterators
    // are models of BidirectionalIterator.
    /*
    BOOST_FOREACH (bpt::ptree::value_type &v,
                   pt.get_child ("flight_date.airport_codes")) {
      _airportCodeList.insert (v.second.data());
    }
    */
  }

  // Saves the BomPropertyTree structure to the specified JSON file
  std::string BomPropertyTree::save() const {
    std::ostringstream oStr;

    // Create an empty property tree object
    bpt::ptree pt;

    // Put airline code in property tree
    pt.put ("flight_date.airline_code", _airlineCode);

    // Put flight number level in property tree
    pt.put ("flight_date.flight_number", _flightNumber);

    // Put the flight departure date in property tree
    const std::string& lDepartureDateStr =
      boost::gregorian::to_simple_string (_departureDate);
    pt.put ("flight_date.departure_date", lDepartureDateStr);

    // Iterate over the airport codes in the set and put them in the
    // property tree. Note that the put function places the new
    // key at the end of the list of keys. This is fine most of
    // the time. If you want to place an item at some other place
    // (i.e. at the front or somewhere in the middle), this can
    // be achieved using a combination of the insert and put_own
    // functions.
    bpt::ptree lAirportCodeArray;
    BOOST_FOREACH (const std::string& name, _airportCodeList) {
      lAirportCodeArray.push_back (std::pair<bpt::ptree::key_type,
                                             bpt::ptree::data_type> ("", name));
    }
    pt.put_child ("flight_date.airport_codes", lAirportCodeArray);
    //pt.push_back (std::make_pair ("flight_date.airport_codes", lAirportCodeArray));

    // Write the property tree to the JSON stream.
    write_json (oStr, pt);

    return oStr.str();
  }

}
