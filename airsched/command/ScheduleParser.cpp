// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <string>
// StdAir
#include <stdair/bom/Inventory.hpp>
// AirSched
#include <airsched/command/ScheduleParserHelper.hpp>
#include <airsched/command/ScheduleParser.hpp>

namespace AIRSCHED {

  // //////////////////////////////////////////////////////////////////////
  void ScheduleParser::
  generateInventories (const stdair::Filename_T& iFilename,
                       stdair::BomRoot& ioBomRoot) {

    // Initialise the Flight-Period file parser.
    FlightPeriodFileParser lFlightPeriodParser (ioBomRoot, iFilename);

    // Parse the CSV-formatted schedule input file, and generate the
    // corresponding Inventories for the airlines.
    lFlightPeriodParser.generateInventories ();
  }

}
