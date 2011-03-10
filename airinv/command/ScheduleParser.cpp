// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <string>
// StdAir
#include <stdair/bom/Inventory.hpp>
// Airinv
#include <airinv/command/ScheduleParserHelper.hpp>
#include <airinv/command/ScheduleParser.hpp>
#include <airinv/command/InventoryManager.hpp>

namespace AIRINV {

  // //////////////////////////////////////////////////////////////////////
  void ScheduleParser::
  generateInventories (const stdair::Filename_T& iFilename,
                       stdair::BomRoot& ioBomRoot) {

    // Initialise the Flight-Period file parser.
    FlightPeriodFileParser lFlightPeriodParser (ioBomRoot, iFilename);

    // Parse the CSV-formatted schedule input file, and generate the
    // corresponding Inventories for the airlines.
    lFlightPeriodParser.generateInventories ();
      
    // Complete the BomRoot BOM building
    // Create the routings for all the inventories.
    InventoryManager::createDirectAccesses (ioBomRoot);
  }

}
