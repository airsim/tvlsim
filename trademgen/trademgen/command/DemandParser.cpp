// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <string>
// STDAIR
#include <stdair/bom/Inventory.hpp>
// TRADEMGEN
#include <trademgen/basic/DictionaryManager.hpp>
#include <trademgen/command/DemandParserHelper.hpp>
#include <trademgen/command/DemandParser.hpp>

namespace TRADEMGEN {

  // //////////////////////////////////////////////////////////////////////
  void DemandParser::
  generateDemand (const stdair::Filename_T& iFilename,
                  stdair::BomRoot& ioBomRoot,
                  stdair::UniformGenerator_T& ioSharedGenerator,
                  const POSProbabilityMass_T& iDefaultPOSProbablityMass) {
    // Initialise the demand file parser.
    DemandFileParser lDemandParser (ioBomRoot, ioSharedGenerator,
                                    iDefaultPOSProbablityMass, iFilename);

    // Parse the CSV-formatted demand input file, and generate the
    // corresponding DemandCharacteristic objects.
    lDemandParser.generateDemand ();

  }

}
