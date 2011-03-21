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
#include <airinv/command/InventoryParserHelper.hpp>
#include <airinv/command/InventoryParser.hpp>
#include <airinv/command/InventoryManager.hpp>

namespace AIRINV {

  // //////////////////////////////////////////////////////////////////////
  void InventoryParser::
  buildInventory (const stdair::Filename_T& iInventoryFilename,
                  stdair::BomRoot& ioBomRoot) {

    // Check that the file path given as input corresponds to an actual file
    const bool doesExistAndIsReadable =
      stdair::BasFileMgr::doesExistAndIsReadable (iInventoryFilename);
    if (doesExistAndIsReadable == false) {
      std::ostringstream oMessage;
      oMessage << "The inventory input file, '" << iInventoryFilename
               << "', can not be retrieved on the file-system";
      throw stdair::FileNotFoundException (oMessage.str());
    }

    // Initialise the inventory file parser.
    InventoryFileParser lInventoryParser (ioBomRoot, iInventoryFilename);

    // Parse the CSV-formatted inventory input file, and generate the
    // corresponding Inventory-related objects.
    lInventoryParser.buildInventory();
      
    // Complete the BomRoot BOM building: create the routings for all
    // the inventories.
    InventoryManager::createDirectAccesses (ioBomRoot);
  }

}
