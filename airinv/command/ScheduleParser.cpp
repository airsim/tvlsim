// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <sstream>
// StdAir
#include <stdair/basic/BasFileMgr.hpp>
#include <stdair/bom/BomRoot.hpp>
// Airinv
#include <airinv/command/ScheduleParserHelper.hpp>
#include <airinv/command/ScheduleParser.hpp>
#include <airinv/command/InventoryManager.hpp>

namespace AIRINV {

  // //////////////////////////////////////////////////////////////////////
  void ScheduleParser::
  generateInventories (const stdair::Filename_T& iScheduleFilename,
                       stdair::BomRoot& ioBomRoot) {

    // Check that the file path given as input corresponds to an actual file
    bool doesExistAndIsReadable =
      stdair::BasFileMgr::doesExistAndIsReadable (iScheduleFilename);
    if (doesExistAndIsReadable == false) {
      std::ostringstream oMessage;
      oMessage << "The schedule input file, '" << iScheduleFilename
               << "', can not be retrieved on the file-system";
      throw stdair::FileNotFoundException (oMessage.str());
    }

    // Initialise the Flight-Period file parser.
    FlightPeriodFileParser lFlightPeriodParser (ioBomRoot, iScheduleFilename);

    // Parse the CSV-formatted schedule input file, and generate the
    // corresponding Inventories for the airlines.
    lFlightPeriodParser.generateInventories ();
      
    // Complete the BomRoot BOM building
    // Create the routings for all the inventories.
    InventoryManager::createDirectAccesses (ioBomRoot);

    // Build the similar flight-date sets and the corresponding guillotine
    // blocks.
    InventoryManager::buildSimilarFlightDateSets (ioBomRoot);
  }

}
