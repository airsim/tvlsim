// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// StdAir
#include <stdair/basic/BasFileMgr.hpp>
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

    // Check that the file path given as input corresponds to an actual file
    const bool doesExistAndIsReadable =
      stdair::BasFileMgr::doesExistAndIsReadable (iFilename);
    if (doesExistAndIsReadable == false) {
      STDAIR_LOG_ERROR ("The demand input file '" << iFilename
                        << "' does not exist or can not be read");
      
      throw DemandInputFileNotFoundException ("The demand file '" + iFilename
                                              + "' does not exist or can not "
                                              "be read");
    }

    // Initialise the demand file parser.
    DemandFileParser lDemandParser (ioEventQueue, ioSharedGenerator,
                                    iDefaultPOSProbablityMass, iFilename);

    // Parse the CSV-formatted demand input file, and generate the
    // corresponding DemandCharacteristic objects.
    lDemandParser.generateDemand();
  }

}
