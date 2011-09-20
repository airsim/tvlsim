// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <string>
// StdAir
#include <stdair/basic/BasFileMgr.hpp>
//#include <stdair/bom/Inventory.hpp>
#include <stdair/service/Logger.hpp>
// AirSched
#include <simfqt/command/FareParserHelper.hpp>
#include <simfqt/command/FareParser.hpp>

namespace SIMFQT {

  // //////////////////////////////////////////////////////////////////////
  void FareParser::fareRuleGeneration (const FareFilePath& iFareFilename,
                                       stdair::BomRoot& ioBomRoot) {

    const stdair::Filename_T lFilename = iFareFilename.name();

    // Check that the file path given as input corresponds to an actual file
    const bool doesExistAndIsReadable =
      stdair::BasFileMgr::doesExistAndIsReadable (lFilename);
    if (doesExistAndIsReadable == false) {
      STDAIR_LOG_ERROR ("The fare input file, '" << lFilename
                        << "', can not be retrieved on the file-system");
      throw FareInputFileNotFoundException ("The fare input file '" + lFilename
                                            + "' does not exist or can not "
                                            "be read");
    }

    // Initialise the fare file parser.
    FareRuleFileParser lFareRuleFileParser (ioBomRoot, lFilename);

    // Parse the CSV-formatted fare input file and generate the
    // corresponding fare rules.
    lFareRuleFileParser.generateFareRules ();

  }

}
