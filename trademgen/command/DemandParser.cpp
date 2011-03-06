// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// StdAir
#include <stdair/basic/RandomGeneration.hpp>
#include <stdair/bom/Inventory.hpp>
// TraDemGen
#include <trademgen/command/DemandParserHelper.hpp>
#include <trademgen/command/DemandParser.hpp>

namespace TRADEMGEN {

  // //////////////////////////////////////////////////////////////////////
  void DemandParser::
  generateDemand (const stdair::Filename_T& iFilename,
                  stdair::EventQueue& ioEventQueue,
                  stdair::RandomGeneration& ioSharedGenerator,
                  const POSProbabilityMass_T& iDefaultPOSProbablityMass) {
    // Initialise the demand file parser.
    DemandFileParser lDemandParser (ioEventQueue, ioSharedGenerator,
                                    iDefaultPOSProbablityMass, iFilename);

    // Parse the CSV-formatted demand input file, and generate the
    // corresponding DemandCharacteristic objects.
    lDemandParser.generateDemand();
  }

}
